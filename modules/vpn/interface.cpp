extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

#include "monitor.h"
#include "ipsec.h"
#include "cert/CertUtils.h"

lua_State * theState = NULL;
cmonitor * theMonitor = NULL;
cipsec     *  theVPN = NULL;

static int installcert(lua_State * L) {
	const char * szcert = luaL_checkstring(L, 1);
	CertUtils cert;
	int ret = cert.installCaCert(szcert);
	lua_pushinteger(L, ret);
	return 1;
}

static int uninstallcert(lua_State * L) {
	const char * certname = luaL_checkstring(L, 1);
	CertUtils cert;
	int ret = cert.uninstall(certname);
	lua_pushinteger(L, ret);
	return 1;
}

static int connect(lua_State *L) {
	const char * ip     = luaL_checkstring(L, 1);
	const char * user = luaL_checkstring(L, 2);
	const char * pwd = luaL_checkstring(L, 3);
	int cb  = luaL_ref(L, LUA_REGISTRYINDEX); 
	int ret = theVPN->connect(ip,user,pwd,cb);
	lua_pushinteger(L, ret);
	return	 1;
}

static int disconnect(lua_State *L) {
	int ret = theVPN->disconnect();
	lua_pushinteger(L, ret);
	return 1;
}

static int destory(lua_State * L){
	if(theVPN){
		theVPN->stop();
		delete theVPN;
		theVPN = NULL;
	}
	return 0;
}

static const struct luaL_Reg myLib[]={
	{"destory",destory},
	{"installcert",installcert },
	{"uninstallcert",uninstallcert },
	{"disconnect",disconnect },
	{"connect",connect },
	{NULL,NULL}
};

#ifdef WIN32
extern "C" __declspec(dllexport) int luaopen_vpn(lua_State *L){
#else
extern "C" int luaopen_vpn(lua_State *L){
#endif
	printf("luaopen_vpn\n");
	if( !theVPN){
		theState = L;
		theVPN = new cipsec;
		theVPN->start();
	}
	lua_newtable(L);
	const luaL_Reg * la = &myLib[0];
	for(;la->name!=NULL; la++){
		lua_pushcclosure(L,la->func,0);
		lua_setfield(L,-2,la->name);
	}
	lua_pop(L,0);
	return 1;
}
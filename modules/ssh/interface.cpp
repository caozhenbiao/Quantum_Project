extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

#include "ssh.h"
lua_State * theState = NULL;
cssh * theSSH = NULL;

static int connect(lua_State * L) {
	const char * ip = luaL_checkstring(L, 1);
	unsigned short port = luaL_checkinteger(L, 2);
	const char * name = luaL_checkstring(L, 3);
	const char * pwd = luaL_checkstring(L, 4);
	int handle = theSSH->connect_password( ip,port,name,pwd);
	lua_pushinteger( L, handle );
	return 1;
}

static int connect_privatekey(lua_State * L) {
	const char * ip = luaL_checkstring(L, 1);
	unsigned short port = luaL_checkinteger(L, 2);
	const char * name = luaL_checkstring(L, 3);
	const char * keyfile = luaL_checkstring(L, 4);
	int handle = theSSH->connect_privatekey(ip, port, name, keyfile);
	lua_pushinteger(L, handle);
	return 1;
}

static int disconnect(lua_State * L) {
	int handle = luaL_checkinteger(L, 1);
	int ret = theSSH->shutdown( handle );
	lua_pushinteger(L, ret);
	return 1;
}

static int request(lua_State *L) {
	int handle = luaL_checkinteger(L, 1);
	const char * cmd = luaL_checkstring(L, 2);
	int callback = luaL_ref(L, LUA_REGISTRYINDEX);
	int ret = theSSH->execute(handle, cmd, callback);
	lua_pushinteger(L, ret);
	return 1;
}

static int destory(lua_State * L) {
	if (theSSH) {
		theSSH->stop();
		delete theSSH;
		theSSH = NULL;
	}
	return 0;
}

static const struct luaL_Reg myLib[]={
	{"connect",connect },
	{"connect_privatekey",connect_privatekey},
	{"disconnect",disconnect },
	{"request",request },
	{"destory",destory},
	{NULL,NULL}
};

#ifdef WIN32
extern "C" __declspec(dllexport) int luaopen_ssh(lua_State *L){
#else
extern "C" int luaopen_ssh(lua_State *L){
#endif
	printf("luaopen_ssh\n");
	if( !theSSH ){
		theState = L;
		theSSH = new cssh;
		theSSH->start();
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

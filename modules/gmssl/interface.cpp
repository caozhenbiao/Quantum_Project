#ifndef __LUA_INC
#define __LUA_INC
extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}
#endif

#include "myssl.h"
cmyssl * myssl = NULL;

static int _getPeerCert(lua_State * L){
	const char *ip = luaL_checkstring (L, 1); 
	lua_Integer  port = luaL_checkinteger(L, 2);
	printf("ip:%s port:%d\n", ip, (int)port);
	const char*  cf = myssl->getPeerCert(ip, (unsigned short)port);
	lua_pushstring(L, cf);
	return 1;
}

static int Destory(lua_State * L ){
	if (myssl) {
		free(myssl);
		myssl = NULL;
	}
	return 0;
}
 
static const struct luaL_Reg myLib[] = {
	{"getPeerCert", _getPeerCert},
	{"Destory", Destory},
	{NULL, NULL},
};

#ifdef WIN32
extern "C" __declspec(dllexport) int luaopen_gmssl(lua_State *L){
#else
extern "C" int luaopen_gmssl(lua_State *L){
#endif
	printf("luaopen_gmssl\n");
	if( !myssl ){
		myssl = new cmyssl;
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


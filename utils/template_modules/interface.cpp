extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

#include "netcard.h"
lua_State * theState = NULL;
cnetcard * theNetCard = NULL;

static int authorize(lua_State * L) {
	if (!theNetCard)
		return 0;
	const char * pkey = luaL_checkstring(L, 1);
	int ret = theNetCard->getinfo((char*)pkey);
	lua_pushnumber( L, ret );
	return 1;
}

static int destory(lua_State * L){
	if( theNetCard ){
		theNetCard->stop();
		delete theNetCard;
		theNetCard = NULL;
	}
	return 0;
}

static int request_delay(lua_State *L) {
	if (!theNetCard)
		return 0;
	const char * url = luaL_checkstring(L, 1);
	int delay = (int)lua_tointeger(L, 2);
	int callback = luaL_ref(L, LUA_REGISTRYINDEX);
	theNetCard->request(url,delay,callback);
	return 0;
}

static int request(lua_State *L) {
	if (!theNetCard)
		return 0;
	const char * url = luaL_checkstring(L, 1);
	int callback = luaL_ref(L, LUA_REGISTRYINDEX);
	theNetCard->request(url, callback);
	return 0;
}

static const struct luaL_Reg myLib[]={
	{"destory",destory},
	{"request_delay",request_delay },
	{"request",request },
	{NULL,NULL}
};

#ifdef WIN32
extern "C" __declspec(dllexport) int luaopen_new_project(lua_State *L){
#else
extern "C" int luaopen_new_project(lua_State *L){
#endif
	printf("luaopen_new_project\n");
	if( !theNetCard ){
		theState = L;
		theNetCard = new cnetcard;
		theNetCard->start();
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

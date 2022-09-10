extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

#include <vector>
#include <string>
#include "regedit.h"
 
lua_State * theState = NULL;
cregedit *  theRegedit = NULL;

static int openKey(lua_State * L) {
	HKEY hMaster = (HKEY)luaL_checkinteger(L, 1);
	const char *path = luaL_checkstring(L, 2);
	HKEY hKey = 0;
	int ret = theRegedit->openKey( hMaster, path, hKey );
	lua_pushinteger(L, ret);
	lua_pushinteger(L, lua_Integer(hKey) );
	return 2;
}

static int createKey(lua_State * L) {
	printf("lua createKey\n");
	HKEY hMaster = (HKEY)luaL_checkinteger(L, 1);
	const char *path = luaL_checkstring(L, 2);
	HKEY hKey = 0;
	int ret = theRegedit->createKey(hMaster, path, hKey);
	lua_pushinteger(L, ret);
	lua_pushinteger(L, lua_Integer(hKey));
	return 2;
}

static int closeKey(lua_State * L) {
	HKEY hKey = (HKEY)luaL_checkinteger(L, 1);
	int ret = theRegedit->closeKey(hKey);
	lua_pushinteger(L, ret);
	return 1;
}

static int readStrValue( lua_State * L ) {
	static char value[1024];
	HKEY hKey  = (HKEY)luaL_checkinteger(L, 1);
	const char *name = luaL_checkstring(L, 2);
	memset(&value, 0x00, sizeof(value));
	int ret = theRegedit->readValue(hKey, name, value, 1024);
	lua_pushinteger(L, ret);
	lua_pushstring(L, value);
	return 2;
}

static int writeStrValue(lua_State * L) {
	HKEY hKey = (HKEY)luaL_checkinteger(L, 1);
	const char *name = luaL_checkstring(L, 2);
	const char *value = luaL_checkstring(L, 3);
	int ret = theRegedit->writeValue(hKey, name, value);
	lua_pushinteger(L, ret);
	return 1;
}

static int readIntValue(lua_State * L) {
	int value = 0;
	HKEY hKey = (HKEY)luaL_checkinteger(L, 1);
	const char *name = luaL_checkstring(L, 2);
	memset(&value, 0x00, sizeof(value));
	int ret = theRegedit->readValue(hKey, name, value);
	lua_pushinteger(L, ret);
	lua_pushinteger(L, value);
	return 2;
}

static int writeIntValue(lua_State * L) {
	HKEY hKey = (HKEY)luaL_checkinteger(L, 1);
	const char *name = luaL_checkstring(L, 2);
	int value = (int)luaL_checkinteger(L, 3);
	memset(&value, 0x00, sizeof(value));
	int ret = theRegedit->writeValue( hKey, name, value);
	lua_pushinteger(L, ret);
	return 1;
}

static int destory(lua_State * L){
	if(theRegedit){
		delete theRegedit;
		theRegedit = NULL;
	}
	return 0;
}

static const struct luaL_Reg myLib[]={
	{"destory",destory},
	{"openKey",openKey},
	{"createKey",createKey},
	{"closeKey",closeKey},
	{"readStrValue",readStrValue},
	{"writeStrValue",writeStrValue},
	{"readIntValue",readIntValue},
	{"writeIntValue",writeIntValue},
	{NULL,NULL}
};

#ifdef _WIN32
extern "C" __declspec(dllexport) int luaopen_regedit(lua_State *L){
#else
extern "C" int luaopen_regedit(lua_State *L){
#endif
	printf("luaopen_regedit\n");
	if( !theRegedit){
		theState = L;
		theRegedit = new cregedit;
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

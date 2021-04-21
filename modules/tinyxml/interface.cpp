#ifndef __LUA_INC
#define __LUA_INC
extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}
#endif

#include "myxml.h"
cmyxml * myxml = NULL;

static int LoadFile(lua_State * L){
	const char *file = luaL_checkstring (L, 1); 
	lua_pushinteger(L, myxml->load(file));
	return 1;
}

static int Destory(lua_State * L ){
	myxml->destory();
	return 0;
}

static int SaveFile(lua_State * L ){
	//LUAI_MEM doc = luaL_checkinteger(L, 1);
	//myxml->SaveFile();
	return 0;
}

//FirstChildElement
static int FirstChildElement(lua_State * L){
	lua_Integer   idx = luaL_checkinteger(L, 1);
	const char *    n = luaL_checkstring(L, 2);
	int  idc          = myxml->FirstChildElement(idx, n );
	lua_pushinteger(L, idc );
	return 1;
}

static int NextSiblingElement(lua_State * L){
	lua_Integer    idx = luaL_checkinteger(L, 1);
	const char *   n   = luaL_checkstring(L, 2);
	int           idn  = myxml->NextSiblingElement(idx, n );
	lua_pushinteger(L, idn );
	return 1;
}

static int Attribute(lua_State * L){
	lua_Integer   idx = luaL_checkinteger(L, 1);
	const char *   n  = luaL_checkstring(L, 2);
	const char *   r  = myxml->Attribute( idx, n );
	lua_pushstring(L, r);
	return 1;
}

static int GetText(lua_State * L){
	lua_Integer idx  = luaL_checkinteger(L, 1);
	const char *   r = myxml->GetText( idx );
	lua_pushstring(L, r);
	return 1;
}
 
static const struct luaL_Reg myLib[] = {
	{"LoadFile", LoadFile},
	{"Destory", Destory},
	{"SaveFile", SaveFile},
	{"FirstChildElement", FirstChildElement},
	{"NextSiblingElement", NextSiblingElement},
	{"Attribute", Attribute},
	{"GetText", GetText},
	{NULL, NULL},
};

#ifdef WIN32
extern "C" __declspec(dllexport) int luaopen_tinyxml(lua_State *L){
#else
extern "C" int luaopen_tinyxml(lua_State *L){
#endif
	printf("luaopen_tinyxml\n");
	if( !myxml ){
		myxml = new cmyxml;
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


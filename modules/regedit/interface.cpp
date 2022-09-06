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

std::string _cmdPopen(const std::string& cmdLine) {
	char buffer[1024] = { '\0' };
	FILE* pf = NULL;
	pf = _popen(cmdLine.c_str(), "r");
	if (NULL == pf) {
		printf("open pipe failed\n");
		return std::string("");
	}
	std::string ret;
	while (fgets(buffer, sizeof(buffer), pf)) {
		ret += buffer;
	}
	_pclose(pf);
	return ret;
}

static int cmdPopen(lua_State * L) {
	const char *subkey = luaL_checkstring(L, 1);
	std::string retstr =  _cmdPopen(subkey);
	lua_pushstring(L, retstr.c_str() );
	return 1;
}

static int openKey(lua_State * L) {
	int hMaster = (int)luaL_checkinteger(L, 1);
	const char *subkey = luaL_checkstring(L, 2);
	//theRegedit->openKey((HKEY)hMaster, subkey);
	lua_pushinteger(L, 1);
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
	{"cmdPopen",cmdPopen},
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

extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}
#include "./websock.h"
#include <map>
#include <string>
#ifdef WIN32
#include <process.h>
#endif

lua_State * theState = NULL;
websock * theWebsock = NULL;
static int lua_callback = LUA_REFNIL;

#ifdef WIN32
static void GB2312ToUTF8(std::string& pOut, const char *pText, int pLen) {
	char buf[4];
	int nLength = pLen * 3;
	char* rst = new char[nLength];
	memset(buf, 0, 4);
	memset(rst, 0, nLength);
	int i = 0;
	int j = 0;
	while (i < pLen) {
		if (*(pText + i) >= 0)
			rst[j++] = pText[i++];
		else {
			wchar_t pbuffer;
			::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pText + i, 2, &pbuffer, 1);
			char* pchar = (char *)&pbuffer;
			buf[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
			buf[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
			buf[2] = (0x80 | (pchar[0] & 0x3F));
			unsigned short int tmp = 0;
			tmp = rst[j] = buf[0];
			tmp = rst[j + 1] = buf[1];
			tmp = rst[j + 2] = buf[2];
			j += 3;
			i += 2;
		}
	}
	rst[j] = 0;
	pOut = rst;
	delete[]rst;
	return;
}
#else
static void GB2312ToUTF8(std::string& pOut, const char *pText, int pLen) {
	pOut = pText;
}

#endif

//lua script
int responsetolua(unsigned int act, char* data, int len) {
	lua_Number nret = -1;
	if (0 == luaL_lock(theState)) {
		lua_rawgeti(theState, LUA_REGISTRYINDEX, lua_callback);
		lua_pushinteger(theState, act);
		lua_pushinteger(theState, len);
		lua_pushstring(theState, data);
		lua_pcall(theState, 3, 1, 0);
		lua_isnumber(theState, -1);
		nret = lua_tonumber(theState, -1);
		lua_pop(theState, 1);
		luaL_unlock(theState);
	}
	return (int)nret;
}

static int start(lua_State *L){
	const char* ip   = luaL_checkstring(L,1);
	lua_Integer port = luaL_checkinteger(L,2);
	theWebsock->start( ip, (short)port);
	lua_callback = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_pushinteger(L,0);
	return 1;
}

static int stop(lua_State *L){
	luaL_unref(L, LUA_REGISTRYINDEX, lua_callback);
	theWebsock->stop();
	return 0;
}

static int sendfrm(lua_State *L){
	size_t  size   = (size_t)luaL_checkinteger(L,2);
	const char* buf    = luaL_checklstring(L,1, &size);
	int nsnd = theWebsock->sends((unsigned char*)buf,size);
	lua_pushnumber(L, nsnd );
	return 1;
}

static int sendutf8(lua_State *L) {
	const char* str = luaL_checkstring(L, 1);
	std::string utfstr;
	GB2312ToUTF8(utfstr, str, strlen(str));
	int nsnd = theWebsock->sends( utfstr.c_str() );
	lua_pushnumber(L, nsnd);
	return 1;
}

static int sendstr(lua_State *L){
	const char* str = luaL_checkstring(L,1);
	int nsnd = theWebsock->sends(str);
	lua_pushnumber(L, nsnd );
	return 1;
}

static int getip(lua_State *L){
	char szip[256]={0};
	const char* nc = luaL_checkstring(L,1);
	theWebsock->getip(nc,szip);
	lua_pushstring(L, szip );
	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int destory(lua_State *L){
	if( theWebsock ){
		delete theWebsock;
		theWebsock = NULL;
	}
	return 0;
}

//�ⲿ�ӿڵ�������
static const struct luaL_Reg mylib[]={
	{"destory",destory},
	{"start",start},
	{"stop",stop},
	{"sendfrm",sendfrm},
	{"sendstr",sendstr},
	{"sendutf8",sendutf8 },
	{"getip",getip},
	{NULL,NULL}
};

#ifdef WIN32
	extern "C" __declspec(dllexport) int luaopen_websock(lua_State *L){
#else
	extern "C" int luaopen_websock(lua_State *L){
#endif
	printf("luaopen_websock\n");
	if( !theState ){
		theState = L;
		theWebsock = new websock;
	}
	lua_newtable(L);
	const luaL_Reg * la = &mylib[0];
	for( ; la->name!=NULL; la++ ){
		lua_pushcclosure(L,la->func, 0);
		lua_setfield(L,-2,la->name);
	}
	lua_pop(L,0);
	return 1;
}






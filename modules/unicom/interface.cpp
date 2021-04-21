extern "C" {
#include "../../platform/smartx/libs/lua/src/lua.h"
#include "../../platform/smartx/libs/lua/src/lualib.h"
#include "../../platform/smartx/libs/lua/src/lauxlib.h"
}
#include "../../platform/smartx/include/define.h"
#include "machine.h"

machine   * theMachine = NULL;
lua_State * theState = NULL;
char g_recvBuffer[65535] = { 0 };

//ͨ����
static int openchn(lua_State *L) {
	int  type = luaL_checkinteger(L, 1);
	const char * name = luaL_checkstring(L, 2);
	const char * args = luaL_checkstring(L, 3);
	int ret = theMachine->open(name, type, args);
	lua_pushinteger(L, ret);
	return 1;
}

//ͨ���ر�
static int closechn(lua_State *L) {
	const char * name = luaL_checkstring(L, 1);
	int ret = theMachine->close(name);
	lua_pushinteger(L, ret);
	return 1;
}

static int senddata(lua_State *L) {
	const char * name = luaL_checkstring(L, 1);
	const char * data = luaL_checkstring(L, 2);
	lua_Integer   len = luaL_checkinteger(L, 3);
	printf(name);
	int ret = theMachine->send(name, data, len);
	lua_pushinteger(L,ret);
	return 1;
}

static int recvdata(lua_State *L) {
	const char * name = luaL_checkstring(L, 1);
	memset(g_recvBuffer, 0x00, sizeof(g_recvBuffer));
	int recved = theMachine->recv(name, g_recvBuffer, sizeof(g_recvBuffer));
	lua_pushstring(L, g_recvBuffer);
	lua_pushinteger(L, recved);
	return 2;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ģ�����ٶ���ʵ����ɾ���������
static int destory(lua_State *L){
	if (theMachine) {
		delete theMachine;
		theMachine = NULL;
	}
	return 0;
}

//�ⲿ�ӿڵ�������
static const struct luaL_Reg mylib[]={
	{"destory",destory},
	{"openchn",openchn },
	{"closechn",closechn },
	{"senddata",senddata },
	{"recvdata",recvdata },
	{NULL,NULL}
};

#ifdef WIN32
	extern "C" __declspec(dllexport) int luaopen_unicom(lua_State *L){
#else
	extern "C" int luaopen_unicom(lua_State *L){
#endif
	printf("luaopen_unciom\n");
	if( !theState ){
		theState = L; 
		theMachine = new machine;
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

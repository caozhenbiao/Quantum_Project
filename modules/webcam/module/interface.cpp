extern "C" {
#include "../../smartx/libs/lua/src/lua.h"
#include "../../smartx/libs/lua/src/lualib.h"
#include "../../smartx/libs/lua/src/lauxlib.h"
}
#include "../../smartx/include/define.h"
#include "../../smartx/include/event.h"
#ifdef WIN32
#include <process.h>
#endif


lua_State * theState = NULL;
static int lua_callback = LUA_REFNIL;

 
int backtoLua( int chn, int tp, char * val ){
	luaL_glock(theState);
	lua_rawgeti( theState, LUA_REGISTRYINDEX, lua_callback);
	lua_pushinteger(theState, chn);
	lua_pushinteger(theState, tp );
	lua_pushstring(theState, val );
	lua_pcall(theState,3,0,0);
	luaL_unglock(theState);
	return 0;
}

//创建通道
static int createCHN( lua_State *L ){
	const char * addr = luaL_checkstring(L,1);
	lua_Integer idx = luaL_checkinteger(L,2);
	return 0;
}

//start
static int start(lua_State *L){
	lua_callback = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_pushinteger(L,0);
	return 1;
}

//stop
static int stop(lua_State *L){
	return 0;
}


//stop
static int sendyk(lua_State *L){
	lua_Integer chn = luaL_checkinteger(L,1);
	lua_Integer pnt = luaL_checkinteger(L,2);
	//先查找
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//模块销毁对象实例，删除相关数据
static int destory(lua_State *L){
	return 0;
}

//外部接口导出定义
static const struct luaL_Reg mylib[]={
	{"destory",destory},
	{"createCHN",createCHN},
	{"start",start},
	{"stop",stop},
	{"sendyk",sendyk},
	{NULL,NULL}
};

#ifdef WIN32
	extern "C" __declspec(dllexport) int luaopen_module(lua_State *L){
#else
	extern "C" int luaopen_module(lua_State *L){
#endif
	printf("luaopen_module\n");
	if( !theState ){
		theState = L; 
		//在这创建对象实例

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

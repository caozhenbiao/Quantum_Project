#include "luadisp.h"
#include "./liblua/src/lua.h"
#include "./liblua/src/lauxlib.h"
#include "./liblua/src/lualib.h"
#include <winsock.h>
#define WIN32_LEAN_AND_MEAN  //这很重要，解决winsock.h winsock2.h冲突
#include "include/map.h"
#include "include/http.h"
 
static int luatimer = LUA_REFNIL;
static int luaclose = LUA_REFNIL;
map_t(lua_State*) luastates;
static lua_State * main_state=NULL;
char*(*remote_dispath)(char*, char*, unsigned);
int timer_delay = 1000;

//Luascript to quark;
extern  int install(lua_State* L) {
	timer_delay = (unsigned)lua_tointeger(L, 1);
	luaclose = luaL_ref(L, LUA_REGISTRYINDEX); 
	luatimer = luaL_ref(L, LUA_REGISTRYINDEX);
	return 0;
}

//远程调用
extern int remote_execute(lua_State* L) {
	const char* uri = luaL_checkstring(L, 1);
	const char* data = luaL_checkstring(L, 2);
	unsigned len = (unsigned)lua_tointeger(L, 3);
	char * data_out = (char*)malloc(sizeof(char*));
	memset(data_out, 0x00, sizeof(char*));
	int data_out_len = http_post_request(uri, data, len, &data_out);
	lua_pushinteger(L,data_out_len);
	lua_pushstring(L, data_out);
	free( data_out );
	return 2;
}

int panic(lua_State *L) {
	lua_writestringerror("PANIC: unprotected error in call to Lua API (%s)\n",lua_tostring(L, -1));
	return 0;
}

int luadisp_start(const char* script) {
	printf("start business service, script:%s\n", script);
	main_state = luaL_newstate();
	luaL_openlibs(main_state);
	if (main_state && luaL_loadfile(main_state, script)) {
		if (main_state) lua_atpanic(main_state, &panic);
		printf("script error or file:%s  error:%d mission!\n", script, lua_error(main_state));
		lua_close(main_state);
		main_state = NULL;
		return 0;
	}
	map_init(&luastates);
	map_set(&luastates, script, main_state);
	lua_register(main_state, "install", install);
	lua_register(main_state, "remote_execute", remote_execute);
	lua_pcall(main_state, 0, 0, 0);
	return 1;
}

int luadisp_stop( void ) {
	if (!main_state)	 return 0;
	if (luaL_lock(main_state) == 0) {
		lua_rawgeti(main_state, LUA_REGISTRYINDEX, luaclose);
		lua_pcall(main_state, 1, 0, 0);
		luaL_unlock(main_state);
	}
	luaL_unref(main_state, LUA_REGISTRYINDEX, luaclose);
	luaL_unref(main_state, LUA_REGISTRYINDEX, luatimer);
	lua_close(main_state);
	map_deinit(&luastates);
	return 0;
}

//websvr execute with http request   file.func, data
char*  luadisp_execute(char* iface, char* data, unsigned len) {
	char* retval = "{\"error\":-1}";
	char script[64] = { 0 };
	char func[64] = { 0 };
	if (2 != sscanf(iface, "%[^.].%[^ ]*", script, func)) {
		strcpy(script, "main.lua");
		strcpy(func, iface);
	}else {
		strcat(script, ".lua");
	}
	//loaded script
	lua_State * state = *(map_get(&luastates, script));
	if (state) {
		if (0 == luaL_lock(state)) {
			lua_getglobal(state, func);
			lua_pushstring(state, data);
			lua_pcall(state, 1, 1, 0);
			retval = (char*)lua_tostring(state, -1);
			luaL_unlock(state);
		}
		return retval;
	}
	//unload script
	lua_State * lua = luaL_newstate();
	luaL_openlibs(lua);
	if (luaL_dofile(lua, script)) {
		printf("%s,script error or file mission!\n", script);
		lua_close(lua);
		return retval;
	}
	map_set(&luastates, script, lua);
	if (0 == luaL_lock(lua)) {
		lua_getglobal(lua, func);
		lua_pushstring(lua, data);
		lua_pcall(lua, 1, 1, 0);
		retval = (char*)lua_tostring(lua, -1);
		luaL_unlock(lua);
	}
	return retval;
}

//触发LUA
void luadisp_dotimer( void* tms ) {
	static unsigned int ntimes = 0;
	if (main_state && luaL_lock(main_state) == 0) {
		lua_rawgeti(main_state, LUA_REGISTRYINDEX, luatimer);
		lua_pushnumber(main_state, ntimes++);
		lua_pcall(main_state, 1, 0, 0);
		luaL_unlock(main_state);
	}
}

//关闭
void luadisp_doclose(void) {
	if (main_state && luaL_lock(main_state) == 0) {
		lua_rawgeti(main_state, LUA_REGISTRYINDEX, luaclose);
		lua_pcall(main_state, 1, 0, 0);
		luaL_unlock(main_state);
	}
}

//获取LUA定时器时间
int luadisp_ticktime() {
	if (timer_delay < 10) {
		timer_delay = 10;
	}	
	return timer_delay;
}

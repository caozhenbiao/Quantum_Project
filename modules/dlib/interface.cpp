extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}
#include "event.h"
#include "dlibwraper.h"
lua_State * theState = NULL;
static int lua_callback = LUA_REFNIL;
static dlibwraper * thedlib = NULL;
event_handle mydispath;


//返回至lua script
void responsetolua(unsigned int act, char* data, int len) {
	if (0 == luaL_lock(theState)) {
		lua_rawgeti(theState, LUA_REGISTRYINDEX, lua_callback);
		lua_pushinteger(theState, act);
		lua_pushinteger(theState, len);
		lua_pushstring(theState, data);
		lua_pcall(theState, 3, 0, 0);
		luaL_unlock(theState);
	}
}

//start
static int start(lua_State *L){
	if (0 != event_wait(mydispath))
		return 0;
	event_reset(mydispath);
	bool bmonitor = (bool)luaL_checkinteger(L, 1);
	lua_callback = luaL_ref(L, LUA_REGISTRYINDEX);
	int ret = thedlib->start(bmonitor);
	lua_pushinteger(L, ret );
	event_set(mydispath);
	return 1;
}

//stop
static int stop(lua_State *L){
	if (0 != event_wait(mydispath))
		return 0;
	event_reset(mydispath);
	thedlib->stop();
	event_set(mydispath);
	return 0;
}

static int addtracktarget(lua_State *L) {
	if (0 != event_wait(mydispath))
		return 0;
	event_reset(mydispath);
	size_t len = (size_t)luaL_checkinteger(L, 1);
	const char * buf = luaL_checklstring(L, 2, &len);
	int l = (int)luaL_checkinteger(L, 3);
	int t = (int)luaL_checkinteger(L, 4);
	int w = (int)luaL_checkinteger(L, 5);
	int h = (int)luaL_checkinteger(L, 6);
	thedlib->add_tracker_target((char*)buf, len, l, t, w, h);
	event_set(mydispath);
	return 0;
}

static int targetpositions(lua_State *L) {
	if (0 != event_wait(mydispath))
		return 0;
	event_reset(mydispath);
	size_t len = (size_t)luaL_checkinteger(L, 1);
	const char * buf = luaL_checklstring(L, 2, &len);
	std::vector<rect> boxs;
	int cnt = thedlib->get_tracking_targets((char*)buf, len, boxs);
	if( cnt > 0) {
		lua_pushlstring(L, (const char*)&boxs[0], sizeof(rect) * cnt);
		event_set(mydispath);
		return 1;
	}
	event_set(mydispath);
	return 0;
}

//mutil object detect
static int addobjectmodel(lua_State *L) {
	if (0 != event_wait(mydispath))
		return 0;
	event_reset(mydispath);
	const char * svm = luaL_checkstring(L, 1);
	int ret = (int)thedlib->add_detect_model(svm);
	lua_pushinteger(L, ret);
	event_set(mydispath);
	return 1;
}

static int objectpositions(lua_State *L) {
	if (0 != event_wait(mydispath))
		return 0;
	event_reset(mydispath);
	size_t len = (size_t)luaL_checkinteger(L, 1);
	const char * buf = luaL_checklstring(L, 2, &len);
	std::vector<int> classes;
	std::vector<rect> boxs;
	int cnt = thedlib->get_detecting_targets((char*)buf, len, classes, boxs);
	if( cnt > 0 ) {
		lua_pushlstring(L, (const char*)&classes[0], sizeof(int) * cnt);
		lua_pushlstring(L, (const char*)&boxs[0], sizeof(rect) * cnt);
		event_set(mydispath);
		return 2;
	}
	event_set(mydispath);
	return 0;
}

//face descriptors
static int setfacemodel(lua_State *L) {
	if (0 != event_wait(mydispath))
		return 0;
	event_reset(mydispath);
	const char * marks = luaL_checkstring(L, 1);
	const char * network = luaL_checkstring(L, 2);
	int ret = (int)thedlib->set_face_model(marks,network);
	lua_pushinteger(L, ret);
	event_set(mydispath);
	return 1;
}

static int markfeatures(lua_State *L) {
	if (0 != event_wait(mydispath))
		return 0;
	event_reset(mydispath);
	size_t len = (size_t)luaL_checkinteger(L, 1);
	const char * buf = luaL_checklstring(L, 2, &len);
	std::vector<rect> boxs;
	std::vector<feature> feats;
	int cnt = thedlib->get_face_features((char*)buf, len, boxs, feats);
	if( cnt > 0 ){
		lua_pushlstring(L, (const char*)&boxs[0], sizeof(rect) * cnt);
		lua_pushlstring(L, (const char*)&feats[0], sizeof(feature) * cnt);
		event_set(mydispath);
		return 2;
	}
	event_set(mydispath);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//模块销毁对象实例，删除相关数据
static int destory(lua_State *L){
	event_destroy(mydispath);
	return 0;
}

//外部接口导出定义
static const struct luaL_Reg mylib[]={
	{"destory",destory},
	{"start",start},
	{"stop",stop},
	{"addtracktarget",addtracktarget},
	{"targetpositions",targetpositions},
	{"addobjectmodel",addobjectmodel},
	{"objectpositions",objectpositions},
	{"setfacemodel",setfacemodel},
	{"markfeatures",markfeatures},
	{NULL,NULL}
};

#ifdef WIN32
	extern "C" __declspec(dllexport) int luaopen_dlib(lua_State *L){
#else
	extern "C" int luaopen_dlib(lua_State *L){
#endif
	printf("luaopen_dlib\n");
	if( !theState ){
		theState = L;
		mydispath = event_create(false, true);
		//在这创建对象实例
		thedlib = new dlibwraper();
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

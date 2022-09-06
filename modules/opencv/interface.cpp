#include "opencv.h"
extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}
copencv * theopencv = NULL;

//打开OPCV分类器
static int open(lua_State *L){
	const char * haar = luaL_checkstring(L,1);
	int min = (int)luaL_checkinteger(L,2);
	int max = (int)luaL_checkinteger(L,3);
	theopencv->open(haar,min,max);
	return 0;
}

////识别头像
//static int recognition(lua_State *L){
//	const char * haar = luaL_checkstring(L,1);
//	const char * file = luaL_checkstring(L,2);
//	std::vector<CvRect> v;
//	if( theopencv->recognition(haar,file,v) <= 0  )
//		return 0;
//	lua_newtable(L);
//	for( size_t i = 0; i<v.size(); i++ ){
//		char sztext[256]={0};
//		sprintf( sztext,"%d,%d,%d,%d",v[i].x,v[i].y,v[i].width,v[i].height);
//		lua_pushstring(L,sztext);
//		lua_rawseti(L,-2,i);
//	}
//	return 1;
//}

//文件对象检测
static int file_objcount(lua_State *L){
	const char * file = luaL_checkstring(L,1);
	std::vector<CvRect> v;
	if( theopencv->detect(file,v) <= 0)
		return 0;
	lua_newtable(L);
	for( size_t i = 0; i<v.size(); i++ ){
		char sztext[256]={0};
		sprintf( sztext,"%d,%d,%d,%d",v[i].x,v[i].y,v[i].width,v[i].height);
		lua_pushstring(L,sztext);
		lua_rawseti(L,-2,i);
	}
	return 1;
}

//圈定帧数据对象是否存在(用于人流统计)
static int stream_objcount(lua_State *L){
	const char* jpgdata = luaL_checkstring(L,1);
	const char* szphoto = luaL_checkstring(L,2);
	int ncnt = theopencv->detect((char*)jpgdata,(char*)szphoto);
	lua_pushinteger(L,ncnt);
	return 1;
}

//找出帧数据中对象坐标(用于深度学习)
static int stream_coordinate(lua_State *L){
	size_t  jpgsize = (size_t)luaL_checkinteger(L, 1);
	const char*   jpgdata = luaL_checklstring(L, 2, &jpgsize);
	const char*   szphoto = luaL_checkstring(L,3);
	std::vector<CvRect> v;
	if( theopencv->detect((char*)jpgdata,(char*)szphoto,v)<=0)
		return 0;
	lua_newtable(L);
	for( size_t i = 0; i < v.size(); i++ ){
		char sztext[256]={0};
		sprintf( sztext, "%d,%d,%d,%d", v[i].x,v[i].y,v[i].width,v[i].height );
		lua_pushstring(L,sztext);
		lua_rawseti(L,-2,i);
	}
	return 1;
}

//屏幕截屏
static int screenshot(lua_State *L) {
	const char* filepath = luaL_checkstring(L, 1);
	int ret = theopencv->screenshot((char*)filepath);
	lua_pushinteger(L, ret);
	return 1;
}

static int close(lua_State *L){
	theopencv->close();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int destory(lua_State *L){
	if( theopencv ){
		delete theopencv;
		theopencv = NULL;
	}
	return 0;
}

static const struct luaL_Reg mylib[]={
	{"destory",destory},
	{"open",open},
	{"file_objcount",file_objcount},
	{"stream_objcount",stream_objcount},
	{"stream_coordinate",stream_coordinate},
	{"close",close},
	{"screenshot",screenshot},
	{NULL,NULL}
};

#ifdef WIN32
	extern "C" __declspec(dllexport) int luaopen_opencv(lua_State *L){
#else
	extern "C" int luaopen_opencv(lua_State *L){
#endif
	printf("luaopen_opencv\n");
	if( theopencv ){
		theopencv = new copencv;
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


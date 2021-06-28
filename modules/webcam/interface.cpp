extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}
#include "./cammgr.h"
#include "./event.h"
#include <map>
#ifdef _WIN32
#include <process.h>
#endif

lua_State * theState = NULL;
ccammgr * theCameras = NULL;
pthread_t threadid = 0;
std::map<std::string, int> theWatchList;
static event_handle theExitEvt = NULL;
static int lua_callback = LUA_REFNIL;

static winapi watching(void* param){
	unsigned int * waittime  = (unsigned int*)param;
	while( 0 != event_timedwait(theExitEvt,*waittime)){
		for(std::map<std::string,int>::iterator it = theWatchList.begin(); it!= theWatchList.end();it++ ){
			if (it->second == 0)
				continue;
			std::string frm;
			if (--theWatchList[it->first] != 0 && theCameras->getfrm(it->first.c_str(), JPG, frm) > 100) {
				if (0 == luaL_lock(theState)) {
					lua_rawgeti(theState, LUA_REGISTRYINDEX, lua_callback);
					lua_pushstring(theState, &it->first[0]);
					lua_pushlstring(theState, &frm[0], frm.size());
					lua_pushinteger(theState, frm.size());
					lua_pcall(theState, 3, 0, 0);
					luaL_unlock(theState);
				}
			}
		}
	}
	event_set( theExitEvt );
	printf("watch pthread exit\n");
	return 0;
}

//添加摄像头
static int addcam(lua_State *L ){
	const char* mark  = luaL_checkstring(L,1);
	int         type  = (int)luaL_checkinteger(L,2);
	const char* url   = luaL_checkstring(L,3);
	printf("addcammer %s:%d,%s\n",mark,type,url);
	theCameras->add(mark,url,type);
	theCameras->play(mark,false);
	return 0;
}

//删除摄像头
static int delcam(lua_State *L ){
	const char* mark = luaL_checkstring(L,1);
	printf("del cam:%s\n",mark);
	theCameras->stop(mark);
	theCameras->del(mark);
	return 0;
}

//开启、关闭监视,无参数时退出线程
static int display(lua_State *L){
	static lua_Integer waittime = luaL_checkinteger(L,1);
	if( waittime >= 10 ){ //只有36FPS以上每帧才将被显示 1000/36
		lua_callback = luaL_ref(L, LUA_REGISTRYINDEX);
		theExitEvt = event_create( false, false);
#ifdef WIN32
		threadid  = _beginthreadex(NULL,0,watching,(void*)&waittime,0,NULL);
#else
		if( pthread_create(&threadid, NULL, watching, (void*)&waittime) != 0) 
			printf("pthread_create failed! \n");
#endif
		lua_pushinteger(L,0);
	}else{
		event_set(theExitEvt);
		event_timedwait(theExitEvt,100000);
		lua_pushinteger(L,1);
	}
	return 1;
}

//显示返馈(tms:持续时间)
static int monitor(lua_State *L){
	const char* mark = luaL_checkstring(L, 1);
	int         tms = (int)luaL_checkinteger(L, 2);
	if (tms != 0)  //没有做线程处理，暂时这样处理
		theWatchList[mark] = tms;
	else
		theWatchList.erase(mark); //没有
	lua_pushinteger(L, 0);
	return 1;
}

//拍照
static int takephoto( lua_State *L ){
	const char * id = luaL_checkstring(L,1);
	const char * fp = luaL_checkstring(L,2);
	std::string frm;
	FILE * pFile = NULL;
	if( theCameras->getfrm(id,JPG,frm)>100 && (pFile=fopen(fp,"wb"))){
		fwrite( &frm[0], frm.size(),1, pFile);
		fclose( pFile );
	}
	lua_pushlstring(L,&frm[0], frm.size());
	lua_pushinteger(L,frm.size());
	return 2;
}

//快照
static int snapshot( lua_State *L ){
	const char * id = luaL_checkstring(L,1);
	std::string frm;
	if (theCameras->getfrm(id, JPG, frm)) {
		lua_pushlstring(L, &frm[0], frm.size());
		lua_pushinteger(L, frm.size());
		return 2;
	}
	return 0;
}

//镜头移动
static int move(lua_State *L ){
	const char * id   = luaL_checkstring(L,1);
	int dir   = (int)luaL_checkinteger(L,2);
	int times = (int)luaL_checkinteger(L,3);
	theCameras->move( id, dir,times );
	return 0;
}

//存储视频
static int storage(lua_State *L ){
	const char * id   = luaL_checkstring(L,1);
	lua_Integer bsave  = luaL_checkinteger(L,2);
	theCameras->storage(id,(bsave!=0) );
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int destory(lua_State *L){
	event_set( theExitEvt );
	if( theCameras ){
		delete theCameras;
		theCameras = NULL;
	}
	return 0;
}

static const struct luaL_Reg mylib[]={
	{"destory",destory},
	{"addcam",addcam},
	{"delcam",delcam},
	{"display",display},
	{"monitor",monitor},
	{"snapshot",snapshot},
	{"takephoto",takephoto},
	{"move",move},
	{"storage",storage},
	{NULL,NULL}
};

#ifdef WIN32
	extern "C" __declspec(dllexport) int luaopen_webcam(lua_State *L){
#else
	extern "C" int luaopen_webcam(lua_State *L){
#endif
	printf("luaopen_webcam\n");
	if( !theState ){
		theState = L;
		theCameras = new ccammgr;
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

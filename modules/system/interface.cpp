extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

#include <vector>
#include <string>
#include "netcard.h"
#include "filesys.h"
#include "mainbroad.h"
#include "winos.h"
#include "monitor.h"

#include "base/at_exit.h"
#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/location.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/strings/string_number_conversions.h"
#include "base/task_runner.h"
#include "base/threading/thread.h"
#include "base/time/time.h"
#include "base/process/process.h"

lua_State * theState = NULL;
std::vector<std::string> gOutput;
cnetcard * theNetCard = NULL;
scoped_ptr<base::Thread> g_thread_;

//callback function must lock and unlock.
void monitor_callback(int cb, int event, const  char * name) {
	luaL_lock(theState);
	lua_rawgeti(theState, LUA_REGISTRYINDEX, cb);
	lua_pushinteger(theState, event);
	lua_pushstring(theState, name);
	lua_pcall(theState, 2, 0, 0);
	luaL_unlock(theState);
}

static char* xreplace( const char *  str ) {
	static char ret[1024] = { 0 };
	memset(ret, 0x00, 1024);
	int nlen = strlen(str);
	for (int j = 0; j < nlen && nlen < 1024 ; j++) {
		if (str[j] != '\'') {
			ret[j] = str[j];
		}
		else {
			ret[j] = ' ';
		}
	}
	return ret;
}

static char* yreplace(const char *  str) {
	static char ret[1024] = { 0 };
	memset(ret, 0x00, 1024);
	int nlen = strlen(str);
	for (int j = 0; j < nlen && nlen < 1024; j++) {
		if (str[j] != '\'') {
			ret[j] = str[j];
		}
		else {
			ret[j] = ' ';
		}
	}
	return ret;
}

//¼à¿ØUSBÉè±¸
static int monitorUsb(lua_State *L) {
	int cb = luaL_ref(L, LUA_REGISTRYINDEX);
	StartMonitor( cb );
	return 1;
}

 static int cmdExecute(lua_State * L) {
	const char *cmd = luaL_checkstring(L, 1);
	printf("lua cmd execute:%s\n", cmd);
	FILE* pf = NULL;
	std::string retstr;
	pf = _popen(cmd, "r");
	if (NULL != pf) {
		char buffer[1024] = { '\0' };
		while (fgets(buffer, 1024, pf) != NULL) {
			if (buffer[strlen(buffer) - 1] == '\n') {
				buffer[strlen(buffer) - 1] = '\0';
			}
			retstr += buffer;
		}
		_pclose(pf);
	}
	lua_pushstring(L, retstr.c_str());
	return 1;
}

static int getnetcards(lua_State * L){
	std::vector<netcardinfo> v;
	theNetCard->getnetcards( v );
	lua_newtable(L);
	lua_pushnumber(L,-1);
	lua_rawseti(L,-2,0);
	for( size_t i=0; i<v.size(); i++ ){
		char szinfo[1024] = { 0 };
		sprintf_s(szinfo, "%s;%s;%s;%s", v[i].name, v[i].ip, v[i].mask, v[i].gate);
		printf("cardinfo:%s\n",szinfo);
		lua_pushstring( L,szinfo );
		lua_rawseti(L,-2,i);
	}
	lua_pop(L,0);
	return 1;
}

static int setnetcardip(lua_State * L) {
	const char * name = luaL_checkstring(L, 1);
	const char * ip   = luaL_checkstring(L, 2);
	const char * mask = luaL_checkstring(L, 3);
	const char * gate = luaL_checkstring(L, 4);
	int nret = theNetCard->setip(name, ip, mask, gate);
	lua_pushinteger(L, nret);
	return 1;
}

static int Sleep(lua_State * L){
	unsigned int nsleep = (unsigned int)luaL_checkinteger(L,1);
#ifdef _WIN32
	Sleep( nsleep );
#else
	usleep( nsleep * 1000);
#endif
	return 0;
}

static int beep(lua_State * L){
	unsigned int nfreq   = (unsigned int)luaL_checkinteger(L,1);
	unsigned int ndelay = (unsigned int)luaL_checkinteger(L,2);
	buzzer( nfreq, ndelay );
	return 0;
}

static int osinfo( lua_State * L) {
	std::string osname;
	std::string osversion;
	getOsInfo(osname, osversion);
	lua_pushstring(L, osname.c_str());
	lua_pushstring(L, osversion.c_str());
	return 2;
}

 
static int cpuinfo(lua_State * L) {
	int freq;
	unsigned int guid;
	std::string mid;
	std::string type;
	getCpuInfo(freq, guid, mid, type);
	lua_pushinteger(L, freq);
	lua_pushinteger(L, guid);
	lua_pushstring(L, mid.c_str());
	lua_pushstring(L, type.c_str());
	return 4;
}

static int diskinfo(lua_State * L) {
	const char* path = luaL_checkstring(L, 1);
	unsigned int total;
	unsigned int nfree;
	getDiskInfo(path, total, nfree);
	lua_pushinteger(L, total);
	lua_pushinteger(L, nfree);
	return 2;
}

static int meminfo(lua_State * L) {
	int decimal_total;
	int decimal_avl;
	getMemoryInfo(decimal_total, decimal_avl);
	lua_pushnumber(L, decimal_total);
	lua_pushnumber(L, decimal_avl);
	return 2;
}

static int ListDirFile(lua_State * L){
	const char * dir = luaL_checkstring(L,1);
	vector<string> v;
	getFiles(dir, v);
	if( v.size() <=0 )
		return 0;
	lua_newtable(L);
	for( size_t i = 0; i<v.size(); i++){
		lua_pushstring(L,v[i].c_str());
		lua_rawseti(L,-2,i);
	}
	return 1;
}

static int ListUserInfo(lua_State * L) {
	std::vector<std::string> userList;
	GetAllUserInfo(userList);
	lua_newtable(L);
	for (size_t i = 0; i < userList.size(); i++) {
		lua_pushstring(L, userList[i].c_str());
		lua_rawseti(L, -2, i);
	}
	return 1;
}

static int RemoveUser(lua_State * L) {
	const char * userName = luaL_checkstring(L, 1);
	int ret = RemoveUser(userName);
	lua_pushinteger(L, ret);
	return 1;
}

static int netConnected(lua_State * L) {
	int ret = theNetCard->IsNetwork();
	lua_pushinteger(L, ret);
	return 1;
}

static int CreatePath(lua_State * L){
	const char * dir = luaL_checkstring(L,1);
	int nret = createpath( dir );
	lua_pushinteger(L,nret );
	return 1;
}

static int removefile(lua_State * L) {
	const char * file = luaL_checkstring(L, 1);
	int nret = remove( file );
	lua_pushinteger(L, nret);
	return 1;
}

static int authorize(lua_State * L) {
	const char * pkey = luaL_checkstring(L, 1);
	std::string val = getAuthorize((char*)pkey);
	lua_pushstring(L, val.c_str());
	return 1;
}

static int disableService(lua_State * L) {
	const char * serverName = luaL_checkstring(L, 1);
	bool ret = DisableService( serverName );
	lua_pushboolean( L, ret );
	return 1;
}

static int closeService(lua_State * L) {
	const char * serverName = luaL_checkstring(L, 1);
	bool ret = CloseService(serverName);
	lua_pushboolean(L, ret);
	return 1;
}

static int startService(lua_State * L) {
	const char * serverName = luaL_checkstring(L, 1);
	bool ret = StartService(serverName);
	lua_pushboolean(L, ret);
	return 1;
}

static int enumService(lua_State * L) {
	std::vector<serviceInfo> services;
	int  sizes = EnumService( services );
	lua_newtable(L);
	for( unsigned int i = 0; i<services.size(); i++ ){
		static char szbuf[1024] = { 0 };
		memset(szbuf, 0x00, 1024);
		int nlen =sprintf_s(szbuf, "{'name':'%s','display':'%s','status':%d}",
			xreplace( services[i].name.c_str() ),
			yreplace( services[i].display.c_str() ),
			services[i].status);
		lua_pushlstring(L, szbuf , nlen );
		lua_rawseti(L, -2, i+1);
	}
	return 1;
}

static int checkWinUser(lua_State * L) {
	const char * name = luaL_checkstring(L, 1);
	const char * pwd = luaL_checkstring(L, 2);
	bool ret = CheckWinUser( name, pwd );
	lua_pushboolean(L, ret);
	return 1;
}

static int mondUserPass(lua_State * L) {
	const char * name = luaL_checkstring(L, 1);
	const char * opwd = luaL_checkstring(L, 2);
	const char * npwd = luaL_checkstring(L, 3);
	bool ret = MondPass(name, opwd,npwd);
	lua_pushboolean(L, ret);
	return 1;
}

static int destory(lua_State * L){
	if( theNetCard ){
		delete theNetCard;
		theNetCard = NULL;
		g_thread_->Stop();
		g_thread_.reset();
	}
	return 0;
}

//callback function must lock and unlock.
void callback(int cb, int n) {
	luaL_lock(theState);
	lua_rawgeti(theState, LUA_REGISTRYINDEX, cb);
	lua_pushnumber(theState, n);
	lua_pcall(theState, 1, 0, 0);
	luaL_unref(theState, LUA_REGISTRYINDEX, cb);
	luaL_unlock(theState);
}

static int callback_test(lua_State *L) {
	int nx = (int)lua_tointeger(L, 1);
	int dl = (int)lua_tointeger(L, 2);
	int cb = luaL_ref(L, LUA_REGISTRYINDEX);
	//g_thread_->message_loop_proxy()->PostTask(FROM_HERE,base::Bind(&callback, cb, nx));
	g_thread_->message_loop_proxy()->PostDelayedTask(FROM_HERE,
		base::Bind(&callback, cb, nx),
		base::TimeDelta::FromMilliseconds(dl));
	return 0;
}

static const struct luaL_Reg myLib[]={
	{"destory",destory},
	{"cmdExecute",cmdExecute},
	{"getnetcards",getnetcards},
	{"setnetcardip",setnetcardip },
	{"ListDirFile",ListDirFile},
	{"CreatePath",CreatePath},
	{"osinfo",osinfo },
	{"cpuinfo",cpuinfo },
	{"meminfo",meminfo },
	{"diskinfo",diskinfo },
	{"authorize",authorize },
	{"removefile",removefile },
	{"Sleep",Sleep},
	{"beep",beep},

	{"disableService",disableService},
	{"closeService",closeService},
	{"startService",startService},
	{"enumService",enumService},
	{"netConnected",netConnected },
	{"callback_test",callback_test },
	{"ListUserInfo",ListUserInfo},
	{"RemoveUser",RemoveUser},
	{"monitorUsb",monitorUsb},
	{"checkWinUser", checkWinUser},
	{"mondUserPass",mondUserPass},
	{NULL,NULL}
};

#ifdef _WIN32
extern "C" __declspec(dllexport) int luaopen_system(lua_State *L){
#else
extern "C" int luaopen_system(lua_State *L){
#endif
//#ifndef DEBUG
//	if (-1 == authenticate()) {
//		printf("Unauthorized used,Please contact smartx.\n");
//		return 0;
//	}
//#endif
	printf("luaopen_system\n");
	if( !theNetCard ){
		theState = L;
		theNetCard = new cnetcard;
	}
	lua_newtable(L);
	const luaL_Reg * la = &myLib[0];
	for(;la->name!=NULL; la++){
		lua_pushcclosure(L,la->func,0);
		lua_setfield(L,-2,la->name);
	}
	lua_pop(L,0);
	base::AtExitManager exit_manager;
	base::Thread::Options options;
	options.message_loop_type = base::MessageLoop::TYPE_IO;
	g_thread_.reset(new base::Thread("Thread"));
	g_thread_->StartWithOptions(options);
	return 1;
}

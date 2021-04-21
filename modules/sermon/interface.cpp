extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}
#include "event.h"
#include "define.h"
#include "machine.h"
#include "./kernel/datamgr.h"
#include "./kernel/comm.h"
extern void getNetCardList();

machine   * theMachine = NULL;
lua_State * theState = NULL;
static int lua_callback = LUA_REFNIL;
comm      * theCommer = NULL;

void responsetolua( std::string name, task * tsk ){
	struct tm t;
	long long times = tsk->datastamp;
	time_t tsec = (times - BASETIME) / 10000000;
	int    tmsec = int((times - BASETIME) % 10000000);
	localtime_x(&t, &tsec);
	char strtime[256] = { 0 };
	sprintf(strtime, "%02d-%02d %02d:%02d:%02d.%03d",t.tm_mon+1,t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, tmsec / 10000);
	//luaL_glock(theState);
	lua_rawgeti(theState, LUA_REGISTRYINDEX, lua_callback);
	lua_pushstring(theState, name.c_str() );
	lua_pushinteger(theState, tsk->seq);
	lua_pushinteger(theState, tsk->mark );
	lua_pushinteger(theState, tsk->datadir);
	lua_pushstring(theState,  strtime);
	lua_pushlstring(theState, tsk->buffer, tsk->buflen);
	lua_pushinteger(theState, tsk->buflen);
	lua_pcall(theState, 7, 0, 0);
	//luaL_unglock(theState);
}
 
//����ͨ��
static int addtask(lua_State *L) {
	int argcnt = lua_gettop(L);
	if (argcnt < 2) {
		lua_pushinteger(L, -1);
		return 1;
	}
	task tsk;
	tsk.trigger = theMachine;
	const char * name = luaL_checkstring(L, 1);
	tsk.mark = luaL_checkinteger(L, 2);
	if (argcnt > 3) {
		size_t       len = luaL_checkinteger(L, 4);
		const char * data = luaL_checklstring(L, 3, &len);
		strcpy(tsk.buffer, data);
		tsk.buflen = len;
	}else{
		strcpy(tsk.buffer, luaL_checkstring(L,3));
	}
	if (argcnt > 4) {
		tsk.delay = luaL_checkinteger(L, 5);
	}
	theMachine->addtask(name, &tsk, false);
	lua_pushinteger( L, tsk.seq );
	return 1;
}

//����ͨ��
static int createcnn( lua_State *L ){
	const char * name = luaL_checkstring(L,1);
	const char * path = luaL_checkstring(L, 2);
	lua_Integer ht    = luaL_checkinteger(L,3);
	theMachine->createmodule(name,path,ht);
	return 0;
}

//ɾ��ͨ��
static int removechn(lua_State *L) {
	const char * name = luaL_checkstring(L, 1);
	theMachine->removemodule( name );
	return 0;
}

//ͨ����
static int openchn(lua_State *L) {
	const char * name = luaL_checkstring(L, 1);
	const char * args = luaL_checkstring(L, 2);
	task tsk;
	strcpy( tsk.buffer, name );
	tsk.mark = open_channel;
	strcpy(tsk.buffer, args);
	tsk.trigger = theMachine;
	theMachine->addtask(name, &tsk, false);
	return 0;
}

//ͨ���ر�
static int closechn(lua_State *L) {
	const char * name = luaL_checkstring(L, 1);
	task tsk;
	strcpy(tsk.buffer, name);
	tsk.mark = close_channel;
	tsk.trigger = theMachine;
	theMachine->addtask(name, &tsk, false);
	return 0;
}

static int setparams(lua_State *L) {
	const char * name = luaL_checkstring(L, 1);
	const char * args = luaL_checkstring(L, 2);
	task tsk;
	strcpy(tsk.buffer, args);
	tsk.mark = set_parameter;
	tsk.trigger = theMachine;
	theMachine->addtask(name, &tsk, false);
	printf("setparams name:%s args:%s\n", name, args);
	return 0;
}

static int senddata(lua_State *L) {
	const char * name = luaL_checkstring(L, 1);
	const char * data = luaL_checkstring(L, 2);
	lua_Integer   len = luaL_checkinteger(L, 3);
	task tsk;
	strcpy(tsk.buffer, data);
	tsk.buflen = len;
	tsk.mark = send_data;
	tsk.trigger = theMachine;
	theMachine->addtask(name, &tsk, false);
	return 0;
}

static int recvdata(lua_State *L) {
	const char * name = luaL_checkstring(L, 1);
	task tsk;
	tsk.mark = recv_data;
	tsk.trigger = theMachine;
	theMachine->addtask(name, &tsk, false);
	return 0;
}

static int request(lua_State *L) {
	int argcnt = lua_gettop(L);
	if (argcnt < 2) {
		lua_pushinteger(L, -1);
		return 1;
	}
	task tsk;
	tsk.trigger = theMachine;
	tsk.mark = fly_back;
	const char * name = luaL_checkstring(L, 1);
	if (argcnt > 2) {
		size_t       len = luaL_checkinteger(L, 3);
		const char * data = luaL_checklstring(L, 2, &len);
		strcpy(tsk.buffer, data);
		tsk.buflen = len;
	}
	else {
		strcpy(tsk.buffer, luaL_checkstring(L, 2));
	}
	if (argcnt > 3) {
		tsk.delay = luaL_checkinteger(L, 4);
	}
	theMachine->addtask(name, &tsk, false);
	lua_pushinteger(L, tsk.seq);
	return 1;
}

static int setdelay(lua_State *L) {
	const char * name = luaL_checkstring(L, 1);
	lua_Integer overtime = luaL_checkinteger(L, 2);
	xbase * dev = theMachine->getmodulebyname(name);
	dev->setdelay(overtime);
	return 0;
}

//��ȡ�豸״̬
static int getparams(lua_State *L) {
	const char * name = luaL_checkstring(L, 1);
	xbase * dev = theMachine->getmodulebyname(name);
	lua_pushinteger(L, (dev)?int(dev->mystatus) : -1);
	return 1;
}

static int getstatus(lua_State *L) {
	const char * name = luaL_checkstring(L, 1);
	xbase * dev = theMachine->getmodulebyname(name); 
	if (dev) {
		lua_pushinteger(L, dev->mystatus);
		lua_pushinteger(L, dev->getinports());
		lua_pushinteger(L, dev->getexports());
		return 3;
	}
	return 0;
}

//��ʷ����
static int printhistorydata(lua_State *L) {
	static char lastname[256] = { 0 };
	if (lua_gettop(L) == 2) {
		const char * name = luaL_checkstring(L, 1);
		theMachine->transhistorydata(name, luaL_checkstring(L, 2));
		strcpy(lastname, name);
	}else if (lua_gettop(L) == 1) {
		const char * name = luaL_checkstring(L, 1);
		theMachine->transhistorydata(name, NULL);
		strcpy(lastname, "");
	}else if ( strlen(lastname) ) {
		theMachine->transhistorydata(lastname, NULL);
		strcpy(lastname, "");
	}
	lua_pushinteger(L, 0);
	return 1;
}

//��ʷ����
static int stophistorydata(lua_State *L) {
	static char lastname[256] = { 0 };
	const char * name = luaL_checkstring(L, 1);
	strcpy(lastname, name);
	theMachine->transhistorydata(name,NULL);
	lua_pushinteger(L, 0);
	return 1;
}

//����machine
static int start(lua_State *L) {
	lua_Integer waittime = luaL_checkinteger(L, 1);
	lua_callback = luaL_ref(L, LUA_REGISTRYINDEX);
	theMachine->start();
	lua_pushinteger(L, 0);
	return 1;
}

//�ر�machine
static int stop(lua_State *L){
	theMachine->stop();
	return 0;
}


//ͨ����
static int comopen(lua_State *L) {
	const char * args = luaL_checkstring(L, 1);
	if (!theCommer)theCommer = new comm();
	int ret = theCommer->open( (char*)args );
	lua_pushinteger(L, ret);
	return 1;
}

static int comclose(lua_State *L) {
	if (theCommer) {
		int ret = theCommer->close();
		lua_pushinteger(L, ret);
		delete theCommer;
		theCommer = NULL;
		return 1;
	}
	return 0;
}

static int comrequest(lua_State *L) {
	if (theCommer) {
		const char * data = luaL_checkstring(L, 1);
		lua_Integer   len = luaL_checkinteger(L, 2);
		static char buf[1024] = { 0 };
		memset(buf, 0x00, 1024);
		theCommer->write((char*)data, len);
		int ret = theCommer->read(buf, 1024);
		lua_pushlstring(L, buf, ret);
		lua_pushinteger(L, ret);
		return 2;
	}
}

static int comwrite(lua_State *L) {
	if (theCommer) {
		const char * data = luaL_checkstring(L, 1);
		lua_Integer   len = luaL_checkinteger(L, 2);
		int ret = theCommer->write((char*)data, len);
		lua_pushinteger(L, ret);
		return 1;
	}
	return 0;
}

static int comread(lua_State *L) {
	if (theCommer) {
		char buf[1024] = { 0 };
		memset(buf, 0x00, 1024);
		int ret = theCommer->read(buf, 1024);
		printf("read:%s\n", buf);
		lua_pushlstring(L, buf, ret);
		lua_pushinteger(L, ret);
		return 2;
	}
	return 0;
}

// ping
static int ping(lua_State * L) {
	const char * ip = luaL_checkstring(L, 1);
	int timeout = luaL_checkinteger(L, 2);
	int bufsize = luaL_checkinteger(L, 3);
	int usedtime = 0;
	int nRet = theMachine->ping(ip, bufsize, timeout, usedtime);
	lua_pushinteger(L, nRet);
	lua_pushinteger(L, usedtime);
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
	{"createcnn",createcnn},
	{"removechn",removechn },
	{"openchn",openchn },
	{"closechn",closechn },
	{"setparams",setparams },
	{"getparams",getparams },
	{"getstatus",getstatus },
	{"senddata",senddata },
	{"recvdata",recvdata },
	{"request",request },
	{"addtask",addtask },
	{"printhistorydata",printhistorydata },
	{"stophistorydata",stophistorydata },
	{"setdelay",setdelay },
	{"start",start},
	{"stop",stop},
	{"comopen",comopen },
	{"comclose",comclose },
	{"comrequest",comrequest },
	{"comwrite",comwrite },
	{"comread",comread },
	{"ping",ping },
	{NULL,NULL}
};

#ifdef WIN32
	extern "C" __declspec(dllexport) int luaopen_sermon(lua_State *L){
#else
	extern "C" int luaopen_sermon(lua_State *L){
#endif
	printf("luaopen_sermon\n");
	if( !theState ){
		theState = L; 
		theMachine = new machine;
		getNetCardList();
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

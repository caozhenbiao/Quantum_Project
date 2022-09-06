#include "business.h"
#include "taskmgr.h"
#include <map>

extern "C" {
#include "./liblua/src/lua.h"
#include "./liblua/src/lauxlib.h"
#include "./liblua/src/lualib.h"
}

#ifdef _WIN32
#include <Mmsystem.h>
#pragma comment( lib,"winmm.lib" )
#else
#endif

static int luatrans = LUA_REFNIL;
static int luatimer = LUA_REFNIL;
static int luaclose = LUA_REFNIL;
cbusiness * business = NULL;
lua_State * theState = NULL;
std::map<std::string, lua_State*> luastates;

std::string luakey(const char* file) {
	std::string lp = file;
	int npos = lp.rfind("/");
	if (npos > 0) 
		return lp.substr(npos + 1);
	return lp;
}

extern "C" int install(lua_State* L) {
	theState = L;
	unsigned int n = (unsigned int)lua_tointeger(L, 1);
	luaclose = luaL_ref(L, LUA_REGISTRYINDEX); 
	luatimer = luaL_ref(L, LUA_REGISTRYINDEX);
	luatrans = luaL_ref(L, LUA_REGISTRYINDEX);
	if (business) {
		printf("settime:%d\n", n);
		business->settimer(n);
	}
	return 0;
}

extern "C" int transmit(lua_State* L) {
	size_t   size = (size_t)luaL_checkinteger(L, 1);
	const char* buf = luaL_checklstring(L, 2, &size);
	sockaddr_in  addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(1234);
	std::string data(buf,size);
	taskmgr::posttask(taskmgr::NET,
		base::Bind(&cbusiness::onluatrans, 
		base::Unretained(business), (sockaddr*)&addr, data));
	lua_pushnumber(L, 0);
	return 1;
}

static int panic(lua_State *L) {
	lua_writestringerror("PANIC: unprotected error in call to Lua API (%s)\n",lua_tostring(L, -1));
	return 0;  /* return to Lua to abort */
}

bool cbusiness::start(const char* file) {
	printf("start business service, script:%s\n", file);
	business = this;
	theState = luaL_newstate();
	luaL_openlibs(theState);
	if (luaL_loadfile(theState, file)) {
		if (theState) lua_atpanic(theState, &panic);  
		printf("script error or file:%s  error:%d mission!\n", file, lua_error(theState));
		lua_close(theState);
		theState = NULL;
		return false;
	}
	lua_register(theState, "install", install);
	lua_register(theState, "transmit", transmit);
	lua_pcall(theState, 0, 0, 0);
	luastates.insert(make_pair(luakey(file), theState));
	return true;
}

void cbusiness::stop() {
	if (!theState)	return;
	lua_rawgeti(theState, LUA_REGISTRYINDEX, luaclose);
	lua_pcall(theState, 0, 0, 0);
	luaL_unref(theState, LUA_REGISTRYINDEX, luaclose);
	luaL_unref(theState, LUA_REGISTRYINDEX, luatimer);
	luaL_unref(theState, LUA_REGISTRYINDEX, luatrans);
	lua_close(theState);
	theState = NULL;
}

void cbusiness::settimer(unsigned int n) {
	timerdelay = n;
	taskmgr::postdelayedtask(taskmgr::LUA, base::Bind(&cbusiness::dotimer,
		base::Unretained(this), base::TimeDelta::FromMilliseconds(n)),
		base::TimeDelta::FromMilliseconds(n));
}

//websvr execute with http request   file.func, data
std::string cbusiness::execute(std::string ifunc, std::string data) {
	std::string retval("{\"error\":-1}");
	int ipos = ifunc.find('.');
	if ( ipos < 0 && theState) {
		if (0 == luaL_lock(theState)) {
			lua_getglobal(theState, ifunc.c_str());
			lua_pushstring(theState, data.c_str());
			lua_pcall(theState, 1, 1, 0);
			retval = (char*)lua_tostring(theState, -1);
			luaL_unlock(theState);
		}
	}else {
		std::string file = ifunc.substr(0, ipos) + ".lua";
		ifunc = ifunc.substr(ipos+1);
		std::map<std::string, lua_State*>::iterator ifnd = luastates.find(file);
		if (ifnd != luastates.end()) {
			if (0 == luaL_lock(ifnd->second)) {
				lua_getglobal(ifnd->second, ifunc.c_str());
				lua_pushstring(ifnd->second, data.c_str());
				lua_pcall(ifnd->second, 1, 1, 0);
				retval = (char*)lua_tostring(ifnd->second, -1);
				luaL_unlock(ifnd->second);
			}
		}
		else {
			lua_State * lua = luaL_newstate();
			luaL_openlibs(lua);
			if (luaL_dofile(lua, file.c_str())) {
				printf("%s,script error or file mission!\n", file.c_str());
				lua_close(lua);
			}
			else {
				luastates.insert(std::make_pair(file, lua));
				if (0 == luaL_lock(lua)) {
					lua_getglobal(lua, ifunc.c_str());
					lua_pushstring(lua, data.c_str());
					lua_pcall(lua, 1, 1, 0);
					retval = (char*)lua_tostring(lua, -1);
					luaL_unlock(lua);
				}
			}
		}
	}
	return retval;
}

void cbusiness::dotimer( base::TimeDelta interval) {
	static unsigned int ntimes = 0;
	if (0 == luaL_lock(theState)) {
		lua_rawgeti(theState, LUA_REGISTRYINDEX, luatimer);
		lua_pushnumber(theState, ntimes++);
		lua_pcall(theState, 1, 0, 0);
		luaL_unlock(theState);
	}
	taskmgr::postdelayedtask(taskmgr::LUA, 
		base::Bind(&cbusiness::dotimer,base::Unretained(this),interval),
		interval);
}



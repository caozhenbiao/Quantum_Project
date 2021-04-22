#include <stdio.h>
#include "netcard.h"
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

extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

scoped_ptr<base::Thread> g_thread=NULL;
extern lua_State * theState;

void cnetcard::start() {
	base::Thread::Options options;
	options.message_loop_type = base::MessageLoop::TYPE_IO;
	g_thread.reset(new base::Thread("Project_Thread"));
	g_thread->StartWithOptions(options);
}

void cnetcard::stop() {
	g_thread->Stop();
	g_thread.reset();
}
 
int cnetcard::getinfo(char* info) {

	return 0;
}

int cnetcard::IsNetwork() {

	return 0;
}

int cnetcard::request(const char * data, int delay, int callback) {
	return g_thread->message_loop_proxy()->PostDelayedTask(FROM_HERE,
																base::Bind(&cnetcard::request_replay, base::Unretained(this), callback, data),
																base::TimeDelta::FromMilliseconds(delay));
}

int cnetcard::request(const char * data, int callback) {
	return g_thread->message_loop_proxy()->PostTask(FROM_HERE,
																base::Bind(&cnetcard::request_replay, base::Unretained(this), callback, data));
}

void cnetcard::request_replay( int callback, const char* data ) {
	int n = 0;
	if (0 == luaL_lock(theState)) {
		lua_rawgeti(theState, LUA_REGISTRYINDEX, callback);
		lua_pushnumber(theState, n);
		lua_pcall(theState, 1, 0, 0);
		luaL_unref(theState, LUA_REGISTRYINDEX, callback);
		luaL_unlock(theState);
	}
}
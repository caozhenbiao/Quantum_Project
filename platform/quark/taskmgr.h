#ifndef _TASKMGR_H_
#define _TASKMGR_H_

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

#ifdef _WIN32
#include <process.h>
#include <direct.h>
#include <io.h>
#define WIN32_LEAN_AND_MEAN  //这很重要，解决winsock.h winsock2.h冲突
#include <windows.h>
#define ACCESS _access   
#define MKDIR(a) _mkdir((a))   
#define mssleep(x) Sleep(x)
#define localtime_x(x,y) localtime_s(x,y)
#define close(s) closesocket(s)
#else
#include <unistd.h>
#include <stdarg.h>   
#include <sys/stat.h> 
#define ACCESS access   
#define MKDIR(a) mkdir((a),0755)   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define sprintf_s snprintf
#define __int64 long long
#define Sleep(x) usleep(x * 1000)
#include <sys/vfs.h>
#include <sys/time.h>
#include <mntent.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#define localtime_x(x,y) localtime_r(y,x)
#endif

class taskmgr:base::RunLoop {
public:
	enum ID {
		LUA,
		WEB,
		NET,
	  	ID_COUNT
	};
public:
	 static void start();
	 static void stop();
	 static void run(bool console = 0);
public:
	 static bool posttask(ID identifier, const base::Closure& task);
	 static bool postdelayedtask(ID identifier, const base::Closure& task, base::TimeDelta delay);
	 static bool posttaskreplay(ID identifier, const base::Closure& task, const base::Closure& reply);
};
#endif

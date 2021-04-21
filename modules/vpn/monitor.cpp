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

extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

#include <Iphlpapi.h>
#pragma comment ( lib, "iphlpapi.lib")

scoped_ptr<base::Thread> g_thread = NULL;

void cmonitor::start(){
	base::Thread::Options options;
	options.message_loop_type = base::MessageLoop::TYPE_IO;
	g_thread.reset(new base::Thread("ipsec_Thread"));
	g_thread->StartWithOptions(options);
}

void cmonitor::stop() {
	g_thread->Stop();
	g_thread.reset();
}

int cmonitor::speed(int delay, int callback) {
	return g_thread->message_loop_proxy()->PostDelayedTask(FROM_HERE,
		base::Bind(&cmonitor::supeedupdate, base::Unretained(this), callback),
		base::TimeDelta::FromMilliseconds(delay));
}

void cmonitor::supeedupdate( int callback ) {
	MIB_IFTABLE* m_pIfTable = nullptr;
	DWORD m_dwSize = sizeof(MIB_IFTABLE);
	m_pIfTable = (MIB_IFTABLE *)malloc(m_dwSize);
	int rtn;
	rtn = GetIfTable(m_pIfTable, &m_dwSize, FALSE);
	if (rtn == ERROR_INSUFFICIENT_BUFFER)	//如果函数返回值为ERROR_INSUFFICIENT_BUFFER，说明m_pIfTable的大小不够
	{
		free(m_pIfTable);
		m_pIfTable = (MIB_IFTABLE *)malloc(m_dwSize);	//用新的大小重新开辟一块内存
	}
	//获取当前所有的连接，并保存到m_connections容器中
	unsigned short dwin = 0;
	unsigned short dwou = 0;
	GetIfTable(m_pIfTable, &m_dwSize, FALSE);
	for (int i = 0; i < m_pIfTable->dwNumEntries; i++) {
		if (strcmp((char*)m_pIfTable->table[i].bDescr, "_vpnName") == 0) {
			dwin = m_pIfTable->table[i].dwInOctets;
			dwou = m_pIfTable->table[i].dwOutOctets;
			break;
		}
	}
}




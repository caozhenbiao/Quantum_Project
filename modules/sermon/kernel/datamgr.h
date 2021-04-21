#ifndef _DATAMGR_H_
#define _DATAMGR_H_
#include "taskdef.h"
#include "../define.h"
#include "../event.h"
#include <vector>

#include "../common/serial.h"


//数据信息记录统计
class datamgr {
public:
    datamgr();
	virtual ~datamgr() {};
	void setpath(const char* path);
public:
	void saveframe(int dir, char* data, int len);
	void requestHistoryData(task * tsk);
	void stopGetHistoryData();
	int  getinports();
	int  getexports();
 
protected:
	std::vector<Rec>  rcitem;
	char          mypath[256];     //数据存储
	unsigned int  inports;         //输入统计( read )
	unsigned int  exports;         //输出统计( write )
	time_t        lastcommtime;

private:
	static winapi historythread( void * arg );
	pthread		  threadid;
	bool          threadExit;
	task          historyTask;
};

#endif

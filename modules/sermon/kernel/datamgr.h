#ifndef _DATAMGR_H_
#define _DATAMGR_H_
#include "taskdef.h"
#include "../define.h"
#include "../event.h"
#include <vector>

#include "../common/serial.h"


//������Ϣ��¼ͳ��
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
	char          mypath[256];     //���ݴ洢
	unsigned int  inports;         //����ͳ��( read )
	unsigned int  exports;         //���ͳ��( write )
	time_t        lastcommtime;

private:
	static winapi historythread( void * arg );
	pthread		  threadid;
	bool          threadExit;
	task          historyTask;
};

#endif

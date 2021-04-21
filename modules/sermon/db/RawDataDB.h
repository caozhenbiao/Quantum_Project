#ifndef _CRAWDATADB_H_
#define _CRAWDATADB_H_
#include<stdio.h>
#include"CSQLite.h"
#include"sqlite3.h"
#include<vector>
#include <string.h>
#include <string>
#include<time.h>
#ifndef WIN32
#include <sys/time.h>
typedef unsigned long long ULONGLONG;
#endif
 
#define TIMEOUT	  1
#define FRAMELEN  1024 * 5
#define ULONGLONG_ unsigned long long 

typedef struct{
	int dir;
	long long timeScale;
	int size;
	char buf[FRAMELEN];
}Rec;

#define MAXRECORDMEMERY	100
typedef enum EFrameDirType 
{
	e_FrameAll,e_FrameIn,e_FrameOut
} EFDT;
class CRawDataDB : public CSQLiteDB
{
public:
	CRawDataDB(void);
	virtual ~CRawDataDB(void);
	void Close();
	bool Save(std::vector<Rec>* pdq);
	bool SetDir(const char *szDir);
protected:
	long long GetLastFT(void);
	int  CheckTime(long lTime, int nDays=1);//检查当前记录是否属于当前文件
	bool Open(const std::string& DBName);
	int  Open(long lTime); // -1:error, 0: opened 1: open new file
	bool Add(CSQLiteStatement& stmt,const Rec& pDN);
	std::string m_szDir;//文件路径
	bool m_bOpen;		//数据库是否打开标志
	long m_lMinT;		//文件名转换后最小时间，如2014-09-12 00:00:00
	long long m_llLastT;//当前记录最后一条记录时间
	std::string m_strCurFileName;//当前文件名
};
#endif

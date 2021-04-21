#ifndef CHANNELLOG_H
#define CHANNELLOG_H
#include <stdio.h>
#include <stdarg.h>
#include <string>
#ifndef LOGUDPPORT
#define LOGUDPPORT	989
#endif
#define MAXDATASIZE 10240
extern bool m_bDebug;

#define LOG CLogger::createInstance()->Log

enum eLogType 
{
	eHardError = 1,
	eSoftError = 2,
	eParameterError = 4,
	eWriteDBError = 8,
	eBusinessError = 16,
	eTipBusiness = 32,
	eTipMessage = 64
};

class CLogger
{
public:
	CLogger(){}
	virtual ~CLogger(){}
public:
	void setParam(const char* name,char* path, int level);
	void Log(const eLogType type, const char* lpszFormat, ...);
	static CLogger* createInstance( void ){
		if (m_pLogInstance == NULL){
			m_pLogInstance = new CLogger();
			return m_pLogInstance;
		}
		else
			return m_pLogInstance;
	}
protected:
	static CLogger* m_pLogInstance;
};
#endif

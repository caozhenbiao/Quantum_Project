#include "RawDataDB.h"
#include "../kernel/Log.h"
#ifndef WIN32
class CFileTime
{
public:
static const ULONGLONG Millisecond = 10000;
static const ULONGLONG Second = Millisecond * static_cast<ULONGLONG>(1000);
static const ULONGLONG Minute = Second * static_cast<ULONGLONG>(60);
static const ULONGLONG Hour = Minute * static_cast<ULONGLONG>(60);
static const ULONGLONG Day = Hour * static_cast<ULONGLONG>(24);
static const ULONGLONG Week = Day * static_cast<ULONGLONG>(7);
};
#include <unistd.h>
#define	sprintf_s(buffer, buffer_size, ...) (sprintf(buffer, ##__VA_ARGS__))
#endif

//将数据变为秒输出
long long CTimeVal(long long llFT)
{
	long long ft;
	ft = llFT - 116444736000000000ll;
	ft /= 10;
	return  long (ft / 1000000L);	
}


CRawDataDB::CRawDataDB(void)
{
	m_bOpen = false;
	m_szDir = "";
}

CRawDataDB::~CRawDataDB(void)
{
	if( m_bOpen )
		Close();
}
void CRawDataDB::Close()
{
	m_bOpen = false;
	close();
}

int CRawDataDB::CheckTime(long lTime, int nDays)
{
	if(!m_bOpen)			//Not open
		return -2;
	if(lTime < m_lMinT)	    //early
		return -1;
	if(lTime >= (m_lMinT + 86400 * nDays)) //下一天的文件
		return 1;
	else				   //否则，表示当天的文件
		return 0;
}

//打开数据库文件
int CRawDataDB::Open(long lTime) // 0: within current file   1: new open   -?: error
{
	char szDate[20];
	struct tm tm1, *ptm=&tm1;
	std::string szFileName;
	long lTZ;
	if(m_bOpen && (lTime >= m_lMinT ) && (lTime < m_lMinT + 86400))  //within current file
	{
		return 0;
	}
	if(m_bOpen)  //opened
	{
	}
	Close();
#ifdef WIN32
	_get_timezone(&lTZ);
	_localtime32_s(ptm,&lTime);
#else
	lTZ = timezone;
	localtime_r(&lTime, ptm);
#endif
	long lLocalTime = lTime - lTZ;
	m_lMinT = lLocalTime / 86400;
	m_lMinT = m_lMinT * 86400 + lTZ;
	sprintf_s(szDate,20,"%04d-%02d-%02d", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday);
	szFileName += m_szDir; 
	szFileName += szDate;
	szFileName += "_rawdata.db";
	if(Open(szFileName)){
		m_llLastT = GetLastFT();
		return 1;
	}
	else
		return -1;
}

bool CRawDataDB::Open(const std::string& DBName)
{
	if (m_bOpen) 
	{
		if(strcmp(m_strCurFileName.c_str(), DBName.c_str())==0)
			return m_bOpen;
	}
	m_strCurFileName = DBName;
	try
	{
		m_bOpen = open(m_strCurFileName.c_str());		
		if (!m_bOpen)
		{
			LOG(eWriteDBError, "create or open database %s error", DBName.c_str());
			return m_bOpen;
		}
		std::string sql;
		execDML("begin transaction;");
		if(!tableExists("RD"))
		{
			sql = "CREATE TABLE RD(FT Integer NOT NULL PRIMARY KEY,Dir Integer NOT NULL, Data BLOB NOT NULL)";
			execDML(sql.c_str());
		}
		if(!tableExists("RDTEMP"))
		{
			sql = "CREATE TABLE RDTEMP(FT Integer NOT NULL PRIMARY KEY,Dir Integer NOT NULL, Data BLOB NOT NULL)";
			execDML(sql.c_str());
		}
		if(!tableExists("COMMSTATUS"))
		{
			sql = "CREATE TABLE COMMSTATUS(Time Integer NOT NULL,InErrByteCount Integer NOT NULL,InByteCount Integer NOT NULL, OutErrByteCount Integer NOT NULL, OutByteCount Integer NOT NULL, status Integer NOT NULL)";
			execDML(sql.c_str());
		}
		execDML("commit transaction;");;
	}
	catch (CSQLiteException& e) 
	{
		LOG(eWriteDBError, "open database:%s",e.errorCodeAsString(e.errorCode()));
	}
	catch( ... )
	{
		LOG(eWriteDBError, "打开数据库异常,请检查是否有读写权限");
	}
	return m_bOpen;
}

bool CRawDataDB::Add(CSQLiteStatement& stmt,const Rec& pDN)
{
	bool bRet = false;
	if(pDN.size)
	{
		__int64 iFT = pDN.timeScale;
		if(iFT<=m_llLastT)
			iFT = m_llLastT + 10;
		m_llLastT = iFT;
		stmt.bind(1, iFT);
		stmt.bind(2, pDN.dir);
		stmt.bind(3,(unsigned char*)pDN.buf,pDN.size);
		if (stmt.execDML()!=1)
			bRet = false;
		else
			bRet = true;
	}
	else
	{
		LOG(eTipMessage, "Try to save 0 Size Data... ");
		bRet = true;
	}
	return bRet;
}

long long CRawDataDB::GetLastFT(void)
{
	long long llLast = 0;
	if(m_bOpen){
		CSQLiteQuery query = execQuery("select FT from RD order by rowid DESC limit 1");
		if(!query.eof())
			llLast = query.getInt64Field(0);
	}
	return llLast;
}

//注意:当运行此save时网路中断的情况,最好把tempRD放本地
bool CRawDataDB::Save(std::vector<Rec>* pdq)
{
	bool bRet = false;
	try {
		CSQLiteStatement stmt;
		for(std::vector<Rec>::const_iterator it = pdq->begin(); it != pdq->end();it++)
		{		
			int nRet = CheckTime(CTimeVal(it->timeScale));
			if(nRet != 0) //此记录不属于当天文件
			{
				if(nRet >= -1 ){                 //opened
					//stmt.reset();
					execDML("INSERT INTO RD SELECT * FROM RDTEMP");
					execDML("DELETE FROM RDTEMP");
					execDML("COMMIT TRANSACTION");	
					stmt.finalize();
					Close();
				}
				Open(CTimeVal(it->timeScale));  //open a new file
				execDML("BEGIN TRANSACTION");
				stmt=compileStatement("INSERT INTO RDTEMP(FT,Dir,Data) VALUES (?,?,?)");
			}
			if(!Add(stmt,*it))
			{
			}
		}
		execDML("INSERT INTO RD SELECT * FROM RDTEMP");
		execDML("DELETE FROM RDTEMP");
		execDML("COMMIT TRANSACTION");
		stmt.finalize();
		bRet = true;
	} 
	catch (CSQLiteException& e){
		LOG(eWriteDBError,"save channel db:%s",e.errorCodeAsString(e.errorCode()));
		execDML("ROLLBACK TRANSACTION");
	}
	return bRet;
}

bool CRawDataDB::SetDir(const char *szDirectory)
{
	m_szDir = szDirectory;
	return true;
}

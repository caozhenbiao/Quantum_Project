#ifndef _MYSERIALPORT_H_
#define _MYSERIALPORT_H_
#include "ctb-0.16/serport.h"
#include <time.h>
#include <string>
#include <string.h>
#include <vector>
#include <queue>
#include <map>

#ifdef WIN32
#include <io.h>
#include <direct.h>
#include "..\db\CSQLite.h"
//#include "..\db\public.h"
#include "..\db\RawDataDB.h"
#else
#include <sys/io.h>
#include "../db/CSQLite.h"
//#include "../db/public.h"
#include "../db/RawDataDB.h"
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
typedef unsigned char BYTE;
#endif

#ifndef WIN32
#define	_snprintf snprintf
#define sprintf_s snprintf
#define sscanf_s sscanf
typedef unsigned char BYTE;
#else
#define	mkdir(DirName,   mode) (mkdir(DirName))
#endif

//#define TIMEOUT	  1
//#define FRAMELEN  1024
//#define ULONGLONG_ unsigned long long 

struct serial_icounter_structx {
	int cts;;
	int rx;
	int frame;
	int buf_overrun;
	int reserved[9];
};


//struct serial_icounter_structx {
//	int cts, dsr, rng, dcd;
//	int rx, tx;
//	int frame, overrun, parity, brk;
//	int buf_overrun;
//	int reserved[9];
//};


struct DataHeader{
	int iTimeSecond;
	int iTimeSS;
	int iPLength;
	int iLength;
};

struct DataItem{
	DataHeader header;
	int iTimeSecond;
	int iTimeSS;
	int direct;
	::std::string src;
	::std::string dest;
	::std::string data;
};

//ctb ·â×°
class CMySerial : public ctb::SerialPort{
public:
	CMySerial();
	~CMySerial();
public:
	int setserialparam(char * param, int len);
	char * getserialparam();
	int getlasterror();
};

int gethisdata(char * datapath, char * starttime, char *endtime, std::vector< Rec >&rawdatals);
int gethisdata(char * savepath, char * datapath, char *starttime, long long interval);
long long getTimeScale();
bool strDivide(  std::vector< std::string> &_strlist,const  std::string src,const  std::string div);
time_t changeStrToTime(const  std::string time_str,const  std::string format = "%d-%d-%d %d:%d:%d");
const  std::string Byte2HexStr(const unsigned char* input, const int datasize);
int   CreateDir(const   char   *sPathName);
bool fileSearch(std::string dir,std::string starttime ,std::string endtime ,std::vector<std::string>& list,std::string ext);

#endif


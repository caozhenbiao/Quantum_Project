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
#else
#include <sys/io.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/time.h>
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


#define MAXRECVSIZE 65535

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

//ctb ��װ
class CMySerial : public ctb::SerialPort{
public:
	CMySerial();
	~CMySerial();
public:
	int setserialparam(char * param, int len);
	char * getserialparam();
	int getlasterror();
};

long long getTimeScale();
bool strDivide(  std::vector< std::string> &_strlist,const  std::string src,const  std::string div);
time_t changeStrToTime(const  std::string time_str,const  std::string format = "%d-%d-%d %d:%d:%d");
const  std::string Byte2HexStr(const unsigned char* input, const int datasize);
bool fileSearch(std::string dir,std::string starttime ,std::string endtime ,std::vector<std::string>& list,std::string ext);

#endif


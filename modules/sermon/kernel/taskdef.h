#ifndef _TASK_H_
#define _TASK_H_
#include <memory.h>
class xbase;
#define DATAMAXSIZE 1024*5        //数据报文最大长度
#define MAXRECVSIZE DATAMAXSIZE/3 //接收数据最大长度

#ifdef WIN32
#define BASETIME 116444736000000000L
#else
#define BASETIME 116444736000000000ll
#endif

//日志类别
enum trace_level{
	debug     = 1,
    command   = 2
};

enum data_dir {
	dir_unknow = 0,
	dir_recv = 1,
	dir_send = 2
};

//任务标识
enum task_mark{
	open_channel     = 1,
	close_channel    = 2,
	set_parameter    = 4,
	send_data        = 8,
	recv_data        = 16,
	fly_back         = 32,
	invoke_data      = 64,
	read_histdata    = 128
};

//模块类别
enum hardtype{
	SSP = 0,   //单通道串口
	DSP = 1,   //双通道串口
	CSP = 2,   //关连串口  
	TCPS = 3,
	TCPC = 7,
	UDP = 4,
	GPIO = 5,
	XSP = 6,
	SNP = 7,  //网口监听(PCAP)
};

//模块状态
enum status{
	REMOVE   = -1,    //处于被删除状态
	INITIAL  = 0,
	NORMAL   = 1,   //正常状态
	ABNORMAL = 2  //异常状态
};

static int g_task_seq = 0;
//任务定义
struct task{
	task(){memset(this,0x00,sizeof(task));seq = ++g_task_seq;}
	task(task* tsk){memcpy(this,tsk,sizeof(task));}
	int          mark;                  //任务标识
	xbase *      trigger;               //任务发起模块
	char         buffer[DATAMAXSIZE];   //任务数据/参数
	int          buflen;                //数据/参数长度
	char         callback[20];          //回调函数标识,现在主要LUA使用
	long long    datastamp;             //数据时标
	int          datadir;               //数据方向
	int          retcode;               //任务执行返回值
	bool         direct;                //完成后重定向到machine
	int          delay;                 //延迟时间
	int          seq;
};

//全局应用函数
int  getdevicename(char * name, int len);
int  gettracepath( char* path, int len);
int  getkeeptimes();
int  gettracelevel();
char hex2asc(char* shex);
int  bin2hex(unsigned char *src, int slen, char * dst, int dlen);

long long get_time_scale();

#ifndef WIN32
int GetPrivateProfileString(const char *AppName, const char *key, const char *defaultvalue, char *lpReturnedString, const int nSize, const char *lpFileName);
int GetPrivateProfileInt(const char *AppName, const char *key, int defaultvalue, const char *lpFileName);
#endif 




#endif 

#ifndef _TASK_H_
#define _TASK_H_
#include <memory.h>
class xbase;
#define DATAMAXSIZE 1024*5        //���ݱ�����󳤶�
#define MAXRECVSIZE DATAMAXSIZE/3 //����������󳤶�

#ifdef WIN32
#define BASETIME 116444736000000000L
#else
#define BASETIME 116444736000000000ll
#endif

//��־���
enum trace_level{
	debug     = 1,
    command   = 2
};

enum data_dir {
	dir_unknow = 0,
	dir_recv = 1,
	dir_send = 2
};

//�����ʶ
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

//ģ�����
enum hardtype{
	SSP = 0,   //��ͨ������
	DSP = 1,   //˫ͨ������
	CSP = 2,   //��������  
	TCPS = 3,
	TCPC = 7,
	UDP = 4,
	GPIO = 5,
	XSP = 6,
	SNP = 7,  //���ڼ���(PCAP)
};

//ģ��״̬
enum status{
	REMOVE   = -1,    //���ڱ�ɾ��״̬
	INITIAL  = 0,
	NORMAL   = 1,   //����״̬
	ABNORMAL = 2  //�쳣״̬
};

static int g_task_seq = 0;
//������
struct task{
	task(){memset(this,0x00,sizeof(task));seq = ++g_task_seq;}
	task(task* tsk){memcpy(this,tsk,sizeof(task));}
	int          mark;                  //�����ʶ
	xbase *      trigger;               //������ģ��
	char         buffer[DATAMAXSIZE];   //��������/����
	int          buflen;                //����/��������
	char         callback[20];          //�ص�������ʶ,������ҪLUAʹ��
	long long    datastamp;             //����ʱ��
	int          datadir;               //���ݷ���
	int          retcode;               //����ִ�з���ֵ
	bool         direct;                //��ɺ��ض���machine
	int          delay;                 //�ӳ�ʱ��
	int          seq;
};

//ȫ��Ӧ�ú���
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

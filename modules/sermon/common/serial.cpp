 
#include "serial.h"
#include "../define.h"
#include <map>

#ifndef WIN32
#include <sys/ioctl.h>
#endif
typedef std::vector<std::string> SeqList;

std::map<char,BYTE> serPARITY;
std::map<char,ctb::Parity> ctbPARITY;

CMySerial::CMySerial(){
	serPARITY['N'] = 0;//NOPARITY;
	serPARITY['O'] = 1;//ODDPARITY;
	serPARITY['E'] = 2;//EVENPARITY;
	serPARITY['M'] = 3;//MARKPARITY;
	serPARITY['S'] = 4;//SPACEPARITY;

	ctbPARITY['N'] = ctb::ParityNone;
	ctbPARITY['O'] = ctb::ParityOdd;
	ctbPARITY['E'] = ctb::ParityEven;
	ctbPARITY['M'] = ctb::ParityMark;
	ctbPARITY['S'] = ctb::ParitySpace;
}
CMySerial::~CMySerial()
{

}

#ifdef WIN32
int CMySerial::setserialparam(char * param, int len)
{
	std::vector< std::string> vs;
	std::string sparam = param;
	strDivide(vs,sparam," ");
	char cparam[100] = {0};
	sprintf(cparam,"%s",vs.at(0).c_str());

	DCB dcb;
	if( ! GetCommState( fd, &dcb ) ) {
		printf("GetCommState error! \n");
		return -1;
	}	

	char cdatabits,cparity,cstopbits;
	char szbaundrate[100] = {0};
	if( 4 != sscanf(param,"%[^,],%c%c%c",szbaundrate,&cdatabits,&cparity,&cstopbits ))
		return -1;

	//要求设置波特率
	if( strcmp(szbaundrate,"-") != 0 ){
		DWORD  nbaudrate = 0;
		nbaudrate = atoi( szbaundrate );
		if( -1 == SetBaudrate(nbaudrate)){
			printf("nbaudrate error! %d \n",nbaudrate);
			return -1;
		}
		dcb.BaudRate   = nbaudrate;
	}

	//其它参数修改
	bool bfix = false;
	if( cdatabits != '-' ){
		if( cdatabits < '5' || cdatabits > '8')
			return -1;
		char sztmp[]={cdatabits,0};
		dcb.ByteSize   = atoi(sztmp);
		m_dcs.wordlen  = dcb.ByteSize;
		bfix = true;
	}
	if( cparity != '-'){
		if( cparity != 'N' && cparity!='O' && cparity!='E' && cparity!='M' && cparity != 'S')
			return -1;
		dcb.Parity     = serPARITY[cparity];
		m_dcs.parity   = ctbPARITY[cparity];
		bfix = true;
	}
	//停止位
	if( cstopbits != '-'){
		if( cstopbits =='1'){
			dcb.StopBits = ONESTOPBIT;
			m_dcs.stopbits = ONESTOPBIT;
			bfix = true;
		}
		else if( cstopbits =='2'){
			dcb.StopBits = TWOSTOPBITS;
			m_dcs.stopbits = TWOSTOPBITS;
			bfix = true;
		}
		else
			return -1;
	}
	if(bfix && !SetCommState( fd, &dcb ) ){ 
		printf("SetCommState error! \n");
		return -1;
	}
	printf( "getserialparam:%s\n", getserialparam() );
	return 0;
}
#else
int CMySerial::setserialparam(char * param, int len)
{
	char cdatabits,cparity, cstopbits;
	char szbaundrate[100] = {0};
	if( 4 != sscanf(param,"%[^,],%c%c%c",szbaundrate,&cdatabits,&cparity,&cstopbits ))
		return -1;

	//要求设置波特率
	if( strcmp(szbaundrate,"-") != 0 ){
		unsigned int nbaudrate = atoi( szbaundrate );
		if( -1 == SetBaudrate(nbaudrate)){
			printf("nbaudrate error! %d \n",nbaudrate);
			return -1;
		}
	}

	tcdrain( fd );
	tcgetattr( fd, &t );
	switch(cdatabits){
	case 5:
		t.c_cflag |= CS5;
		m_dcs.wordlen = 5;
		break;
	case 6:
		t.c_cflag |= CS6;
		m_dcs.wordlen = 6;
		break;
	case 7:
		t.c_cflag |= CS7;
		m_dcs.wordlen = 7;
		break;
	case 8:
		t.c_cflag |= CS8;
		m_dcs.wordlen = 8;
		break;
	default:
		break;     
	}

	if('N' == cparity){
		t.c_cflag &= ~PARENB;
		m_dcs.parity = ctb::ParityNone;
	}
	else if('O' == cparity){
		t.c_cflag |= PARENB | CMSPAR | PARODD;
		m_dcs.parity = ctb::ParityOdd;
	}
	else if('E' == cparity){
		t.c_cflag |= PARENB | CMSPAR;
		t.c_cflag &= ~PARODD;
		m_dcs.parity = ctb::ParityEven;
	}

	if('1' ==  cstopbits){
		t.c_cflag &= ~CSTOPB;
		m_dcs.stopbits = 1;
	}
	else if('2' ==  cstopbits){
		t.c_cflag |= CSTOPB;
		m_dcs.stopbits = 2;
	}

	tcsetattr( fd,TCSANOW, &t );
	tcgetattr( fd, &t );
	return 0;
}
#endif

char *  CMySerial::getserialparam()
{
	static char buf[25];
	const char ac[5] = {'N','O','E','M','S'};
	memset(buf,0,sizeof(buf));
	sprintf_s(buf,sizeof(buf)-1,"%d,%i%c%i",
		m_dcs.baud,
		m_dcs.wordlen,
		ac[m_dcs.parity],
		m_dcs.stopbits);
	return buf;
}

long long getTimeScale()
{
	static long long time;
#ifdef WIN32
	SYSTEMTIME t;
	struct tm tm1;
	GetLocalTime(&t);
	tm1.tm_year = t.wYear - 1900;
	tm1.tm_mon  = t.wMonth - 1;
	tm1.tm_mday = t.wDay;
	tm1.tm_hour = t.wHour;
	tm1.tm_min  = t.wMinute;
	tm1.tm_sec  = t.wSecond;
	tm1.tm_isdst = -1;  
	time =  mktime(&tm1) * 10000000  + t.wMilliseconds * 10000 + 116444736000000000L;
#else
	struct	timeval    m_tv;
	gettimeofday(&m_tv,NULL);
	time = m_tv.tv_sec;
	time *= 10000000;
	time += (m_tv.tv_usec*10);
	time += 116444736000000000ll;
#endif
	return time;
}

bool strDivide(  std::vector< std::string> &_strlist,const  std::string src,const  std::string div)
{
	std::string _src = src;
	std::string::size_type _pos = _src.find(div);
	while( std::string::npos != _pos)
	{
		std::string _buf = "";
		_buf = _src.substr(0,_pos);
		_strlist.push_back(_buf);
		_src = _src.erase(0,_pos+div.size());
		_pos = _src.find(div.c_str());
	}
	if(!_src.empty()){
		_strlist.push_back(_src);
	}
	return true;
};


std::string rpc2TimeFm(std::string filename)
{
	std::string str = "rawdata.db";
	std::string times;
	int i=filename.find(str);
	if(i!=-1)
	{
		times=filename.substr(0,i-1);
		times+="-00:00:00";
	}
	return times;
}

time_t changeStrToTime(const  std::string time_str,const  std::string format)
{
	struct tm tm1;     
	sscanf(time_str.c_str(), format.c_str() ,       
		&(tm1.tm_year),   
		&(tm1.tm_mon),   
		&(tm1.tm_mday),  
		&(tm1.tm_hour),  
		&(tm1.tm_min),  
		&(tm1.tm_sec),  
		&(tm1.tm_wday),  
		&(tm1.tm_yday));  

	tm1.tm_year -= 1900;  
	tm1.tm_mon --;  
	tm1.tm_isdst=-1;  

	time_t time1; 
	time1 = mktime(&tm1);

	return time1;
}

#ifndef WIN32
bool fileSearch(std::string dir,std::string starttime ,std::string endtime ,std::vector<std::string>& list,std::string ext)
{
	std::string curdir = dir;  
	if(curdir[curdir.size()-1] != '/')
	{
		curdir.push_back('/');  
	}
	DIR *dfd;
	if( (dfd = opendir(curdir.c_str())) == NULL )          // curdir: /usr/CDR_LJ_03-25/061/
	{
		fprintf(stderr, "open %s error with msg is: %s\n",curdir.c_str(),strerror(errno));
		return false;
	}
	struct dirent    *dp;
	while ((dp = readdir(dfd)) != NULL)
	{
		char path2[300];
		sprintf(path2,"%s%s",curdir.c_str(),dp->d_name);
		struct stat info;
		if(stat(path2,&info) < 0)
		{
			fprintf(stderr, "open %s error with msg is: %s\n",path2,strerror(errno));
			return false;
		} 

		// if (dp->d_type == DT_DIR)   //DT_DIR:??í?????
		if(S_ISDIR(info.st_mode))
		{
			if ( 0==strcmp(dp->d_name, ".") || 0==strcmp(dp->d_name, "..") )
			{	
				continue;
			}        
		}
		else
		{
			std::string str = "rawdata"+ext;
			std::string strName=dp->d_name;
			// if(strName.find(str,0)!=-1  && strName.find(str,0)!=0)
			if(strName.find(str,0) == 11 && 21 == strName.size())
			{
				int cmpTime = changeStrToTime(rpc2TimeFm(strName));
				if(changeStrToTime(starttime) <= cmpTime+86400-1)
				{
					if(cmpTime<=changeStrToTime(endtime))
					{
						//printf("findname:%s\n",strName.c_str());
						list.push_back(dir+"/"+strName);   // ???t?D??\\???t??    :/usr/CDR_LJ_03-25/061/2014-03-23?arawdata.db
					}
				}
			}
		} 
	}
	closedir(dfd);
	return true;
}

#else

bool fileSearch(std::string dir,std::string starttime ,std::string endtime ,std::vector<std::string>& list,std::string ext)
{
	_finddata_t fileInfo;
	std::string strfind=dir;
	//为文件夹双斜杠
	//printf("%s\n",dir.c_str());
	if(strfind[strfind.size()-1] == '\n')
	{
		if(strfind[strfind.size()-2] != '\\')
		{  
			strfind=dir+"\\*";  
		}
		else
		{
			strfind=dir+"*"; 
		}
	}
	else
	{
		if(strfind[strfind.size()-1] != '\\')
		{  
			strfind=dir+"\\*";  
		}
		else
		{
			strfind=dir+"*"; 
		}
	}
	//printf("strfind:%s \n",strfind.c_str());
	int  handle = _findfirst(strfind.c_str(), &fileInfo);
	//文件没有找到，打印失败
	if (-1==handle){
		printf("(fileSearch)failed to transfer files\n");
		return false;
	}
	do{
		//判断是否有子目录
		if (fileInfo.attrib & _A_SUBDIR){
			//这个语句很重要，过滤掉.和..
			if( (strcmp(fileInfo.name,".") != 0 ) &&(strcmp(fileInfo.name,"..") != 0)){
				std::string newPath = dir + "\\" + fileInfo.name;   //加上双杠加上文件夹名继续遍历
				fileSearch(newPath,starttime,endtime,list,ext);        //递归调用
			}
		}
		else{
			std::string str = "rawdata"+ext;
			std::string strName=fileInfo.name;

			if(strName.find(str,0)!=-1){		
				long long cmpTime = changeStrToTime(rpc2TimeFm(strName));
				//printf("%s %I64d %s\n",strName.c_str(),cmpTime,rpc2TimeFm(strName).c_str());
				if(changeStrToTime(starttime) <= cmpTime+86400-1){   

					if(cmpTime<=changeStrToTime(endtime)){

						list.push_back(dir+"\\"+strName);
					}
				}								
			}
		}
	}while (_findnext(handle, &fileInfo) == 0);
	return true;
}
#endif

int   CreateDir(const   char   *sPathName) 
{ 
	char   DirName[256]; 
	strcpy(DirName,sPathName); 
	int i;
	int len = strlen(DirName); 
#ifndef WIN32
	if(DirName[len-1]!='/') 
		strcat(DirName,   "/"); 
#else
	if(DirName[len-1]!='\\') 
		strcat_s(DirName,   "\\"); 
#endif
	len   =   strlen(DirName); 

	for(i=1;   i<len;   i++) 
	{ 
#ifndef WIN32
		if(DirName[i]=='/') 
#else
		if(DirName[i]=='\\') 
#endif

		{ 
			DirName[i]   =   0; 
			if(ACCESS(DirName,0) != 0) 
			{ 
				if(MKDIR(DirName)==-1) 
				{  
					//CLogger::createInstance()->Log(eSoftError,"mkdir %s error",DirName);
					perror("mkdir error");  
					return   -1;  
				} 
			} 
#ifndef WIN32
			DirName[i]   =   '/'; 
#else
			DirName[i]   =   '\\'; 
#endif
		} 
	} 

	return   1; 
}   


//int gethisdata(char *savepath, char * datapath, char *starttime, char *endtime, std::vector< Rec > &rawdatals)

int gethisdata(char * datapath, char *starttime, char *endtime, std::vector< Rec > & rawdatals)
{
	//fprintf(stderr, "savepath:%s datapath:%s starttime: %s endtime: %s \n",savepath,datapath,starttime, endtime);
	std::string dir = datapath;
	std::string stime_start = starttime;
	std::string stime_end = endtime;
	std::vector<std::string> list;
	std::string ext = ".db";

	fileSearch(dir,stime_start,stime_end,list,ext);
	time_t tstart = changeStrToTime(stime_start,"%d-%d-%d %d:%d:%d");
	time_t tend = changeStrToTime(stime_end,"%d-%d-%d %d:%d:%d");

	__int64 start = tstart*10000000LL+116444736000000000LL;
	__int64 end = tend*10000000LL+116444736000000000LL;
	__int64 ninterval = tend - tstart;

	unsigned int i;
	for (i = 0; i < list.size(); i++)
	{
		char ctime[100] = {0};
		char ctemp[100] = {0};
		sprintf_s(ctime,100,"%s",stime_start.c_str());
		sprintf_s(ctemp,100,"%s_rawdata.db",ctime);
		/*****************从文件中读记录***********************/
		CSQLiteDB db;
		if(!db.open(list[i].c_str())){
			printf("error!\n");
			return -1;
		}
		std::string strSQL;
		char findRD[256] = {0};
		sprintf(findRD,"select count(*) from sqlite_master where type = 'table' and name = 'RD'");
		strSQL=findRD;
		CSQLiteQuery query;
		try{
			query = db.execQuery(strSQL.c_str());
			if(!query.getIntField(0))
			{
				continue;
			}
		}catch (CSQLiteException& e) {
			printf("db query error %d\n",e.errorCode());
			break;
		}
		//printf("run here! ninterval:%d\n",ninterval);
		char buff[256]={0};
		if(ninterval > 0)
		{	
			sprintf(buff,"SELECT FT,Dir,Data FROM RD WHERE FT BETWEEN %lld AND %lld",start,end);
			strSQL=buff;
		}
		else if(ninterval == 0)
		{
			sprintf(buff,"SELECT FT,Dir,Data FROM RD WHERE FT>=%lld",start);
			strSQL=buff;
		}
		else
		{
			sprintf(buff,"SELECT FT,Dir,Data FROM RD WHERE FT<=%lld",start);
			strSQL=buff;
		}
		try{
			query = db.execQuery(strSQL.c_str());
		}catch (CSQLiteException& e) {
			printf("db query error %d!!\n",e.errorCode());
			break;
		}
		while (!query.eof())
		{
			__int64 iFT;
			int nDir;
			const BYTE* pbyTemp;
			int nLen;
			Rec dn;
			//获取时间值,将时间转换成RCITime结构
			iFT = query.getInt64Field(0);
			dn.timeScale = iFT;
			//获取数据传送方向
			nDir = query.getIntField(1);
			dn.dir = nDir;

			//获取数据data
			pbyTemp = query.getBlobField(2,nLen);
			memcpy(dn.buf,pbyTemp,nLen);
			dn.size = nLen;
			//fprintf(stderr, "pbyTemp: %s\n",pbyTemp);
			//将记录存储到容器里
			rawdatals.push_back(dn);
			//读取下一条记录
			query.nextRow();
		}
	}
	return 0;
}

int CMySerial::getlasterror()
{
	unsigned long nlasterror = 1;
#ifdef WIN32
	COMSTAT comstat;
	ClearCommError( fd, &nlasterror, &comstat );
#else
	struct serial_icounter
	{
		int cts, dsr, rng, dcd;
		int rx, tx;
		int frame, overrun, parity, brk;
		int buf_overrun;
		int reserved[9];
	};
	struct serial_icounter ctl;
	ioctl((int)fd, TIOCGICOUNT, &ctl );
	static int gbrk = 0;
	static int gframe = 0;
	if( ctl.brk == gbrk && ctl.frame == gframe ){
		nlasterror = 0;
	}
	gbrk = ctl.brk;
	gframe = ctl.frame;
	//printf("%d %d %d \n", nlasterror, ctl.rx - grx, ctl.brk-gbrk);
	//gbrk = ctl.brk;
	//printf("%d %d %d %d %d %d %d %d %d %d \n", ctl.cts, ctl.dsr, ctl.rng,ctl.dcd, ctl.rx, ctl.tx, ctl.frame, ctl.overrun, ctl.parity,ctl.brk);
#endif
	return nlasterror;
}


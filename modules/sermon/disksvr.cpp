#include "disksvr.h"
#include "./kernel/Log.h"
#include "./kernel/taskdef.h"

void moveoldfile(const char* path, time_t otime, const char* exname){
	LOG(eTipMessage, "machine check and move old-file in dir:%s", path);
#ifdef _WIN32
#else
	DIR *dfd;
	if ((dfd = opendir(path)) == NULL) {
		LOG(eSoftError, "open %s error", path);
		return;
	}
	struct dirent *dp;
	while ((dp = readdir(dfd)) != NULL) {
		if (strlen(exname) == 0) {//无扩展名,目录
			if (dp->d_type == DT_DIR && (0 == strcmp(dp->d_name, ".") || 0 == strcmp(dp->d_name, "..")))
				continue;
		}else {
			if (NULL == strstr(dp->d_name, exname))
				continue;
		}
		char szfile[256] = { 0 };
		sprintf(szfile, "%s%s", path, dp->d_name);
		struct stat e;
		int nst = stat(szfile, &e);
		if (nst == 0 && e.st_mtime<otime) {
			int ret = remove(szfile);
			LOG(eTipMessage, "remove old file1: %s  ret:%d", szfile, ret);
		}
	}
#endif
}

//启动磁盘服务
void disksvr::start(){
	exitthread = false;
	myevent  = event_create(false, false);
#ifdef WIN32
	threadid = _beginthreadex(NULL,0, disksvrthread,this, 0, NULL);
#else
	if( pthread_create(&threadid, NULL, disksvrthread, this) != 0) 
		printf("pthread_create failed! \n");
#endif
}       

//退出磁盘服务
void disksvr::stop(){
	exitthread = true;
	event_set( myevent );
#ifdef WIN32
	WaitForSingleObject((HANDLE)threadid, 1);
#else
	pthread_join(threadid, NULL);
#endif
	event_destroy( myevent );
	myevent=NULL;
}

//磁盘服务线程
winapi disksvr::disksvrthread(void * argv){
	disksvr * svr = (disksvr *)argv;
	int nKTime = getkeeptimes();
	char sztracepath[256]={0};
	gettracepath(sztracepath,sizeof(sztracepath));
	while( !svr->exitthread ){
		if( event_timedwait(svr->myevent,3600*1000) == 1 ){
			for(std::map<std::string,std::string>::iterator it= svr->workdir.begin();it!= svr->workdir.end();it++)
				moveoldfile( it->first.c_str(), time(NULL)-(nKTime*24*3600), "db");
			moveoldfile( sztracepath, time(NULL)-(nKTime*24*3600), "");
		}
	}
	return 0;
}

//
void disksvr::addmonitor( const char* path ) {
	workdir.insert(std::make_pair(path, path));
}

void disksvr::delmonitor(const char* path) {
	workdir.erase( path );
}

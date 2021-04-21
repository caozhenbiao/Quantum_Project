#include "datamgr.h"
#include "../machine.h"
#include "../db/RawDataDB.h"
#include "./Log.h"


int datamgr_gethisdata(char * datapath, char *starttime, char *endtime, std::vector< Rec > & rawdatals)
{
	//fprintf(stderr, "savepath:%s datapath:%s starttime: %s endtime: %s \n",savepath,datapath,starttime, endtime);
	std::string dir = datapath;
	std::string stime_start = starttime;
	std::string stime_end = endtime;
	std::vector<std::string> list;
	std::string ext = ".db";

	fileSearch(dir, stime_start, stime_end, list, ext);
	time_t tstart = changeStrToTime(stime_start, "%d-%d-%d %d:%d:%d");
	time_t tend = changeStrToTime(stime_end, "%d-%d-%d %d:%d:%d");

	__int64 start = tstart * 10000000LL + BASETIME;
	__int64 end = tend * 10000000LL + BASETIME;
	__int64 ninterval = tend - tstart;

	unsigned int i;
	for (i = 0; i < list.size(); i++)
	{
		char ctime[100] = { 0 };
		char ctemp[100] = { 0 };
		sprintf_s(ctime, 100, "%s", stime_start.c_str());
		sprintf_s(ctemp, 100, "%s_rawdata.db", ctime);
		/*****************���ļ��ж���¼***********************/
		CSQLiteDB db;
		if (!db.open(list[i].c_str())) {
			printf("error!\n");
			return -1;
		}
		std::string strSQL;
		char findRD[256] = { 0 };
		sprintf(findRD, "select count(*) from sqlite_master where type = 'table' and name = 'RD'");
		strSQL = findRD;
		CSQLiteQuery query;
		try {
			query = db.execQuery(strSQL.c_str());
			if (!query.getIntField(0))
			{
				continue;
			}
		}
		catch (CSQLiteException& e) {
			printf("db query error %d\n", e.errorCode());
			break;
		}
		//printf("run here! ninterval:%d\n",ninterval);
		char buff[256] = { 0 };
		if (ninterval > 0)
		{
			sprintf(buff, "SELECT FT,Dir,Data FROM RD WHERE FT BETWEEN %lld AND %lld", start, end);
			strSQL = buff;
		}
		else if (ninterval == 0)
		{
			sprintf(buff, "SELECT FT,Dir,Data FROM RD WHERE FT>=%lld", start);
			strSQL = buff;
		}
		else
		{
			sprintf(buff, "SELECT FT,Dir,Data FROM RD WHERE FT<=%lld", start);
			strSQL = buff;
		}
		try {
			query = db.execQuery(strSQL.c_str());
		}
		catch (CSQLiteException& e) {
			printf("db query error %d!!\n", e.errorCode());
			break;
		}
		int cnt = 0;
		while (!query.eof() && 2000>cnt++)
		{
			__int64 iFT;
			int nDir;
			const BYTE* pbyTemp;
			int nLen;
			Rec dn;
			//��ȡʱ��ֵ,��ʱ��ת����RCITime�ṹ
			iFT = query.getInt64Field(0);
			dn.timeScale = iFT;
			//��ȡ���ݴ��ͷ���
			nDir = query.getIntField(1);
			dn.dir = nDir;

			//��ȡ����data
			pbyTemp = query.getBlobField(2, nLen);
			memcpy(dn.buf, pbyTemp, nLen);
			dn.size = nLen;
			//fprintf(stderr, "pbyTemp: %s\n",pbyTemp);
			//����¼�洢��������
			rawdatals.push_back(dn);
			//��ȡ��һ����¼
			query.nextRow();
		}
	}
	return 0;
}


//������ʷ��¼���Ա�����
void datamgr_exporttofile(std::vector< Rec>& rawdatals) {
	FILE * ftmp = NULL;
	char * szpath = "./webui/tmp/history.txt";
#ifdef WIN32
	fopen_s(&ftmp, szpath, "w+");
#else
	ftmp = fopen(szpath, "w+");
#endif
	for (size_t i = 0; i < rawdatals.size(); i++) {
		char szline[10240] = { 0 };
		struct tm t;
		long long times = rawdatals.at(i).timeScale;
		time_t tsec = (times - BASETIME) / 10000000;
		int    tmsec = int((times - BASETIME) % 10000000);
		localtime_x(&t, &tsec);
		int pos = sprintf(szline, "%02d-%02d %02d:%02d:%02d.%03d %d ", t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec,tmsec/10000,rawdatals.at(i).dir);
		int len = bin2hex((unsigned char*)rawdatals.at(i).buf, rawdatals.at(i).size, &szline[pos], 10240 - pos);
 		fwrite(szline, len+pos, 1, ftmp);
		fwrite("\n", 1, 1, ftmp);
	}
	fclose(ftmp);
}

datamgr::datamgr():threadExit(true) {
}

//д������,����¼��DB
void datamgr::saveframe(int dir, char* data, int len) {
	if (len <= 0)
		return;
	//if (dir == e_FrameIn)
	//	inports += len;
	//else
	//	exports += len;
	Rec rec;
	rec.size = len;
	rec.dir = dir;
	rec.timeScale = get_time_scale();
	memcpy(&rec.buf, data, len);
	rcitem.push_back(rec);
	if (rcitem.size() > MAXRECORDMEMERY) {
		CRawDataDB dbfile;
		dbfile.SetDir(mypath);
		dbfile.Save(&rcitem);
		rcitem.clear();
	}
}

//�������ݴ洢·��
void datamgr::setpath(const char* path) {
	char folder[256] = {0};
	sprintf_s(folder, sizeof(folder), "%s/", path);
	for (size_t i = 1; i<strlen(folder); i++) {
		if (folder[i] == '/' || folder[i] == '\\') {
			folder[i] = 0;
			if (ACCESS(folder, 0) != 0) {
				if (MKDIR(folder) == -1) {
					perror("mkdir error");
					return;
				}
			}
			folder[i] = '/';
		}
	}
	memset(mypath, 0x00, sizeof(mypath));
	strncpy(mypath, folder, strlen(folder));
}

//��ȡ��ʷ����
winapi datamgr::historythread(void * arg){
	datamgr * mgr = (datamgr*)arg;
	//ʱ�����
	char starttime[256] = {0};
	char endtime[256] = {0};
	if (2 == sscanf(mgr->historyTask.buffer, "%[^;];%[^\n]", starttime, endtime)) {
		//ʱ�䴦��endtime����������
		time_t tend = changeStrToTime(endtime, "%d-%d-%d %d:%d:%d");
		if (tend == -1) {
			tend = changeStrToTime(starttime, "%d-%d-%d %d:%d:%d") + atoi(endtime);
			struct tm t;
			localtime_x(&t, &tend);
			sprintf(endtime, "%d-%d-%d %d:%d:%d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
		}
		//��ȡ��ʷ����
		static std::vector<Rec> rawdatals;
		datamgr_gethisdata(mgr->mypath, starttime, endtime, rawdatals);
		datamgr_exporttofile(rawdatals);

		//������������
		task newtask;
		for (size_t i = 0; i<rawdatals.size() && !mgr->threadExit; i++) {
			memset(newtask.buffer, 0, sizeof(newtask.buffer));
			memcpy(newtask.buffer, rawdatals.at(i).buf, rawdatals.at(i).size);
			newtask.datastamp = rawdatals.at(i).timeScale;
			newtask.datadir = rawdatals.at(i).dir;
			newtask.buflen = rawdatals.at(i).size;
			newtask.trigger = (xbase*)mgr;
			newtask.mark = read_histdata;
			newtask.retcode = 1;
			mgr->historyTask.trigger->addtask(&newtask);
			Sleep(20);
		}
		printf("printhistorydata exit;\n");
		//��ʷ��¼�������,�����˳�
		if (!mgr->threadExit) {
			newtask.trigger = (xbase*)mgr;
			newtask.mark = read_histdata;
			newtask.retcode = 2;
			mgr->historyTask.trigger->addtask(&newtask);
		}
		rawdatals.clear();
	}
	mgr->threadExit = true;
	return 0;
}

void datamgr::requestHistoryData(task * tsk) {
	if (strlen(tsk->buffer) == 0) {
		threadExit = true;
		return;
	}
	if (!threadExit)
		return;
	threadExit = false;
	memcpy(&historyTask, tsk,sizeof(task));
#ifdef WIN32
	threadid = _beginthreadex(NULL, 0, historythread, this, 0, NULL);
#else
	if(pthread_create(&threadid, NULL, historythread, this) != 0)
		printf("pthread_create failed! \n");
#endif
}

void datamgr::stopGetHistoryData() {
	threadExit = true;
}

int datamgr::getinports() {
	return inports;
}

int datamgr::getexports() {
	return exports;
}

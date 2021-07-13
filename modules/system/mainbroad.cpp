#include "filesys.h"
#include "md5.h"
#ifdef _WIN32
#define ACCESS _access   
#define MKDIR(a) _mkdir((a)) 
#include <Windows.h>
#include <stdio.h>
#pragma warning(disable: 4996) // avoid GetVersionEx to be warned
#else
#include <stdarg.h>   
#include <sys/stat.h> 
#define ACCESS access   
#define MKDIR(a) mkdir((a),0755)   
#define sprintf_s snprintf
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <sys/vfs.h>
#include <map>
#include <string>
#include <regex.h>
#endif

void buzzer(unsigned int freq, unsigned int duration) {
#ifdef _WIN32
	Beep(freq, duration);
#else
	int fd = open("/dev/tty10", O_RDONLY);
	if (fd != -1) {
		ioctl(fd, KDMKTONE, (freq << 16) + (1193180 / duration));
	}
#endif
}
 
// ---- get os info ---- //
void getOsInfo(std::string& os_name, std::string& version ){
#ifdef _WIN32
	OSVERSIONINFO osver = { sizeof(OSVERSIONINFO) };
	GetVersionEx(&osver);
	if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)
		os_name = "Windows 2000";
	else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
		os_name = "Windows XP";
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0)
		os_name = "Windows 2003";
	else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
		os_name = "windows vista";
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)
		os_name = "windows 7";
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
		os_name = "windows 10";
	char szver[256] = { 0 };
	sprintf(szver, "%d.%d", osver.dwMajorVersion, osver.dwMinorVersion);
	version = szver;
#else
	FILE *fp = fopen("/proc/version", "r");
	if (NULL == fp)
		printf("failed to open version\n");
	char szTest[1000] = { 0 };
	memset(szTest, 0, sizeof(szTest));
	if( NULL == fgets(szTest, sizeof(szTest) - 1, fp)){ // leave out \n
		//printf("fgets null\n");
	}
	char ver[256] = { 0 };
	sscanf(szTest, "%*s%*s%s", ver);
	os_name = "linux";
	version = ver;
	fclose(fp);
#endif
}

// ---- get memory info ---- //
void getMemoryInfo( int& decimal_total, int& decimal_avl){
#ifdef _WIN32
	MEMORYSTATUSEX statusex;
	statusex.dwLength = sizeof(statusex);
	if (GlobalMemoryStatusEx(&statusex)){
		unsigned long long total = 0, remain_total = 0, avl = 0, remain_avl = 0;
		total = statusex.ullTotalPhys / 1048576;
		avl = statusex.ullAvailPhys / 1048576;
		decimal_total = (int)total;
		decimal_avl = (int)avl;
	}
#else
	FILE *fp = fopen("/proc/meminfo", "r");
	if (NULL == fp)
		printf("failed to open meminfo\n");
	char szTest[1000] = { 0 };
	std::map<std::string,unsigned int> vinfo;
	while (!feof(fp)) {
		memset(szTest, 0, sizeof(szTest));
		if( NULL == fgets(szTest, sizeof(szTest) - 1, fp)){
			//printf("fgets null\n");
		}
		char sztitle[256] = { 0 };
		int  value = 0;
		sscanf(szTest, "%[^:]:%d", sztitle, &value);
		std::string key(sztitle);
		key.erase(0, key.find_first_not_of("	"));
		key.erase(key.find_last_not_of("	") + 1);
		vinfo[key] = value;
	}
	decimal_total = vinfo["MemTotal"]/1024;
	decimal_avl = vinfo["MemFree"]/1024;
	fclose(fp);
#endif
}

void getCpuInfo(int& freq, unsigned int& guid, std::string& mid, std::string& type) {
#ifdef _WIN32
	char cpuType[49] = { 0 };
	BYTE szCpu[16] = { 0 };
	unsigned int uCpuID = 0U;
	int start, over;
	memset(cpuType, 0, sizeof(cpuType));
	__asm {
		mov eax, 0
		cpuid
		mov dword ptr szCpu[0], ebx
		mov dword ptr szCpu[4], edx
		mov dword ptr szCpu[8], ecx
		mov eax, 1
		cpuid
		mov uCpuID, edx
	}
	for (DWORD t = 0; t < 3; t++) {
		unsigned int veax = 0x80000002 + t;
		unsigned int deax;
		unsigned int debx;
		unsigned int decx;
		unsigned int dedx;
		__asm {
			mov eax, veax
			cpuid
			mov deax, eax
			mov debx, ebx
			mov decx, ecx
			mov dedx, edx
		}
		memcpy(cpuType + 16 * t + 0, &deax, 4);
		memcpy(cpuType + 16 * t + 4, &debx, 4);
		memcpy(cpuType + 16 * t + 8, &decx, 4);
		memcpy(cpuType + 16 * t + 12, &dedx, 4);
	}
	_asm {
		RDTSC
		mov start, eax
	}
	Sleep(50);
	_asm {
		RDTSC
		mov over, eax
	}
	guid = uCpuID;
	freq = (over - start)/50000;
	mid  = (char*)szCpu;
	type = cpuType;
#else
	FILE *fp = fopen("/proc/cpuinfo", "r");
	if (NULL == fp) {
		printf("failed to open cpuinfo\n");
		return;
	}
	char szTest[1000] = { 0 };
	std::map<std::string, std::string> vinfo;
	while (!feof(fp)){
		memset(szTest, 0, sizeof(szTest));
		if( NULL == fgets(szTest, sizeof(szTest) - 1, fp)){
			//printf("fgett null\n");
		}
		char sztitle[256] = { 0 };
		char szvalue[256] = { 0 };
		sscanf(szTest, "%[^:]:%[^\r\n]", sztitle, szvalue);
		std::string key(sztitle);
		key.erase(0, key.find_first_not_of("	"));
		key.erase(key.find_last_not_of("	") + 1);
		std::string val(szvalue);
		val.erase(0, val.find_first_not_of("	"));
		val.erase(val.find_last_not_of("	") + 1);
		vinfo[key] = val;
	}
#if defined(ARM_LINUX)
	freq = atoi(vinfo["BogoMIPS"].c_str());
	mid  = vinfo["Hardware"];
	type = vinfo["model name"];
#else
	freq = atoi(vinfo["cpu MHz"].c_str());
	mid  = vinfo["vendor_id"];
	type = vinfo["model name"]; 
#endif 
	fclose(fp);
#endif
}

void getDiskInfo(const char* path, unsigned int& total, unsigned int& free) {
#ifdef _WIN32
	DWORD64 qwFreeBytesToCaller, qwTotalBytes, qwFreeBytes;
	GetDiskFreeSpaceEx(path,(PULARGE_INTEGER)&qwFreeBytesToCaller,
		(PULARGE_INTEGER)&qwTotalBytes,
		(PULARGE_INTEGER)&qwFreeBytes);
	free = (unsigned int)(qwFreeBytes/1024/1024);
	total = (unsigned int)(qwTotalBytes/1024/1024);
#else
	struct statfs sfs;
	statfs(path, &sfs);
	//int percent = (sfs.f_blocks - sfs.f_bfree) * 100 / (sfs.f_blocks - sfs.f_bfree + sfs.f_bavail) + 1;
	//unsigned long long blocksize = sfs.f_bsize;
	//unsigned long long totalsize = blocksize * sfs.f_blocks;
	total = 4 * sfs.f_blocks/1024;
	free  = 4 * sfs.f_bavail/1024;
#endif
}

std::string getAuthorize( char * prikey ) {
	MD5 md5 = MD5(prikey);
	return md5.smartxchksum();
}

int authenticate() {
	FILE *fp = fopen("./system.key", "r");
	if (!fp)
		return -1;
	char ch = 0;
	std::string sskey;
	while (fread(&ch, 1, 1, fp) == 1) sskey.push_back(ch);
	fclose(fp);
	//
	int freq;
	unsigned int guid;
	std::string mid;
	std::string type;
	getCpuInfo(freq, guid, mid, type);
	char devkey[256] = { 0 };
	sprintf(devkey, "%s%10u", mid.c_str(), guid);
	MD5 md5 = MD5(devkey);
	std::string schk = md5.smartxchksum();
	return sskey.find( schk );
}

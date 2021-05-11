#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include "machine.h"

#ifdef _WIN32
#include<crtdbg.h>
TCHAR SVCNAME[128] = "quarksvr";
TCHAR SVCDESC[128]   = "quark for quantum project";
#else
#include <mcheck.h>
#define TCHAR char
#endif

#ifdef _WIN32
char *optarg = NULL;
int getopt( int argc, char **argv, char *opts ) {
	static int sp = 1;
	static int opterr = 1;
	static int optind = 1;
	static int optopt;
	register int c;
	register char *cp;
	if (sp == 1)
		if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0')
			return(-1);
		else if (strcmp(argv[optind], "--") == 0) {
			optind++;
			return(-1);
		}
	optopt = c = argv[optind][sp];
	if (c == ':' || (cp = strchr(opts, c)) == 0) {
		printf(": illegal option --%c ", c);
		if (argv[optind][++sp] == '\0') {
			optind++;
			sp = 1;
		}
		return('?');
	}
	if (*++cp == ':') {
		if (argv[optind][sp + 1] != '\0')
			optarg = &argv[optind++][sp + 1];
		else if (++optind >= argc) {
			printf(": option requires an argument --%c ", c);
			sp = 1;
			return('?');
		}
		else
			optarg = argv[optind++];
		sp = 1;
	}
	else {
		if (argv[optind][++sp] == '\0') {
			sp = 1;
			optind++;
		}
		optarg = 0;
	}
	return(c);
}
#endif

#ifdef _DEBUG //这个要加上，否则不会输出定义到那个文件中（及不包含存在内存泄露的该cpp文件的相关信息）  
#define new  new(_NORMAL_BLOCK, __FILE__, __LINE__)  
#endif 

//smartx main
#ifdef _WIN32
int MyMain(int argc, char * argv[]) {
#else
int main(int argc, char* argv[]) {
#endif
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
	_CrtDumpMemoryLeaks();
#else
	mtrace();
#endif
	printf("[Quantum Project v1.0] \nquark start.\n");
	base::AtExitManager exit_manager;
	macopt opt;
	memset(&opt, 0x00, sizeof(macopt));
	strcpy(opt.webpath, "./webui");
	strcpy(opt.commsvrip, "127.0.0.1");
	strcpy(opt.websvrip, "127.0.0.1");
	opt.commsvrport = 0;
	opt.websvrport = 0;
	int c = 0;
	while ((c = getopt(argc, argv, "k:s:u:w:m:n:x:y:c")) != -1) {
		switch (c) {
		case 'k':memset(opt.runkey, 0x00, 40); strncpy(opt.runkey, optarg, 40); break;
		case 's':memset(opt.script, 0x00, 256); strncpy(opt.script, optarg, 256); break;
		case 'u':memset(opt.option, 0x00, 256); strncpy(opt.option, optarg, 256); break;
		case 'w':memset(opt.webpath, 0x00, 256); strncpy(opt.webpath, optarg, 256); break;
		case 'm':memset(opt.commsvrip, 0x00, 32); strncpy(opt.commsvrip, optarg, 32); break;
		case 'n':opt.commsvrport = atoi(optarg); break;
		case 'x':memset(opt.websvrip, 0x00, 32); strncpy(opt.websvrip, optarg, 32); break;
		case 'y':opt.websvrport = atoi(optarg); break;
		case 'c':opt.console = true; break;
		default:break;
		}
	}
	taskmgr::start();
	cmachine machine;
	if (machine.poweron(opt)) {
		taskmgr::run(opt.console);
		machine.poweroff();
	}
	taskmgr::stop();
#ifndef _WIN32
	muntrace();
#endif
	printf("\nquark exit!\n");
	return 0;
}

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "machine.h"

static int  exit_flag;  /* Program termination flag */
#ifdef _WIN32
char *optarg = NULL;
int getopt(int argc, char **argv, const char *opts) {
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
	if (c == ':' || (cp = (char*)strchr(opts, c)) == 0) {
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

static void signal_handler(int sig_num){
    switch (sig_num) {
#ifndef _WIN32
    case SIGCHLD:
        while (waitpid(-1, &sig_num, WNOHANG) > 0);
        break;
#endif /* !_WIN32 */
    default:
        exit_flag = sig_num;
        break;
    }
}

int main(int argc, char *argv[]){
#ifndef _WIN32
    (void) signal(SIGCHLD, signal_handler);
    (void) signal(SIGPIPE, SIG_IGN);
#endif /* _WIN32 */
    (void) signal(SIGTERM, signal_handler);
    (void) signal(SIGINT, signal_handler);

	struct macopt opt;
	memset(&opt, 0x00, sizeof(opt));
	strcpy(opt.webs_path, "./webui");
	strcpy(opt.webs_ip, "127.0.0.1");
	opt.webs_port = 8080;
	int c = 0;
	while ((c = getopt(argc, argv, "i:k:s:u:w:m:n:x:y:c")) != -1) {
		switch (c) {
		case 'k':memset(opt.runkey, 0x00, 40); strncpy(opt.runkey, optarg, 40); break;
		case 's':memset(opt.script, 0x00, 256); strncpy(opt.script, optarg, 256); break;
		case 'u':memset(opt.option, 0x00, 256); strncpy(opt.option, optarg, 256); break;
		case 'w':memset(opt.webs_path, 0x00, 256); strncpy(opt.webs_path,optarg, 256); break;
		case 'x':memset(opt.webs_ip, 0x00, 32); strncpy(opt.webs_ip, optarg, 32); break;
		case 'y':opt.webs_port = atoi(optarg); break;
		case 'c':opt.console = 1; break;
		default:break;	
		}
	}
	machine_start( &opt );
    while (exit_flag == 0) {
		static char sz[256] = { 0 };
		if (fgets(sz, 256, stdin) == NULL)
			continue;
		if (strcmp(sz, "exit\n") == 0)
			break;
		machine_console_dispath(sz);
    }
	machine_stop();
    return (0);
}

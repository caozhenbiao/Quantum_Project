#ifndef _ATTESTOR_H_
#define _ATTESTOR_H_
class attestor{
public:
	struct identify {
		unsigned char  type;
		unsigned short freq;
		unsigned int   expire;
		unsigned char  mac[8];
		unsigned char  md3[4];
		unsigned char  cs;
	};
public:
	static bool prove(char* pubkey);
  static bool prove(char* pubkey,identify& idf);
	static void makeprove(char* cmdline);
	static void command(char* func, char* args);
};
#endif



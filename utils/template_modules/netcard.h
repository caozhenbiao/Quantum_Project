#ifndef  __TEMPLATE_MODULE__
#define __TEMPLATE_MODULE__

static class cnetcard
{
public:
	void start();
	void stop();
public:
	int getinfo( char* info);
	int IsNetwork();
	int request(const char * data, int delay, int callback);
	int request(const char * data, int callback);
	void request_replay(int callback, const char* data);
};

#endif


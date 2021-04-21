#ifndef  __SCP_MODULE__
#define __SCP_MODULE__

#include <libssh2.h>
#include <map>

class cscp
{
public:
	void start();
	void stop();

public:
	int execute(int handle, const char* command, int callback);

private:
	int login(const char* ip, unsigned short port, const char* user, const char* pwd);
	void do_execute(int handle, const char* command, int callback);

private:

};

#endif


#ifndef  __TEMPLATE_MODULE__
#define __TEMPLATE_MODULE__

#include <libssh2.h>
#include <map>

class cssh
{
	struct peerinfo {
		LIBSSH2_CHANNEL * channel; //ͨ??
		LIBSSH2_SESSION   * session;//?ػ?
		int sock;
	};

public:
	void start();
	void stop();

public:
	int connect_password(const char* ip, unsigned short port, const char* user, const char* pwd);
	int connect_privatekey(const char* ip, unsigned short port, const char* user, const char* file);
	int shutdown( int handle );
	int execute(int handle, const char* command, int callback );

private:
	void do_execute(int handle, const char* command, int callback );

private:
	std::map<int, peerinfo> peerlists;
};

#endif


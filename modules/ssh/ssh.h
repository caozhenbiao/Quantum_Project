#ifndef  __TEMPLATE_MODULE__
#define __TEMPLATE_MODULE__

#include <libssh2.h>
#include <map>

class cssh
{
	struct peerinfo {
		LIBSSH2_CHANNEL * channel; //通道
		LIBSSH2_SESSION   * session;//回话
		int sock;
	};

public:
	void start();
	void stop();

public:
	int login(const char* ip, unsigned short port, const char* user, const char* pwd);
	int login_pubkey_file(const char* ip, unsigned short port, const char* user, const char* file);
	int login_pubkey(const char* ip, unsigned short port, const char* user, const char* key);
	int shutdown( int handle );
	int execute(int handle, const char* command, int callback );

private:
	void do_execute(int handle, const char* command, int callback );

private:
	std::map<int, peerinfo> peerlists;
};

#endif


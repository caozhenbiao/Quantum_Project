#include <stdio.h>
#include "ssh.h"
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#endif

/*libbase*/
#include "base/at_exit.h"
#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/location.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/strings/string_number_conversions.h"
#include "base/task_runner.h"
#include "base/threading/thread.h"
#include "base/time/time.h"
#include "base/process/process.h"

/*lua*/
extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

scoped_ptr<base::Thread> g_thread=NULL;
extern lua_State * theState;

void cssh::start() {
#ifdef _WIN32
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata);
#endif
	base::AtExitManager exit_manager;
	base::Thread::Options options;
	options.message_loop_type = base::MessageLoop::TYPE_IO;
	g_thread.reset(new base::Thread("Project_Thread"));
	g_thread->StartWithOptions(options);
}

void cssh::stop() {
	g_thread->Stop();
	g_thread.reset();
#ifdef _WIN32
	WSACleanup();
#endif
}

int cssh::connect_password(const char* ip, unsigned short port, const char* user, const char* pwd ) {
	peerinfo pinfo;
	sockaddr_in sshserver;
	sshserver.sin_family = AF_INET;
	sshserver.sin_port = htons(22);
	if ((sshserver.sin_addr.s_addr = inet_addr(ip)) == INADDR_NONE) {
		fprintf(stderr, "\nthe address is invalid!/n");
		return -1;
	}

	if ((pinfo.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
		fprintf(stderr, "\nfailed to create a socket!/n");
		return -1;
	}

	if (connect(pinfo.sock, (struct sockaddr*)(&sshserver), sizeof(struct sockaddr_in)) != 0){
		fprintf(stderr, "\nfailed to connect,please check your username or passwd!/n");
#ifdef _WIN32
		closesocket(pinfo.sock);
#else
		close(pinfo.sock);
#endif
		return -1;
	}

	if (!(pinfo.session = libssh2_session_init())){
		printf( "init SSH session failed!/n");
#ifdef _WIN32
		closesocket(pinfo.sock);
#else
		close(pinfo.sock);
#endif
		return -1;
	}
	libssh2_session_startup(pinfo.session, pinfo.sock);
	libssh2_hostkey_hash(pinfo.session, LIBSSH2_HOSTKEY_HASH_MD5);
	char * authlist = libssh2_userauth_list(pinfo.session, user, strlen(user));
	if (strstr(authlist, "password") == NULL) {
		libssh2_session_free(pinfo.session);
#ifdef _WIN32
		closesocket(pinfo.sock);
#else
		close(pinfo.sock);
#endif
		return -1;
	}

	libssh2_userauth_password(pinfo.session, user, pwd);
	libssh2_session_set_blocking(pinfo.session, 1);
	if (!(pinfo.channel = libssh2_channel_open_session(pinfo.session)) || libssh2_channel_shell(pinfo.channel)){
		libssh2_session_free(pinfo.session);
#ifdef _WIN32
		closesocket(pinfo.sock);
#else
		close(pinfo.sock);
#endif
		return -1;
	}
	char bhead[2048] = { 0 };
	libssh2_channel_read(pinfo.channel, bhead, sizeof(bhead));
	static int handle = 0;
	peerlists[++handle] = pinfo;
	return handle;
}

int cssh::connect_privatekey(const char* ip, unsigned short port, const char* user, const char* file) {
	peerinfo pinfo;
	struct sockaddr_in sshserver;
	sshserver.sin_family = AF_INET;
	sshserver.sin_port = htons(port);
	if ((sshserver.sin_addr.s_addr = inet_addr(ip)) == INADDR_NONE) {
		fprintf(stderr, "\nthe address is invalid!/n");
		return -1;
	}

	if ((pinfo.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		fprintf(stderr, "\nfailed to create a socket!/n");
		return -1;
	}

	if (connect(pinfo.sock, (struct sockaddr*)(&sshserver), sizeof(struct sockaddr_in)) != 0) {
		fprintf(stderr, "\nfailed to connect,please check your username or passwd!/n");
#ifdef _WIN32
		closesocket(pinfo.sock);
#else
		close(pinfo.sock);
#endif
		return -1;
	}

	if (!(pinfo.session = libssh2_session_init())) {
		printf("init SSH session failed!/n");
#ifdef _WIN32
		closesocket(pinfo.sock);
#else
		close(pinfo.sock);
#endif
		return -1;
	}

	libssh2_session_startup(pinfo.session, pinfo.sock);
	libssh2_hostkey_hash(pinfo.session, LIBSSH2_HOSTKEY_HASH_SHA1);
	char * authlist = libssh2_userauth_list(pinfo.session, user, strlen(user));
	if (strstr(authlist, "publickey") == NULL) {
		libssh2_session_free(pinfo.session);
#ifdef _WIN32
		closesocket(pinfo.sock);
#else
		close(pinfo.sock);
#endif
		return -1;
	}
	int result = libssh2_userauth_publickey_fromfile_ex(pinfo.session,user, strlen(user),NULL, file,NULL);
	if ( result != 0 ){
		char * error = NULL;
		int len = 0;
		int errbuf = 0;
		libssh2_session_last_error(pinfo.session, &error, &len, errbuf);
		printf(error);
		libssh2_session_free(pinfo.session);
#ifdef _WIN32
		closesocket(pinfo.sock);
#else
		close(pinfo.sock);
#endif
		return -1;
	}
	libssh2_session_set_blocking(pinfo.session, 1);
	if (!(pinfo.channel = libssh2_channel_open_session(pinfo.session)) || libssh2_channel_shell(pinfo.channel)) {
		libssh2_session_free(pinfo.session);
#ifdef _WIN32
		closesocket(pinfo.sock);
#else
		close(pinfo.sock);
#endif
		return -1;
	}
	char bhead[2048] = { 0 };
	libssh2_channel_read(pinfo.channel, bhead, sizeof(bhead));
	static int handle = 0;
	peerlists[++handle] = pinfo;
	return handle;
}

int cssh::execute(int handle, const char* command, int callback) {
	return g_thread->message_loop_proxy()->PostTask(FROM_HERE,
						 base::Bind(&cssh::do_execute, base::Unretained(this), handle, command, callback));
}

void cssh::do_execute(int handle, const char * command, int callback ){
	std::map<int, peerinfo>::iterator ifnd = peerlists.find(handle);
	if (ifnd == peerlists.end())
		return;
  static char retbuf[10240] = { 0 };
	memset(retbuf, 0x00, sizeof(retbuf));
	libssh2_channel_write(ifnd->second.channel, command, strlen(command));
	libssh2_channel_read(ifnd->second.channel, retbuf, sizeof(retbuf));
	if (0 == luaL_lock(theState)) {
		lua_rawgeti(theState, LUA_REGISTRYINDEX, callback);
		lua_pushstring(theState, retbuf);
		lua_pcall(theState, 1, 0, 0);
		luaL_unref(theState, LUA_REGISTRYINDEX, callback);
		luaL_unlock(theState);
	}
}

int cssh::shutdown( int handle ) {
	std::map<int, peerinfo>::iterator ifnd = peerlists.find(handle);
	if (ifnd == peerlists.end())
		return -1;
	libssh2_channel_close(ifnd->second.channel);
	libssh2_session_disconnect(ifnd->second.session, "Thank you for playing");
	libssh2_session_free(ifnd->second.session);
#ifdef _WIN32
	closesocket(ifnd->second.sock);
#else
	close(ifnd->second.sock);
#endif
	peerlists.erase(ifnd->first);
	return 0;
}

#include <stdio.h>
//#include <tchar.h>
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

#include "scp.h"
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

extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

scoped_ptr<base::Thread> scp_thread = NULL;
extern lua_State * theState;

void cscp::start() {
#ifdef _WIN32
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata);
#endif
	base::Thread::Options options;
	options.message_loop_type = base::MessageLoop::TYPE_IO;
	scp_thread.reset(new base::Thread("SCP_Thread"));
	scp_thread->StartWithOptions(options);
}

void cscp::stop() {
	scp_thread->Stop();
	scp_thread.reset();
#ifdef _WIN32
	WSACleanup();
#endif
}

int cscp::login(const char* ip, unsigned short port, const char* user, const char* pwd) {
	struct sockaddr_in sshserver;
	sshserver.sin_family = AF_INET;
	sshserver.sin_port = htons(22);
	if ((sshserver.sin_addr.s_addr = inet_addr(ip)) == INADDR_NONE) {
		fprintf(stderr, "\nthe address is invalid!/n");
		return -1;
	}

	int sock;
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		fprintf(stderr, "\nfailed to create a socket!/n");
		return -1;
	}

	if (connect(sock, (struct sockaddr*)(&sshserver), sizeof(struct sockaddr_in)) != 0) {
		fprintf(stderr, "\nfailed to connect,please check your username or passwd!/n");
#ifdef _WIN32
		closesocket(sock);
#else
		close(sock);
#endif
		return -1;
	}

	LIBSSH2_SESSION   * session;
	if (!(session = libssh2_session_init())) {
		printf("init SSH session failed!/n");
#ifdef _WIN32
		closesocket(sock);
#else
		close(sock);
#endif
		return -1;
	}
	libssh2_session_handshake(session, sock);
	libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

	char * authlist = libssh2_userauth_list(session, user, strlen(user));
	if (strstr(authlist, "password") == NULL) {
		libssh2_session_free(session);
#ifdef _WIN32
		closesocket(sock);
#else
		close(sock);
#endif
		return -1;
	}

	libssh2_userauth_password(session, user, pwd);

	/* Request a file via SCP */
	struct stat fileinfo;
	const char *scppath = "/tmp/TEST";
	LIBSSH2_CHANNEL * channel = libssh2_scp_recv(session, scppath, &fileinfo);

	if (!channel) {
		fprintf(stderr, "Unable to open a session: %d\n",
			libssh2_session_last_errno(session));

		//goto shutdown;
	}

	off_t got = 0;
	while (got < fileinfo.st_size) {
		char mem[1024];
		int amount = sizeof(mem);
		if ((fileinfo.st_size - got) < amount) {
			amount = fileinfo.st_size - got;
		}
		int rc = libssh2_channel_read(channel, mem, amount);
		if (rc > 0) {
			write(1, mem, rc);
		}
		else if (rc < 0) {
			fprintf(stderr, "libssh2_channel_read() failed: %d\n", rc);
			break;
		}
		got += rc;
	}
//shutdown:
	libssh2_channel_free(channel);
	libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
	libssh2_session_free(session);
#ifdef _WIN32
	closesocket(sock);
#else
	close(sock);
#endif
	return 0;
}

int cscp::execute(int handle, const char* command, int callback) {
	return scp_thread->message_loop_proxy()->PostTask(FROM_HERE,
		base::Bind(&cscp::do_execute, base::Unretained(this), handle, command, callback));
}

void cscp::do_execute(int handle, const char * command, int callback) {
	if (0 == luaL_lock(theState)) {
		lua_rawgeti(theState, LUA_REGISTRYINDEX, callback);
		//lua_pushstring(theState, retbuf);
		lua_pcall(theState, 1, 0, 0);
		luaL_unref(theState, LUA_REGISTRYINDEX, callback);
		luaL_unlock(theState);
	}
}

/*
int main(int argc, char *argv[])
{
	unsigned long hostaddr;
	int sock, i, auth_pw = 1;
	struct sockaddr_in sin;
	const char *fingerprint;
	LIBSSH2_SESSION *session;
	LIBSSH2_CHANNEL *channel;
	const char *username = "username";
	const char *password = "password";
	const char *scppath = "/tmp/TEST";
	struct stat fileinfo;
	int rc;
	off_t got = 0;

#ifdef WIN32
	WSADATA wsadata;

	WSAStartup(MAKEWORD(2, 0), &wsadata);
#endif

	if (argc > 1) {
		hostaddr = inet_addr(argv[1]);
	}
	else {
		hostaddr = htonl(0x7F000001);
	}
	if (argc > 2) {
		username = argv[2];
	}
	if (argc > 3) {
		password = argv[3];
	}
	if (argc > 4) {
		scppath = argv[4];
	}

	rc = libssh2_init(0);

	if (rc != 0) {
		fprintf(stderr, "libssh2 initialization failed (%d)\n", rc);
		return 1;
	}

	///* Ultra basic "connect to port 22 on localhost"
	 * Your code is responsible for creating the socket establishing the
	 * connection
	 ///
	sock = socket(AF_INET, SOCK_STREAM, 0);

	sin.sin_family = AF_INET;
	sin.sin_port = htons(22);
	sin.sin_addr.s_addr = hostaddr;
	if (connect(sock, (struct sockaddr*)(&sin),
		sizeof(struct sockaddr_in)) != 0) {
		fprintf(stderr, "failed to connect!\n");
		return -1;
	}

	/ Create a session instance
	session = libssh2_session_init();

	if (!session)
		return -1;

	// ... start it up. This will trade welcome banners, exchange keys,
	// and setup crypto, compression, and MAC layers
	//
	rc = libssh2_session_handshake(session, sock);

	if (rc) {
		fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
		return -1;
	}

	/* At this point we havn't yet authenticated.  The first thing to do
	 * is check the hostkey's fingerprint against our known hosts Your app
	 * may have it hard coded, may go to a file, may present it to the
	 * user, that's your call
	 /
	fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

	fprintf(stderr, "Fingerprint: ");
	for (i = 0; i < 20; i++) {
		fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
	}
	fprintf(stderr, "\n");

	if (auth_pw) {
		/* We could authenticate via password /
		if (libssh2_userauth_password(session, username, password)) {

			fprintf(stderr, "Authentication by password failed.\n");
			goto shutdown;
		}
	}
	else {
		// Or by public key 
		if (libssh2_userauth_publickey_fromfile(session, username,

			"/home/username/.ssh/id_rsa.pub",
			"/home/username/.ssh/id_rsa",
			password)) {
			fprintf(stderr, "\tAuthentication by public key failed\n");
			goto shutdown;
		}
	}

	// Request a file via SCP 
	channel = libssh2_scp_recv(session, scppath, &fileinfo);


	if (!channel) {
		fprintf(stderr, "Unable to open a session: %d\n",
			libssh2_session_last_errno(session));

		goto shutdown;
	}


	while (got < fileinfo.st_size) {
		char mem[1024];
		int amount = sizeof(mem);

		if ((fileinfo.st_size - got) < amount) {
			amount = fileinfo.st_size - got;
		}

		rc = libssh2_channel_read(channel, mem, amount);

		if (rc > 0) {
			write(1, mem, rc);
		}
		else if (rc < 0) {
			fprintf(stderr, "libssh2_channel_read() failed: %d\n", rc);

			break;
		}
		got += rc;
	}

	libssh2_channel_free(channel);

	channel = NULL;

shutdown:

	libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");

	libssh2_session_free(session);


	closesocket(sock);
	fprintf(stderr, "all done\n");

	libssh2_exit();


	return 0;
}

*/
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
#include "ssh.h"

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

int cssh::login(const char* ip, unsigned short port, const char* user, const char* pwd ) {
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

static int name(LIBSSH2_SESSION *session, unsigned char **sig, size_t *sig_len,
	const unsigned char *data, size_t data_len, void **abstract) {

	printf((char*)data);
	return 0;
}

int cssh::login_pubkey(const char* ip, unsigned short port, const char* user, const char* key) {
	printf("cssh::login_publickey\n");
	peerinfo pinfo;
	struct sockaddr_in sshserver;
	sshserver.sin_family = AF_INET;
	sshserver.sin_port = htons(port);
	if ((sshserver.sin_addr.s_addr = inet_addr(ip)) == INADDR_NONE) {
		printf("the address is invalid!/n");
		return -1;
	}

	if ((pinfo.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("failed to create a socket!/n");
		return -1;
	}

	if (connect(pinfo.sock, (struct sockaddr*)(&sshserver), sizeof(struct sockaddr_in)) != 0) {
		printf("failed to connect,please check your username or passwd!/n");
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
	libssh2_hostkey_hash(pinfo.session, LIBSSH2_HOSTKEY_HASH_MD5);
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

	const char private_key[] ="-----BEGIN RSA PRIVATE KEY----- \
MIIEowIBAAKCAQEA3QugLJtb4dlQWEYjBIT1yM6yHKlAhFKGo + q4BSoQhyoRcsWP \
lAN / wHc49HVwm1xeMPYeqZCOrUMBMcJMamv46e5XQh / zmLGmpT2tqsntVopQFg / 1 \
O58PSOTAYrUzkFuo05BLVWNZn51p3WkSGkabGxXpIVxCpdjqbCp3zK118dkVJEK7 \
kSK6tlmtMRO2QyBe0lcsdHD6CSDIWp44eRHJwMqbjULcAnMsXIm + yMTYc2pMQ6Wa \
E6EVzyL23vIOJwx / gr1TrNpqyl / G93p3rIrsZ3Yh9daZSG6XCpu03FEmjtgQ8LGN \
eOK6XAjDqTZUsOA / 2tcQJWkbYjRrPM7b8KvaewIDAQABAoIBABzEU48alOWak4M5 \
dXs0FoyQoctrF3NhDhatCE / bvpTom6a1IU9g + TJCEc52rjX52mRC05C7uGXCD5i7 \
MZiz / ovrMb3wTJJ1pDtjAIP2jYWysGN + piqhM6sH4iW2cUR7OUh3QeaV7GcCZHGl \
J + 71e2AWBOYLjLzqsu9qw3aGqI / Vs5a5zePF6HxHVoX6Ml3XlHtKNXlFVrwoaoHo \
kB + hdPjEhvqfZT2A2nmabj9ZQ8fpO0pZOLVs0zde6SenDL6M7CSEZbsi / Irsnwnq \
r7WNHzUfUi + e0SV + oIaBlNb2fQBmJRuZlV / YuXEbvzHaKQl6JsS9vN4epA9rTkXJ \
5OpSr / ECgYEA + ZBt95kI3MQMwpjZqnDEtWz28kjDoqfxhgksToYYKYS8EhqcW15a \
5AT0ZhTVeu6SwV8fMZ6naHCCBaQZGI29mH7LfXMBmLvWxmlPCxmO2C / qpCIgfW8 / \
pP04Tpo / Sad7WFhOoaNQ624x2NaCyV0i2LMiPcs8po + 9K5RnO6XtidMCgYEA4r7r \
1Cj4z / vZwPvqb3idNIzx / QPh7YfuBBY5CLV8iMk0F / SqSd3AVqz6xcE / FPd / 87fv \
4T + 2VXL5ps4gSy8vL30wprnPdySudllZ3nHiCHGUGLzHlau / QhlUxYpGz5STZMU1 \
W95FYX + eLetNnaW0sO + QCOTrOlIUY0DJg8VeG7kCgYBJz2ymuS1E1fsMJtbhsP6M \
xhB3cgvidJdbVCU1iDVnOinBlPs / jrWFEO4Trv / AnwTqii6Yp1rN6dCayIDfoTQ9 \
AOtATB5hxNRt1EGbf9CeofVc2 / cOAjQpoPbhUH99UjqJ03xWFA5fwzSc9Dy0P4aJ \
3tk2u4BRptuFIxwxKQ8LHwKBgQDTMofQppUSjIAHu91iFNNApmOjiqE + sNSm49gd \
34VulxI5mf + fK7hCQ36VAely6xE3 / kSl5sU7lAsrLmVwAjQOJKpTMVEnpXgG7jTB \
+ AtUWA5ceEHijAVqWNPX53MLWFVtkC6iMkcniEnadYhm5XiHMaxUTH05ZkhtJmRM \
o2KDqQKBgBbogwC / jW / kmu1G7P544sv3jRbxdhkbDNqBB//N9enewu+XRfVcnmOk \
fFyEOO6TQUvbqt7jf5XTxA + FFtXmdtsE8AT4L / CelI2QdAY9c8uqY0 + 6CyOnAQzL \
+ 2dwMZOyYv97o4yC + fZibKN86zw0rpYNrApQlDGqcEdc3bImxgrS \
---- - END RSA PRIVATE KEY---- - ";

	
	char retval[1024] = { 0 };
	//const char public_key[] = "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDdC6Asm1vh2VBYRiMEhPXIzrIcqUCEUoaj6rgFKhCHKhFyxY+UA3/Adzj0dXCbXF4w9h6pkI6tQwExwkxqa/jp7ldCH/OYsaalPa2qye1WilAWD/U7nw9I5MBitTOQW6jTkEtVY1mfnWndaRIaRpsbFekhXEKl2OpsKnfMrXXx2RUkQruRIrq2Wa0xE7ZDIF7SVyx0cPoJIMhanjh5EcnAypuNQtwCcyxcib7IxNhzakxDpZoToRXPIvbe8g4nDH+CvVOs2mrKX8b3enesiuxndiH11plIbpcKm7TcUSaO2BDwsY144rpcCMOpNlSw4D/a1xAlaRtiNGs8ztvwq9p7 root@sg-monitor";
	const char public_key[] = "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDdC6Asm1vh2VBYRiMEhPXIzrIcqUCEUoaj6rgFKhCHKhFyxY+UA3/Adzj0dXCbXF4w9h6pkI6tQwExwkxqa/jp7ldCH/OYsaalPa2qye1WilAWD/U7nw9I5MBitTOQW6jTkEtVY1mfnWndaRIaRpsbFekhXEKl2OpsKnfMrXXx2RUkQruRIrq2Wa0xE7ZDIF7SVyx0cPoJIMhanjh5EcnAypuNQtwCcyxcib7IxNhzakxDpZoToRXPIvbe8g4nDH+CvVOs2mrKX8b3enesiuxndiH11plIbpcKm7TcUSaO2BDwsY144rpcCMOpNlSw4D/a1xAlaRtiNGs8ztvwq9p7 root@sg-monitor";
	
	
	//int result = libssh2_userauth_publickey_frommemory(pinfo.session, 
	//	user, strlen(user), 
	//	key2, strlen(key2),
	//	key1, strlen(key1), 
	//	retval);

	//int result = libssh2_userauth_publickey_fromfile_ex(pinfo.session,
	//	user, strlen(user),
	//	"D:\\Quantum_Project_v1.0\\thirdlibs\\libssh2-1.9.0\\tests\\id_rsa_1.pub", 
	//	"D:\\Quantum_Project_v1.0\\thirdlibs\\libssh2-1.9.0\\tests\node_id_rsa",
	//	NULL);

	size_t len;
	int type;
	int result = 0;
	unsigned int expected_len = 0;
	char *expected_hostkey = NULL;

	const char *hostkey = libssh2_session_hostkey(pinfo.session, &len, &type);
	if (hostkey == NULL) {
		printf("libssh2_session_hostkey");
		return 1;
	}
	if (type == LIBSSH2_HOSTKEY_TYPE_RSA) {
		result = libssh2_base64_decode(pinfo.session, &expected_hostkey, &expected_len,private_key, strlen(private_key));
	}
	else {
		printf( "Unexpected type of hostkey: %i\n", type);
		return 1;
	}

	const char public_file[] = "id_rsa_1.pub";
	const char private_file[] = "node_id_rsa";

	const char rv[1024] = { 0 };
	//result = libssh2_userauth_publickey_frommemory(pinfo.session, 
	//	user, strlen(user), 
	//	public_key, strlen(public_key),
	//	private_key, strlen(private_key),
	//	rv);

	result = libssh2_userauth_publickey_frommemory(pinfo.session,
		user, strlen(user),
		public_file, strlen(public_file),
		private_file, strlen(private_file),
		rv);

	//int result =  libssh2_userauth_publickey(pinfo.session, user, (unsigned char*)key2, strlen(key2), name, 0 );
	if (result != 0)
	{
		char * error = NULL;
		int len = 0;
		int errbuf = 0;
		libssh2_session_last_error(pinfo.session, &error, &len, errbuf);
		printf(error);
	}

	//if (memcmp(hostkey, expected_hostkey, len) != 0) {
	//	fprintf(stderr, "Hostkeys do not match\n");
	//	return 1;
	//}

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


int cssh::login_pubkey_file(const char* ip, unsigned short port, const char* user, const char* file) {
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
	libssh2_hostkey_hash(pinfo.session, LIBSSH2_HOSTKEY_HASH_MD5);
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

	libssh2_userauth_publickey_fromfile(pinfo.session, user, "id_rsa_1.pub","node_id_rsa",file);

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

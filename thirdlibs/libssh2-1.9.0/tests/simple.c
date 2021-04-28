// ssh2client.cpp : 定义控制台应用程序的入口点。

#include <stdio.h>
#include <tchar.h>

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>

#include <libssh2.h>

#define LOGOUT_HZW 1000
#define EXIT_HZW 1001
#define BUFFER_MAX 1024

int client(char ip[], char user[], char pwd[]);

int _tmain(int argc, char* argv[])
{
	char ip[17] = "5.183.101.155";
	char user[50] = "root";
	char pwd[50] = "JIEJHID9@didhi";
	int ret = 0;
//	system("color 72");
//	printf("\t\t\t\t***********SSH CLIENT****************\n");
//	printf("please input remote host's ip,such as [127.0.0.1]: ");
//	scanf_s("%s", ip);
//AGAIN:
//	printf("please input username: ");
//	scanf_s("%s", user);
//	printf("please input pwd: ");
//	getchar();
	char ch;
	int i = 0;
	//while ((ch = _getch()) != 0x0d)
//	{
//		pwd[i++] = ch;
//	}
//	pwd[i] = '\0';
#ifdef WIN32
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata);
#endif
	ret = client(ip, user, pwd);
	switch (ret)
	{
	case EXIT_HZW:
	case LOGOUT_HZW:
		//goto AGAIN;
	default:
		break;
	}
	return 0;
}
int client(char ip[], char user[], char pwd[])
{
	int sock;
	int i;
	int auth_pw = 1; //认证方式
	int rc;//回话ID
	struct sockaddr_in sshserver;
	LIBSSH2_CHANNEL *channel; //通道
	LIBSSH2_SESSION *session;//回话
	const char *fingerprint;
	LIBSSH2_POLLFD *fds = NULL;
	int nfds = 1;
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		fprintf(stderr, "\nfailed to create a socket!/n");
		return -1;
	}
	sshserver.sin_family = AF_INET;
	sshserver.sin_port = htons(22);
	if ((sshserver.sin_addr.s_addr = inet_addr(ip)) == INADDR_NONE)
	{
		fprintf(stderr, "\nthe address is invalid!/n");
		return -1;
	}
	if (connect(sock, (struct sockaddr*)(&sshserver), sizeof(struct sockaddr_in)) != 0)
	{
		fprintf(stderr, "\nfailed to connect,please check your username or passwd!/n");
		return -1;
	}
	else
	{
		// printf("connect success\n");
	}
	if (!(session = libssh2_session_init()))
	{
		fprintf(stderr, "init SSH session failed!/n");
		goto CLOSESOCKET;
	}
	if ((rc = libssh2_session_startup(session, sock)))
	{
		fprintf(stderr, "failure establishing SSH session: %d/n", rc);
		libssh2_session_free(session);
		goto CLOSESOCKET;
	}    

	if ((fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_MD5)))
	{
		//printf("\nFingerprint(HASH_MD5): ");
		//for (i = 0; i < 16; i++)
		//{
		//	printf("%02X ", (unsigned char)fingerprint[i]);
		//}
		//printf("/n");
	}

	char * authlist = libssh2_userauth_list(session, user, strlen(user));
	if (strstr(authlist, "publickey") == NULL) {
		libssh2_session_free(session);
#ifdef _WIN32
		closesocket(sock);
#else
		close(sock);
#endif
		return -1;
	}



	const char private_key[] = "-----BEGIN RSA PRIVATE KEY----- \
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

	const char *hostkey = libssh2_session_hostkey(session, &len, &type);
	if (hostkey == NULL) {
		printf("libssh2_session_hostkey");
		return 1;
	}
	if (type == LIBSSH2_HOSTKEY_TYPE_RSA) {
		result = libssh2_base64_decode(session, &expected_hostkey, &expected_len, private_key, strlen(private_key));
	}
	else {
		printf("Unexpected type of hostkey: %i\n", type);
		return 1;
	}

	result = libssh2_userauth_publickey_frommemory(session,
		user, strlen(user),
		//public_key, strlen(public_key),
		NULL,0,
		private_key, strlen(private_key),
		 NULL);


	
	//char * userauthlist;
	//userauthlist = libssh2_userauth_list(session, user, strlen(user));

	//if (auth_pw)
	//{

	//	if ((libssh2_userauth_password(session, user, pwd)))
	//	{
	//		printf("\nAuthentication by password failed.maby user or pwd is error!/n");
	//		goto SHUTDOWN;
	//	}
	//	else
	//	{
	//		// printf("\nlogin success!\n");
	//	}
	//}


	//if (!strcmp(user, "root"))
	//	printf("[%s@localhost ~]# ", user);
	//else
	//	printf("[%s@localhost ~]$ ", user);
	libssh2_session_set_blocking(session, 1);//设置阻塞模式
	if (!(channel = libssh2_channel_open_session(session)))
	{
		fprintf(stderr, "unable to open a session\n");
		goto SHUTDOWN;
	}

	//libssh2_channel_setenv(channel, "FOO", "bar");

	//if (libssh2_channel_request_pty(channel, "vanilla"))
	//{
	//	fprintf(stderr, "Failed requesting pty\n");
	//	goto SKIP_SHELL;
	//}
	if (libssh2_channel_shell(channel))
	{
		fprintf(stderr, "unable to request shell on allocated pty");
		goto SHUTDOWN;
	}
	//libssh2_channel_set_blocking(channel, 0);


	char bbf1[1024] = { 0 };
	libssh2_channel_read(channel, bbf1, 1024);
	printf(bbf1);



	char * cmd = "dir\n";
	libssh2_channel_write(channel, cmd, 4);

	char bbf2[1024] = { 0 };
	libssh2_channel_read(channel, bbf2, 1024);
	printf(bbf2);

	char * cmd2 = "date\n";
	memset(bbf2, 0x00, sizeof(bbf2));
	libssh2_channel_write(channel, cmd2, 5);
	libssh2_channel_read(channel, bbf2, 1024);
	printf(bbf2);

	int tag = 0;//标志位
	char bufc = '\0';
	while (1)
	{
		if ((fds = (LIBSSH2_POLLFD*)malloc(sizeof(LIBSSH2_POLLFD))) == NULL)
			break;
		fds[0].type = LIBSSH2_POLLFD_CHANNEL;
		fds[0].fd.channel = channel;
		fds[0].events = LIBSSH2_POLLFD_POLLIN;
		fds[0].revents = LIBSSH2_POLLFD_POLLIN;

		if (libssh2_poll(fds, nfds, 0) > 0)
		{
			char bbf[1024] = { 0 };
			libssh2_channel_read(channel, &bbf, 1024);
			printf(bbf);
			//if (tag == 1)
			//{
			//	fprintf(stdout, "%c", bufc);
			//}
			//else if (bufc == 0x3e)
			//{
			//	tag = 0;
			//}
			//fflush(stdout);
		}

		//if (_kbhit())
		//{
		//	if (tag == 0)
		//		tag = 1;
		//	else
		//	{
		//		bufc = getch();
		//		if (bufc == -32)
		//		{
		//			getch();
		//		}
		//		else if (bufc == '\b')
		//		{
		//			printf("\b \b");
		//		}
		//		else
		//		libssh2_channel_write(channel, &bufc, 1);
		//	}
		//}
		free(fds);
		if (libssh2_channel_eof(channel) == 1)
			break;
	}
	libssh2_channel_close(channel);
	//  Sleep(3000);
	return 0;
SKIP_SHELL:
	if (channel)
	{
		libssh2_channel_free(channel);
		channel = NULL;
	}
SHUTDOWN:
	libssh2_session_disconnect(session, "Thank you for playing");
	libssh2_session_free(session);
CLOSESOCKET:
#ifdef WIN32
	Sleep(1000);
	closesocket(sock);
#else
	sleep(1);
	close(sock);
#endif
	return 0;
}

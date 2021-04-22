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
	char ip[17] = "192.241.252.154";
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

	
char * userauthlist;
	userauthlist = libssh2_userauth_list(session, user, strlen(user));


	if (auth_pw)
	{

		if ((libssh2_userauth_password(session, user, pwd)))
		{
			printf("\nAuthentication by password failed.maby user or pwd is error!/n");
			goto SHUTDOWN;
		}
		else
		{
			// printf("\nlogin success!\n");
		}
	}
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

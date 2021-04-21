#include <stdio.h>
#include "netcard.h"
 
int cnetcard::getnetcards(std::vector<netcardinfo>& v ){
#ifdef _WIN32
	HANDLE   hMprConfig;   
	DWORD   dwRet=0;   
	PIP_INTERFACE_INFO   plfTable=NULL;   
	IP_ADAPTER_INDEX_MAP   AdaptMap;   
	DWORD   dwBufferSize=0;   
	WCHAR   szFriendName[256];   
	DWORD   tchSize=sizeof(WCHAR)*256;   
	ZeroMemory(&szFriendName,tchSize);   
	dwRet=MprConfigServerConnect   (NULL,&hMprConfig);   
	dwRet=GetInterfaceInfo(NULL,&dwBufferSize);   
	if(dwRet==ERROR_INSUFFICIENT_BUFFER)   {   
		plfTable=(PIP_INTERFACE_INFO)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwBufferSize);   
		GetInterfaceInfo(plfTable,&dwBufferSize);   
	}      
	for(long   i=0;i<plfTable->NumAdapters;i++){   
		AdaptMap=plfTable->Adapter[i];   
		dwRet=MprConfigGetFriendlyName(hMprConfig,AdaptMap.Name,(PWCHAR)szFriendName,tchSize);   
		netcardinfo netcard;
		memset(&netcard, 0, sizeof(netcardinfo) );
		int nLen = WideCharToMultiByte( CP_ACP,NULL, szFriendName,-1,NULL,0,NULL,FALSE );
		WideCharToMultiByte (CP_ACP,NULL,szFriendName,-1, netcard.name,nLen,NULL,FALSE);
		printf("insert:cardname=%s", netcard.name);
		v.push_back(netcard);
	}   
	HeapFree(GetProcessHeap(),HEAP_ZERO_MEMORY,plfTable);   
#else
	int i=0;
	int sockfd;
	struct ifconf ifconf;
	char buf[512];
	struct ifreq *ifreq;
	ifconf.ifc_len = 512;
	ifconf.ifc_buf = buf;
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0){
		printf("socket crate error");
		return -1;
	}
	ioctl(sockfd, SIOCGIFCONF, &ifconf);
	ifreq = (struct ifreq*)buf;  
//	ioctl(sockfd, SIOCGIFNETMASK, &ifconf);
//	ifreq = (struct ifreq*)buf;
	for(i=(ifconf.ifc_len/sizeof(struct ifreq)); i>0; i--){
		//printf("local addr = [%s]\n", inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
		netcardinfo netcard;
		memset(&netcard, 0x00, sizeof(netcard));
		strcpy(netcard.name, ifreq->ifr_name);
		strcpy(netcard.ip, inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
		strcpy(netcard.mask, inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_netmask))->sin_addr));
		strcpy(netcard.gate, inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_broadaddr))->sin_addr));
		v.push_back(netcard);
		ifreq++;
	}
	close(sockfd);
#endif
	return 0;
}


int cnetcard::setip(const char * card, const char* ip, const char* mask, const char* gate) {
	int nRet = -1;
#ifdef _WIN32
	char szcmd[1024] = { 0 };
	sprintf_s(szcmd, "netsh interface ip set address \"%s\" static %s %s %s", card, ip, mask, gate);
	system(szcmd);
	printf(szcmd);
	::Sleep(500);
	char host_name[255];
	if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR) {
		nRet = WSAGetLastError();
		printf("Error %d when getting local host name.", nRet);
		return nRet;
	}
	unsigned long ipcode = inet_addr(ip);
	for (int i = 0; i<10; i++) {
		struct hostent *phe = gethostbyname(host_name);
		if (phe == 0) {
			printf("Yow! Bad host lookup.");
		}
		for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
			struct in_addr addr;
			memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
			if (ipcode == inet_addr(inet_ntoa(addr))) {
				printf("show:successall=0");
				return 0;
			}
		}
		::Sleep(1000);
	}
	return nRet;
#else
	int fd;
	struct ifreq ifr;
	struct sockaddr_in *sin;
	struct rtentry  rt;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		printf("socket   error");
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, card);
	sin = (struct sockaddr_in*)&ifr.ifr_addr;
	sin->sin_family = AF_INET;
	//ipaddr
	if (inet_aton(ip, &(sin->sin_addr)) < 0)
	{
		printf("inet_aton   error");
		return -2;
	}

	if (ioctl(fd, SIOCSIFADDR, &ifr) < 0)
	{
		printf("ioctl   SIOCSIFADDR   error");
		return -3;
	}
	//netmask
	if (inet_aton(mask, &(sin->sin_addr)) < 0)
	{
		printf("inet_pton   error");
		return -4;
	}
	if (ioctl(fd, SIOCSIFNETMASK, &ifr) < 0)
	{
		perror("setmask");
		return -5;
	}
	//gateway
	memset(&rt, 0, sizeof(struct rtentry));
	memset(sin, 0, sizeof(struct sockaddr_in));
	sin->sin_family = AF_INET;
	sin->sin_port = 0;

	if (inet_aton(gate, &sin->sin_addr)<0)
	{
		printf("inet_aton error");
	}
	memcpy(&rt.rt_gateway, sin, sizeof(struct sockaddr_in));
	((struct sockaddr_in *)&rt.rt_dst)->sin_family = AF_INET;
	((struct sockaddr_in *)&rt.rt_genmask)->sin_family = AF_INET;
	rt.rt_flags = RTF_GATEWAY;

	if (ioctl(fd, SIOCADDRT, &rt) < 0)
	{
		printf("ioctl error\n");
		close(fd);
		return -1;
	}
	close(fd);
	nRet = 0;
#endif
	return nRet;
}



int cnetcard::IsNetwork(){
	int bOnline = 1;
#ifdef _WIN32
	CoInitialize(NULL);
	IUnknown *pUnknown = NULL;
	HRESULT Result = CoCreateInstance(CLSID_NetworkListManager, NULL, CLSCTX_ALL, IID_IUnknown, (void **)&pUnknown);
	if (SUCCEEDED(Result))
	{
		INetworkListManager *pNetworkListManager = NULL;
		if (pUnknown)
			Result = pUnknown->QueryInterface(IID_INetworkListManager, (void **)&pNetworkListManager);
		if (SUCCEEDED(Result)){
			VARIANT_BOOL IsConnect = VARIANT_FALSE;
			if (pNetworkListManager)
				Result = pNetworkListManager->get_IsConnectedToInternet(&IsConnect);
			if (SUCCEEDED(Result)){
				bOnline = (IsConnect == VARIANT_TRUE) ? 1 : 0;
			}
		}
		if (pNetworkListManager)
			pNetworkListManager->Release();
	}
	if (pUnknown)
		pUnknown->Release();
	CoUninitialize();
#endif
	return bOnline;
}
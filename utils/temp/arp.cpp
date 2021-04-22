#include "netcapture.h"
#include "define.h"

//����ARP��
int netcapture::sendpkg(pcap_t* hdr, unsigned int nhost){
	arp_packet ipkg;
	memset(ipkg.eth.dest_mac,0xFF,6);           //Ŀ��MAC��ַΪ�㲥��ַ��FF-FF-FF-FF-FF-FF 
	memcpy(ipkg.eth.source_mac,m_macAddr,6);     //ԴMAC��ַ 
	ipkg.eth.eh_type = htons(0x0806);           //�ϲ�Э��ΪARPЭ�飬0x0806 
	//ARPͷ
	ipkg.arp.hardware_type = htons(0x0001);		//Ӳ�����ͣ�Ethernet��0x0001
	ipkg.arp.protocol_type = htons(0x0800);		//�ϲ�Э�����ͣ�IPΪ0x0800 
	ipkg.arp.add_len = 0x06;					//Ӳ����ַ���ȣ�MAC��ַ����Ϊ0x06 
	ipkg.arp.pro_len = 0x04; 					//Э���ַ���ȣ�IP��ַ����Ϊ0x04 
	ipkg.arp.option = htons(0x0001); 			//������ARP����Ϊ1 
	memcpy(ipkg.arp.sour_addr,m_macAddr,6);		//ԴMAC��ַ 
	ipkg.arp.sour_ip = 0;						//ԴIP��ַ 
	memset(ipkg.arp.dest_addr,0,6); 			//Ŀ��MAC��ַ�����0 
	ipkg.arp.dest_ip = nhost;
	memset(ipkg.arp.padding,0,18);				//������ݣ�18B 
//#ifdef WIN32
//	struct sockaddr_in dest;
//	memset(&dest, 0, sizeof(dest));
//	dest.sin_family = AF_INET;
//	dest.sin_port = htons(0);
//	dest.sin_addr.s_addr = ipkg.arp.dest_ip;
//	int n = sendto(m_socket, (const char*)&ipkg, sizeof(arp_packet), 0, (struct sockaddr*)&dest, sizeof(sockaddr_in));
//	printf("sendto:%d\n",n);
//#else
	if( pcap_sendpacket(hdr, (unsigned char*)&ipkg, sizeof(arp_packet)) == -1){ 
		fprintf(stderr,"pcap_sendpacket error.\n"); 
		return -1;
	}
//#endif
	return 0;
}

//arp
int netcapture::doArp(const char* ip, unsigned char* mac){
	int nRet = -1;
	unsigned int starttime = GetTickCount();
	unsigned int destaddr  = inet_addr(ip);
	m_usedtime = 0;
	//��pcap
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t*   adhandle;
	if ( (adhandle= pcap_open_live(m_netcard,65536,1,1000,errbuf) ) == NULL){
		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", m_netcard);
		return nRet;
	}

//#ifdef WIN32
//	WSADATA wsdata;
//	WSAStartup(0x0202,&wsdata);
//	m_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
//	int timeout = 1000;
//	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
//	timeout = 1000;
//	setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
//#endif

	//���ù�����
	struct bpf_program fp;
	char filter_exp[] = "arp";
	if (pcap_compile(adhandle, &fp, filter_exp, 0, 0) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(adhandle));
	}else{
		if (pcap_setfilter(adhandle, &fp) == -1) {
			fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(adhandle));
			return -1;
		}else{
			while( starttime + /*m_timeout*/10000 > GetTickCount() ){
				sendpkg(adhandle, destaddr );
				u_char *pkt_data;
				struct pcap_pkthdr *header;
				if( pcap_next_ex(adhandle, &header,(const u_char**)&pkt_data) <= 0)
					continue;
				arp_packet pkg;
				memset(&pkg, 0x00, sizeof(arp_packet));
				memcpy(&pkg, pkt_data, sizeof(arp_packet));
				in_addr ina;
				ina.s_addr = pkg.arp.sour_ip;
				printf("%s\n", inet_ntoa(ina));
				if( destaddr == pkg.arp.sour_ip ){
					nRet = 0;
					memcpy( mac, pkg.arp.sour_addr, 6);
					break;
				}
				//char recvbuf[MAX_PACKET];
				//struct sockaddr_in from;
				//int fromlen = sizeof(from);
				//int nrecv = ::recvfrom(m_socket,recvbuf, MAX_PACKET, 0, (struct sockaddr*)&from, (socklen_t*)&fromlen);
				//if( nrecv != -1){
				//	printf("%d", from.sin_addr.S_un.S_addr );
				//	if( destaddr == from.sin_addr.S_un.S_addr ){
				//		nRet = 0;
				//		break;
				//	}
				//}
			}
		}
	}
	m_usedtime = starttime - GetTickCount();
//#ifdef WIN32
//	closesocket(m_socket);
//	WSACleanup();
//#endif
	pcap_close(adhandle);
	return nRet;
}









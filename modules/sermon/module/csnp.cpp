#include "csnp.h"
#include "../kernel/Log.h"
#include "../kernel/datamgr.h"

#ifdef WIN32
#include <winsock2.h>
#endif

/* 4 bytes IP address */
typedef struct ip_address {
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header {
	u_char  ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
	u_char  tos;            // Type of service 
	u_short tlen;           // Total length 
	u_short identification; // Identification
	u_short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
	u_char  ttl;            // Time to live
	u_char  proto;          // Protocol
	u_short crc;            // Header checksum
	ip_address  saddr;      // Source address
	ip_address  daddr;      // Destination address
	u_int   op_pad;         // Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header {
	u_short sport;          // Source port
	u_short dport;          // Destination port
	u_short len;            // Datagram length
	u_short crc;            // Checksum
}udp_header;

// TCP首部
typedef struct tcp_header {
	unsigned short  source_port;       // (16 bits)                         Winsock 内置函数 ntohs（），主要作用将大端转换为小端！
	unsigned short  destination_port;  // (16 bits)                         Winsock 内置函数 ntohs（），主要作用将大端转换为小端！
	unsigned long	seq_number;        // Sequence Number (32 bits)         大小端原因，高低位4个8bit的存放顺序是反的，intel使用小端模式
	unsigned long	ack_number;        // Acknowledgment Number (32 bits)   大小端原因，高低位4个8bit的存放顺序是反的，intel使用小端模式
	unsigned short  info_ctrl;         // Data Offset (4 bits), Reserved (6 bits), Control bits (6 bits)                intel使用小端模式
	unsigned short  window;            // (16 bits)
	unsigned short  checksum;          // (16 bits)
	unsigned short  urgent_pointer;    // (16 bits)
} tcp_header;

//获得网卡的MAC地址/网卡的设备名称 
#ifdef WIN32
#include <Packet32.h>
int getmacbydevname(char* pDevName, unsigned char *mac) {
	memset(mac, 0, sizeof(mac));
	LPADAPTER lpAdapter = PacketOpenAdapter(pDevName);
	if (!lpAdapter || (lpAdapter->hFile == INVALID_HANDLE_VALUE)) {
		return -1;
	}
	PPACKET_OID_DATA OidData = (PPACKET_OID_DATA)malloc(6 + sizeof(PACKET_OID_DATA));
	if (OidData == NULL) {
		PacketCloseAdapter(lpAdapter);
		return -1;
	}
	// Retrieve the adapter MAC querying the NIC driver 
	OidData->Oid = 0x01010102;//OID_802_3_CURRENT_ADDRESS; 
	OidData->Length = 6;
	memset(OidData->Data, 0, 6);
	BOOLEAN Status = PacketRequest(lpAdapter, FALSE, OidData);
	if (Status) {
		memcpy(mac, (u_char*)(OidData->Data), 6);
	}
	free(OidData);
	PacketCloseAdapter(lpAdapter);
	return 0;
}
#else
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
int getmacbyiface(const char *iface, int *ifindex, unsigned int *addr, unsigned char *mac)
{
	int fd;
	struct ifreq ifr;
	struct sockaddr_in *our_ip;
	memset(&ifr, 0, sizeof(ifr));
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, iface, 16);
	if (addr) {
		if (ioctl(fd, SIOCGIFADDR, &ifr) != 0) {
			::close(fd);
			return -1;
		}
		our_ip = (struct sockaddr_in *) &ifr.ifr_addr;
		*addr = our_ip->sin_addr.s_addr;
		printf("ip of %s = %s \n", iface, inet_ntoa(our_ip->sin_addr));
	}
	if (ifindex) {
		if (ioctl(fd, SIOCGIFINDEX, &ifr) != 0) {
			::close(fd);
			return -1;
		}
		printf("adapter index %d", ifr.ifr_ifindex);
		*ifindex = ifr.ifr_ifindex;
	}
	if (mac) {
		if (ioctl(fd, SIOCGIFHWADDR, &ifr) != 0) {
			::close(fd);
			return -1;
		}
		memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
	}
	::close(fd);
	return 0;
}
#endif

//获取网卡列表
void getNetCardList() {
	pcap_if_t *alldevs;
	pcap_if_t *d;
	char errbuf[PCAP_ERRBUF_SIZE];
	if (pcap_findalldevs(&alldevs, errbuf) == -1) {
		printf("Error in pcap_findalldevs: %s\n", errbuf);
		return;
	}
	for (d = alldevs; d; d = d->next) {
		unsigned long ip = 0;
		unsigned long netmask = 0;
		unsigned char mac[6] = {0};
		for (pcap_addr_t * px = d->addresses; px; px = px->next) {
			ip = ((struct sockaddr_in*)px->addr)->sin_addr.s_addr;
#ifdef WIN32
			netmask = ((struct sockaddr_in*)px->netmask)->sin_addr.s_addr;
#else
			netmask = 0x00FFFFFF;
#endif
			if (ip && netmask)
				break;
		}
		//获取网卡MAC地址，
#ifdef WIN32 
		getmacbydevname(d->name, mac);
#else
		getmacbyiface(d->name, NULL, (unsigned int*)&ip, mac);
#endif
		printf("[%s] %02X-%02X-%02X-%02X-%02X-%02X %s\n",
				d->name,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],d->description);
	}
	pcap_freealldevs(alldevs);
}

//执行任务
void csnp::dotask(task * tsk) {
	switch (tsk->mark) {
	case fly_back:
		flyback( tsk );
		return;
	case close_channel:
		close();
		return;
	default:
		break;
	}
	xbase::dotask(tsk);
}

//模块循环工作，如监听，状状检测等
void csnp::dowork() {
	if ( mystatus == NORMAL) {
		char buf[DATAMAXSIZE] = { 0 };
		int len = readdata(buf, sizeof(buf));
		if( len > 0 ){
			task tsk;
			tsk.datadir = dir_recv;
			tsk.trigger = this;
			tsk.mark = recv_data;
			tsk.datastamp = get_time_scale();
			tsk.retcode = 0;
			memcpy(tsk.buffer, buf, len);
			tsk.buflen = len;
			observer->addtask(&tsk);
		}
		return;
	}

	if (mystatus ==  INITIAL) {
		open(myparams);
		return;
	}

	if (mystatus == ABNORMAL) {
		open(myparams);
		return;
	}

}

//打开串口
bool csnp::open( char* param ){
	char name[100];
	char rule[100];
	if (2 != sscanf(param, "%[^,],%[^\n]", name, rule)) {
		LOG(eParameterError, "analyze param:%s error!", param);
		return false;
	}
	mystatus = INITIAL;
	//打开pcap
	char errbuf[PCAP_ERRBUF_SIZE];
	if ((adhandle = pcap_open_live(name, 65536, 1, 1000, errbuf)) == NULL) {
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", name);
		return false;
	}
	//设置过滤器
	struct bpf_program fp;
	//char filter_exp[] = "ether proto\\arp";
	if (pcap_compile(adhandle, &fp, rule, 0, 0) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", rule, pcap_geterr(adhandle));
		return false;
	}
	if (pcap_setfilter(adhandle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", rule, pcap_geterr(adhandle));
		return false;
	}
	mystatus = NORMAL;
	return true;
}

//关闭模块
bool csnp::close(){
	mystatus = REMOVE;
	stop();
	if( rcitem.size()>0){
		CRawDataDB dbfile;
		dbfile.SetDir( mypath );
		dbfile.Save( &rcitem );
		rcitem.clear();
	}
	pcap_close(adhandle);
	return true;
}

//重新设置串口参数
int csnp::setparam(char* args, int len){
	LOG(eTipMessage,"%s setparam:%s",myname,args);
	//原参数表，更新参数表
	char name[100];
	char rule[100];
	if (2 != sscanf(args, "%[^,],%[^\n]", name, rule)) {
		LOG(eParameterError, "analyze param:%s error!", args);
	}
	//重新打开
	if( mystatus == NORMAL ){
		pcap_close(adhandle);
		mystatus = INITIAL;
	}
	//打开pcap
	char errbuf[PCAP_ERRBUF_SIZE];
	if ((adhandle = pcap_open_live(name, 65536, 1, 1000, errbuf)) == NULL) {
		printf("\nUnable to open the adapter. %s is not supported by WinPcap\n", name);
		return 0;
	}
	//设置过滤器
	struct bpf_program fp;
	//char filter_exp[] = "ether proto\\arp";
	if (pcap_compile(adhandle, &fp, rule, 0, 0) == -1) {
		printf("Couldn't parse filter %s: %s\n", rule, pcap_geterr(adhandle));
		return 0;
	}
	if (pcap_setfilter(adhandle, &fp) == -1) {
		printf("Couldn't install filter %s: %s\n", rule, pcap_geterr(adhandle));
		return 0;
	}
	mystatus = NORMAL;
	return 0;
}

int csnp::readdata(char* data, int len) {
	int package = 0;
	unsigned char *pkt_data;
	struct pcap_pkthdr *header;
	package = pcap_next_ex(adhandle, &header, (const unsigned char**)&pkt_data);
	if (package <= 0 ||  header->len >= DATAMAXSIZE)
		return 0;
	struct tm *ltime;
	char timestr[16];
	time_t local_tv_sec = header->ts.tv_sec;
	ltime = localtime(&local_tv_sec);
	strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);
	/* retireve the position of the ip header */
	ip_header * ih = (ip_header *)(pkt_data +14); //length of ethernet header/* retireve the position of the tcp header *///从IPV4首部中取出"首部长度(4 bits)"
	unsigned int ip_len = (ih->ver_ihl & 0xf) * 4;
	//强制类型转换，便于用自己的命名处理
	tcp_header * th = (tcp_header *)((u_char*)ih + ip_len);
	/* print ip addresses and udp ports */
	printf("%s.%.6d len(%d) %d.%d.%d.%d.%d -> %d.%d.%d.%d.%d\n", timestr, header->ts.tv_usec, header->len,
			ih->saddr.byte1,ih->saddr.byte2,ih->saddr.byte3,ih->saddr.byte4,ntohs(th->source_port),
			ih->daddr.byte1,ih->daddr.byte2,ih->daddr.byte3,ih->daddr.byte4,ntohs(th->destination_port));
	int headlen = 10 + sizeof(ip_header) + sizeof(tcp_header);
	int datalen = header->caplen - headlen;
	if (datalen > 0 && datalen < DATAMAXSIZE-headlen ) {
		memcpy(data, (char*)pkt_data + headlen, datalen );
		datamgr::saveframe(e_FrameIn, data, datalen);
		inports += datalen;
	}
	return datalen;
}

int csnp::writedata(char* data, int len) {
	int wroted = 0;
	wroted = pcap_sendpacket(adhandle, (unsigned char*)data, len);
	if (wroted < 1) {
		fprintf(stderr, "pcap_sendpacket error.\n");
		return -1;
	}
	datamgr::saveframe(e_FrameOut, data, len); //wroted怎会很大呢？？
	return wroted;
}

int csnp::flyback( task * tsk ) {
	task reptask(tsk);
	reptask.trigger = this;
	reptask.datadir = dir_send;
	reptask.datastamp = get_time_scale();
	reptask.mark = tsk->mark | send_data;
	writedata(tsk->buffer, tsk->buflen);
	//observer->addtask(&reptask);
	Sleep(tsk->delay);
	reptask.datadir = dir_recv;
	reptask.mark = tsk->mark | recv_data;
	reptask.buflen = readdata(reptask.buffer,sizeof(reptask.buffer));
	observer->addtask(&reptask);
	return 0; 
};






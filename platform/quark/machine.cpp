#include "machine.h"
#include "include/attestor.h"

//machine poweron
bool cmachine::poweron(macopt& opt){
//#ifndef DEBUG
//	if(!attestor::prove(opt.runkey))
//		return false;
//#endif
	if(opt.commsvrport) ccommsvr::start(opt.commsvrip,opt.commsvrport);
	if(opt.websvrport) cwebsvr::start(opt.websvrip,opt.websvrport, opt.webpath);
	if(strlen(opt.script)>0) cbusiness::start(opt.script);
	return true;
}

//machin poweroff
bool cmachine::poweroff(){
	ccommsvr::stop();
	cbusiness::stop();
	cwebsvr::stop();
	return true;
}

//ON LUA THREAD
void cmachine::oncomtask(sockaddr* addr, std::string data){
	int npos = data.find(':');
	if (npos <= 0)
		return;
	std::string ifunc = data.substr(0, npos);
	std::string idata = data.substr(npos + 1);
	std::string retval = cbusiness::execute(ifunc, idata);
	taskmgr::posttask(taskmgr::NET,base::Bind(&ccommsvr::write, base::Unretained(this), addr,retval));
	printf("comtask:%s(%s)=%s\n", ifunc.c_str(), idata.c_str(), retval.c_str());
}

//ON NET THREAD
void cmachine::onluatrans(sockaddr* addr, std::string data) {
	ccommsvr::write(addr, data);
	//taskmgr::PostTask(taskmgr::NET, base::Bind(&ccommsvr::write, base::Unretained(this), addr, data, len));
}

//ON LUA THREAD
void cmachine::onwebtask(int sock, std::string ifunc, std::string data){
	std::string retval = cbusiness::execute(ifunc, data);
	taskmgr::posttask(taskmgr::WEB, base::Bind(&cwebsvr::replay, base::Unretained(this), sock, retval));
	printf("webtask:%s(%s)=%s\n", ifunc.c_str(), data.c_str(), retval.c_str());
}

#ifndef __V2RAY_H__
#define __V2RAY_H__
#include <string>
#include "vpnbase.h"

class v2ray : public vpnbase
{
public:
	void start();
	void stop();
	int connect(const char*ip, const char* user, const char* pwd, int callback);
	int disconnect();

private:
	bool ProcessRun(std::string cmdline, int type);

	bool setProxy();
	bool unsetProxy();

	void checkonline();

	std::string v2RayExecFilePath;
};

#endif

#pragma once
class vpnbase
{
public:
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual int connect(const char*ip, const char* user, const char* pwd, int callback) = 0;
	virtual int disconnect() = 0;


};

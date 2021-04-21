#ifndef _SPEED_MONITOR_
#define _SPEED_MONITOR_

class cmonitor {
public:
	void start();
	void stop();

public:
	int speed(int delay, int callback);

private:
	void supeedupdate(int callback);
};

#endif
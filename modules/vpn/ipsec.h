#ifndef RASCREATEVPN_H
#define RASCREATEVPN_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma message("library is linking with \"rasapi32.lib\"")
#pragma comment(lib, "rasapi32.lib")

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <Ras.h>
#include <RasError.h>

#include "rasdlg.h"
#include <tchar.h>
#include "strsafe.h"
#include "vpnbase.h"

#define PHONE_NUMBER_LENGTH 20
#define DEVICE_NAME_LENGTH 100
#define DEVICE_TYPE_LENGTH 16

class  cipsec : public vpnbase {
public:
	 void start();
	 void stop();
	 int connect( const char*ip, const char* user, const char* pwd, int callback);
	 int disconnect();

private:
    bool addvpn(const char* vpnServer);
    bool setCredentials(const char* userName, const char* password);
    bool connect();
    bool getStatus();
    bool disConnect();
    bool removeVpn();
		bool checkOnline(const char* pszEntryName, bool doDisConnect);
		void CheckConnect();
		int GetVPN(const char* pszEntryName);

private:
    static void WINAPI RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);
    RASCREDENTIALS _rascredentials;
    HRASCONN _hRasConn;
};

#endif //RASCREATEVPN_H

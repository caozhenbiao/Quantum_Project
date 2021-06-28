#include "ipsec.h"

extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

extern   lua_State * theState;
int theCallback;

const char* _vpnName = "quarkVPN";

void cipsec::start() {
	HKEY hKEY;
	LPCTSTR data_Set_Rasman = _T("System\\CurrentControlSet\\Services\\Rasman\\Parameters");
	LSTATUS result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set_Rasman, 0, KEY_ALL_ACCESS, &hKEY);
	if (ERROR_SUCCESS == result) {
		// if openReg succeed
		DWORD dwValue = 1;
		if (ERROR_SUCCESS != ::RegSetValueEx(hKEY, _T("NegotiateDH2048_AES256"), 0, REG_DWORD, (CONST BYTE*)&dwValue, sizeof(DWORD))) {
			return;
		}
		//程序结束，关闭打开的hKEY
		::RegCloseKey(hKEY);
	}
	else {
		// if openReg failed
		LPTSTR errorText = NULL;
		//parse error message
		FormatMessage(
			// use system message tables to retrieve error text
			FORMAT_MESSAGE_FROM_SYSTEM
			// allocate buffer on local heap for error text
			| FORMAT_MESSAGE_ALLOCATE_BUFFER
			// Important! will fail otherwise, since we're not
			// (and CANNOT) pass insertion parameters
			| FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
			result,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&errorText,  // output
			0, // minimum size for output buffer
			NULL);
		if (NULL != errorText) {
			// ... do something with the string `errorText` - log it, display it to the user, etc.
			// release memory allocated by FormatMessage()
			LocalFree(errorText);
			errorText = NULL;
		}
		return;
	}
	LPCTSTR data_Set_PolicyAgent = _T("System\\CurrentControlSet\\Services\\PolicyAgent");
	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set_PolicyAgent, 0, KEY_ALL_ACCESS, &hKEY)) {
		DWORD dwValue = 2;
		if (ERROR_SUCCESS != ::RegSetValueEx(hKEY, _T("AssumeUDPEncapsulationContextOnSendRule"), 0, REG_DWORD, (CONST BYTE*)&dwValue, sizeof(DWORD))) {
			return;
		}
		//程序结束，关闭打开的hKEY
		::RegCloseKey(hKEY);
	}
	else {
		return;
	}
	return;

}

void cipsec::stop() {
}

int cipsec::connect( const char*ip, const char* user, const char* pwd, int callback) {
	theCallback = callback;
	CheckConnect();
	addvpn(ip);
	setCredentials(user, pwd);
	return connect()?0:1;
}

int cipsec::disconnect() {
	luaL_unref(theState, LUA_REGISTRYINDEX, theCallback);
	disConnect();
	return removeVpn() ? 0 : 1;
}

bool cipsec::addvpn(const char* vpnServer) {
    DWORD dwError = ERROR_SUCCESS;
    char* lpszDeviceName = "vpn";
    // Allocate heap memory and initialize RASENTRY structure
    DWORD dwBufferSize = 0;
    RasGetEntryProperties(NULL, "", NULL, &dwBufferSize, NULL, NULL);
    LPRASENTRY lpentry = (LPRASENTRY)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);

    // Allocate heap memory and initialize RASDIALDLG structure
    LPRASDIALDLG lpInfo = (LPRASDIALDLG)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(RASDIALDLG));
    if (lpentry == NULL || lpInfo == NULL) {
        printf("HeapAlloc failed\n");
        HeapFree(GetProcessHeap(), 0, lpentry);
        HeapFree(GetProcessHeap(), 0, lpInfo);
        return false;
    }
    // The RASDIALDLG and RASENTRY dwSize members have to be initialized or the RasDialDlg()
    // RasSetEntryProperties() APIs will fail below.
    lpInfo->dwSize = sizeof(RASDIALDLG);
    //ZeroMemory(lpentry, sizeof(RASENTRY));
    lpentry->dwSize = dwBufferSize;
    lpentry->dwFramingProtocol = RASFP_Ppp;
    lpentry->dwfNetProtocols = RASNP_Ip | RASNP_Ipv6;
    lpentry->dwfOptions = 0;
    lpentry->dwType = RASET_Vpn;
    lpentry->dwVpnStrategy = VS_Ikev2Only;
    lpentry->dwEncryptionType = ET_Optional;
    lpentry->dwRedialCount = 3;
    lpentry->dwRedialPause = 60;
    lpentry->dwCustomAuthKey = 0;
    lpentry->dwDialExtraPercent =75;
    lpentry->dwHangUpExtraPercent = 10;
    lpentry->dwSubEntries = 0;

    lpentry->dwfOptions = lpentry->dwfOptions | RASEO_ModemLights;
    lpentry->dwfOptions = lpentry->dwfOptions | RASEO_RequireEAP;

    lpentry->dwfOptions = lpentry->dwfOptions | RASEO_PreviewUserPw;
    lpentry->dwfOptions = lpentry->dwfOptions | RASEO_PreviewDomain;
    lpentry->dwfOptions = lpentry->dwfOptions | RASEO_ShowDialingProgress;
    lpentry->dwfOptions = lpentry->dwfOptions | RASEO_RemoteDefaultGateway;
    lpentry->dwfOptions = lpentry->dwfOptions | RASEO_IpHeaderCompression;

    lpentry->dwfOptions2 = lpentry->dwfOptions2 | RASEO2_IPv6RemoteDefaultGateway;
    lpentry->dwfOptions2 = lpentry->dwfOptions2 | RASEO2_DontNegotiateMultilink; //不为单路连接协商多路链接
    lpentry->dwfOptions2 = lpentry->dwfOptions2 | RASEO2_Internet;
    lpentry->dwfOptions2 = lpentry->dwfOptions2 | RASEO2_ReconnectIfDropped;//断线重拨
    lpentry->dwfOptions2 = lpentry->dwfOptions2 | RASEO2_UsePreSharedKey;
    lpentry->dwfOptions2 = lpentry->dwfOptions2 | RASEO2_UseTypicalSettings;

    dwError |= StringCchCopyN(lpentry->szLocalPhoneNumber, RAS_MaxPhoneNumber, vpnServer, PHONE_NUMBER_LENGTH);
    dwError |= StringCchCopyN(lpentry->szDeviceName, RAS_MaxDeviceName, lpszDeviceName, DEVICE_NAME_LENGTH);
    dwError |= StringCchCopyN(lpentry->szDeviceType, RAS_MaxDeviceType, RASDT_Vpn, DEVICE_TYPE_LENGTH);

    if (dwError != S_OK) {
        printf("Structure initilization failed:  Error = %d\n", dwError);
        HeapFree(GetProcessHeap(), 0, lpentry);
        HeapFree(GetProcessHeap(), 0, lpInfo);
        return false;
    }

    // Validate the new entry's name
    dwError = RasValidateEntryName(NULL, _vpnName);
    if (dwError != ERROR_SUCCESS) {
        printf("RasValidateEntryName failed:  Error = %d\n", dwError);
        HeapFree(GetProcessHeap(), 0, lpentry);
        HeapFree(GetProcessHeap(), 0, lpInfo);
        return false;
    }

    // Create and set the new entry's properties
    dwError = RasSetEntryProperties(NULL, _vpnName, lpentry, lpentry->dwSize, NULL, 0);
    if (dwError != ERROR_SUCCESS) {
        printf("RasSetEntryProperties failed:  Error = %d\n",  dwError);
        HeapFree(GetProcessHeap(), 0, lpentry);
        HeapFree(GetProcessHeap(), 0, lpInfo);
        return false;
    }
    HeapFree(GetProcessHeap(), 0, lpentry);
    HeapFree(GetProcessHeap(), 0, lpInfo);
    return true;
}

bool cipsec::checkOnline(const char* pszEntryName, bool doDisConnect)
{
	bool isActive = false;
	DWORD dwCb = 0;
	DWORD dwRet = ERROR_SUCCESS;
	DWORD dwConnections = 0;
	LPRASCONN lpRasConn = nullptr;

	// Call RasEnumConnections with lpRasConn = NULL. dwCb is returned with the required buffer size and
	// a return code of ERROR_BUFFER_TOO_SMALL
	dwRet = RasEnumConnections(lpRasConn, &dwCb, &dwConnections);

	if (dwRet == ERROR_BUFFER_TOO_SMALL)
	{
		// Allocate the memory needed for the array of RAS structure(s).
		lpRasConn = (LPRASCONN)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
		if (lpRasConn == nullptr)
		{
			return false;
		}
		// The first RASCONN structure in the array must contain the RASCONN structure size
		lpRasConn[0].dwSize = sizeof(RASCONN);

		// Call RasEnumConnections to enumerate active connections
		dwRet = RasEnumConnections(lpRasConn, &dwCb, &dwConnections);

		// If successful, print the names of the active connections.
		if (ERROR_SUCCESS == dwRet)
		{
			for (DWORD i = 0; i < dwConnections; i++)
			{
				RASCONNSTATUS rasConStatus = { 0 };
				rasConStatus.dwSize = sizeof(RASCONNSTATUS);

				if (ERROR_SUCCESS != ::RasGetConnectStatus(lpRasConn[i].hrasconn, &rasConStatus))
				{
					break;
				}

				if (rasConStatus.rasconnstate == RASCS_Connected || rasConStatus.rasconnstate == RASCS_AuthNotify || rasConStatus.rasconnstate == RASCS_PortOpened)
				{
					if (strcmp(lpRasConn[i].szEntryName, pszEntryName) == 0) {
						isActive = true;
						if (doDisConnect) {
							DWORD dwError = RasHangUp(lpRasConn[i].hrasconn);
						}
						break;
					}
				}

				if ((rasConStatus.rasconnstate == RASCS_Disconnected) || (rasConStatus.dwError != 0))
				{
					break;
				}
				else {
					// if other status need disconnect, otherwise call RasDial() failed.
					if (strcmp(lpRasConn[i].szEntryName, pszEntryName) == 0) {
						isActive = true;
						if (doDisConnect) {
							DWORD dwError = RasHangUp(lpRasConn[i].hrasconn);
						}
						break;
					}
				}
				Sleep(50); // Presumably something else is in the process of establishing the connection. Wait and loop.
			}
		}
		//Deallocate memory for the connection buffer
		HeapFree(GetProcessHeap(), 0, lpRasConn);
		lpRasConn = NULL;
		return isActive;
	}

	// There was either a problem with RAS or there are no connections to enumerate
	if (dwConnections >= 1)
	{
	}
	else
	{
		//LOG(INFO) << "There are no active RAS connections.";
	}
	return isActive;
}


// first remove old Connect
void cipsec::CheckConnect() {
	checkOnline(_vpnName, true);
	if (GetVPN(_vpnName) == 0) {
		removeVpn();
	}
}

//get vpn if have
int cipsec::GetVPN(const char* pszEntryName)
{
	DWORD rasEntrySize = 0;
	RASENTRY rasEntry;
	ZeroMemory(&rasEntry, sizeof(RASENTRY));

	int ret = RasGetEntryProperties(NULL, NULL, NULL, &rasEntrySize, NULL, NULL);

	if (ret == ERROR_BUFFER_TOO_SMALL || ret == ERROR_INVALID_SIZE)
	{
		rasEntry.dwSize = rasEntrySize > sizeof(RASENTRY) ? sizeof(RASENTRY) : rasEntrySize;
		rasEntrySize = rasEntry.dwSize;
	}

	ret = RasGetEntryProperties(NULL, pszEntryName, &rasEntry, &rasEntrySize, NULL, NULL);
	if (ret == 0) {
	}

	// if diff config with we ,delete
	if (ret == 0 && !strcmp(rasEntry.szDeviceType, "vpn") && !strcmp(rasEntry.szDeviceName, "WAN Miniport (IKEv2)")) {
		return 1;
	}
	else
	{
		return 0;
	}

	return 0;
}

bool cipsec::setCredentials(const char* userName, const char* password) {
    DWORD dwError = ERROR_SUCCESS;
    RASCREDENTIALS theRascredentials;
    //bit flags of using username and password
    theRascredentials.dwMask = RASCM_UserName| RASCM_Password;
    //StringCchCopy(theRascredentials.szUserName, sizeof (userName) + 1, userName);
    strcpy(theRascredentials.szUserName, userName);
    strcpy(theRascredentials.szPassword, password);
    theRascredentials.dwSize = sizeof(theRascredentials);
    //用户密码保存
    dwError = RasSetCredentials(NULL, _vpnName, &theRascredentials, TRUE);
    _rascredentials = theRascredentials;
    return ERROR_SUCCESS == dwError;
}

bool cipsec::connect() {
    DWORD Ret;
    RASDIALPARAMS theRasDialParams = {};
    strcpy(theRasDialParams.szUserName, _rascredentials.szUserName);
    strcpy(theRasDialParams.szPassword, _rascredentials.szPassword);
    strcpy(theRasDialParams.szEntryName, _vpnName);
    theRasDialParams.dwSize = sizeof(theRasDialParams);   
	printf("USERNAME:%s\n", _rascredentials.szUserName);
	printf("PWDNAME:%s\n", _rascredentials.szPassword);

    // RasDial: should be Asynchronous Mode
    HRASCONN hRasConn = NULL;  //a handle to the RAS connection
    if ((Ret = RasDial(NULL, NULL, &theRasDialParams, 0, reinterpret_cast<void*>(&RasDialFunc), &hRasConn)) != ERROR_SUCCESS) {
        printf("RasDial failed with error %u\n", Ret);
        return false;
    }
    this->_hRasConn = hRasConn;
    // setup vpn event
    HANDLE vpnEvent = CreateEvent(NULL,FALSE, FALSE,TEXT("vpn_status"));
    if (vpnEvent == NULL) {
        printf("CreateEvent failed (%d)\n", GetLastError());
        return false;
    }
    RasConnectionNotification(hRasConn, vpnEvent, RASCN_Connection | RASCN_Disconnection);
    return true;
}

bool cipsec::getStatus() {
    RASCONNSTATUS connectionStatus;
    connectionStatus.dwSize = sizeof(connectionStatus);
    if (RasGetConnectStatus(_hRasConn, &connectionStatus) == ERROR_SUCCESS) {
        if (connectionStatus.rasconnstate == RASCS_Connected) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool cipsec::disConnect() {
    DWORD dwError;
    RASCONNSTATUS connectionStatus;
    connectionStatus.dwSize = sizeof(connectionStatus);
    dwError = RasHangUp(this->_hRasConn);
    //wait for connection is properly closed
    while ((RasGetConnectStatus(_hRasConn, &connectionStatus) == ERROR_SUCCESS) &&
           (connectionStatus.rasconnstate == RASCS_Connected) ) {
        Sleep(0);
    }
    return ERROR_SUCCESS == dwError;
}

bool cipsec::removeVpn() {
    DWORD dwError = RasDeleteEntry(NULL, _vpnName);
    if (dwError != ERROR_SUCCESS) {
        printf("RasDeleteEntry failed: Error = %d\n", dwError);
        return false;
    }
    return true;
}

void WINAPI cipsec::RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError){
    char  szRasString[256]; // Buffer for error string
    if (dwError) {
        RasGetErrorStringA((UINT)dwError, szRasString, 256);
        printf("Error: %d - %s\n",dwError, szRasString);
        return;
    }
    // Map each of the RasDial states and display on the
    // screen the next state that RasDial is entering
    //switch (rasconnstate) {
    //case RASCS_ConnectDevice:
    //    printf ("Connecting device...\n");
    //    break;
    //case RASCS_Connected:
			 // printf("Connected device...\n");
    //    break;
    //default:
    //    printf ("Unmonitored RAS activity.\n");
    //    break;
    //}
		if (0 == luaL_lock(theState)) {
			lua_rawgeti(theState, LUA_REGISTRYINDEX, theCallback);
			lua_pushinteger(theState, rasconnstate);
			lua_pushinteger(theState, dwError);
			lua_pcall(theState, 2, 0, 0);
			luaL_unlock(theState);
		}
}



#include "v2ray.h"
#include <string>
#include <Windows.h>
#include <shellapi.h>
#include <atlconv.h>
#include <atlstr.h>
#include <stdio.h>
#include <WinInet.h>
#include "curl.h"

const  LPSTR g_v2ray_proxy = "127.0.0.1:8892";
const LPSTR localhost = "localhost";
int proxyPort = 8892;
PROCESS_INFORMATION process_info;

void v2ray::start() {
	v2RayExecFilePath = "v2ray.exe";
	std::string configfile = "";
	std::string cmdline = std::string("v2ray.exe -config config.gen.json -format json");
	if (!ProcessRun(cmdline, 0)) {
		return;
	}
}

void v2ray::stop() {
	unsetProxy();
	WaitForSingleObject(process_info.hProcess, 100);
	::TerminateProcess(process_info.hProcess, 0);
}

int v2ray::connect(const char*ip, const char* user, const char* pwd, int callback) {

	return 0;
}


int v2ray::disconnect() {

	return 0;
}

bool v2ray::ProcessRun(std::string cmdline, int type) {
	HANDLE hPipeRead = NULL;
	HANDLE hPipeWrite = NULL;

	STARTUPINFO start_info;
	ZeroMemory(&start_info, sizeof(STARTUPINFO));
	start_info.cb = sizeof(STARTUPINFO);
	start_info.dwFlags |= STARTF_USESTDHANDLES;
	start_info.dwFlags |= STARTF_USESHOWWINDOW;
	start_info.wShowWindow = SW_HIDE;
	start_info.hStdOutput = hPipeRead; // output_pipe.GetWriteHandle().Get();     //意思是：子进程的stdout输出到hStdOutWrite
	start_info.hStdError = hPipeWrite; // error_pipe.GetWriteHandle().Get();        //意思是：子进程的stderr输出到hStdErrWrite


	bool create_process = CreateProcess(
		v2RayExecFilePath.c_str(),
		(LPTSTR)cmdline.c_str(),
		NULL, NULL, TRUE, 0, NULL, NULL, &start_info, &process_info);

	if (create_process) {
		setProxy();
	}

	//CHAR szBuffer[1024 * 8] = { 0 };
	//DWORD dwBytesRead = 0;
	//do {
		//if (!::ReadFile(hPipeWrite, szBuffer, sizeof(szBuffer) - 1, &dwBytesRead, NULL)) {
		//	break;
		//}
		//printf(szBuffer);

		//if (!::ReadFile(hPipeRead, szBuffer, sizeof(szBuffer) - 1, &dwBytesRead, NULL)) {
		//	break;
		////}
		//printf(szBuffer);


	//} while (::WaitForSingleObject(process_info.hProcess, 0) == WAIT_TIMEOUT);



	return true;
}


bool v2ray::setProxy() {
	INTERNET_PER_CONN_OPTION_LIST list;
	BOOL bReturn;
	DWORD dwBufSize = sizeof(list);
	list.dwSize = sizeof(list);
	list.pszConnection = nullptr;
	list.dwOptionCount = 3;
	list.pOptions = new INTERNET_PER_CONN_OPTION[3];

	if (nullptr == list.pOptions)
	{
		return false;
	}

	// Set flags.
	list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
	list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT | PROXY_TYPE_PROXY;
	// Set proxy name.
	list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
	list.pOptions[1].Value.pszValue = g_v2ray_proxy;
	// Set proxy override.
	list.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	list.pOptions[2].Value.pszValue = localhost;

	// Set the options on the connection.
	bReturn = InternetSetOption(nullptr, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);

	delete[] list.pOptions;
	InternetSetOption(nullptr, INTERNET_OPTION_SETTINGS_CHANGED, nullptr, 0);
	InternetSetOption(nullptr, INTERNET_OPTION_REFRESH, nullptr, 0);
	return bReturn;
}

bool v2ray::unsetProxy() {
	INTERNET_PER_CONN_OPTION_LIST list;
	BOOL bReturn;
	DWORD dwBufSize = sizeof(list);
	// Fill out list struct.
	list.dwSize = sizeof(list);
	// nullptr == LAN, otherwise connectoid name.
	list.pszConnection = nullptr;
	// Set three options.
	list.dwOptionCount = 1;
	list.pOptions = new INTERNET_PER_CONN_OPTION[list.dwOptionCount];

	// Make sure the memory was allocated.
	if (nullptr == list.pOptions)
	{
		// Return FALSE if the memory wasn't allocated.
	}

	// Set flags.
	list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
	list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT;
	//
	// Set the options on the connection.
	bReturn = InternetSetOption(nullptr, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);
	if (!bReturn)
	{
		//LOG_W("InternetSetOption INTERNET_OPTION_PER_CONNECTION_OPTION failed");
	}
	delete[] list.pOptions;
	InternetSetOption(nullptr, INTERNET_OPTION_SETTINGS_CHANGED, nullptr, 0);
	InternetSetOption(nullptr, INTERNET_OPTION_REFRESH, nullptr, 0);
	return true;
}

size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
	if (NULL == str || NULL == buffer)
	{
		return -1;
	}

	char* pData = (char*)buffer;
	str->append(pData, size * nmemb);
	return nmemb;
}

void v2ray::checkonline() {
	std::string content;
	CURL* curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_PROXY, "127.0.0.1:8892");
	std::string host_ip;
	std::string strIpApi = "http://ip-api.com/json";
	//SetCurlCommonOptions(curl_handle);
	curl_easy_setopt(curl_handle, CURLOPT_URL, strIpApi.c_str());
	curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, NULL);
	//curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&content);
	curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 5);
	curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 5);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, false);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, false);
	CURLcode res = curl_easy_perform(curl_handle);
	if (res == CURLE_OK && content.size() > 0) {
		//Json::Reader reader;
		//Json::Value root;
		//if (reader.parse(content, root) && root.isObject()) {
		//	if (root.isMember("query") && root["query"].isString()) {
		//		host_ip = root["query"].asString();
		//	}
		//}
	}
	curl_easy_cleanup(curl_handle);
}
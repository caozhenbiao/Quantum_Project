#define   INITGUID 
#include "gpoedit.h"
#include<iostream>
#include<Windows.h>
#include<GPEdit.h>
#include<Guiddef.h>
#include <winnt.h>

LRESULT cgpoedit::DisableTaskMgr(int mark)
{
	::CoInitialize(NULL);
	LRESULT status;
	LRESULT hr = S_OK;
	IGroupPolicyObject*pGPO = NULL;
	hr = CoCreateInstance(CLSID_GroupPolicyObject, NULL, CLSCTX_INPROC_SERVER, IID_IGroupPolicyObject, (LPVOID*)&pGPO);
	if (hr == S_OK)
	{
		printf("GPO创建成功\n");
	}
	else
	{
		printf("GPO创建失败\n");
		return E_FAIL;
	}
	DWORD dwSection = GPO_SECTION_USER;
	HKEY hGPOKey = 0;
	hr = pGPO->OpenLocalMachineGPO(GPO_OPEN_LOAD_REGISTRY);
	if (SUCCEEDED(hr))
	{
		printf("打开本地机器成功\n");
	}
	else
	{
		printf("打开本地失败\n");
		return E_FAIL;
	}
	hr = pGPO->GetRegistryKey(dwSection, &hGPOKey);
	if (SUCCEEDED(hr))
	{
		printf("加载注册表成功\n");
	}
	else
	{
		printf("加载注册表失败\n");
		return E_FAIL;
	}

	//禁用任务管理器
	HKEY hKey = NULL;
	if (mark == 1)
	{
		status = RegOpenKeyEx(hGPOKey, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0,
			KEY_WRITE, &hKey);
		if (status != ERROR_SUCCESS)
		{
			status = RegCreateKeyEx(hGPOKey, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0,
				NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
			if (status == S_OK)
			{
				printf("创建键值成功\n");
			}
			else
			{
				printf("创建键值失败\n");
				return E_FAIL;
			}
		}
		DWORD lpData = 1;
		status = RegSetKeyValue(hKey, NULL, "DisableTaskMgr", REG_DWORD, (LPCVOID)&lpData, 4);
		status = RegCloseKey(hKey);
	}

	GUID Registerid = REGISTRY_EXTENSION_GUID;
	GUID ThisGuid = {
		0x0F6B957E,
		0x509E,
		0x11D1,
		{0xA7, 0xCC, 0x00, 0x00, 0xF8, 0x75, 0x71, 0xE3}
	};
	RegCloseKey(hGPOKey);
	status = pGPO->Save(FALSE, TRUE, &Registerid, &ThisGuid);
	pGPO->Release();
	::CoUninitialize();
	return 0;
}

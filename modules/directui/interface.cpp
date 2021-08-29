
#include "StdAfx.h"
extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

#include "duiwindow.h"
#include <string.h>
#include <vector>
#include <string>
#include <process.h>
#include <map>

lua_State * theState = NULL;
static int lua_Notify  = LUA_REFNIL;
static int lua_Message = LUA_REFNIL;
std::map<HWND, DuiLib::CDuiWidow *> window_list;
HANDLE hStartEvent = NULL;

void inDestoryWindow( HWND hWnd ) {
	auto ifndx = window_list.find(hWnd);
	if (ifndx != window_list.end()) {
		free(ifndx->second);
		window_list.erase(ifndx);
	}
}

DuiLib::CControlUI * find_control(HWND hWnd, std::string path) {
	size_t pos = 0;
	size_t len = path.length();
	DuiLib::CContainerUI * layout = NULL;
	DuiLib::CControlUI   * control = NULL;
	auto ifndwnd = window_list.find(hWnd);
	if( ifndwnd != window_list.end() ) {
		while (pos < len) {
			int find_pos = path.find(".", pos);
			if (find_pos < 0) {
				std::string ctlname = path.substr(pos, len - pos);
				if (layout)
					control = layout->FindSubControl(ctlname.c_str());
				else
					control = ifndwnd->second->m_PaintManager.FindControl(ctlname.c_str());
				break;
			}
			std::string layname = path.substr(pos, find_pos - pos);
			pos = find_pos + 1;
			if (layout)
				layout = static_cast<DuiLib::CContainerUI*>(ifndwnd->second->m_PaintManager.FindControl(layname.c_str()));
			else
				layout = static_cast<DuiLib::CContainerUI*>(layout->FindSubControl(layname.c_str()));
		}
	}
	return control;
}

unsigned int __stdcall workthread(void* lParam) {
	DuiLib::CDuiWidow * pFrame = new DuiLib::CDuiWidow();
	if (pFrame == NULL) return 0;
	pFrame->CreateWithXML(NULL,(char*)lParam);
	//LPCTSTR show = pFrame->m_PaintManager.GetWindowCustomAttribute("visible");
	//if( show )
	//	pFrame->ShowWindow((_tcsicmp(show,_T("true"))== 0));
	//else
	//	pFrame->ShowWindow(true);
	window_list.insert(make_pair(pFrame->GetHWND(),pFrame));
	SetEvent(hStartEvent);
	DuiLib::CPaintManagerUI::MessageLoop();
	return 0;
}
 
//lua notify
int NotifyUI2LUA(HWND hWnd, LPCSTR type, LPCSTR name) {
	lua_Number nret = -1;
	//if (0 == luaL_lock(theState)) {
	lua_rawgeti(theState, LUA_REGISTRYINDEX, lua_Notify);
	lua_pushinteger(theState, (lua_Integer)hWnd);
	lua_pushstring(theState, type);
	lua_pushstring(theState, name);
	lua_pcall(theState, 3, 1, 0);
	lua_isnumber(theState, -1);
	nret = lua_tonumber(theState, -1);
	lua_pop(theState, 1);
	//	luaL_unlock(theState);
	//}
	return (int)nret;
}

//lua message
int MessageUI2LUA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	lua_Number nret = -1;
	//if (0 == luaL_lock(theState)) {
	lua_rawgeti(theState, LUA_REGISTRYINDEX, lua_Message);
	lua_pushinteger(theState, (lua_Integer)hWnd);
	lua_pushinteger(theState, uMsg);
	lua_pushinteger(theState, wParam);
	lua_pushinteger(theState, lParam);
	lua_pcall(theState, 4, 1, 0);
	lua_isnumber(theState, -1);
	nret = lua_tonumber(theState, -1);
	lua_pop(theState, 1);
	//	luaL_unlock(theState);
	//}
	return (int)nret;
}

static int mainFrame(lua_State * L) {
	const char* xml  = luaL_checkstring(L, 1);
	lua_Message = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_Notify  = luaL_ref(L, LUA_REGISTRYINDEX);
	hStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	_beginthreadex(NULL, 0, workthread, (void*)xml, 0, NULL);
	WaitForSingleObject(hStartEvent, INFINITE);
	CloseHandle(hStartEvent);
	lua_pushinteger(L, window_list.size()?(lua_Integer)window_list.begin()->first:0);
	return 1;
}

static int createWindow(lua_State * L) {
	HWND hWnd = (HWND)luaL_checkinteger(L, 1);
	const char* xml = luaL_checkstring(L, 2);
	DuiLib::CDuiWidow * frmwin = new DuiLib::CDuiWidow();
	if (frmwin == NULL) return 0;
	frmwin->CreateWithXML(hWnd,xml);
	window_list.insert(make_pair(frmwin->GetHWND(), frmwin));
	lua_pushinteger(L,(lua_Integer)frmwin->GetHWND());
	return 1;
}

static int closeWindow(lua_State * L) {
	HWND hWnd = (HWND)luaL_checkinteger(L, 1);
	auto ifnd = window_list.find(hWnd);
	if (ifnd != window_list.end()) {
		ifnd->second->Close();
		window_list.erase(ifnd);
	}
	lua_pushinteger(L, 0);
	return 1;
}

static int setVisible(lua_State* L) {
	HWND hWnd = (HWND)luaL_checkinteger(L, 1);
	const char * path = luaL_checkstring(L, 2);
	bool bShow = (bool)luaL_checkinteger(L, 3);
	DuiLib::CControlUI * fUI = find_control(hWnd,path);
	if ( fUI ) {fUI->SetVisible((bool)bShow);}
	lua_pushinteger(L, fUI?1:0);
	return 1;
}

static int setText(lua_State* L) {
	HWND hWnd = (HWND)luaL_checkinteger(L, 1);
	const char * path = luaL_checkstring(L, 2);
	const char * text = luaL_checkstring(L, 3);
	DuiLib::CControlUI * fUI = find_control(hWnd, path);
	if (fUI){fUI->SetText( text );}
	lua_pushinteger(L, fUI ? 1 : 0);
	return 1;
}

static int getText(lua_State* L) {
	HWND hWnd = (HWND)luaL_checkinteger(L, 1);
	const char * path = luaL_checkstring(L, 2);
	DuiLib::CControlUI * fUI = find_control(hWnd, path);
	std::string text = fUI?fUI->GetText():"";
	lua_pushstring(L, text.c_str());
	return 1;
}

static int setEnabled(lua_State* L) {
	HWND hWnd = (HWND)luaL_checkinteger(L, 1);
	const char * path = luaL_checkstring(L, 2);
	bool enable = (bool)luaL_checkinteger(L, 3);
	DuiLib::CControlUI * fUI = find_control(hWnd, path);
	if (fUI) {fUI->SetEnabled(enable);}
	lua_pushinteger(L, fUI ? 1 : 0);
	return 1;
}

static int showWindow(lua_State* L) {
	HWND hWnd = (HWND)luaL_checkinteger(L, 1);
	bool show = (bool)luaL_checkinteger(L, 2);
	auto ifndx = window_list.find(hWnd);
	if (ifndx != window_list.end()) {
		ifndx->second->ShowWindow(show);
	}
	return 0;
}

static int destory(lua_State * L) {
	::CoUninitialize();
	return 0;
}

static const struct luaL_Reg myLib[] = {
	{"mainFrame", mainFrame},
	{"createWindow",createWindow},
	{"closeWindow",closeWindow},
	{"setVisible", setVisible},
	{"setText", setText},
	{"setEnabled", setEnabled},
	{"getText", getText},
	{"showWindow", showWindow},
	{"destory",destory},
	{NULL, NULL},
};

#ifdef WIN32
extern "C" __declspec(dllexport) int luaopen_directui(lua_State *L){
#else
extern "C" int luaopen_directui(lua_State *L){
#endif
	printf("luaopen_directui\n");
	lua_newtable(L);
	DuiLib::CPaintManagerUI::SetResourcePath(DuiLib::CPaintManagerUI::GetInstancePath());
	::CoInitialize(NULL);
	if (!theState) {
		theState = L;
	}
	const luaL_Reg * la = &myLib[0];
	for(;la->name!=NULL; la++){
		lua_pushcclosure(L,la->func,0);
		lua_setfield(L,-2,la->name);
	}
	lua_pop(L,0);
	return 1;
}


extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

#include <string.h>
#include <vector>
#include <string>
#include "odbc.h"

#ifdef WIN32
#include <windows.h>
#include <lm.h>
void UTF_8ToUnicode(wchar_t* pOut,const char *pText){
	char* uchar = (char *)pOut; 
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F); 
} 
void UnicodeToUTF_8(char* pOut,const wchar_t* pText) {
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后 
	char* pchar = (char *)pText; 
	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));
} 
void UnicodeToGB2312(char* pOut,wchar_t uData){
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL); 
}
void Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer) {
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
} 
void GB2312ToUTF_8(std::string& pOut,char *pText, int pLen){  
	char buf[4] = {0}; 
	int nLength = pLen* 3; 
	char* rst = new char[nLength]; 	
	memset(rst,0,nLength);  
	int i = 0 ,j = 0; 
	while(i < pLen) { 
		//如果是英文直接复制就可以 
		if( *(pText + i) >= 0) 
			rst[j++] = pText[i++];
		else{  
			wchar_t pbuffer;  
			Gb2312ToUnicode(&pbuffer,pText+i);
			UnicodeToUTF_8(buf,&pbuffer);
			rst[j] = buf[0];   
			rst[j+1] = buf[1]; 
			rst[j+2] = buf[2]; 
			j += 3; 
			i += 2; 
		}   
	}  

	rst[j] ='\0';  	//返回结果  
	pOut = rst;
	delete []rst;
	return;   
}   
void UTF_8ToGB2312(char*pOut, const char *pText, int pLen)  {
	char Ctemp[4];   
	memset(Ctemp,0,4); 
	int i =0 ,j = 0; 
	while(i < pLen) {  
		if(pText[i] >= 0) 
			pOut[j++] = pText[i++]; 
		else{  
			WCHAR Wtemp; 
			UTF_8ToUnicode(&Wtemp,pText + i);
			UnicodeToGB2312(Ctemp,Wtemp); 
			pOut[j] = Ctemp[0];
			pOut[j + 1] = Ctemp[1];  
			i += 3;   
			j += 2;  
		}   
	} 
	pOut[j] ='\0'; 
	return; 
} 
std::string utf2gb( const char* data ){
	if(!data)
		return "NULL";
	char szout[1024]={0};
	UTF_8ToGB2312(szout, data, strlen(data));
	return std::string(szout);
}

std::string gb2utf( char* data){
	std::string str;
	GB2312ToUTF_8( str,data, strlen(data) );
	return str;
}
#else
#include <iconv.h>
#include <string.h>
int code_convert(const char *from_charset,const char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen){
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;
	cd = iconv_open(to_charset,from_charset);
	if (cd==0)
		return -1;
	memset(outbuf,0,outlen);
	if (iconv(cd,pin,&inlen,pout,&outlen)<0)
		return -1;
	iconv_close(cd);
	return 0;
}
int u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen){
	return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}
int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen){
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}
#endif

cmyodbc   * theODBC = NULL;
static int connect(lua_State * L) {
	const char *dns = luaL_checkstring(L, 1);
	const char *usr = luaL_optstring(L, 2, NULL);
	const char *pwd = luaL_optstring(L, 3, NULL);
	int idbc = theODBC->connect(dns, usr, pwd);
	lua_pushinteger(L, idbc);
	return 1;
}

static int disconnect(lua_State * L) {
	int idbc = (int)luaL_checkinteger(L, 1);
	int nRet = theODBC->disconnect( idbc );
	lua_pushinteger(L, nRet);
	return 1;
}

static int execute(lua_State * L){
	int idbc  = (int)luaL_checkinteger(L, 1);
	const char *sql = luaL_checkstring (L, 2);
	int nRet = theODBC->execute(idbc, sql);
	lua_pushinteger(L,nRet);
	return 1;
}

static int freeHandle(lua_State * L){
	int hsmt = (int)luaL_checkinteger(L, 1);
	theODBC->freestmt( hsmt );
	return 0;
}

static int NumResultCols(lua_State * L){
	int hsmt = (int)luaL_checkinteger(L, 1);
	int cols = theODBC->getcolcount( hsmt );
	lua_pushinteger(L,cols); 
	return 1;
}

static int DescribeCol(lua_State * L){
	int hsmt = (int)luaL_checkinteger(L,1);
	int i    = (int)luaL_checkinteger(L,2);
	static char szcol[256] = {0};
	memset(&szcol,0x00,256);
	theODBC->getcoltext(hsmt,i,szcol);
	for(size_t i=0; i<strlen(szcol);i++) 
		if( szcol[i]=='\"')szcol[i] = '\'';
	lua_pushstring(L, szcol); 
	return 1;
}

static int Fetch(lua_State * L){
	int hsmt = (int)luaL_checkinteger(L,1);
	int nret = theODBC->fetch( hsmt );
	lua_pushinteger(L, nret); 
	return 1;
}

static int GetData(lua_State * L){
	int hsmt = (int)luaL_checkinteger(L,1);
	int i    = (int)luaL_checkinteger(L,2);
	static char szcol[256] = {0};
	memset(&szcol,0x00,256);
	theODBC->getdata( hsmt, i, szcol ,256);
	for( size_t i=0; i<strlen(szcol); i++ )
		if( szcol[i]=='\"')szcol[i] = '\'';
#ifdef WIN32
	lua_pushstring(L, szcol); 
#else
	lua_pushstring(L, szcol); 
#endif
	return 1;
}

static int GetTitle(lua_State * L) {
	int hsmt = (int)luaL_checkinteger(L, 1);
	int i = (int)luaL_checkinteger(L, 2);
	static char sztext[256] = { 0 };
	memset(&sztext, 0x00, 256);
	theODBC->getcoltext(hsmt, i, sztext);
	lua_pushstring(L, sztext);
	return 1;
}


//模块销毁对象实例，删除相关数据
static int destory(lua_State *L) {
	if (theODBC) {
		theODBC->close();
		delete theODBC;
		theODBC = NULL;
	}
	return 0;
}


static const struct luaL_Reg myLib[] = {
	{"destory", destory },
	{"connect", connect},
	{"disconnect", disconnect },
	{"execute", execute},
	{"freeHandle", freeHandle},
	{"NumResultCols", NumResultCols},
	{"DescribeCol", DescribeCol},
	{"Fetch", Fetch},
	{"GetData", GetData},
	{"GetTitle",GetTitle },
	{NULL, NULL},
};

#ifdef WIN32
extern "C" __declspec(dllexport) int luaopen_odbc(lua_State *L){
#else
extern "C" int luaopen_odbc(lua_State *L){
#endif
	printf("luaopen_odbc\n");
	if( !theODBC ){
		theODBC = new cmyodbc;
		theODBC->setup();
	}
	lua_newtable(L);
	const luaL_Reg * la = &myLib[0];
	for(;la->name!=NULL; la++){
		lua_pushcclosure(L,la->func,0);
		lua_setfield(L,-2,la->name);
	}
	lua_pop(L,0);
	return 1;
}
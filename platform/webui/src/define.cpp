#include "define.h"




//做为解Url使用
char CharToInt(char ch) {
	if (ch >= '0' && ch <= '9')return (char)(ch - '0');
	if (ch >= 'a' && ch <= 'f')return (char)(ch - 'a' + 10);
	if (ch >= 'A' && ch <= 'F')return (char)(ch - 'A' + 10);
	return -1;
}

char StrToBin(char *str) {
	char tempWord[2];
	char chn;
	tempWord[0] = CharToInt(str[0]);                         //make the B to 11 -- 00001011
	tempWord[1] = CharToInt(str[1]);                         //make the 0 to 0 -- 00000000
	chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000
	return chn;
}


std::string UrlDecode(std::string &src){
	std::string ret;
	char ch;
	int ii;
	for (size_t i=0; i<src.length(); i++) {
		if (int(src[i])==37) {
			sscanf(src.substr(i+1,2).c_str(), "%x", &ii);
			ch=static_cast<char>(ii);
			ret+=ch;
			i=i+2;
		} else {
			ret+=src[i];
		}
	}
	return (ret);
}


int split(const std::string& str, std::vector<std::string>& ret_, std::string sep){
	if (str.empty())return 0;
	std::string tmp;
	std::string::size_type pos_begin = str.find_first_not_of(sep);
	std::string::size_type comma_pos = 0;
	while (pos_begin != std::string::npos){
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != std::string::npos){
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}else{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}
		if (!tmp.empty()){
			ret_.push_back(UrlDecode(tmp));
			tmp.clear();
		}
	}
	return 0;
}

#ifdef WIN32
void UTF_8ToUnicode(wchar_t* pOut,const char *pText){  
	char* uchar = (char *)pOut; 
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F); 
} 
void UnicodeToUTF_8(char* pOut,const wchar_t* pText){
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后 
	char* pchar = (char *)pText; 
	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));
} 
void UnicodeToGB2312(char* pOut,wchar_t uData){
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL); 
}
void Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer){
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
		if( *(pText + i) >= 0){  
			rst[j++] = pText[i++];
		}else{  
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
void UTF_8ToGB2312(char*pOut, const char *pText, int pLen){   
	char Ctemp[4];   
	memset(Ctemp,0,4); 
	int i =0 ,j = 0; 
	while(i < pLen){  
		if(pText[i] >= 0){  
			pOut[j++] = pText[i++]; 
		}else{  
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


//把url GB2312解码
std::string UrlGB2312Decode(std::string str) {
	std::string output = "";
	char tmp[2];
    int i = 0, len = (int)str.length();
	while (i<len) {
		if (str[i] == '%') {
			tmp[0] = str[i + 1];
			tmp[1] = str[i + 2];
			output += StrToBin(tmp);
			i = i + 3;
		}
		else if (str[i] == '+') {
			output += ' ';
			i++;
		}
		else {
			output += str[i];
			i++;
		}
	}
	return output;
}





//把url utf8解码
std::string UrlUTF8Decode(std::string str) {
	char output[1024] = { 0 };
	std::string temp = UrlGB2312Decode(str);//
    UTF_8ToGB2312(output, (char *)temp.data(), (int)strlen(temp.data()));
	return output;
}

std::string utf2gb( const char* data ){
	if(!data)
		return "NULL";
	char szout[1024]={0};
    UTF_8ToGB2312(szout, data, (int)strlen(data));
	return std::string(szout);
}
std::string gb2utf( char* data){
	std::string str;
    GB2312ToUTF_8( str,data, (int)strlen(data) );
	return str;
}
#else
int code_convert(const char *from_charset,const char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen){
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;
	cd = iconv_open(to_charset,from_charset);
	if (cd==0)
		return -1;
	memset(outbuf,0,outlen);
	if( iconv(cd,pin,&inlen,pout,&outlen) == 0xFFFF)
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

#ifndef _XODBC_H_
#define _XODBC_H_

#if defined(_WIN32)
#include <windows.h>
#include <sqlext.h>
#elif defined(INFORMIX)
#include "infxcli.h"
//#elif defined(UNIXODBC)
#else
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <string.h>
#endif
#include <map>

class cmyodbc{
public:
	int setup();
	int close();
	int connect(const char* dns, const char* usr, const char* pwd);
	int disconnect(int idbc);
	int execute( int idbc,const char * sql);
	int freestmt(int index);
	int getcolcount( int index);
	int getcoltext( int index, int i, char* szcol );
	int fetch( int index);
	int getdata( int index, int pos,  char * data, unsigned int len);
private:
	HENV        m_hEnv;
private:
	std::map<int,HDBC>  myHdbcs;
	std::map<int,HSTMT> myHstmts;
	int                 myIndex;
};
#endif














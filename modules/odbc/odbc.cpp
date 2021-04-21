#include "odbc.h"
#include <stdio.h>


int cmyodbc::setup() {
	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	ret = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	if (ret == -1) {
		printf("SQLSetEnvAttr: %d\n", ret);
		return ret;
	}
	myIndex = 0;
	return 0;
}

//关闭链结
int cmyodbc::close() {
	std::map<int, HDBC>::iterator it = myHdbcs.begin();
	for (; it != myHdbcs.end(); it++) {
		SQLDisconnect(it->second);
		SQLFreeHandle(SQL_HANDLE_DBC, it->second);
	}
	return 0;
}

int cmyodbc::connect(const char* dns, const char* usr, const char* pwd){
	HDBC hDbc = NULL;
	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &hDbc);
	ret = SQLConnectA(hDbc,(SQLCHAR *)dns,strlen(dns),(SQLCHAR *)usr,strlen(usr),(SQLCHAR *)pwd,strlen(pwd));
	if(ret == -1 ) {
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		printf("connect error: %d\n", ret ); 
		return ret;
	}
	++myIndex;
	myHdbcs[myIndex] = hDbc;
	return myIndex;
}

int cmyodbc::disconnect( int idbc ) {
	SQLDisconnect(myHdbcs[idbc]);
	SQLFreeHandle(SQL_HANDLE_DBC, myHdbcs[idbc]);
	myHdbcs.erase(idbc);
	return 0;
}

//执行
int cmyodbc::execute(int idbc, const char * sql ){
	HSTMT hsmt = NULL;
	SQLAllocHandle(SQL_HANDLE_STMT, myHdbcs[idbc], &hsmt);
	SQLRETURN retcode = SQLExecDirectA(hsmt, (SQLCHAR *)sql, strlen(sql) );
	if( retcode == SQL_SUCCESS ){
		++myIndex;
		myHstmts[myIndex] = hsmt;
		return myIndex;
	}
	SQLCHAR szSqlState[256];
	SQLINTEGER pfNativeError;
	SQLCHAR szErrorMsg[256];
	SQLSMALLINT pcbErrorMsg;
	printf("sql:%s(%d)\n", sql, retcode );
	//for(size_t i=0;i<strlen(sql);i++)printf("%02x ",(unsigned char)sql[i]);printf("\r\n");
	SQLErrorA( m_hEnv, myHdbcs[idbc],hsmt,szSqlState,&pfNativeError,szErrorMsg,256, &pcbErrorMsg );
	printf("State:%s, NativeError:%d, Msg:%s\n",szSqlState,(int)pfNativeError, szErrorMsg);
	return -1;
}

int cmyodbc::freestmt(int index){
	SQLFreeHandle(SQL_HANDLE_STMT, myHstmts[index]);
	myHstmts.erase(index);
	return 0;
}

//获取列数
int cmyodbc::getcolcount(int index){
	short cols = 0;
	SQLNumResultCols(myHstmts[index], &cols);
	return cols;
}

//获取表头
int cmyodbc::getcoltext( int index, int i, char* szcol ){
	SQLDescribeColA(myHstmts[index],i,(SQLCHAR *)szcol, 256, NULL, NULL, NULL, NULL, NULL);
	return 0;
}

//获取每行数据
int cmyodbc::fetch( int index){
	return SQLFetch( myHstmts[index] );
}

//获取FETCH所在列数据
int cmyodbc:: getdata( int index, int pos,  char * data, unsigned int len){
	SQLGetData(myHstmts[index], pos, SQL_C_CHAR, data, len, NULL);
	return 0;
}


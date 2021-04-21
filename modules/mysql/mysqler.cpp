#include "mysqler.h"
#include <stdio.h>
#include <string.h>
int cmyodbc::connect(const char* dns, const char* usr, const char* pwd){
	 myconn = mysql_init(NULL);
	 if(!mysql_real_connect(myconn,"localhost","root","123456789","smartx",0,NULL,0)){  
		printf("无法连接数据库:%s",mysql_error(myconn));
		return -1;
	 }
	 myIndex = 0;
	// mysql_query(myconn,"set names gbk");//防止乱码。设置和数据库的编码一致就不会乱码
	return 0;
}

//执行
int cmyodbc::execute( const char * sql ){
	if( mysql_query( myconn , sql )){
		return -1;
	}
	myHstmts[++myIndex] = mysql_use_result(myconn);
	return myIndex;
}

//释放查询句柄
int cmyodbc::freestmt(int index){
	mysql_free_result(myHstmts[index]);
	myRows.erase(index);
	myHstmts.erase(index);
	return 0;
}

//获取列数
int cmyodbc::getcolcount(int index){
	short cols = 0;
	cols = mysql_num_fields( myHstmts[index] );
	return cols;
}

//获取表头
int cmyodbc::getcoltext( int index, int i, char* szcol ){
	//SQLDescribeCol(myHstmts[index],i,(SQLCHAR *)szcol, 256, NULL, NULL, NULL, NULL, NULL);
	return 0;
}

//获取每行数据
int cmyodbc::fetch( int index){
	MYSQL_ROW row = mysql_fetch_row( myHstmts[index] );
	if( !row ){
		return -1;
	}
	myRows[index] = row;
	return 0;
}

//获取FETCH所在列数据
int cmyodbc:: getdata(int index, int pos, char * data, unsigned int len){
	strncpy( data, myRows[index][pos],len);
	return 0;
}

//关闭链结
int cmyodbc::close(){
	mysql_close( myconn );
	return 0;
}

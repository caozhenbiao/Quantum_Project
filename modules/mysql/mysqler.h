#ifndef _MYSQLER_H_
#define _MYSQLER_H_
#include <map>

#ifdef WIN32
#include <winsock.h> //ע��˳��Ҫ����mysql.hǰ
#include <mysql.h>
#else
#include <mysql/mysql.h>//����̨��Ŀ��Ҫ��mysql.h֮ǰinclude <winsock.h>
// please install libmysql++-dev( apt-get install ..)
#endif
class cmyodbc{
public:
	int connect(const char* dns, const char* usr, const char* pwd);
	int close();
	int execute( const char * sql);
	int freestmt(int index);
	int getcolcount( int index);
	int getcoltext( int index, int i, char* szcol );
	int fetch( int index);
	int getdata( int index, int pos,  char * data, unsigned int len);
private:
	MYSQL * myconn;
private:
	std::map<int,MYSQL_RES*> myHstmts;
	std::map<int,MYSQL_ROW>  myRows;
	int                      myIndex;
};
#endif














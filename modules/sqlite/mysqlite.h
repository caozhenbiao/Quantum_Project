#ifndef _MYSQLITE_H_
#define _MYSQLITE_H_

#include "sqlite3.h"
#include <map>

class mysqlite{
public:
	int open(const char* file, const char* pwd);
	int execute(int db, const char * sql);
	int prepare(int db, const char * sql);
	int setup(int mt);
	const char* column_text(int mt, int col);
	int column_int(int mt, int col);
	double column_double(int mt, int col);
	const void * column_blob(int mt, int col, int& len);
	int finalize(int mt);
	int close(int db);
	int bind_blob(int mt, int col, void* blod, int len);
	int bind_int(int mt, int col, int val);
	int bind_text(int mt, int col, const char* val);

private:
	std::map<int, sqlite3*>       mydbs;
	std::map<int, sqlite3_stmt*>  mymts;
};
#endif



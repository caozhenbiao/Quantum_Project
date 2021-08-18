#include "mysqlite.h"
#include <stdio.h>
#include <string.h>

int mysqlite::open(const char* file, const char* pwd){
	sqlite3 * mydb = NULL;
	if( sqlite3_open(file, &mydb) != 0 || !mydb )
		return -1;
	//sqlite3_key(mydb, pwd, strlen(pwd));
	static int db_idx = 0;
	mydbs[++db_idx] = mydb;
	return db_idx;
}

//ִ��
int mysqlite::execute( int db, const char * sql ){
	char *errmsg = NULL;
	int ret = sqlite3_exec(mydbs[db], sql, 0, 0, &errmsg);
	return ret;
}

int mysqlite::prepare( int db, const char * sql ) {
	sqlite3_stmt * stmt = NULL;
	const char *zTail;
	if (sqlite3_prepare_v2(mydbs[db], sql, -1, &stmt, &zTail) != SQLITE_OK)
		return -1;
	static int mt_idx = 0;
	mymts[++mt_idx] = stmt;
	return mt_idx;
}

int mysqlite::setup( int mt ) {
	if (SQLITE_ROW == sqlite3_step(mymts[mt]))
		return 0;
	return -1;
}

const char* mysqlite::column_text( int mt, int col ) {
	const unsigned char * text = sqlite3_column_text(mymts[mt], col);
	return (const char*)text;
}

int mysqlite::column_int(int mt, int col) {
	return sqlite3_column_int(mymts[mt], col);
}

double mysqlite::column_double(int mt, int col) {
	return sqlite3_column_double(mymts[mt], col);
}
const void* mysqlite::column_blob(int mt, int col, int& len) {
	len = sqlite3_column_bytes(mymts[mt],col);
	return sqlite3_column_blob(mymts[mt], col);
}

int mysqlite::finalize(int mt) {
	int ret = sqlite3_finalize(mymts[mt]);
	mymts.erase(mt);
	return ret;
}

int mysqlite::close(int db) {
	return sqlite3_close(mydbs[db] );
}

int mysqlite::bind_blob(int mt, int col, void* blod, int len) {
	return sqlite3_bind_blob(mymts[mt], col, blod, len, NULL);
}

int mysqlite::bind_int(int mt, int col, int val) {

	return sqlite3_bind_int(mymts[mt], col, val);

}

int mysqlite::bind_text(int mt, int col, const char* val) {

	return sqlite3_bind_text(mymts[mt], col, val, (int)strlen(val), NULL);
}
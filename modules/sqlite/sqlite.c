#include "sqlite.h"
#include "sqlite3.h"
#include <stdio.h>
#include <string.h>
#include "./map.h"

map_t(sqlite3*) map_sqlite_ins;
map_t(sqlite3_stmt*) map_stmt_ins;

void sqlite_init() {
	map_init(&map_sqlite_ins);
	map_init(&map_stmt_ins);
}

void sqlite_deinit() {
	map_deinit(&map_sqlite_ins);
	map_deinit(&map_stmt_ins);
}

int sqlite_open(const char* file, const char* pwd){
	sqlite3 * mydb = NULL;
	if( sqlite3_open(file, &mydb) != 0 || !mydb )
		return -1;
	//sqlite3_key(mydb, pwd, strlen(pwd));
	static int db_idx = 100;
	map_set(&map_sqlite_ins, ++db_idx, mydb);
	return db_idx;
}

//
int sqlite_execute( int db, const char * sql ){
	int ret = -1;
	sqlite3 * mydb = *(map_get(&map_sqlite_ins, db));
	if (mydb) {
		char *errmsg = NULL;
		ret = sqlite3_exec(mydb, sql, 0, 0, &errmsg);
	}
	return ret;
}

int sqlite_prepare( int db, const char * sql ) {
	sqlite3 * mydb = *(map_get(&map_sqlite_ins, db));
	sqlite3_stmt * stmt = NULL;
	if (mydb) {
		const char *zTail;
		if (sqlite3_prepare_v2(mydb, sql, -1, &stmt, &zTail) != SQLITE_OK)
			return -1;
	}
	static int mt_idx = 0;
	map_set(&map_stmt_ins, ++mt_idx, stmt);
	return mt_idx;
}

int sqlite_setup( int mt ) {
	sqlite3_stmt * mymt = *(map_get(&map_stmt_ins, mt));
	if (mymt && SQLITE_ROW == sqlite3_step(mymt))
		return 0;
	return -1;
}

const char* sqlite_column_text( int mt, int col ) {
	sqlite3_stmt * mymt = *(map_get(&map_stmt_ins, mt));
	if (mymt) {
		const unsigned char * text = sqlite3_column_text(mymt, col);
		return (const char*)text;
	}
	return "";
}

int sqlite_column_int(int mt, int col) {
	sqlite3_stmt * mymt = *(map_get(&map_stmt_ins, mt));
	if (mymt) {
		return sqlite3_column_int(mymt, col);
	}
	return -1;
}

double sqlite_column_double(int mt, int col) {
	sqlite3_stmt * mymt = *(map_get(&map_stmt_ins, mt));
	if (mymt) {
		return sqlite3_column_double(mymt, col);
	}
	return -1;
}
const void* sqlite_column_blob(int mt, int col, int* len) {
	sqlite3_stmt * mymt = *(map_get(&map_stmt_ins, mt));
	if (mymt) {
		*len = sqlite3_column_bytes(mymt, col);
		return sqlite3_column_blob(mymt, col);
	}
	return NULL;
}

int sqlite_finalize(int mt) {
	sqlite3_stmt * mymt = *(map_get(&map_stmt_ins, mt));
	if (mymt) {
		int ret = sqlite3_finalize(mymt);
		map_remove(&map_stmt_ins, mt);
		return ret;
	}
	return -1;
}

int sqlite_close(int db) {
	sqlite3 * mydb = *(map_get(&map_sqlite_ins, db));
	if (mydb) {
		return sqlite3_close(mydb);
	}
	return -1;
}

int sqlite_bind_blob(int mt, int col, void* blod, int len) {
	sqlite3_stmt * mymt = *(map_get(&map_stmt_ins, mt));
	if (mymt) {
		return sqlite3_bind_blob(mymt, col, blod, len, NULL);
	}
	return -1;
}

int sqlite_bind_int(int mt, int col, int val) {
	sqlite3_stmt * mymt = *(map_get(&map_stmt_ins, mt));
	if (mymt) {
		return sqlite3_bind_int(mymt, col, val);
	}
	return -1;
}

int sqlite_bind_text(int mt, int col, const char* val) {
	sqlite3_stmt * mymt = *(map_get(&map_stmt_ins, mt));
	if (mymt) {
		return sqlite3_bind_text(mymt, col, val, (int)strlen(val), NULL);
	}
	return -1;
}

int sqlite_table_count(int db, const char* name,const char* cond) {
	int retcount = 0;
	char sql[1024] = { 0 };
	sprintf(sql, "select count(*) from %s %s", name, cond);
	sqlite3_stmt * stmt = NULL;
	const char *zTail;
	sqlite3 * mydb = *(map_get(&map_sqlite_ins, db));
	if (mydb && sqlite3_prepare_v2(mydb, sql, -1, &stmt, &zTail) == SQLITE_OK) {
		if (SQLITE_ROW == sqlite3_step(stmt)) {
			retcount = sqlite3_column_int(stmt, 0);
		}
	}
	sqlite3_finalize( stmt );
	return retcount;
}

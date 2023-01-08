#ifndef _MYSQLITE_H_
#define _MYSQLITE_H_
void sqlite_init();
void sqlite_deinit();
int sqlite_open(const char* file, const char* pwd);
int sqlite_execute(int db, const char * sql);
int sqlite_prepare(int db, const char * sql);
int sqlite_setup(int mt);
const char* sqlite_column_text(int mt, int col);
int sqlite_column_int(int mt, int col);
double sqlite_column_double(int mt, int col);
const void * sqlite_column_blob(int mt, int col, int* len);
int sqlite_finalize(int mt);
int sqlite_close(int db);
int sqlite_bind_blob(int mt, int col, void* blod, int len);
int sqlite_bind_int(int mt, int col, int val);
int sqlite_bind_text(int mt, int col, const char* val);
int sqlite_table_count(int db, const char* name,const char* cond);
#endif



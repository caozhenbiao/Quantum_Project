#ifndef _CSQLITE_H_
#define _CSQLITE_H_
#define CPPSQLITE_ERROR 1000
#include "sqlite3.h"

#ifndef WIN32
//typedef long long __int64;
#define __int64 long long
#endif

class CSQLiteException
{
public:

	CSQLiteException(const int nErrCode, char* szErrMess, bool bDeleteMsg = true);
	CSQLiteException(const CSQLiteException&  e);
	virtual ~CSQLiteException();
	const int errorCode() {return m_nErrCode;}
	const char* errorMessage() {return m_pszErrMess;}
	static const char* errorCodeAsString(int nErrCode);
private:
	int m_nErrCode;
	char* m_pszErrMess;
};

/////////////////////////////////////////////////////////////////////////////////////////////////

class CSQLiteQuery
{
public:
	CSQLiteQuery();
	CSQLiteQuery(const CSQLiteQuery& rQuery);
	CSQLiteQuery(sqlite3* pDB, sqlite3_stmt* pVM, bool bEof, bool bOwnVM = true);
	CSQLiteQuery& operator=(const CSQLiteQuery& rQuery);
	virtual ~CSQLiteQuery();
	int numFields();
	int fieldIndex(const char* szField);
	const char* fieldName(int nCol);
	const char* fieldDeclType(int nCol);
	int fieldDataType(int nCol);
	const char* fieldValue(int nField);
	const char* fieldValue(const char* szField);
	int getIntField(int nField, int nNullValue = 0);
	int getIntField(const char* szField, int nNullValue = 0);
	__int64 getInt64Field(int nField, __int64 nNullValue = 0);
	__int64 getInt64Field(const char* szField, __int64 nNullValue = 0);
	double getFloatField(int nField, double fNullValue = 0.0);
	double getFloatField(const char* szField, double fNullValue = 0.0);
	const char* getStringField(int nField, const char* szNullValue = "");
	const char* getStringField(const char* szField, const char* szNullValue = "");
	const unsigned char* getBlobField(int nField, int& nLen);
	const unsigned char* getBlobField(const char* szField, int& nLen);
	bool fieldIsNull(int nField);
	bool fieldIsNull(const char* szField);
	bool eof();
	void nextRow();
	void finalize();
private:
	void checkVM();
private:
	sqlite3* m_pDB;
	sqlite3_stmt* m_pStmt;
	bool m_bEof;
	int m_nCols;
	bool m_bOwnStmt;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/*
class CSQLiteTable
{
public:
	CSQLiteTable();
	CSQLiteTable(const CSQLiteTable& rTable);
	CSQLiteTable(char** paszResults, int nRows, int nCols);
	virtual ~CSQLiteTable();
	CSQLiteTable& operator=(const CSQLiteTable& rTable);
	int numFields();
	int numRows();
	const char* fieldName(int nCol);
	const char* fieldValue(int nField);
	const char* fieldValue(const char* szField);
	int getIntField(int nField, int nNullValue = 0);
	int getIntField(const char* szField, int nNullValue = 0);
	sqlite3_int64 getInt64Field(int nField, sqlite3_int64 nNullValue = 0);
	sqlite3_int64 getInt64Field(const char* szField, sqlite3_int64 nNullValue = 0);
	double getFloatField(int nField, double fNullValue = 0.0);
	double getFloatField(const char* szField, double fNullValue = 0.0);
	const char* getStringField(int nField, const char* szNullValue = "");
	const char* getStringField(const char* szField, const char* szNullValue = "");
	bool fieldIsNull(int nField);
	bool fieldIsNull(const char* szField);
	void setRow(int nRow);
	void finalize();
private:
	void checkResults();
private:
	int m_nCols;
	int m_nRows;
	int m_nCurrentRow;
	char** m_paszResults;
};
*/
/////////////////////////////////////////////////////////////////////////////////////////////////

class CSQLiteStatement
{
public:
	CSQLiteStatement();
	CSQLiteStatement(const CSQLiteStatement& rStatement);
	CSQLiteStatement(sqlite3* pDB, sqlite3_stmt* pVM);
	virtual ~CSQLiteStatement();
	CSQLiteStatement& operator=(const CSQLiteStatement& rStatement);
	int execDML();
	CSQLiteQuery execQuery();
	void bind(int nParam, const char* szValue);
	void bind(int nParam, const int nValue);
	void bind(int nParam, const __int64 nValue);
	void bind(int nParam, const double dwValue);
	void bind(int nParam, const unsigned char* blobValue, int nLen);
	void bindNull(int nParam);
	void reset();
	void finalize();
private:
	void checkDB();
	void checkVM();
private:
	sqlite3* m_pDB;
	sqlite3_stmt* m_pStmt;
};

/////////////////////////////////////////////////////////////////////////////////////////////////

class CSQLiteDB
{
public:
	CSQLiteDB();
	virtual ~CSQLiteDB();
	bool open(const char* szFile);
	void close();
	bool tableExists(const char* szTable);
	bool indexExists(const char* szIndex);
	int execDML(const char* szSQL);
	CSQLiteQuery execQuery(const char* szSQL);
	int execScalar(const char* szSQL);
//	CSQLiteTable getTable(const char* szSQL);
	CSQLiteStatement compileStatement(const char* szSQL);
	sqlite_int64 lastRowId();
	void interrupt() {sqlite3_interrupt(m_pDB);}
	void setBusyTimeout(int nMillisecs);
	static const char* SQLiteVersion() {return SQLITE_VERSION;}
private:
	CSQLiteDB(const CSQLiteDB& db);
	CSQLiteDB& operator=(const CSQLiteDB& db);
	sqlite3_stmt* compile(const char* szSQL);
	void checkDB();
private:
	sqlite3* m_pDB;
	int m_nBusyTimeoutMs;
};

#endif


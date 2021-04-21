#include "CSQLite.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

#ifndef WIN32
#include <unistd.h>
#define	sprintf_s(buffer, buffer_size, ...) (sprintf(buffer, ##__VA_ARGS__))
#endif

// Named constant for passing to CSQLiteException when passing it a string
// that cannot be deleted.
static const bool DONT_DELETE_MSG = false;

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

//*****************************************************************************************
CSQLiteException::CSQLiteException(const int nErrCode,
								   char* szErrMess,
								   bool bDeleteMsg/*=true*/)
								   : m_nErrCode(nErrCode)
{
	m_pszErrMess = sqlite3_mprintf("%s[%d]: %s",
		errorCodeAsString(nErrCode),
		nErrCode,
		szErrMess ? szErrMess : "");

	if (bDeleteMsg && szErrMess)
	{
		sqlite3_free(szErrMess);
	}
}

//*****************************************************************************************					
CSQLiteException::CSQLiteException(const CSQLiteException&  e)
: m_nErrCode(e.m_nErrCode)
{
	m_pszErrMess = 0;
	if (e.m_pszErrMess)
	{
		m_pszErrMess = sqlite3_mprintf("%s", e.m_pszErrMess);
	}
}

//*****************************************************************************************
const char* CSQLiteException::errorCodeAsString(int nErrCode)
{
	switch (nErrCode)
	{
	case SQLITE_OK          : return "SQLITE_OK";
	case SQLITE_ERROR       : return "SQLITE_ERROR";
	case SQLITE_INTERNAL    : return "SQLITE_INTERNAL";
	case SQLITE_PERM        : return "SQLITE_PERM";
	case SQLITE_ABORT       : return "SQLITE_ABORT";
	case SQLITE_BUSY        : return "SQLITE_BUSY";
	case SQLITE_LOCKED      : return "SQLITE_LOCKED";
	case SQLITE_NOMEM       : return "SQLITE_NOMEM";
	case SQLITE_READONLY    : return "SQLITE_READONLY";
	case SQLITE_INTERRUPT   : return "SQLITE_INTERRUPT";
	case SQLITE_IOERR       : return "SQLITE_IOERR";
	case SQLITE_CORRUPT     : return "SQLITE_CORRUPT";
	case SQLITE_NOTFOUND    : return "SQLITE_NOTFOUND";
	case SQLITE_FULL        : return "SQLITE_FULL";
	case SQLITE_CANTOPEN    : return "SQLITE_CANTOPEN";
	case SQLITE_PROTOCOL    : return "SQLITE_PROTOCOL";
	case SQLITE_EMPTY       : return "SQLITE_EMPTY";
	case SQLITE_SCHEMA      : return "SQLITE_SCHEMA";
	case SQLITE_TOOBIG      : return "SQLITE_TOOBIG";
	case SQLITE_CONSTRAINT  : return "SQLITE_CONSTRAINT";
	case SQLITE_MISMATCH    : return "SQLITE_MISMATCH";
	case SQLITE_MISUSE      : return "SQLITE_MISUSE";
	case SQLITE_NOLFS       : return "SQLITE_NOLFS";
	case SQLITE_AUTH        : return "SQLITE_AUTH";
	case SQLITE_FORMAT      : return "SQLITE_FORMAT";
	case SQLITE_RANGE       : return "SQLITE_RANGE";
	case SQLITE_ROW         : return "SQLITE_ROW";
	case SQLITE_DONE        : return "SQLITE_DONE";
	case CPPSQLITE_ERROR    : return "CPPSQLITE_ERROR";
	default: return "UNKNOWN_ERROR";
	}
}

//*****************************************************************************************
CSQLiteException::~CSQLiteException()
{
	if (m_pszErrMess)
	{
		sqlite3_free(m_pszErrMess);
		m_pszErrMess = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
//  CSQLiteQuery

//*****************************************************************************************
CSQLiteQuery::CSQLiteQuery()
{
	m_pStmt = 0;
	m_bEof = true;
	m_nCols = 0;
	m_bOwnStmt = false;
}

//*****************************************************************************************
CSQLiteQuery::CSQLiteQuery(const CSQLiteQuery& rQuery)
{
	m_pStmt = rQuery.m_pStmt;
	// Only one object can own the VM
	const_cast<CSQLiteQuery&>(rQuery).m_pStmt = 0;
	m_bEof = rQuery.m_bEof;
	m_nCols = rQuery.m_nCols;
	m_bOwnStmt = rQuery.m_bOwnStmt;
}

//*****************************************************************************************
CSQLiteQuery::CSQLiteQuery(sqlite3* pDB,
						   sqlite3_stmt* pVM,
						   bool bEof,
						   bool bOwnVM/*=true*/)
{
	m_pDB = pDB;
	m_pStmt = pVM;
	m_bEof = bEof;
	m_nCols = sqlite3_column_count(m_pStmt);
	m_bOwnStmt = bOwnVM;
}

//*****************************************************************************************
CSQLiteQuery::~CSQLiteQuery()
{
	try {
		finalize();
	} catch (...) {
	}
}

//*****************************************************************************************
CSQLiteQuery& CSQLiteQuery::operator=(const CSQLiteQuery& rQuery)
{
	try {
		finalize();
	} catch (...) {
	}
	m_pStmt = rQuery.m_pStmt;
	// Only one object can own the VM
	const_cast<CSQLiteQuery&>(rQuery).m_pStmt = 0;
	m_bEof = rQuery.m_bEof;
	m_nCols = rQuery.m_nCols;
	m_bOwnStmt = rQuery.m_bOwnStmt;
	return *this;
}

//*****************************************************************************************
int CSQLiteQuery::numFields()
{
	checkVM();
	return m_nCols;
}

//*****************************************************************************************
const char* CSQLiteQuery::fieldValue(int nField)
{
	checkVM();

	if (nField < 0 || nField > m_nCols - 1)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			(char *)"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return (const char*)sqlite3_column_text(m_pStmt, nField);
}

//*****************************************************************************************
const char* CSQLiteQuery::fieldValue(const char* szField)
{
	int nField = fieldIndex(szField);
	return (const char*)sqlite3_column_text(m_pStmt, nField);
}

//*****************************************************************************************
int CSQLiteQuery::getIntField(int nField, int nNullValue/*=0*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return nNullValue;
	}
	else
	{
		return sqlite3_column_int(m_pStmt, nField);
	}
}

//*****************************************************************************************
int CSQLiteQuery::getIntField(const char* szField, int nNullValue/*=0*/)
{
	int nField = fieldIndex(szField);
	return getIntField(nField, nNullValue);
}

__int64 CSQLiteQuery::getInt64Field(int nField, __int64 nNullValue)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return nNullValue;
	}
	else
	{
		return sqlite3_column_int64(m_pStmt, nField);
	}
}

__int64 CSQLiteQuery::getInt64Field(const char* szField, __int64 nNullValue)
{
	int nField = fieldIndex(szField);
	return getInt64Field(nField, nNullValue);
}

//*****************************************************************************************
double CSQLiteQuery::getFloatField(int nField, double fNullValue/*=0.0*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return fNullValue;
	}
	else
	{
		return sqlite3_column_double(m_pStmt, nField);
	}
}

//*****************************************************************************************
double CSQLiteQuery::getFloatField(const char* szField, double fNullValue/*=0.0*/)
{
	int nField = fieldIndex(szField);
	return getFloatField(nField, fNullValue);
}

//*****************************************************************************************
const char* CSQLiteQuery::getStringField(int nField, const char* szNullValue/*=""*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return szNullValue;
	}
	else
	{
		return (const char*)sqlite3_column_text(m_pStmt, nField);
	}
}

//*****************************************************************************************
const char* CSQLiteQuery::getStringField(const char* szField, const char* szNullValue/*=""*/)
{
	int nField = fieldIndex(szField);
	return getStringField(nField, szNullValue);
}

//*****************************************************************************************
const unsigned char* CSQLiteQuery::getBlobField(int nField, int& nLen)
{
	checkVM();

	if (nField < 0 || nField > m_nCols - 1)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			(char *)"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	nLen = sqlite3_column_bytes(m_pStmt, nField);
	return (const unsigned char*)sqlite3_column_blob(m_pStmt, nField);
}

//*****************************************************************************************
const unsigned char* CSQLiteQuery::getBlobField(const char* szField, int& nLen)
{
	int nField = fieldIndex(szField);
	return getBlobField(nField, nLen);
}

//*****************************************************************************************
bool CSQLiteQuery::fieldIsNull(int nField)
{
	return (fieldDataType(nField) == SQLITE_NULL);
}

//*****************************************************************************************
bool CSQLiteQuery::fieldIsNull(const char* szField)
{
	int nField = fieldIndex(szField);
	return (fieldDataType(nField) == SQLITE_NULL);
}

//*****************************************************************************************
int CSQLiteQuery::fieldIndex(const char* szField)
{
	checkVM();

	if (szField)
	{
		for (int nField = 0; nField < m_nCols; nField++)
		{
			const char* szTemp = sqlite3_column_name(m_pStmt, nField);
			if (strcmp(szField, szTemp) == 0)
				return nField;
		}
	}

	throw CSQLiteException(CPPSQLITE_ERROR,
		(char *)"Invalid field name requested",
		DONT_DELETE_MSG);
}

//*****************************************************************************************
const char* CSQLiteQuery::fieldName(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > m_nCols - 1)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			(char *)"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return sqlite3_column_name(m_pStmt, nCol);
}

//*****************************************************************************************
const char* CSQLiteQuery::fieldDeclType(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > m_nCols-1)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			(char *)"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return sqlite3_column_decltype(m_pStmt, nCol);
}

//*****************************************************************************************
int CSQLiteQuery::fieldDataType(int nCol)
{
	checkVM();

	if (nCol < 0 || nCol > m_nCols - 1)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			(char *)"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return sqlite3_column_type(m_pStmt, nCol);
}

//*****************************************************************************************
bool CSQLiteQuery::eof()
{
	checkVM();
	return m_bEof;
}

//*****************************************************************************************
void CSQLiteQuery::nextRow()
{
	checkVM();

	int nRet = sqlite3_step(m_pStmt);

	if (nRet == SQLITE_DONE)
	{
		// no rows
		m_bEof = true;
	}
	else if (nRet == SQLITE_ROW)
	{
		// more rows, nothing to do
	}
	else
	{
		nRet = sqlite3_finalize(m_pStmt);
		m_pStmt = 0;
		const char* szError = sqlite3_errmsg(m_pDB);
		throw CSQLiteException(nRet,
			(char*)szError,
			DONT_DELETE_MSG);
	}
}

//*****************************************************************************************
void CSQLiteQuery::finalize()
{
	if (m_pStmt && m_bOwnStmt)
	{
		int nRet = sqlite3_finalize(m_pStmt);
		m_pStmt = 0;
		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(m_pDB);
			throw CSQLiteException(nRet, (char*)szError, DONT_DELETE_MSG);
		}
	}
}

//*****************************************************************************************
void CSQLiteQuery::checkVM()
{
	if (m_pStmt == 0)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			(char *)"Null Virtual Machine pointer",
			DONT_DELETE_MSG);
	}
}

/*
CSQLiteTable::CSQLiteTable()
{
	m_paszResults = 0;
	m_nRows = 0;
	m_nCols = 0;
	m_nCurrentRow = 0;
}

CSQLiteTable::CSQLiteTable(const CSQLiteTable& rTable)
{
	m_paszResults = rTable.m_paszResults;
	// Only one object can own the results
	const_cast<CSQLiteTable&>(rTable).m_paszResults = 0;
	m_nRows = rTable.m_nRows;
	m_nCols = rTable.m_nCols;
	m_nCurrentRow = rTable.m_nCurrentRow;
}

CSQLiteTable::CSQLiteTable(char** paszResults, int nRows, int nCols)
{
	m_paszResults = paszResults;
	m_nRows = nRows;
	m_nCols = nCols;
	m_nCurrentRow = 0;
}

CSQLiteTable::~CSQLiteTable()
{
	try {
		finalize();
	} catch (...) {
	}
}

CSQLiteTable& CSQLiteTable::operator=(const CSQLiteTable& rTable)
{
	try {
		finalize();
	} catch (...) {
	}
	m_paszResults = rTable.m_paszResults;
	// Only one object can own the results
	const_cast<CSQLiteTable&>(rTable).m_paszResults = 0;
	m_nRows = rTable.m_nRows;
	m_nCols = rTable.m_nCols;
	m_nCurrentRow = rTable.m_nCurrentRow;
	return *this;
}

void CSQLiteTable::finalize()
{
	if (m_paszResults)
	{
		sqlite3_free_table(m_paszResults);
		m_paszResults = 0;
	}
}

int CSQLiteTable::numFields()
{
	checkResults();
	return m_nCols;
}

int CSQLiteTable::numRows()
{
	checkResults();
	return m_nRows;
}

const char* CSQLiteTable::fieldValue(int nField)
{
	checkResults();

	if (nField < 0 || nField > m_nCols - 1)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	int nIndex = (m_nCurrentRow*m_nCols) + m_nCols + nField;
	return m_paszResults[nIndex];
}

const char* CSQLiteTable::fieldValue(const char* szField)
{
	checkResults();

	if (szField)
	{
		for (int nField = 0; nField < m_nCols; nField++)
		{
			if (strcmp(szField, m_paszResults[nField]) == 0)
			{
				int nIndex = (m_nCurrentRow*m_nCols) + m_nCols + nField;
				return m_paszResults[nIndex];
			}
		}
	}

	throw CSQLiteException(CPPSQLITE_ERROR,
		"Invalid field name requested",
		DONT_DELETE_MSG);
}

int CSQLiteTable::getIntField(int nField, int nNullValue)
{
	if (fieldIsNull(nField))
	{
		return nNullValue;
	}
	else
	{
		return atoi(fieldValue(nField));
	}
}

int CSQLiteTable::getIntField(const char* szField, int nNullValue)
{
	if (fieldIsNull(szField))
	{
		return nNullValue;
	}
	else
	{
		return atoi(fieldValue(szField));
	}
}

sqlite3_int64 CSQLiteTable::getInt64Field(int nField, sqlite3_int64 nNullValue)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return nNullValue;
	}
	else
	{
		return sqlite3_column_int64(m_pStmt, nField);
	}
}

sqlite3_int64 CSQLiteTable::getInt64Field(const char* szField, sqlite3_int64 nNullValue)
{
	int nField = fieldIndex(szField);
	return getInt64Field(nField, nNullValue);
}

double CSQLiteTable::getFloatField(int nField, double fNullValue)
{
	if (fieldIsNull(nField))
	{
		return fNullValue;
	}
	else
	{
		return atof(fieldValue(nField));
	}
}

double CSQLiteTable::getFloatField(const char* szField, double fNullValue)
{
	if (fieldIsNull(szField))
	{
		return fNullValue;
	}
	else
	{
		return atof(fieldValue(szField));
	}
}

const char* CSQLiteTable::getStringField(int nField, const char* szNullValue)
{
	if (fieldIsNull(nField))
	{
		return szNullValue;
	}
	else
	{
		return fieldValue(nField);
	}
}

const char* CSQLiteTable::getStringField(const char* szField, const char* szNullValue)
{
	if (fieldIsNull(szField))
	{
		return szNullValue;
	}
	else
	{
		return fieldValue(szField);
	}
}

bool CSQLiteTable::fieldIsNull(int nField)
{
	checkResults();
	return (fieldValue(nField) == 0);
}

bool CSQLiteTable::fieldIsNull(const char* szField)
{
	checkResults();
	return (fieldValue(szField) == 0);
}

const char* CSQLiteTable::fieldName(int nCol)
{
	checkResults();

	if (nCol < 0 || nCol > m_nCols-1)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return m_paszResults[nCol];
}

void CSQLiteTable::setRow(int nRow)
{
	checkResults();

	if (nRow < 0 || nRow > m_nRows-1)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			"Invalid row index requested",
			DONT_DELETE_MSG);
	}

	m_nCurrentRow = nRow;
}

void CSQLiteTable::checkResults()
{
	if (m_paszResults == 0)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			"Null Results pointer",
			DONT_DELETE_MSG);
	}
}
*/

CSQLiteStatement::CSQLiteStatement()
{
	m_pDB = 0;
	m_pStmt = 0;
}

CSQLiteStatement::CSQLiteStatement(const CSQLiteStatement& rStatement)
{
	m_pDB = rStatement.m_pDB;
	m_pStmt = rStatement.m_pStmt;
	// Only one object can own VM
	const_cast<CSQLiteStatement&>(rStatement).m_pStmt = 0;
}

CSQLiteStatement::CSQLiteStatement(sqlite3* pDB, sqlite3_stmt* pVM)
{
	m_pDB = pDB;
	m_pStmt = pVM;
}

CSQLiteStatement::~CSQLiteStatement()
{
	try {
		finalize();
	} catch (...) {
	}
}

CSQLiteStatement& CSQLiteStatement::operator=(const CSQLiteStatement& rStatement)
{
	m_pDB = rStatement.m_pDB;
	m_pStmt = rStatement.m_pStmt;
	// Only one object can own VM
	const_cast<CSQLiteStatement&>(rStatement).m_pStmt = 0;
	return *this;
}

int CSQLiteStatement::execDML()
{
	checkDB();
	checkVM();

	const char* szError = 0;

	int nRet = sqlite3_step(m_pStmt);

	if (nRet == SQLITE_DONE)
	{
		int nRowsChanged = sqlite3_changes(m_pDB);

		nRet = sqlite3_reset(m_pStmt);

		if (nRet != SQLITE_OK)
		{
			szError = sqlite3_errmsg(m_pDB);
			throw CSQLiteException(nRet, (char*)szError, DONT_DELETE_MSG);
		}

		return nRowsChanged;
	}
	else
	{
		nRet = sqlite3_reset(m_pStmt);
		szError = sqlite3_errmsg(m_pDB);
		throw CSQLiteException(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}

CSQLiteQuery CSQLiteStatement::execQuery()
{
	checkDB();
	checkVM();

	int nRet = sqlite3_step(m_pStmt);
	if (nRet == SQLITE_DONE)
	{
		// no rows
		return CSQLiteQuery(m_pDB, m_pStmt, true, false);
	}
	else if (nRet == SQLITE_ROW)
	{
		// at least 1 row
		return CSQLiteQuery(m_pDB, m_pStmt, false, false);
	}
	else
	{
		nRet = sqlite3_reset(m_pStmt);
		const char* szError = sqlite3_errmsg(m_pDB);
		throw CSQLiteException(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}

void CSQLiteStatement::bind(int nParam, const char* szValue)
{
	checkVM();
	int nRes = sqlite3_bind_text(m_pStmt, nParam, szValue, -1, SQLITE_TRANSIENT);
	if (nRes != SQLITE_OK)
	{
		throw CSQLiteException(nRes,
			(char *)"Error binding string param",
			DONT_DELETE_MSG);
	}
}

void CSQLiteStatement::bind(int nParam, const int nValue)
{
	checkVM();
	int nRes = sqlite3_bind_int(m_pStmt, nParam, nValue);
	if (nRes != SQLITE_OK)
	{
		throw CSQLiteException(nRes,
			(char *)"Error binding int param",
			DONT_DELETE_MSG);
	}
}

void CSQLiteStatement::bind(int nParam, const __int64 nValue)
{
	checkVM();
	int nRes = sqlite3_bind_int64(m_pStmt, nParam, nValue);
	if (nRes != SQLITE_OK)
	{
		throw CSQLiteException(nRes,
			(char *)"Error binding int64 param",
			DONT_DELETE_MSG);
	}
}

void CSQLiteStatement::bind(int nParam, const double dValue)
{
	checkVM();
	int nRes = sqlite3_bind_double(m_pStmt, nParam, dValue);
	if (nRes != SQLITE_OK)
	{
		throw CSQLiteException(nRes,
			(char *)"Error binding double param",
			DONT_DELETE_MSG);
	}
}

void CSQLiteStatement::bind(int nParam, const unsigned char* blobValue, int nLen)
{
	checkVM();
	int nRes = sqlite3_bind_blob(m_pStmt, nParam, (const void*)blobValue, nLen, SQLITE_TRANSIENT);
	if (nRes != SQLITE_OK)
	{
		throw CSQLiteException(nRes,
			(char *)"Error binding blob param",
			DONT_DELETE_MSG);
	}
}

void CSQLiteStatement::bindNull(int nParam)
{
	checkVM();
	int nRes = sqlite3_bind_null(m_pStmt, nParam);
	if (nRes != SQLITE_OK)
	{
		throw CSQLiteException(nRes,
			(char *)"Error binding NULL param",
			DONT_DELETE_MSG);
	}
}

void CSQLiteStatement::reset()
{
	if (m_pStmt)
	{
		int nRet = sqlite3_reset(m_pStmt);
		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(m_pDB);
			throw CSQLiteException(nRet, (char*)szError, DONT_DELETE_MSG);
		}
	}
}

void CSQLiteStatement::finalize()
{
	if (m_pStmt)
	{
		int nRet = sqlite3_finalize(m_pStmt);
		m_pStmt = 0;

		if (nRet != SQLITE_OK)
		{
			const char* szError = sqlite3_errmsg(m_pDB);
			throw CSQLiteException(nRet, (char*)szError, DONT_DELETE_MSG);
		}
	}
}

void CSQLiteStatement::checkDB()
{
	if (m_pDB == 0)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			(char *)"Database not open",
			DONT_DELETE_MSG);
	}
}

void CSQLiteStatement::checkVM()
{
	if (m_pStmt == 0)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			(char *)"Null Virtual Machine pointer",
			DONT_DELETE_MSG);
	}
}


CSQLiteDB::CSQLiteDB()
{
	m_pDB = 0;
	m_nBusyTimeoutMs = 60000; // 60 seconds
}

CSQLiteDB::CSQLiteDB(const CSQLiteDB& db)
{
	m_pDB = db.m_pDB;
	m_nBusyTimeoutMs = 60000; // 60 seconds
}

CSQLiteDB::~CSQLiteDB()
{
	close();
}


CSQLiteDB& CSQLiteDB::operator=(const CSQLiteDB& db)
{
	m_pDB = db.m_pDB;
	m_nBusyTimeoutMs = 60000; // 60 seconds
	return *this;
}


bool CSQLiteDB::open(const char* szFile)
{
	bool bRet = false;
	//printf("DB Open %s\n", szFile);
	int nRet = sqlite3_open(szFile, &m_pDB);
	if (nRet != SQLITE_OK)
	{
		const char* szError = sqlite3_errmsg(m_pDB);
		throw CSQLiteException(nRet, (char*)szError, DONT_DELETE_MSG);
	}
	else
	{
		//sqlite3_soft_heap_limit64(4096000);
		bRet = true;
	}

	setBusyTimeout(m_nBusyTimeoutMs);
	return bRet;
}


void CSQLiteDB::close()
{
	if (m_pDB)
	{
		int rc = sqlite3_close(m_pDB);
		if ( rc == SQLITE_BUSY)
		{
			// shouldn't happen in a good written application but let's handle it
			sqlite3_stmt * stmt;
			try
			{
				while ((stmt = sqlite3_next_stmt(m_pDB, NULL)) != NULL) 
				{
					sqlite3_finalize(stmt);
				}
			}
			catch(...)
			{

			}
			rc = sqlite3_close(m_pDB);
			if (rc != SQLITE_OK) {
				throw CSQLiteException(rc, (char *)"Closing Error", false);
				// throw this application away as it messes with CSQLiteDB object
				// in another thread while closing it here
				// Your choice of how to deal with fatal error
			}
		} 
		m_pDB = 0;
	}
}


CSQLiteStatement CSQLiteDB::compileStatement(const char* szSQL)
{
	checkDB();

	sqlite3_stmt* pVM = compile(szSQL);
	return CSQLiteStatement(m_pDB, pVM);
}


bool CSQLiteDB::tableExists(const char* szTable)
{
	char szSQL[128];
	sprintf_s(szSQL,128, "select count(*) from sqlite_master where type='table' and name='%s'",szTable);
	int nRet = execScalar(szSQL);
	return (nRet > 0);
}
bool CSQLiteDB::indexExists(const char* szIndex)
{
	char szSQL[128];
	sprintf_s(szSQL, 128, "select count(*) from sqlite_master where type='index' and name='%s'",szIndex);
	int nRet = execScalar(szSQL);
	return (nRet > 0);
}


int CSQLiteDB::execDML(const char* szSQL)
{
	checkDB();
	char* szError=0;
	int nRet = sqlite3_exec(m_pDB, szSQL, 0, 0, &szError);
	if (nRet == SQLITE_OK)
	{
		return sqlite3_changes(m_pDB);
	}
	else
	{
		throw CSQLiteException(nRet, szError);
	}
}


CSQLiteQuery CSQLiteDB::execQuery(const char* szSQL)
{
	checkDB();

	sqlite3_stmt* pVM = compile(szSQL);
	int nRet = sqlite3_step(pVM);
	if (nRet == SQLITE_DONE)
	{
		// no rows
		return CSQLiteQuery(m_pDB, pVM, true);
	}
	else if (nRet == SQLITE_ROW)
	{
		// at least 1 row
		return CSQLiteQuery(m_pDB, pVM, false);
	}
	else
	{
		nRet = sqlite3_finalize(pVM);
		const char* szError= sqlite3_errmsg(m_pDB);
		throw CSQLiteException(nRet, (char*)szError, DONT_DELETE_MSG);
	}
}


int CSQLiteDB::execScalar(const char* szSQL)
{
	CSQLiteQuery q = execQuery(szSQL);
	if (q.eof() || q.numFields() < 1)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			(char *)"Invalid scalar query",
			DONT_DELETE_MSG);
	}

	return atoi(q.fieldValue(0));
}

/*
CSQLiteTable CSQLiteDB::getTable(const char* szSQL)
{
	checkDB();

	char* szError=0;
	char** paszResults=0;
	int nRet;
	int nRows(0);
	int nCols(0);

	nRet = sqlite3_get_table(m_pDB, szSQL, &paszResults, &nRows, &nCols, &szError);
	if (nRet == SQLITE_OK)
	{
		return CSQLiteTable(paszResults, nRows, nCols);
	}
	else
	{
		throw CSQLiteException(nRet, szError);
	}
}
*/
//*****************************************************************************************
sqlite_int64 CSQLiteDB::lastRowId()
{
	return sqlite3_last_insert_rowid(m_pDB);
}

//*****************************************************************************************
void CSQLiteDB::setBusyTimeout(int nMillisecs)
{
	m_nBusyTimeoutMs = nMillisecs;
	sqlite3_busy_timeout(m_pDB, m_nBusyTimeoutMs);
}

//*****************************************************************************************
void CSQLiteDB::checkDB()
{
	if (!m_pDB)
	{
		throw CSQLiteException(CPPSQLITE_ERROR,
			(char *)"Database not open",
			DONT_DELETE_MSG);
	}
}

//*****************************************************************************************
sqlite3_stmt* CSQLiteDB::compile(const char* szSQL)
{
	checkDB();

	char* szError=0;
	const char* szTail=0;
	sqlite3_stmt* pVM;

	int nRet = sqlite3_prepare(m_pDB, szSQL, -1, &pVM, &szTail);
	if (nRet != SQLITE_OK)
	{
		throw CSQLiteException(nRet, szError);
	}

	return pVM;
}

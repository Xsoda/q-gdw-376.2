#include "odbc.h"
#define DISPLAY_MAX 50
#define NULL_SIZE 6
/*******************************************/ 
/* Macro to call ODBC functions and        */ 
/* report an error on failure.             */ 
/* Takes handle, handle type, and stmt     */ 
/*******************************************/ 
 
#define TRYODBC(h, ht, x)   {RETCODE rc = x;if (rc != SQL_SUCCESS){ HandleDiagnosticRecord (h, ht, rc);	} if (rc == SQL_ERROR){	log_error("Error in "#x);}} 

connection *create_connection(char *dsn)
{
	connection *conn;
	if ((conn = (connection *)malloc(sizeof *conn)) == NULL)
	{
		return NULL;
	}
	if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &conn->henv)))
	{
		goto _free;
	}
	if (!SQL_SUCCEEDED(SQLSetEnvAttr(conn->henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)))
	{
		SQLFreeHandle(SQL_HANDLE_ENV, conn->henv);
		goto _free;
	}
	if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, conn->henv, &conn->hdbc)))
	{
		SQLFreeHandle(SQL_HANDLE_ENV, conn->henv);
		goto _free;
	}
	if (!SQL_SUCCEEDED(SQLDriverConnect(conn->hdbc, GetDesktopWindow(), (SQLWCHAR *)dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE)))
	{
		SQLFreeHandle(SQL_HANDLE_DBC, conn->hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV, conn->henv);
		goto _free;
	}
	return conn;
_free:
	free(conn);
	return NULL;
}

statement *create_statement(connection *conn)
{
	statement *stmt;
	if ((stmt = (statement *)malloc(sizeof *stmt)))
	{
		return NULL;
	}
	if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, conn->hdbc, &stmt->hstmt)))
	{
		free(stmt);
		return NULL;
	}
	SQLSetStmtAttr(stmt->hstmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_STATIC, 0);
	return stmt;
}

bool prepare_statement(statement *stmt, char *sql)
{
	if (!SQL_SUCCEEDED(SQLPrepare(stmt->hstmt, (SQLWCHAR *)sql, strlen(sql))))
		return false;
	return true;
}

bool set_value_type(statement *stmt, int index, VALUE type, void *value)
{
	SQLINTEGER dwLen;
	switch(type)
	{
	case VALUE_INTEGER:
		if (!SQL_SUCCEEDED(SQLBindParameter(stmt->hstmt, index, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, sizeof SQL_C_ULONG, (SQLPOINTER)value, 0,  &dwLen)))
			return false;
		break;
	case VALUE_STRING:
		if (!SQL_SUCCEEDED(SQLBindParameter(stmt->hstmt, index, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 1024, (SQLPOINTER)value, 0, &dwLen)))
			return false;
	case VALUE_TIME:
		if (!SQL_SUCCEEDED(SQLBindParameter(stmt->hstmt, index, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 1024, (SQLPOINTER)value, 0,  &dwLen)))
			return false;
		break;
	}
	return true;

}

bool execute(statement *stmt)
{
	if (!SQL_SUCCEEDED(SQLExecute(stmt->hstmt)))
		return false;
	return true;
}

bool reset_stmt(statement *stmt)
{
	if (!SQL_SUCCEEDED(SQLFreeStmt(stmt->hstmt, SQL_RESET_PARAMS)))
		return false;
	return true;
}

void close_connection(connection *conn)
{
	SQLDisconnect(conn->hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, conn->hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, conn->henv);
	free(conn);
}
void close_statement(statement *stmt)
{
	column_info *ci = NULL;
	SQLFreeHandle(SQL_HANDLE_STMT, stmt->hstmt);
	while ((ci = stmt->ci) != NULL)
	{
		if (ci->wszbuffer)
			free(ci->wszbuffer);
		free(ci);
	}
	free(stmt);
}

bool execut_direct(statement *stmt, char *sql)
{
	column_info *p_first, *p_this;
	if (!SQL_SUCCEEDED(SQLExecDirect(stmt->hstmt, (SQLWCHAR *)sql, strlen(sql))))
		return false;
	SQLNumResultCols(stmt->hstmt, &stmt->cols);
	if (stmt->cols > 0) // 结果集中的列数大于0，执行的是DQL语句
		;
	else
	{
		SQLRowCount(stmt->hstmt, &stmt->rows);
		if(stmt->rows == -1) // 执行的是DDL/DCL语句
			;
		else
			; // 执行的是DML语句
	}
}
unsigned int get_update_count(statement *stmt)
{
	return stmt->rows;
}
bool absolute(statement *stmt, unsigned int linum)
{
	SQLFetchScroll(stmt->hstmt, SQL_FETCH_ABSOLUTE, linum);
	return true;
}
bool after_last(statement *stmt)
{
	SQLFetchScroll(stmt->hstmt, SQL_FETCH_LAST, 0);
	return true;
}
bool befor_first(statement *stmt)
{
	SQLFetchScroll(stmt->hstmt, SQL_FETCH_FIRST, 0);
	return true;
}
bool next(statement *stmt)
{
	SQLFetchScroll(stmt->hstmt, SQL_FETCH_NEXT, 0);
	return true;
}
bool previous(statement *stmt)
{
	SQLFetchScroll(stmt->hstmt, SQL_FETCH_PRIOR, 0);
	return true;
}
bool relative(statement *stmt, int offset)
{
	SQLFetchScroll(stmt->hstmt, SQL_FD_FETCH_RELATIVE, offset);
	return true;
}
unsigned int get_column_count(statement *stmt)
{
	return stmt->cols;
}
VALUE get_column_type(statement *stmt);
unsigned int get_column_name(statement *stmt, char *cname, int len);
void allocate_column_info(statement *stmt, column_info **ci)
{
	int icol;
	SQLLEN          cchDisplay, ssType; 
	SQLSMALLINT     cchColumnNameLength; 
	column_info *p_this, *p_last;
	for (icol = 0; icol < stmt->cols; icol++)
	{
		if ((p_this = (column_info *)malloc(sizeof column_info)))
		{
			// exit;
		}
		if (!icol)
			*ci = p_this;
		else
			p_last->next = p_this;
		p_last = p_this;
		TRYODBC(stmt->hstmt, SQL_HANDLE_STMT, SQLColAttribute(stmt->hstmt, icol, SQL_DESC_DISPLAY_SIZE, NULL, 0, NULL, &cchDisplay));
		TRYODBC(stmt->hstmt, SQL_HANDLE_STMT, SQLColAttribute(stmt->hstmt, icol, SQL_DESC_CONCISE_TYPE, NULL, 0, NULL, &ssType));
		p_this->fChar = (ssType == SQL_CHAR || ssType == SQL_VARCHAR || ssType == SQL_LONGVARCHAR);
		p_this->next = NULL;
		cchDisplay = cchDisplay > DISPLAY_MAX ? DISPLAY_MAX : cchDisplay;
		p_this->wszbuffer = (WCHAR *)malloc(cchDisplay + 1);
		TRYODBC(stmt->hstmt, SQL_HANDLE_STMT, SQLBindCol(stmt->hstmt, ico1, SQL_C_TCHAR, (SQLPOINTER)p_this->wszbuffer, (cchDisplay + 1) * sizeof(WCHAR), &p_this->indPtr));
		TRYODBC(stmt->hstmt, SQL_HANDLE_STMT, SQLColAttribute(stmt->hstmt, icol, SQL_DESC_NAME, NULL, 0, &cchColumnNameLength, NULL));
		p_this->size = max((SQLSMALLINT)cchDisplay, cchColumnNameLength);
		p_this->size = p_this->size < NULL_SIZE ? NULL_SIZE : p_this->size;
	}
}

void HandleDiagnosticRecord (SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode) 
{ 
    SQLSMALLINT iRec = 0; 
    SQLINTEGER  iError; 
    WCHAR       wszMessage[1000]; 
    WCHAR       wszState[SQL_SQLSTATE_SIZE+1]; 
 
 
    if (RetCode == SQL_INVALID_HANDLE) 
    { 
        log_error("Invalid handle!"); 
        return; 
    } 
 
    while (SQLGetDiagRec(hType, 
                         hHandle, 
                         ++iRec, 
                         wszState, 
                         &iError, 
                         wszMessage, 
                         (SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)), 
                         (SQLSMALLINT *)NULL) == SQL_SUCCESS) 
    { 
        // Hide data truncated.. 
        if (wcsncmp(wszState, L"01004", 5)) 
        { 
            log_error("[%5.5s] %s (%d)\n", wszState, wszMessage, iError); 
        } 
    } 
 
}
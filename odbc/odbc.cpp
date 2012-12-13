#include "odbc.h"
#define DISPLAY_MAX 500
#define NULL_SIZE 6
/*******************************************/ 
/* Macro to call ODBC functions and        */ 
/* report an error on failure.             */ 
/* Takes handle, handle type, and stmt     */ 
/*******************************************/ 
 
#define TRYODBC(h, ht, x)   {RETCODE rc = x;if (rc != SQL_SUCCESS){ HandleDiagnosticRecord (h, ht, rc);	} if (rc == SQL_ERROR){	;}} 

static void HandleDiagnosticRecord (SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode) 
{ 
    SQLSMALLINT iRec = 0; 
    SQLINTEGER  iError; 
    WCHAR       wszMessage[1000]; 
    WCHAR       wszState[SQL_SQLSTATE_SIZE+1]; 
 
 
    if (RetCode == SQL_INVALID_HANDLE) 
    { 
       // log_error("Invalid handle!"); 
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
        } 
    } 
 
}

void allocate_bindings(statement *stmt, binding **ci)
{
	int icol;
	SQLLEN          cchDisplay, ssType; 
	SQLSMALLINT     cchColumnNameLength; 
	binding *p_this, *p_last;
	for (icol = 0; icol < stmt->cols; icol++)
	{
		if ((p_this = (binding *)malloc(sizeof binding)))
		{
			// exit;
		}
		if (!icol)
			*ci = p_this;
		else
			p_last->next = p_this;
		p_last = p_this;
		TRYODBC(stmt->conn->hstmt, SQL_HANDLE_STMT, SQLColAttribute(stmt->conn->hstmt, icol, SQL_DESC_DISPLAY_SIZE, NULL, 0, NULL, &cchDisplay));
		TRYODBC(stmt->conn->hstmt, SQL_HANDLE_STMT, SQLColAttribute(stmt->conn->hstmt, icol, SQL_DESC_CONCISE_TYPE, NULL, 0, NULL, &ssType));
		p_this->fChar = (ssType == SQL_CHAR || ssType == SQL_VARCHAR || ssType == SQL_LONGVARCHAR);
		p_this->next = NULL;
		cchDisplay = cchDisplay > DISPLAY_MAX ? DISPLAY_MAX : cchDisplay;
		p_this->wszbuffer = (WCHAR *)malloc(cchDisplay + 1);
		TRYODBC(stmt->conn->hstmt, SQL_HANDLE_STMT, SQLBindCol(stmt->conn->hstmt, ico1, SQL_C_TCHAR, (SQLPOINTER)p_this->wszbuffer, (cchDisplay + 1) * sizeof(WCHAR), &p_this->indPtr));
		TRYODBC(stmt->conn->hstmt, SQL_HANDLE_STMT, SQLColAttribute(stmt->conn->hstmt, icol, SQL_DESC_NAME, NULL, 0, &cchColumnNameLength, NULL));
		p_this->size = max((SQLSMALLINT)cchDisplay, cchColumnNameLength);
		p_this->size = p_this->size < NULL_SIZE ? NULL_SIZE : p_this->size;
	}
}
int odbc_open(_IN_ SQLWCHAR *dsn, _OUT_ connection **conn)
{
	connection *tmp;
	if ((tmp = (connection *)malloc(sizeof *conn)) == NULL)
	{
		return NULL;
	}
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &tmp->henv) == SQL_ERROR)
	{
		free(tmp);
		return NULL;
	}
	TRYODBC(tmp->henv, SQL_HANDLE_ENV, SQLSetEnvAttr(tmp->henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0));
	TRYODBC(tmp->henv, SQL_HANDLE_ENV, SQLAllocHandle(SQL_HANDLE_DBC, tmp->henv, &tmp->hdbc));
	TRYODBC(tmp->hdbc, SQL_HANDLE_DBC, SQLDriverConnect(tmp->hdbc, GetDesktopWindow(), dsn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE)); 
	TRYODBC(tmp->hdbc, SQL_HANDLE_DBC, SQLAllocHandle(SQL_HANDLE_STMT, tmp->hdbc, &tmp->hstmt));
	*conn = tmp;
	return 1;
}
int odbc_prepare(_IN_ connection *conn, _IN_ SQLWCHAR *sql, _IN_ unsigned int len, _OUT_ statement **stmt)
{
	//SQLPrepare(conn->hstmt, sql, len);
	SQLSMALLINT ssCols;
	statement *_stmt;
	binding *bind;
	SQLExecDirect(conn->hstmt, sql, wcslen(sql));
	SQLNumResultCols(conn->hstmt, &ssCols);
	if (ssCols)
	{
		_stmt = (statement *)malloc(sizeof statement);
		_stmt->cols = ssCols;
		_stmt->conn = conn;
		allocate_bindings(_stmt, &bind);
		_stmt->column_info = bind;
		*stmt = _stmt;
		return 0;
	}
	else
		return odbc_row_count(conn);	
}
int odbc_exec(_IN_ connection *conn, _IN_ SQLWCHAR *sql)
{	
	SQLExecDirect(conn->hstmt, sql, wcslen(sql));	
	return 1;
}
long odbc_row_count(_IN_ connection *conn)
{
	SQLINTEGER count;
	SQLRowCount(conn->hstmt, &count);
	return count;
}
int odbc_column_count(_IN_ statement *stmt)
{
	return stmt->cols;
}
SQLWCHAR * odbc_column_name(_IN_ statement *stmt, _IN_ unsigned int iCol)
{
	binding *bind;
	bind = stmt->column_info;
	if (stmt->cols >= iCol)
		while(iCol)
		{
			if (bind)
				bind = bind->next;
			else
				break;
			iCol--;
		}
	return bind->wszbuffer;
}
SQLWCHAR * odbc_column_text(_IN_ statement *stmt, _IN_ unsigned int iCol)
{
		binding *bind;
	bind = stmt->column_info;
	if (stmt->cols >= iCol)
		while(iCol)
		{
			if (bind)
				bind = bind->next;
			else
				break;
			iCol--;
		}
	return bind->wszbuffer;
}
double odbc_column_double(_IN_ statement *stmt, _IN_ unsigned int iCol)
{
	binding *bind;
	bind = stmt->column_info;
	if (stmt->cols >= iCol)
		while(iCol)
		{
			if (bind)
				bind = bind->next;
			else
				break;
			iCol--;
		}
	return _wtof(bind->wszbuffer);
}
int odbc_column_int(_IN_ statement *stmt, _IN_ unsigned int iCol)
{
		binding *bind;
	bind = stmt->column_info;
	if (stmt->cols >= iCol)
		while(iCol)
		{
			if (bind)
				bind = bind->next;
			else
				break;
			iCol--;
		}
	return _wtoi(bind->wszbuffer);
}
int odbc_step(_IN_ statement *stmt)
{
	SQLRETURN ret = SQLFetchScroll(stmt->conn->hstmt, SQL_FETCH_NEXT, 0);
	if (ret == SQL_NO_DATA)
		return 0;
	else if (ret == SQL_ERROR)
		return -1;
	else if (ret == SQL_SUCCESS)
		return 1;
}
int odbc_finalize(_IN_OUT_ statement **stmt)
{
	binding *bind, *_bind;
	_bind = bind = (*stmt)->column_info;		
	while(bind)
	{
		_bind = _bind->next;
		free(bind);
		bind = _bind;
	}
	SQLFreeStmt((*stmt)->conn->hstmt, SQL_RESET_PARAMS);
	free(*stmt);
	*stmt = NULL;
	return 1;
}

int odbc_close(_IN_OUT_ connection **conn)
{
	SQLDisconnect((*conn)->hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, (*conn)->hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, (*conn)->henv);
	*conn = NULL;
	free(conn);
	return 1;
}
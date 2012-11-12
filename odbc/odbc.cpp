#include "odbc.h"

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
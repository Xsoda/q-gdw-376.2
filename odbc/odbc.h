#ifndef _ODBC_H_
#define _ODBC_H_

#if defined __cplusplus
extern "C" {
#endif

#include <Windows.h>
#include <odbcinst.h>
#include <odbcss.h>
#include <sqlucode.h>
#include <sql.h>

#define _IN_
#define _OUT_
#define _IN_OUT_

typedef struct connection {
  SQLHENV henv;
  SQLHDBC hdbc;
  SQLHSTMT hstmt;
} connection;

typedef struct binding {
  SQLSMALLINT size; // size to display
  WCHAR *wszbuffer; // display buffer
  SQLLEN indPtr; // size or null
  BOOL fChar; // character col?
  struct binding *next; // linked list
} binding;

typedef struct statement {
  connection *conn;  
  SQLSMALLINT cols;
  SQLINTEGER rows;
  binding *column_info;
} statement;

typedef enum {
  VALUE_INTEGER,
  VALUE_STRING,
  VALUE_TIME,
} VALUE;

int odbc_open(_IN_ SQLWCHAR *dsn, _OUT_ connection **conn);
int odbc_prepare(_IN_ connection *conn, _IN_ SQLWCHAR *sql, _IN_ unsigned int len, _OUT_ statement **stmt);
int odbc_exec(_IN_ connection *conn, _IN_ SQLWCHAR *sql);
long odbc_row_count(_IN_ connection *conn);
int odbc_column_count(_IN_ statement *stmt);
SQLWCHAR * odbc_column_name(_IN_ statement *stmt, _IN_ unsigned int iCol);
SQLWCHAR * odbc_column_text(_IN_ statement *stmt, _IN_ unsigned int iCol);
double odbc_column_double(_IN_ statement *stmt, _IN_ unsigned int iCol);
int odbc_column_int(_IN_ statement *stmt, _IN_ unsigned int iCol);
int odbc_step(_IN_ statement *stmt);
int odbc_finalize(_IN_OUT_ statement **stmt);
int odbc_close(_IN_OUT_ connection **conn);

static void HandleDiagnosticRecord (SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
static void allocate_bindings(statement *stmt, binding **ci);
#if defined __cplusplus
}
#endif

#endif // _ODBC_H_
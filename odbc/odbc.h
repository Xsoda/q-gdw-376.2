#ifndef _ODBC_H_
#define _ODBC_H_

#include <Windows.h>
#include <odbcinst.h>
#include <odbcss.h>
#include <sqlucode.h>
#include <sql.h>
#include "../logger/logger.h"

#if _DEBUG
#pragma comment(lib, "../Debug/logger.lib")
#else
#pragma comment(lib, "../Release/logger.lib")
#endif
typedef struct connection {
  SQLHENV henv;
  SQLHDBC hdbc;
} connection;

typedef struct column_info {
  SQLSMALLINT size; // size to display
  WCHAR *wszbuffer; // display buffer
  SQLLEN indPtr; // size or null
  BOOL fChar; // character col?
  struct column_info *next; // linked list
} column_info;

typedef struct statement {
  connection *conn;
  SQLHSTMT hstmt;
  SQLSMALLINT cols;
  SQLINTEGER rows;
  column_info *ci;
} statement;

typedef enum {
  VALUE_INTEGER,
  VALUE_STRING,
  VALUE_TIME,
} VALUE;

connection *create_connection(char *dsn);
statement *create_statement(connection *conn);
bool prepare_statement(statement *stmt, char *sql);
bool set_value_type(statement *stmt, int index, VALUE type, void *value);
bool execute(statement *stmt);
bool reset_stmt(statement *stmt);
void close_connection(connection *conn);
void close_statement(statement *stmt);
bool execut_direct(statement *stmt, char *sql);
unsigned int get_update_count(statement *stmt);
bool absolute(statement *stmt, unsigned int linum);
bool after_last(statement *stmt); 
bool befor_first(statement *stmt);
bool next(statement *stmt);
bool previous(statement *stmt);
bool relative(statement *stmt, int offset);
unsigned int get_column_count(statement *stmt);
VALUE get_column_type(statement *stmt);
unsigned int get_column_name(statement *stmt, char *cname, int len);
void HandleDiagnosticRecord (SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
#endif // _ODBC_H_
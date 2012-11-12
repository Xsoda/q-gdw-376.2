#ifndef _ODBC_H_
#define _ODBC_H_

#include <Windows.h>
#include <odbcinst.h>
#include <odbcss.h>
#include <sqlucode.h>
#include <sql.h>

typedef struct connection {
  SQLHENV henv;
  SQLHDBC hdbc;
} connection;

typedef struct column_info {
  SQLSMALLINT size; // size to display
  char *szbuffer; // display buffer
  SQLLEN indPtr; // size or null
  BOOL fChar; // character col?
  struct column_info *next; // linked list
} column_info;

typedef struct statement {
  connection *conn;
  SQLHSTMT hstmt;
  int cols;
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
#if 0


bool execut_direct(statement *stmt, char *sql);
unsigned int get_update_count(statement *stmt);
bool absolute(statement *stmt, unsigned int linum);
bool after_last(statement *stmt); 
bool befor_first(statement *stmt);
bool next(statement *stmt);
bool previous(statement *stmt);
bool relative(statement *stmt, int offset);
uinsigned int get_column_count(statement *stmt);
VALUE get_column_type(statement *stmt);
unsigned int get_column_name(statement *stmt, char *cname, int len);
// close resultset;
// close statement;
// close connection;
#endif
#endif // _ODBC_H_
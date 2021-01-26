#pragma once

#include <stdbool.h>
#include <mysql.h>

struct configuration {
	const char *host;
	const char *db_username;
	const char *db_password;
	unsigned int port;
	const char *database;

	char username[128];
	char password[128];
};

extern struct configuration conf;

extern int parse_config(const char* path, struct configuration* conf);
extern char *getInput(unsigned int lung, char *stringa, bool hide);
extern bool yesOrNo(char *domanda, char yes, char no, bool predef, bool insensitive);
extern char* multiChoice(const char *domanda, const char* choices[], int num);
extern char** tokenizer (char **token_vector, char *stringa, int tipoDiDato);
extern void print_error (MYSQL *conn, const char *message);
extern void print_stmt_error (MYSQL_STMT *stmt,const char *message);
extern void finish_with_error(MYSQL *conn, char *message);
extern void finish_with_stmt_error(MYSQL *conn, MYSQL_STMT *stmt,const char *message, bool close_stmt);
extern bool setup_prepared_stmt(MYSQL_STMT **stmt, const char *statement, MYSQL *conn);
extern void dump_result_set(MYSQL *conn, MYSQL_STMT *stmt, char *title);
extern void run_as_system_user(MYSQL *conn);
extern void run_as_driver(MYSQL *conn);
extern void run_as_administrator(MYSQL *conn);


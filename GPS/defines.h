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

extern MYSQL_BIND* setup_mysql_bind(int nparam,void ** data, enum_field_types *type, MYSQL_BIND *param);
extern int parse_config(char* path, struct configuration* conf);
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
extern void run_as_serviceManager(MYSQL* conn);

extern void look_for_Driver_Shift(MYSQL *conn);
extern void look_for_info_AboutADriver(MYSQL *conn);
extern void emissione_biglietto(MYSQL *conn);
extern void eliminare_biglietto(MYSQL *conn);
extern void emissione_abbonamento(MYSQL *conn);
extern void eliminare_abbonamento(MYSQL *conn);
extern void add_newWorkShift(MYSQL *conn);
extern void delete_workShift(MYSQL *conn);



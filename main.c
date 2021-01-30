
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#include "defines.h"


typedef enum {
	ADMINISTRATOR = 1,
	DRIVER,
	SYSTEM_USER,
	SERVICE_MANAGER,
	FAILED_LOGIN
} role_t;


struct configuration conf;

static MYSQL* conn;


static role_t attempt_login(MYSQL* conn, char* username, char* password) {
	MYSQL_STMT* login_procedure;
	void *data[3];
	enum_field_types type[3];
	MYSQL_BIND param[3]; // Used both for input and output
	int role = 0;

	if (!setup_prepared_stmt(&login_procedure, "call Login(?, ?, ?)", conn)) {
		print_stmt_error(login_procedure, "Unable to initialize login statement\n");
		goto err2;
	}

	
	data[0]=(void*)username;
	data[1]=(void*)password;
	data[2]=(void*)&role;
	
	type[0]=MYSQL_TYPE_VAR_STRING;
	type[1]=MYSQL_TYPE_VAR_STRING;
	type[2]=MYSQL_TYPE_LONG;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(3,data,type,param);


	if (mysql_stmt_bind_param(login_procedure, param) != 0) { // Note _param
		print_stmt_error(login_procedure, "Could not bind parameters for login");
		goto err;
	}

	// Run procedure
	if (mysql_stmt_execute(login_procedure) != 0) {
		print_stmt_error(login_procedure, "Could not execute login procedure");
		goto err;
	}

	
	data[0]=(void*)&role;
	
	type[0]=MYSQL_TYPE_LONG;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	

	if (mysql_stmt_bind_result(login_procedure, param)) {
		print_stmt_error(login_procedure, "Could not retrieve output parameter");
		goto err;
	}

	// Retrieve output parameter
	if (mysql_stmt_fetch(login_procedure)) {
		print_stmt_error(login_procedure, "Could not buffer results");
		goto err;
	}
	printf("%d", role);
	mysql_stmt_close(login_procedure);
	return (role_t)role;

err:
	mysql_stmt_close(login_procedure);
err2:
	return FAILED_LOGIN;
}

int main(void) {
	role_t role;

	if (!parse_config("Users/Login.json", &conf)) {
		fprintf(stderr, "Unable to load login configuration\n");
		exit(EXIT_FAILURE);
	}

	conn = mysql_init(NULL);
	if (conn == NULL) {
		fprintf(stderr, "mysql_init() failed (probably out of memory)\n");
		exit(EXIT_FAILURE);
	}


	if (mysql_real_connect(conn, conf.host, conf.db_username, conf.db_password, conf.database, conf.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
		fprintf(stderr, "mysql_real_connect() failed\n");
		mysql_close(conn);
		exit(EXIT_FAILURE);
	}
	printf("Username: ");
	getInput(128, conf.username, false);
	printf("Password: ");
	getInput(128, conf.password, true);
	role = attempt_login(conn, conf.username,conf.password);

	switch (role) {
	case DRIVER:
		run_as_driver(conn);
		break;

	case SYSTEM_USER:
		run_as_system_user(conn);
		break;

	case ADMINISTRATOR:
		run_as_administrator(conn);
		break;
		
	case SERVICE_MANAGER:
		run_as_serviceManager(conn);
		break;

	case FAILED_LOGIN:
		fprintf(stderr, "Invalid credentials\n");
		exit(EXIT_FAILURE);
		break;

	default:
		fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
		abort();
	}

	printf("Bye!\n");

	mysql_close(conn);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <unistd.h>
#include <signal.h>
#include "defines.h"

struct configuration conf;
bool c;
static MYSQL* conn;
MYSQL_STMT* stmt;
MYSQL_BIND param[3];
MYSQL_STMT* stmt;
char veicolo[4]={"1234"};
float latitudine;
float longitudine;
#define timeout 5 



void add_lat_long (int signo){
	
	latitudine = 100 + rand()%100 + 0.1*(rand()%10);
	longitudine = 100 + rand()%100 + 0.1*(rand()%10);
	
	if (mysql_stmt_execute(stmt) != 0) {
		print_stmt_error(stmt, "Could not execute gps procedure");
		c=false;
		mysql_stmt_close(stmt);
	}
		
	printf("\nInsertion statement executed correctly: latitudine %f , longitudine %f",latitudine,longitudine);
	alarm(timeout);
}

int main(void) {	
	c=true;
	if (!parse_config("Gps.json", &conf)) {
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
	
	if (!setup_prepared_stmt(&stmt, "call InformazioniGPS(?, ?, ?)", conn)) {
		print_stmt_error(stmt, "Unable to initialize informazioni gps statement\n");
		exit(-1);
		}
		
		
	void *data[3];
	data[0]=(void*)veicolo;
	data[1]=(void*)&latitudine;
	data[2]=(void*)&longitudine;
	
	enum_field_types type[3];
	type[0]=MYSQL_TYPE_VAR_STRING;
	type[1]=MYSQL_TYPE_FLOAT;
	type[2]=MYSQL_TYPE_FLOAT;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(3,data,type,param);
		
	/*param[0].buffer_type = MYSQL_TYPE_STRING; 
	param[0].buffer = veicolo;
	param[0].buffer_length = strlen(veicolo);

	param[1].buffer_type = MYSQL_TYPE_FLOAT; 
	param[1].buffer = &latitudine;
	param[1].buffer_length = sizeof(latitudine);

	param[2].buffer_type = MYSQL_TYPE_FLOAT; 
	param[2].buffer = &longitudine;
	param[2].buffer_length = sizeof(longitudine);*/

	if (mysql_stmt_bind_param(stmt, param) != 0) { 
		print_stmt_error(stmt, "Could not bind parameters for gps");
		goto out;
	}
	
	signal(SIGALRM, add_lat_long);
	alarm(timeout);
	
	while (c){
		pause();
		}
		
	printf("Bye!\n");
out:
	mysql_stmt_close(stmt);
	mysql_close(conn);
	return 0;
}

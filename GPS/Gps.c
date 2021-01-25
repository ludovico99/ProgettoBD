#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <unistd.h>
#include "defines.h"

struct configuration conf;

static MYSQL* conn;

int main(void) {
	MYSQL_STMT* stmt;
	MYSQL_BIND param[3];
	char veicolo[4]={"1234"};
	float latitudine;
	float longitudine;
	

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
	
	
	while (true){
		
		sleep(5);
		latitudine = 100 + rand()%100 + 0.1*(rand()%10);
		longitudine = 100 + rand()%100 + 0.1*(rand()%10);
		
		
		if (!setup_prepared_stmt(&stmt, "call InformazioniGPS(?, ?, ?)", conn)) {
			print_stmt_error(stmt, "Unable to initialize informazioni gps statement\n");
			exit(-1);
		}
		
		
		memset(param, 0, sizeof(param));
		
		param[0].buffer_type = MYSQL_TYPE_STRING; 
		param[0].buffer = veicolo;
		param[0].buffer_length = strlen(veicolo);

		param[1].buffer_type = MYSQL_TYPE_FLOAT; 
		param[1].buffer = &latitudine;
		param[1].buffer_length = sizeof(latitudine);

		param[2].buffer_type = MYSQL_TYPE_FLOAT; 
		param[2].buffer = &longitudine;
		param[2].buffer_length = sizeof(longitudine);

		if (mysql_stmt_bind_param(stmt, param) != 0) { 
			print_stmt_error(stmt, "Could not bind parameters for gps");
			goto out;
		}

	
		if (mysql_stmt_execute(stmt) != 0) {
			print_stmt_error(stmt, "Could not execute gps procedure");
			goto out;
		}
		
		printf("\nInsertion statement executed correctly: latitudine %f , longitudine %f",latitudine,longitudine);
		sleep(5000);
	
	}
	out:
	mysql_stmt_close(stmt);
	printf("Bye!\n");

	mysql_close(conn);
	return 0;
}

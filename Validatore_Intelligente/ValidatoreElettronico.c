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

int codice;

#define timeout 5 

void transportPass_used(int signo){
	codice = rand()%10000+1;
	
	if (!setup_prepared_stmt(&stmt, "call Abbonamento_Utilizzato(?, ?)", conn)) {
		print_stmt_error(stmt, "Unable to initialize transport pass used statement\n");
		exit(-1);
		}
		
		
	memset(param, 0, sizeof(param));
		
	param[0].buffer_type = MYSQL_TYPE_LONG; 
	param[0].buffer = &codice;
	param[0].buffer_length = sizeof(codice);

	param[1].buffer_type = MYSQL_TYPE_STRING; 
	param[1].buffer = veicolo;
	param[1].buffer_length = strlen(veicolo);


	if (mysql_stmt_bind_param(stmt, param) != 0) { 
		print_stmt_error(stmt, "Could not bind parameters for transport pass");
		c=false;
		mysql_stmt_close(stmt);
		return;
	}
	
	if (mysql_stmt_execute(stmt) != 0) {
		print_stmt_error(stmt, "Could not execute transport pass used procedure");
		c=false;
		mysql_stmt_close(stmt);
		return;
	}
		
	printf("\nTransport pass %d correctly marked",codice);
	alarm(timeout);
}



void ticket_used (int signo){
	
	codice = rand()%10000+1;
	
	if (!setup_prepared_stmt(&stmt, "call Biglietto_Utilizzato(?, ?)", conn)) {
		print_stmt_error(stmt, "Unable to initialize ticket_used statement\n");
		exit(-1);
		}
		
		
	memset(param, 0, sizeof(param));
		
	param[0].buffer_type = MYSQL_TYPE_LONG; 
	param[0].buffer = &codice;
	param[0].buffer_length = sizeof(codice);

	param[1].buffer_type = MYSQL_TYPE_STRING; 
	param[1].buffer = veicolo;
	param[1].buffer_length = strlen(veicolo);


	if (mysql_stmt_bind_param(stmt, param) != 0) { 
		print_stmt_error(stmt, "Could not bind parameters for ticket_used");
		c=false;
		mysql_stmt_close(stmt);
		return;
	}
	
	if (mysql_stmt_execute(stmt) != 0) {
		print_stmt_error(stmt, "Could not execute ticket_used procedure");
		c=false;
		mysql_stmt_close(stmt);
		return;
	}
		
	printf("\nTicket %d correctly marked",codice);
	alarm(timeout);
}

int main(void) {	
	c=true;
	if (!parse_config("ValidatoreElettronico.json", &conf)) {
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
	printf("Starting to execute, i'm connected");
riprova:	
	if ((rand()%5)!=1){ //4/5 di possibilita si avere un biglietto utilizzato (come da precedente stima
		signal(SIGALRM, ticket_used);
	}
	else {
		signal(SIGALRM, transportPass_used);
	}
		alarm(timeout);
	while (c){
		pause();
		goto riprova;
		}
		
	printf("Bye!\n");

	
	mysql_close(conn);
	return 0;
}

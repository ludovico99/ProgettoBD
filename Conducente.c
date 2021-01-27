#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"




static void compute_nextDepature(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	int status;
	char matricola_veicolo[5];
	char header[64];
	
		
	printf("\nInserisci la matricola del veicolo del quale si vuole conoscere la prossima partenza(4 cifre): ");
	getInput(5,matricola_veicolo,false);
		
		
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call ProssimaPartenzaPrevista(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to replace driver statements\n", false);
	}

	// Prepare parameters
	
	void *data[2];
	data[0]=(void*)matricola_veicolo;
	data[1]=NULL;
	
	enum_field_types type[1];
	type[0]=MYSQL_TYPE_STRING;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	
	/*param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = matricola_veicolo;
	param[0].buffer_length = strlen(matricola_veicolo);*/
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for computing next departure\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while executing compute next departure.");
		goto out;
	}

	do {
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}
		
		sprintf(header, "Prossima partenza prevista per il veicolo %s",matricola_veicolo);
		dump_result_set(conn, prepared_stmt, header);
		
	    next:
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		
	} while (status == 0);

    out:
	mysql_stmt_close(prepared_stmt);
}


void run_as_driver(MYSQL* conn)
{
	const char* options[2] = { "1","2" };
	int op;

	printf("Switching to driver role...\n");

	if(!parse_config("Users/Conducente.json", &conf)) {
		fprintf(stderr, "Unable to load administrator configuration\n");
		exit(EXIT_FAILURE);
	}


	if (mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while (true) {
		printf("\033[2J\033[H");
		printf("*** What should I do for you? ***\n\n");
		printf("1) Calcolare la Prossima partenza prevista per quel veicolo\n");
		printf("2) Quit\n");

		op = atoi(multiChoice("Select an option", options, 2));

		switch (op) {
		case 1:
			compute_nextDepature(conn);
			break;

		case 2:
			return;

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}
		
		getchar();
	}
}

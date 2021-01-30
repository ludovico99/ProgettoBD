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
	void *data[1];
	enum_field_types type[1];
	
		
	printf("\nInserisci la matricola del veicolo del quale si vuole conoscere la prossima partenza(4 cifre): ");
	getInput(5,matricola_veicolo,false);
		
		
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call ProssimaPartenzaPrevista(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to replace driver statements\n", false);
	}

	// Prepare parameters
	
	data[0]=(void*)matricola_veicolo;
	
	type[0]=MYSQL_TYPE_STRING;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	

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

static void look_for_waypoints(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	int status;
	char codiceTratta[6];
	char header[64];
	void *data[1];
	enum_field_types type[1];
	
		
	printf("\nInserisci la tratta della quale si vogliono conoscere i waypoints e il loro ordinamento(5 cifre): ");
	getInput(6,codiceTratta,false);
		
		
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call Cerca_Waypoint_per_una_Tratta(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to setup look for waypoints statements\n", false);
	}

	// Prepare parameters
	
	data[0]=(void*)codiceTratta;
	
	type[0]=MYSQL_TYPE_STRING;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for looking for waypoints\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while executing the look for waypoint procedure.");
		goto out;
	}

	do {
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}
		
		sprintf(header, "Waypoints relativi alla tratta %s",codiceTratta);
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
	const char* options[3] = { "1","2","3" };
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
		printf("2) Cercare i waypoint per quella tratta\n");
		printf("3) Quit\n");

		op = atoi(multiChoice("Select an option", options, 3));

		switch (op) {
		case 1:
			compute_nextDepature(conn);
			break;
		case 2:
			look_for_waypoints(conn);
			break;

		case 3:
			return;

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}
		
		getchar();
	}
}

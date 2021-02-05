#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

static void compute_distance(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[5];
	char codiceTratta[6];
	char codiceFermata[6];
	char *token_vectorData[4];
	char *token_vectorOrario[3];
	char buff[46];
	void *data[5];
	enum_field_types type[5];

	float result;	

	MYSQL_TIME dataPartenza[1];
	MYSQL_TIME orarioPartenza[1];
	
	memset(dataPartenza, 0, sizeof(dataPartenza));
	memset(orarioPartenza, 0, sizeof(orarioPartenza));
	dataPartenza->time_type = MYSQL_TIMESTAMP_DATE;
	orarioPartenza->time_type = MYSQL_TIMESTAMP_TIME;
	
	
	printf("\nInserisci il codice di tratta (5 cifre): ");
	getInput(6, codiceTratta, false);
	
	printf("\nInserisci il codice di fermata(5 cifre): ");
	getInput(6,  codiceFermata, false);
	
	printf("\nInserisci la data di partenza (yyyy-mm-dd): ");
riprova1:
	getInput(46, buff, false);
	tokenizer(token_vectorData,buff,0);
	for (int i=0; i<3;i++){
		if(token_vectorData[i] == NULL){
			printf ("Data inserita non corretta. Reinserirla (yyyy-mm-hh): ");
	 		goto riprova1;
		}
	}
	
	dataPartenza->year = atoi (token_vectorData[0]);
	dataPartenza->month = atoi (token_vectorData[1]);
	dataPartenza->day = atoi (token_vectorData[2]);
	
	printf("\nInserisci l'orario di partenza (hh:mm): ");
riprova2:
	getInput(46, buff, false);
	tokenizer(token_vectorOrario,buff,1);
	for (int i=0; i<2;i++){
		if(token_vectorOrario[i] == NULL){
			printf ("Orario inserito non corretto. Reinserirlo (hh:mm): ");
	 		goto riprova2;
		}
	}
	orarioPartenza->hour = atoi(token_vectorOrario[0]);
	orarioPartenza->minute = atoi(token_vectorOrario[1]);
	

		
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call Distanza_Veicolo_Fermata(?,?,?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize compute distance statements\n", false);
	}

	
	data[0]=(void*)codiceTratta;
	data[1]=(void*)codiceFermata;
	data[2]=(void*)dataPartenza;
	data[3]=(void*)orarioPartenza;
	data[4]=(void*)&result;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_STRING;
	type[2]=MYSQL_TYPE_DATE;
	type[3]=MYSQL_TYPE_TIME;
	type[4]=MYSQL_TYPE_FLOAT;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(5,data,type,param);
	
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for computing distance\n", true);
		goto out;
	}

	
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while executing compute distance.");
		goto out;
	}
	
	data[0]=(void*)&result;
	type[0]=MYSQL_TYPE_FLOAT;	
	
	memset(param, 0, sizeof(param));
	
	
	setup_mysql_bind(1,data,type,param);
	
	if(mysql_stmt_bind_result(prepared_stmt, param)) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not retrieve output parameter", true);
	}
	
	// Retrieve output parameter
	if(mysql_stmt_fetch(prepared_stmt)) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not buffer results", true);
	}

	printf("Distance computed is %f...\n", result);

    out:
	mysql_stmt_close(prepared_stmt);
}

void run_as_system_user(MYSQL* conn)
{
	const char* options[2] = { "1","2" };
	int op;

	printf("Switching to system user...\n");

	if(!parse_config("Users/UtenteSistema.json", &conf)) {
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
		printf("1) Calcolare la distanza del veicolo ad una fermata\n");
		printf("2) Quit\n");

		op = atoi(multiChoice("Select an option", options, 2));

		switch (op) {
		case 1:
			compute_distance(conn);
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

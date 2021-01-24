#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"


static void add_driver(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[9];

	char buff[10];
	char cf[16];
	char username[45];
	char nome[45];
	char cognome[45];
	MYSQL_TIME data_nascita[1];
	MYSQL_TIME scadenza_patente[1];
	char luogo_nascita[45];
	char numero_patente[10];
	char veicolo_Assegnato[4];
	memset(data_nascita, 0, sizeof(data_nascita));
	memset(scadenza_patente, 0, sizeof(scadenza_patente));
	data_nascita->time_type = MYSQL_TIMESTAMP_DATE;
	scadenza_patente->time_type = MYSQL_TIMESTAMP_DATE;
	
	// Get the required information
	printf("\nInserisci codice fiscale (16 caratteri): ");
	getInput(16, cf, false);
	printf("\nInserisci username (valido): ");
	getInput(45, username, false);
	printf("\nInserisci nome: ");
	getInput(45, nome, false);
	printf("\nInserisci cognome: ");
	getInput(45, cognome, false);
	printf("\nInserisci anno di nascita: ");
	getInput(10, buff, false);
	data_nascita->year = atoi(buff);
	printf("%d",data_nascita->year);
	printf("\nInserisci mese di nascita: ");
	getInput(10, buff, false);
	data_nascita->month =atoi(buff);
	printf("%d",data_nascita->month);
	printf("\nInserisci giorno di nascita: ");
	getInput(10, buff, false);
	data_nascita->day = atoi(buff);
	printf("%d",data_nascita->day);
	printf("\nInserisci luogo di nascita: ");
	getInput(45, luogo_nascita, false);
	printf("\nInserisci numero patente: ");
	getInput(10, numero_patente, false);
	printf("\nInserisci anno di scadenza della patente: ");
	getInput(10, buff, false);
	scadenza_patente->year = atoi(buff);
	printf("%d",scadenza_patente->year);
	printf("\nInserisci mese di scadenza della patente: ");
	getInput(10, buff, false);
	scadenza_patente->month = atoi(buff);
	printf("%d",scadenza_patente->month);
	printf("\nInserisci giorno di scadenza della patente: ");
	getInput(10, buff, false);
	scadenza_patente->day = atoi(buff);
	printf("%d",scadenza_patente->day);
	printf("\nInserisci il veicolo assegnato : ");
	getInput(4, veicolo_Assegnato, false);

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_Conducente(?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize student insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = cf;
	param[0].buffer_length = sizeof(cf);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = username;
	param[1].buffer_length = sizeof(username);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = nome;
	param[2].buffer_length = sizeof(nome);

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = cognome;
	param[3].buffer_length = sizeof(cognome);

	param[4].buffer_type = MYSQL_TYPE_DATE;
	param[4].buffer = data_nascita;
	param[4].buffer_length = sizeof(data_nascita);

	param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[5].buffer = luogo_nascita;
	param[5].buffer_length = sizeof(luogo_nascita);

	param[6].buffer_type = MYSQL_TYPE_STRING;
	param[6].buffer = numero_patente;
	param[6].buffer_length = sizeof(numero_patente);

	param[7].buffer_type = MYSQL_TYPE_DATE;
	param[7].buffer = scadenza_patente;
	param[7].buffer_length = sizeof(scadenza_patente);

	param[8].buffer_type = MYSQL_TYPE_STRING;
	param[8].buffer = veicolo_Assegnato;
	param[8].buffer_length = sizeof(veicolo_Assegnato);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for Driver insertion\n", true);
	}


	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the driver.");
		goto out;
	}
	printf("driver correctly inserted\n");

out:
	mysql_stmt_close(prepared_stmt);
}







static void create_user(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	const char* options[4] = { "1","2", "3" };
	int r;

	// Input for the registration routine
	char username[46];
	char password[46];
	char ruolo[46];

	// Get the required information
	printf("\nUsername: ");
	getInput(46, username, false);
	printf("password: ");
	getInput(46, password, true);
	printf("Assign a possible role:\n");
	printf("\t1) Amministratore Servizio\n");
	printf("\t2) Conducente\n");
	printf("\t3) Utente Sistema\n");
	r = atoi(multiChoice("Select role", options, 3));

	// Convert role into enum value
	switch (r) {
	case 1:
		strcpy(ruolo, "AmministatoreServizio");
		break;
	case 2:
		strcpy(ruolo, "Conducente");
		break;
	case 3:
		strcpy(ruolo, "UtenteSistema");
		break;
	default:
		fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
		abort();
	}

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Crea_Utente(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = username;
	param[0].buffer_length = strlen(username);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = password;
	param[1].buffer_length = strlen(password);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = ruolo;
	param[2].buffer_length = strlen(ruolo);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the user.");
	}
	else {
		printf("User correctly added...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_waypoint(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];
	char buff[128];

	// Input for the registration routine
	float latitudine;
	float longitudine;
	

	// Get the required information
	printf("\nInserisci latitudine: ");
	getInput(128, buff, false);
	latitudine = (float)atof(buff);
	fflush(stdin);
	printf("\nInserisci longitudine: ");
	getInput(128, buff, false);
	longitudine = (float)atof(buff);

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_Waypoint(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize waypoint insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_FLOAT;
	param[0].buffer = &latitudine;
	param[0].buffer_length = sizeof(latitudine);

	param[1].buffer_type = MYSQL_TYPE_FLOAT;
	param[1].buffer = &longitudine;
	param[1].buffer_length = sizeof(longitudine);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for waypoint insertion\n", true);
	}

	
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the waypoint.");
		goto out;
	}
	printf("Wayoint inserito correttamente\n");

out:
	mysql_stmt_close(prepared_stmt);
}





void run_as_administrator(MYSQL* conn)
{
	const char* options[18] = {"1","2", "3", "4", "5","6","7","8","9","10","11","12","13","14","15","16","17","18"};
	int op;

	printf("Switching to administrative role...\n");

	conf.db_username = "AmministratoreServizio";
	conf.db_password = "amministratoreservizio";
	conf.database = "progetto";


	if (mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while (true) {
		printf("\033[2J\033[H");
		printf("*** What should I do for you? ***\n\n");
		printf("1) Associare un veicolo ad una tratta reale\n");
		printf("2) Aggiungere nuovo turno ad un conducente \n");
		printf("3) Elminare turno ad un conducente\n");
		printf("4) Sostituire il turno di un conducente\n");
		printf("5) Emettere biglietto\n");
		printf("6) Emettere abbonamento\n");
		printf("7) Conducente guarito\n");
		printf("8) Cerca conducenti validi per la sostituzione\n");
		printf("9) Aggiungere nuovo conducente\n");
		printf("10) Aggiungere nuova fermata\n");
		printf("11) Aggiungere Manutenzione\n");
		printf("12) Aggiungere tratta reale\n");
		printf("13) Aggiungere tratta stradale\n");
		printf("14) Aggiungere veicolo pubblico\n");
		printf("15) Aggiungere waypoint\n");
		printf("16) Aggiungere Waypoint in una tratta\n");
		printf("17) Crea Utente\n");
		printf("18) Quit\n");

		op = atoi(multiChoice("Select an option", options, 18));
		printf("%d",op);

		switch (op) {
		case 1:
			//add_exam(conn);
			break;
		case 2:
			//add_student(conn);
			break;
		case 3:
			//create_user(conn);
			break;
		case 4:
			//subscribe_to_degree(conn);
			break;
		case 5:
			break;
		case 7:
			//add_exam(conn);
			break;
		case 8:
			//add_student(conn);
			break;
		case 9:
			add_driver(conn);
			break;
		case 10:
			//subscribe_to_degree(conn);
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			//add_exam(conn);
			break;
		case 14:
			//add_student(conn);
			break;
		case 15:
			add_waypoint(conn);
			break;
		case 16:
			//subscribe_to_degree(conn);
			break;
		case 17:
			create_user(conn);
			break;
		case 18:
			return;
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}
		
		getchar();
	}
}

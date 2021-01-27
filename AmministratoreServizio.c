#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "defines.h"

static void replace_workShift(MYSQL* conn){
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[4];
	char buff1[46];
	char buff2[46];
	char *token_vectorInizioTurno[6];
	char *token_vectorFineTurno[6];
	char conducente_cf[17];
	char conducente_sostituto[17];
	MYSQL_TIME inizioTurno[1];
	MYSQL_TIME fineTurno[1];
		
		
	memset(inizioTurno, 0, sizeof(inizioTurno));
	memset(fineTurno, 0, sizeof(fineTurno));
	
	
	printf("\nInserisci il codice fiscale del conducente da sostituire: ");
	getInput(17,conducente_cf,false);
	printf("\nInserisci il codice fiscale del conducente sostituto (nuovo): ");
	getInput(17, conducente_sostituto,false);		
	printf("\nInserisci il datetime di inizio del turno(yyyy-mm-hh hh-mm): ");
riprova1:
	getInput(46, buff1, false);
	tokenizer(token_vectorInizioTurno,buff1,2);
	for (int i=0; i<5;i++){
		if(token_vectorInizioTurno[i] == NULL){
			printf ("Datetime inserito non corretto. Reinserirlo (yyyy-mm-hh hh-mm): ");
	 		goto riprova1;
		}
	}
	
	printf("\nInserisci il datetime corrispondente alla fine del turno (yyyy-mm-hh hh-mm): ");
riprova2:
	getInput(46, buff2, false);
	tokenizer(token_vectorFineTurno,buff2,2);
	for (int i=0; i<5;i++){
		if(token_vectorFineTurno[i] == NULL){
			printf ("Datetime inserito non corretto. Reinserirlo (yyyy-mm-hh hh-mm): ");
	 		goto riprova2;
		}
	}
	
	inizioTurno->year = atoi(token_vectorInizioTurno[0]);
	inizioTurno->month = atoi(token_vectorInizioTurno[1]);
	inizioTurno->day = atoi(token_vectorInizioTurno[2]);
	inizioTurno->hour = atoi(token_vectorInizioTurno[3]);
	inizioTurno->minute = atoi(token_vectorInizioTurno[4]);
	
	inizioTurno->second=0;
	inizioTurno->second_part=0;
	inizioTurno->time_type=MYSQL_TIMESTAMP_DATETIME;
	
	fineTurno->year = atoi(token_vectorFineTurno[0]);
	fineTurno->month = atoi(token_vectorFineTurno[1]);
	fineTurno->day = atoi(token_vectorFineTurno[2]);
	fineTurno->hour = atoi(token_vectorFineTurno[3]);
	fineTurno->minute = atoi(token_vectorFineTurno[4]);
	
	fineTurno->second=0;
	fineTurno->second_part=0;
	fineTurno->time_type=MYSQL_TIMESTAMP_DATETIME;	
	
		
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call SostituzioneTurno(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to replace driver statements\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = conducente_sostituto;
	param[0].buffer_length = strlen(conducente_sostituto);
	
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = conducente_cf;
	param[1].buffer_length = strlen(conducente_cf);
	
	param[2].buffer_type = MYSQL_TYPE_DATETIME;
	param[2].buffer = inizioTurno;
	param[2].buffer_length = sizeof(inizioTurno);
	
	param[3].buffer_type = MYSQL_TYPE_DATETIME;
	param[3].buffer = fineTurno;
	param[3].buffer_length = sizeof(fineTurno);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for replacing driver\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while retrieving the driver replacement.");
		goto out;
	}

	printf("driver replacement procedure correctly done\n");

    out:
	mysql_stmt_close(prepared_stmt);
}
		


static void find_driver_shiftReplacement(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];
	int status;
	char buff1[46];
	char buff2[46];
	char *token_vectorInizioTurno[6];
	char *token_vectorFineTurno[6];
	
	char conducente_cf[17];
	MYSQL_TIME inizioTurno[1];
	MYSQL_TIME fineTurno[1];
		
	memset(inizioTurno, 0, sizeof(inizioTurno));
	memset(fineTurno, 0, sizeof(fineTurno));
			
	printf("\nInserisci il codice fiscale del conducente che si intende sostituire: ");
	getInput(17,conducente_cf,false);
	printf("\nInserisci il datetime corrispondente all' inizio del turno (yyyy-mm-hh hh-mm): ");
riprova1:
	getInput(46, buff1, false);
	tokenizer(token_vectorInizioTurno,buff1,2);
	for (int i=0; i<5;i++){
		if(token_vectorInizioTurno[i] == NULL){
			printf ("Datetime inserito non corretto. Reinserirlo (yyyy-mm-hh hh-mm): ");
	 		goto riprova1;
		}
	}
	
	printf("\nInserisci il datetime corrispondente alla fine del turno (yyyy-mm-hh hh-mm): ");
riprova2:
	getInput(46, buff2, false);
	tokenizer(token_vectorFineTurno,buff2,2);
	for (int i=0; i<5;i++){
		if(token_vectorFineTurno[i] == NULL){
			printf ("Datetime inserito non corretto. Reinserirlo (yyyy-mm-hh hh-mm): ");
	 		goto riprova2;
		}
	}
	
	inizioTurno->year = atoi(token_vectorInizioTurno[0]);
	inizioTurno->month = atoi(token_vectorInizioTurno[1]);
	inizioTurno->day = atoi(token_vectorInizioTurno[2]);
	inizioTurno->hour = atoi(token_vectorInizioTurno[3]);
	inizioTurno->minute = atoi(token_vectorInizioTurno[4]);
	
	inizioTurno->second=0;
	inizioTurno->second_part=0;
	inizioTurno->time_type=MYSQL_TIMESTAMP_DATETIME;
	
	fineTurno->year = atoi(token_vectorFineTurno[0]);
	fineTurno->month = atoi(token_vectorFineTurno[1]);
	fineTurno->day = atoi(token_vectorFineTurno[2]);
	fineTurno->hour = atoi(token_vectorFineTurno[3]);
	fineTurno->minute = atoi(token_vectorFineTurno[4]);
	
	fineTurno->second=0;
	fineTurno->second_part=0;
	fineTurno->time_type=MYSQL_TIMESTAMP_DATETIME;	
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call Cerca_ConducentiValidiperLaSostituzione(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to find drivers for replacement statements\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = conducente_cf;
	param[0].buffer_length = strlen(conducente_cf);
	
	param[1].buffer_type = MYSQL_TYPE_DATETIME;
	param[1].buffer = inizioTurno;
	param[1].buffer_length = sizeof(inizioTurno);
	
	param[2].buffer_type = MYSQL_TYPE_DATETIME;
	param[2].buffer = fineTurno;
	param[2].buffer_length = sizeof(fineTurno);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for find drivers for replacement\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while retrieving the find drivers for replacement.");
		goto out;
	}

	// We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}
		
		
		dump_result_set(conn, prepared_stmt, "Conducenti validi per la sostituzione");
		
		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	    next:
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		
	} while (status == 0);
	
    out:
	mysql_stmt_close(prepared_stmt);
}
	
static void driver_healed (MYSQL *conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];

	char codiceFiscale[17];
	
	printf("\nInserisci il codice fiscale del conducente guarito: ");
	getInput(17, codiceFiscale, false);
	
	if (!setup_prepared_stmt(&prepared_stmt, "call ConducenteGuarito(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize driver healed statements\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = codiceFiscale;
	param[0].buffer_length = strlen(codiceFiscale);
	
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for driver healed procedure\n", true);
	}


	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while executing the driver healed procedure.");
		goto out;
	}
	printf("driver healed procedure correctly done\n");

out:
	mysql_stmt_close(prepared_stmt);
}



static void link_vehicleWithRealRoute(MYSQL* conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];
	char buff[46];
	
	char *token_vectorData[4];
	char *token_vectorOrario[3];
	
	char codiceTratta[6];
	MYSQL_TIME dataPartenza[1];
	MYSQL_TIME orarioPartenza[1];
	char veicoloAssociato[5];
	
	memset(dataPartenza, 0, sizeof(dataPartenza));
	memset(orarioPartenza, 0, sizeof(orarioPartenza));
	
	dataPartenza->time_type = MYSQL_TIMESTAMP_DATE;
	orarioPartenza->time_type = MYSQL_TIMESTAMP_TIME;
	
	
	printf("\nInserisci il codice di tratta (5 cifre): ");
	getInput(6, codiceTratta, false);
	printf("\nInserisci la data di partenza (yyyy-mm-hh): ");
riprova1:
	getInput(46, buff, false);
	tokenizer(token_vectorData,buff,0);
	for (int i=0; i<3;i++){
		if(token_vectorData[i] == NULL){
			printf ("Data inserita non corretta. Reinserirla (yyyy-mm-hh): ");
	 		goto riprova1;
		}
	}
	
	printf("\nInserisci l'orario di partenza (hh-mm): ");
riprova2:
	getInput(46, buff, false);
	tokenizer(token_vectorOrario,buff,1);
	for (int i=0; i<2;i++){
		if(token_vectorOrario[i] == NULL){
			printf ("Orario inserito non corretto. Reinserirlo (hh-mm): ");
	 		goto riprova2;
		}
	}
	
	dataPartenza->year = atoi(token_vectorData[0]);
	dataPartenza->month = atoi(token_vectorData[1]);
	dataPartenza->day = atoi(token_vectorData[2]);


	orarioPartenza->hour = atoi(token_vectorOrario[0]);
	orarioPartenza->minute = atoi(token_vectorOrario[1]);
	orarioPartenza->day=0;
	orarioPartenza->second=0;
	orarioPartenza->second_part=0;

	printf("\nInserisci il veicolo che si vuole assegnare a questa tratta (4 cifre): ");
	getInput(5, veicoloAssociato, false);
	
	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call AssociazioneVeicoli_TrattaReale(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize real route insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = codiceTratta;
	param[0].buffer_length = strlen(codiceTratta);
	
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = veicoloAssociato;
	param[1].buffer_length = strlen(veicoloAssociato);
	
	param[2].buffer_type = MYSQL_TYPE_TIME;
	param[2].buffer = orarioPartenza;
	param[2].buffer_length = sizeof(orarioPartenza);
	
	param[3].buffer_type = MYSQL_TYPE_DATE;
	param[3].buffer = dataPartenza;
	param[3].buffer_length = sizeof(dataPartenza);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for linking vehicle with real route insertion\n", true);
	}


	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the link vehicle with real route.");
		goto out;
	}
	printf("link vehicle with real route correctly inserted\n");

out:
	mysql_stmt_close(prepared_stmt);
}

	

static void add_newWorkShift(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];
	char buff1[46];
	char buff2[46];
	char *token_vectorInizioTurno[6];
	char *token_vectorFineTurno[6];
		
	char conducente_cf[17];
	MYSQL_TIME inizioTurno[1];
	MYSQL_TIME fineTurno[1];
	
	memset(inizioTurno, 0, sizeof(inizioTurno));
	memset(fineTurno, 0, sizeof(fineTurno));
	
	printf("\nInserisci il codice fiscale del conducente: ");
	getInput(17,conducente_cf,false);
	printf("\nInserisci il datetime corrispondente all' inizio del turno (yyyy-mm-hh hh-mm): ");
riprova1:
	getInput(46, buff1, false);
	tokenizer(token_vectorInizioTurno,buff1,2);
	for (int i=0; i<5;i++){
		if(token_vectorInizioTurno[i] == NULL){
			printf ("Datetime inserito non corretto. Reinserirlo (yyyy-mm-hh hh-mm): ");
	 		goto riprova1;
		}
	}
	
	printf("\nInserisci il datetime corrispondente alla fine del turno (yyyy-mm-hh hh-mm): ");
riprova2:
	getInput(46, buff2, false);
	tokenizer(token_vectorFineTurno,buff2,2);
	for (int i=0; i<5;i++){
		if(token_vectorFineTurno[i] == NULL){
			printf ("Datetime inserito non corretto. Reinserirlo (yyyy-mm-hh hh-mm): ");
	 		goto riprova2;
		}
	}
	
	inizioTurno->year = atoi(token_vectorInizioTurno[0]);
	inizioTurno->month = atoi(token_vectorInizioTurno[1]);
	inizioTurno->day = atoi(token_vectorInizioTurno[2]);
	inizioTurno->hour = atoi(token_vectorInizioTurno[3]);
	inizioTurno->minute = atoi(token_vectorInizioTurno[4]);
	
	inizioTurno->second=0;
	inizioTurno->second_part=0;
	inizioTurno->time_type=MYSQL_TIMESTAMP_DATETIME;
	
	fineTurno->year = atoi(token_vectorFineTurno[0]);
	fineTurno->month = atoi(token_vectorFineTurno[1]);
	fineTurno->day = atoi(token_vectorFineTurno[2]);
	fineTurno->hour = atoi(token_vectorFineTurno[3]);
	fineTurno->minute = atoi(token_vectorFineTurno[4]);
	
	fineTurno->second=0;
	fineTurno->second_part=0;
	fineTurno->time_type=MYSQL_TIMESTAMP_DATETIME;
	
	if(!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_NuovoTurno_Conducente(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize work shift statement\n", false);
	}
	
	
	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = conducente_cf;
	param[0].buffer_length = strlen(conducente_cf);
	
	param[1].buffer_type = MYSQL_TYPE_DATETIME;
	param[1].buffer = inizioTurno;
	param[1].buffer_length = sizeof(inizioTurno);
	
	param[2].buffer_type = MYSQL_TYPE_DATETIME;
	param[2].buffer = fineTurno;
	param[2].buffer_length = sizeof(fineTurno);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for work shift\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the work shift.");
		goto out;
	}

	printf("Work shift correctly inserted\n");
    out:
	mysql_stmt_close(prepared_stmt);
}

static void delete_workShift(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];
	char buff1[46];
	char buff2[46];
	char *token_vectorInizioTurno[6];
	char *token_vectorFineTurno[6];
		
	char conducente_cf[17];
	MYSQL_TIME inizioTurno[1];
	MYSQL_TIME fineTurno[1];
	
	memset(inizioTurno, 0, sizeof(inizioTurno));
	memset(fineTurno, 0, sizeof(fineTurno));
	
	inizioTurno->second=0;
	inizioTurno->second_part=0;
	inizioTurno->time_type=MYSQL_TIMESTAMP_DATETIME;
	fineTurno->second=0;
	fineTurno->second_part=0;
	fineTurno->time_type=MYSQL_TIMESTAMP_DATETIME;
	
	
	printf("\nInserisci il codice fiscale del conducente: ");
	getInput(17,conducente_cf,false);
	printf("\nInserisci il datetime corrispondente all' inizio del turno (yyyy-mm-hh hh-mm): ");
riprova1:
	getInput(46, buff1, false);
	tokenizer(token_vectorInizioTurno,buff1,2);
	for (int i=0; i<5;i++){
		if(token_vectorInizioTurno[i] == NULL){
			printf ("Datetime inserito non corretto. Reinserirlo (yyyy-mm-hh hh-mm): ");
	 		goto riprova1;
		}
	}
	
	printf("\nInserisci il datetime corrispondente alla fine del turno (yyyy-mm-hh hh-mm): ");
	riprova2:
	getInput(46, buff2, false);
	tokenizer(token_vectorFineTurno,buff2,2);
	for (int i=0; i<5;i++){
		if(token_vectorFineTurno[i] == NULL){
			printf ("Datetime inserito non corretto. Reinserirlo (yyyy-mm-hh hh-mm): ");
	 		goto riprova2;
		}
	}
	
	inizioTurno->year = atoi(token_vectorInizioTurno[0]);
	inizioTurno->month = atoi(token_vectorInizioTurno[1]);
	inizioTurno->day = atoi(token_vectorInizioTurno[2]);
	inizioTurno->hour = atoi(token_vectorInizioTurno[3]);
	inizioTurno->minute = atoi(token_vectorInizioTurno[4]);
	
	
	fineTurno->year = atoi(token_vectorFineTurno[0]);
	fineTurno->month = atoi(token_vectorFineTurno[1]);
	fineTurno->day = atoi(token_vectorFineTurno[2]);
	fineTurno->hour = atoi(token_vectorFineTurno[3]);
	fineTurno->minute = atoi(token_vectorFineTurno[4]);
	
	if(!setup_prepared_stmt(&prepared_stmt, "call EliminareOrarioConducente(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize work shift statement\n", false);
	}
	
	
	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = conducente_cf;
	param[0].buffer_length = strlen(conducente_cf);
	
	param[1].buffer_type = MYSQL_TYPE_DATETIME;
	param[1].buffer = inizioTurno;
	param[1].buffer_length = sizeof(inizioTurno);
	
	param[2].buffer_type = MYSQL_TYPE_DATETIME;
	param[2].buffer = fineTurno;
	param[2].buffer_length = sizeof(fineTurno);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for work shift elimination\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while retrieving the work shift elimination.");
		goto out;
	}

	printf("Work shift elimination correctly done\n");
    out:
	mysql_stmt_close(prepared_stmt);
}

static void emissione_abbonamento(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	int codiceAbbonamento;
	
	if(!setup_prepared_stmt(&prepared_stmt, "call EmissioneAbbonamento(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize transport pass statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &codiceAbbonamento;
	param[0].buffer_length = sizeof(codiceAbbonamento);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for transport pass\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while retrieving the transport pass.");
		goto out;
	}

	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[0].buffer = &codiceAbbonamento;
	param[0].buffer_length = sizeof(codiceAbbonamento);
	
	if(mysql_stmt_bind_result(prepared_stmt, param)) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not retrieve output parameter", true);
	}
	
	// Retrieve output parameter
	if(mysql_stmt_fetch(prepared_stmt)) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not buffer results", true);
	}

	printf("transport pass correctly added with ID %d...\n", codiceAbbonamento);

    out:
	mysql_stmt_close(prepared_stmt);
}



static void emissione_biglietto(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	int codiceBiglietto;
	
	if(!setup_prepared_stmt(&prepared_stmt, "call EmissioneBiglietto(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize ticket issue statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &codiceBiglietto;
	param[0].buffer_length = sizeof(codiceBiglietto);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for ticket issue\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while retrieving the ticket issue.");
		goto out;
	}

	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[0].buffer = &codiceBiglietto;
	param[0].buffer_length = sizeof(codiceBiglietto);
	
	if(mysql_stmt_bind_result(prepared_stmt, param)) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not retrieve output parameter", true);
	}
	
	// Retrieve output parameter
	if(mysql_stmt_fetch(prepared_stmt)) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not buffer results", true);
	}

	printf("ticket correctly added with ID %d...\n", codiceBiglietto);

    out:
	mysql_stmt_close(prepared_stmt);
}


static void add_waypointToARoute(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];
	char buff[10];

	// Input for the registration routine
	char tratta[6];
	float latitudine;
	float longitudine;
	int numeroOrdine;

	// Get the required information
	printf("\nInserisci il codice di tratta(5 cifre): ");
	getInput(6, tratta, false);
	printf("\nInserisci latitudine: ");
	getInput(10, buff, false);
	latitudine = (float)atof(buff);
	printf("\nInserisci longitudine: ");
	getInput(10, buff, false);
	longitudine = (float)atof(buff);
	printf("\nInserisci il numero che rappresenta l'ordinamento del waypoint all'interno della tratta : ");
	getInput(10, buff, false);
	numeroOrdine=atoi(buff);

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_Waypoint_per_una_tratta(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize waypoint insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = tratta;
	param[0].buffer_length = strlen(tratta);

	param[1].buffer_type = MYSQL_TYPE_FLOAT;
	param[1].buffer = &latitudine;
	param[1].buffer_length = sizeof(latitudine);

	param[2].buffer_type = MYSQL_TYPE_FLOAT;
	param[2].buffer = &longitudine;
	param[2].buffer_length = sizeof(longitudine);
	
	param[3].buffer_type = MYSQL_TYPE_LONG;
	param[3].buffer = &numeroOrdine;
	param[3].buffer_length = sizeof(numeroOrdine);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for waypoint to a route insertion\n", true);
	}

	
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the waypoint to a route.");
		goto out;
	}
	printf("Waypoint to a route correctly inserted\n");

out:
	mysql_stmt_close(prepared_stmt);
}




static void add_realRoute(MYSQL* conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];
	char buff[46];
	char *token_vectorData[4];
	char *token_vectorOrario[3];
	char codiceTratta[6];
	MYSQL_TIME dataPartenza[1];
	MYSQL_TIME orarioPartenza[1];
	char veicoloAssociato[5];
	
	memset(dataPartenza, 0, sizeof(dataPartenza));
	memset(orarioPartenza, 0, sizeof(orarioPartenza));
	
	dataPartenza->time_type = MYSQL_TIMESTAMP_DATE;
	orarioPartenza->time_type = MYSQL_TIMESTAMP_TIME;

	
	printf("\nInserisci il codice di tratta (5 cifre): ");
	getInput(6, codiceTratta, false);
	printf("\nInserisci la data di partenza (yyyy-mm-hh): ");
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
	orarioPartenza->day=0;
	orarioPartenza->second=0;
	orarioPartenza->second_part=0;
	
	printf("\nInserisci il veicolo assegnato a questa tratta (4 cifre): ");
	getInput(5, veicoloAssociato, false);
	

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_TrattaReale(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize real route insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = codiceTratta;
	param[0].buffer_length = strlen(codiceTratta);
	
	param[1].buffer_type = MYSQL_TYPE_DATE;
	param[1].buffer = dataPartenza;
	param[1].buffer_length = sizeof(dataPartenza);
	
	param[2].buffer_type = MYSQL_TYPE_TIME;
	param[2].buffer = orarioPartenza;
	param[2].buffer_length = sizeof(orarioPartenza);
	
	param[3].buffer_type = MYSQL_TYPE_STRING;
	param[3].buffer = veicoloAssociato;
	param[3].buffer_length = strlen(veicoloAssociato);

	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for real route insertion\n", true);
	}


	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the real route.");
		goto out;
	}
	printf("real route correctly inserted\n");

out:
	mysql_stmt_close(prepared_stmt);
}



static void add_vehicle(MYSQL* conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];
	char buff[46];
	
	// Get the required information
	char veicolo[5];
	MYSQL_TIME dataAcquisto[1];
	
	memset(dataAcquisto, 0, sizeof(dataAcquisto));
	
	
	dataAcquisto->time_type = MYSQL_TIMESTAMP_DATE;
	char *token_vector[4];
	
	
	printf("\nInserisci la matricola del veicolo (4 cifre): ");
	getInput(5, veicolo, false);
	printf("\nInserisci la data di acquisto del veicolo(yyyy-mm-hh): ");
riprova:
	getInput(46, buff, false);
	tokenizer(token_vector,buff,0);
	for (int i=0; i<3;i++){
		if(token_vector[i] == NULL){
			printf ("Data inserita non corretta. Reinserirla (yyyy-mm-hh): ");
	 		goto riprova;
		}
	}	
	dataAcquisto->year = atoi (token_vector[0]);
	dataAcquisto->month = atoi (token_vector[1]);
	dataAcquisto->day = atoi (token_vector[2]);
	

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_VeicoloPubblico(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize vehicle insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = veicolo;
	param[0].buffer_length = strlen(veicolo);

	param[1].buffer_type = MYSQL_TYPE_DATE;
	param[1].buffer = dataAcquisto;
	param[1].buffer_length = sizeof(dataAcquisto);



	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for vehicle insertion\n", true);
	}

	
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the vehicle.");
		goto out;
	}
	printf("vehicle correctly inserted\n");

out:
	mysql_stmt_close(prepared_stmt);
}



static void add_route(MYSQL* conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	
	char codiceTratta[6];
	char primaFermata[6];
	char ultimaFermata[6];
	
	
	printf("\nInserisci il codice di tratta(5 cifre): ");
	getInput(6, codiceTratta, false);
	printf("\nInserisci la prima fermata (5 cifre): ");
	getInput(6, primaFermata, false);
	printf("\nInserisci l'ultima fermata (5 cifre): ");
	getInput(6, ultimaFermata, false);

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_TrattaStradale(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize route insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = codiceTratta;
	param[0].buffer_length = strlen(codiceTratta);
	
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = primaFermata;
	param[1].buffer_length = strlen(primaFermata);
	
	param[2].buffer_type = MYSQL_TYPE_STRING;
	param[2].buffer = ultimaFermata;
	param[2].buffer_length = strlen(ultimaFermata);

	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for route insertion\n", true);
	}


	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the vehicle route.");
		goto out;
	}
	printf("route correctly inserted\n");

out:
	mysql_stmt_close(prepared_stmt);
}





static void add_vehicleMaintenance(MYSQL* conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];
	char buff[46];
	char *token_vector[4];
	char veicolo[5];
	MYSQL_TIME data[1];
	float costo;
	char tipoIntervento[46];
	
	memset(data, 0, sizeof(data));
	
	data->time_type = MYSQL_TIMESTAMP_DATE;
	
	
	
	printf("\nInserisci la matricola del veicolo (4 cifre): ");
	getInput(5, veicolo, false);
	printf("\nInserisci la data in cui è avvenuta la manutenzione(yyyy-mm-hh): ");
	
	riprova:
	getInput(46, buff, false);
	tokenizer(token_vector,buff,0);
	for (int i=0; i<3;i++){
		if(token_vector[i] == NULL){
			printf ("Data inserita non corretta. Reinserirla (yyyy-mm-hh): ");
	 		goto riprova;
		}
	}	
	data->year = atoi (token_vector[0]);
	data->month = atoi (token_vector[1]);
	data->day = atoi (token_vector[2]);
	
	printf("\nInserisci il costo della manutenzione: ");
	getInput(46, buff, false);
	costo = (float)atof(buff);
	printf("\nInserisci informazioni sul tipo d'intervento offerto dalla manutenzione: ");
	getInput(46, tipoIntervento, false);

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_Manutenzione(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize maintenance insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = veicolo;
	param[0].buffer_length = strlen(veicolo);

	param[1].buffer_type = MYSQL_TYPE_DATE;
	param[1].buffer = data;
	param[1].buffer_length = sizeof(data);

	param[2].buffer_type = MYSQL_TYPE_FLOAT;
	param[2].buffer = &costo;
	param[2].buffer_length = sizeof(costo);
	
	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = tipoIntervento;
	param[3].buffer_length = strlen(tipoIntervento);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for maintenace insertion\n", true);
	}

	
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the vehicle maintenace.");
		goto out;
	}
	printf("vehicle maintenance correctly inserted\n");

out:
	mysql_stmt_close(prepared_stmt);
}



static void add_busStop(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	char buff[10];

	// Input for the registration routine
	char fermata[6];
	float latitudine;
	float longitudine;
	

	// Get the required information
	printf("\nInserisci il codice di fermata(5 cifre): ");
	getInput(6, fermata, false);
	printf("\nInserisci latitudine: ");
	getInput(10, buff, false);
	latitudine = (float)atof(buff);
	printf("\nInserisci longitudine: ");
	getInput(10, buff, false);
	longitudine = (float)atof(buff);

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_Fermata(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize waypoint insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = fermata;
	param[0].buffer_length = strlen(fermata);

	param[1].buffer_type = MYSQL_TYPE_FLOAT;
	param[1].buffer = &latitudine;
	param[1].buffer_length = sizeof(latitudine);

	param[2].buffer_type = MYSQL_TYPE_FLOAT;
	param[2].buffer = &longitudine;
	param[2].buffer_length = sizeof(longitudine);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for bus stop insertion\n", true);
	}

	
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the bus stop.");
		goto out;
	}
	printf("Bus stop correctly inserted\n");

out:
	mysql_stmt_close(prepared_stmt);
}




static void add_driver(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[9];
	char* token_vector_dataNascita[4];
	char* token_vector_scadenzaPatente[4];
	char buff1[46];
	char buff2[46];
	char cf[17];
	char username[46];
	char nome[46];
	char cognome[46];
	MYSQL_TIME data_nascita[1];
	MYSQL_TIME scadenza_patente[1];
	char luogo_nascita[46];
	char numero_patente[11];
	char veicolo_Assegnato[5];
	
	memset(data_nascita, 0, sizeof(data_nascita));
	memset(scadenza_patente, 0, sizeof(scadenza_patente));
	
	data_nascita->time_type = MYSQL_TIMESTAMP_DATE;
	scadenza_patente->time_type = MYSQL_TIMESTAMP_DATE;
	
	// Get the required information
	printf("\nInserisci codice fiscale (16 caratteri): ");
	getInput(17, cf, false);
	printf("\nInserisci username (valido): ");
	getInput(46, username, false);
	printf("\nInserisci nome: ");
	getInput(46, nome, false);
	printf("\nInserisci cognome: ");
	getInput(46, cognome, false);
	printf("\nInserisci la data di nascita(yyyy-mm-hh): ");
riprova1:
	getInput(46, buff1, false);
	tokenizer(token_vector_dataNascita, buff1,0);
	
	for (int i=0; i<3;i++){
		if(token_vector_dataNascita[i] == NULL){
			printf ("Data inserita non corretta. Reinserirla (yyyy-mm-hh): ");
	 		goto riprova1;
		}
	}	
	printf("\nInserisci luogo di nascita: ");
	getInput(46, luogo_nascita, false);
	printf("\nInserisci numero patente(10 caratteri): ");
	getInput(11, numero_patente, false);
	printf("\nInserisci la data di scadenza della patente(yyyy-mm-hh): ");
riprova2:
	getInput(46, buff2, false);
	tokenizer(token_vector_scadenzaPatente, buff2,0);
	for (int i=0; i<3;i++){
		if(token_vector_scadenzaPatente[i] == NULL){
			printf ("Data inserita non corretta. Reinserirla (yyyy-mm-hh): ");
	 		goto riprova2;
		}
	}
	
	printf("\nInserisci il veicolo assegnato(4 cifre) : ");
	getInput(5, veicolo_Assegnato, false);
	
	data_nascita->year = atoi (token_vector_dataNascita[0]);
	data_nascita->month = atoi (token_vector_dataNascita[1]);
	data_nascita->day = atoi (token_vector_dataNascita[2]);
	
	scadenza_patente->year = atoi (token_vector_scadenzaPatente[0]);
	scadenza_patente->month = atoi (token_vector_scadenzaPatente[1]);
	scadenza_patente->day = atoi (token_vector_scadenzaPatente[2]);
	
	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_Conducente(?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize student insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = cf;
	param[0].buffer_length = strlen(cf);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = username;
	param[1].buffer_length = strlen(username);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = nome;
	param[2].buffer_length = strlen(nome);

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = cognome;
	param[3].buffer_length = strlen(cognome);

	param[4].buffer_type = MYSQL_TYPE_DATE;
	param[4].buffer = data_nascita;
	param[4].buffer_length = sizeof(data_nascita);

	param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[5].buffer = luogo_nascita;
	param[5].buffer_length = strlen(luogo_nascita);

	param[6].buffer_type = MYSQL_TYPE_STRING;
	param[6].buffer = numero_patente;
	param[6].buffer_length = strlen(numero_patente);

	param[7].buffer_type = MYSQL_TYPE_DATE;
	param[7].buffer = scadenza_patente;
	param[7].buffer_length = sizeof(scadenza_patente);

	param[8].buffer_type = MYSQL_TYPE_STRING;
	param[8].buffer = veicolo_Assegnato;
	param[8].buffer_length = strlen(veicolo_Assegnato);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for driver insertion\n", true);
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
	char buff[20];

	// Input for the registration routine
	float latitudine;
	float longitudine;
	

	// Get the required information
	printf("\nInserisci latitudine: ");
	getInput(20, buff, false);
	latitudine = (float)atof(buff);
	printf("\nInserisci longitudine: ");
	getInput(20, buff, false);
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
	
	if(!parse_config("Users/AmministratoreServizio.json", &conf)) {
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
			link_vehicleWithRealRoute(conn);
			break;
		case 2:
			add_newWorkShift(conn);
			break;
		case 3:
			delete_workShift(conn);
			break;
		case 4:
			replace_workShift(conn);
			break;
		case 5:
			emissione_biglietto(conn);
			break;
		case 6:
			emissione_abbonamento(conn);
			break;
		case 7:
			driver_healed(conn);
			break;
		case 8:
			find_driver_shiftReplacement(conn);
			break;
		case 9:
			add_driver(conn);
			break;
		case 10:
			add_busStop(conn);
			break;
		case 11:
			add_vehicleMaintenance(conn);
			break;
		case 12:
			add_realRoute(conn);
			break;
		case 13:
			add_route(conn);
			break;
		case 14:
			add_vehicle(conn);
			break;
		case 15:
			add_waypoint(conn);
			break;
		case 16:
			add_waypointToARoute(conn);
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

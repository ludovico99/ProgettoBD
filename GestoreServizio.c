#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "defines.h"

//Ho inserito queste variabili globali affinchè le funzioni find_workShift e replace_workShift possano eseguire sia singolarmente sia in successione. In quest'ultimo caso evito di reinserire parametri passati in precedenza. 
static char global_conducente_cf[17];
static char global_conducente_selezionato[17];
static MYSQL_TIME global_inizioTurno[1];
static MYSQL_TIME global_fineTurno[1];
static int setted=0;


void add_newWorkShift(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];
	char buff1[46];
	char buff2[46];
	char *token_vectorInizioTurno[6];
	char *token_vectorFineTurno[6];
	void *data[3];
	enum_field_types type[3];
		
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
	
	
	fineTurno->year = atoi(token_vectorFineTurno[0]);
	fineTurno->month = atoi(token_vectorFineTurno[1]);
	fineTurno->day = atoi(token_vectorFineTurno[2]);
	fineTurno->hour = atoi(token_vectorFineTurno[3]);
	fineTurno->minute = atoi(token_vectorFineTurno[4]);
	
	
	
	if(!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_NuovoTurno_Conducente(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize work shift statement\n", false);
	}
	
	
	data[0]=(void*)conducente_cf;
	data[1]=(void*)inizioTurno;
	data[2]=(void*)fineTurno;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_DATETIME;
	type[2]=MYSQL_TYPE_DATETIME;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(3,data,type,param);
	

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

 void delete_workShift(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];
	char buff1[46];
	char buff2[46];
	char *token_vectorInizioTurno[6];
	char *token_vectorFineTurno[6];
	void *data[3];
	enum_field_types type[3];
		
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
	
	
	fineTurno->year = atoi(token_vectorFineTurno[0]);
	fineTurno->month = atoi(token_vectorFineTurno[1]);
	fineTurno->day = atoi(token_vectorFineTurno[2]);
	fineTurno->hour = atoi(token_vectorFineTurno[3]);
	fineTurno->minute = atoi(token_vectorFineTurno[4]);
	
	if(!setup_prepared_stmt(&prepared_stmt, "call Elimina_OrarioConducente(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize work shift statement\n", false);
	}
	
	
	data[0]=(void*)conducente_cf;
	data[1]=(void*)inizioTurno;
	data[2]=(void*)fineTurno;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_DATETIME;
	type[2]=MYSQL_TYPE_DATETIME;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(3,data,type,param);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for work shift elimination\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while deleting the work shift .");
		goto out;
	}

	printf("Work shift elimination correctly done\n");
    out:
	mysql_stmt_close(prepared_stmt);

}
	
	
static void replace_workShift(MYSQL* conn){
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[4];
	char buff1[46];
	char buff2[46];
	
	char *token_vectorInizioTurno[6];
	char *token_vectorFineTurno[6];
	
	void *data[4];
	enum_field_types type[4];
		
	if (setted ==1) goto Driver_Selected;
	
	memset(global_inizioTurno, 0, sizeof(global_inizioTurno));
	memset(global_fineTurno, 0, sizeof(global_fineTurno));
	
	
	printf("\nInserisci il codice fiscale del conducente da sostituire: ");
	getInput(17,global_conducente_cf,false);
	printf("\nInserisci il codice fiscale del conducente sostituto (nuovo): ");
	getInput(17, global_conducente_selezionato,false);		
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
	
	global_inizioTurno->year = atoi(token_vectorInizioTurno[0]);
	global_inizioTurno->month = atoi(token_vectorInizioTurno[1]);
	global_inizioTurno->day = atoi(token_vectorInizioTurno[2]);
	global_inizioTurno->hour = atoi(token_vectorInizioTurno[3]);
	global_inizioTurno->minute = atoi(token_vectorInizioTurno[4]);
	
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
		
	global_fineTurno->year = atoi(token_vectorFineTurno[0]);
	global_fineTurno->month = atoi(token_vectorFineTurno[1]);
	global_fineTurno->day = atoi(token_vectorFineTurno[2]);
	global_fineTurno->hour = atoi(token_vectorFineTurno[3]);
	global_fineTurno->minute = atoi(token_vectorFineTurno[4]);

Driver_Selected:			
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call SostituzioneTurno(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to replace driver statements\n", false);
	}

	data[0]=(void*)global_conducente_selezionato;
	data[1]=(void*)global_conducente_cf;
	data[2]=(void*)global_inizioTurno;
	data[3]=(void*)global_fineTurno;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_STRING;
	type[2]=MYSQL_TYPE_DATETIME;
	type[3]=MYSQL_TYPE_DATETIME;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(4,data,type,param);
	

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
    	setted=0;
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
	void *data[3];
	enum_field_types type[3];
	
	memset(global_inizioTurno, 0, sizeof(global_inizioTurno));
	memset(global_fineTurno, 0, sizeof(global_fineTurno));
			
	printf("\nInserisci il codice fiscale del conducente che si intende sostituire: ");
	getInput(17,global_conducente_cf,false);
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
	
	
	global_inizioTurno->year = atoi(token_vectorInizioTurno[0]);
	global_inizioTurno->month = atoi(token_vectorInizioTurno[1]);
	global_inizioTurno->day = atoi(token_vectorInizioTurno[2]);
	global_inizioTurno->hour = atoi(token_vectorInizioTurno[3]);
	global_inizioTurno->minute = atoi(token_vectorInizioTurno[4]);
	
	global_fineTurno->year = atoi(token_vectorFineTurno[0]);
	global_fineTurno->month = atoi(token_vectorFineTurno[1]);
	global_fineTurno->day = atoi(token_vectorFineTurno[2]);
	global_fineTurno->hour = atoi(token_vectorFineTurno[3]);
	global_fineTurno->minute = atoi(token_vectorFineTurno[4]);
		
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call Cerca_ConducentiValidiperLaSostituzione(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to find drivers for replacement statements\n", false);
	}

	data[0]=(void*)global_conducente_cf;
	data[1]=(void*)global_inizioTurno;
	data[2]=(void*)global_fineTurno;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_DATETIME;
	type[2]=MYSQL_TYPE_DATETIME;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(3,data,type,param);
	

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
riprova3:
	printf ("\nSi desidera scelgliere il conducente da sostituire?\nDigitare yes or no.\nDigitando yes si procederà alla sostituzione del turno.\nIl no porterà al termine della procedura. ");
	getInput(4,buff1,false);
	if(strcmp(buff1,"no")==0) goto out;
	else if (strcmp(buff1,"yes") !=0){
		printf("\nValori inseiriti non validi.\nReinserirli.");
		goto riprova3;
	}
	
	
	printf("\n\nScegliere un conducente tra quelli individuati(codice fiscale): ");
	getInput(17,global_conducente_selezionato,false);
	setted=1;
	replace_workShift(conn);
    out:
	mysql_stmt_close(prepared_stmt);

}



static void link_vehicleWithRealRoute(MYSQL* conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];
	char buff1[46];
	char buff2[46];
	
	char *token_vectorData[4];
	char *token_vectorOrario[3];
	void *data[4];
	enum_field_types type[4];
	
	char codiceTratta[6];
	MYSQL_TIME dataPartenza[1];
	MYSQL_TIME orarioPartenza[1];
	char veicoloAssociato[5];
	
	memset(dataPartenza, 0, sizeof(dataPartenza));
	memset(orarioPartenza, 0, sizeof(orarioPartenza));
	
	printf("\nInserisci il codice di tratta (5 cifre): ");
	getInput(6, codiceTratta, false);
	printf("\nInserisci la data di partenza (yyyy-mm-hh): ");
riprova1:
	getInput(46, buff1, false);
	tokenizer(token_vectorData,buff1,0);
	for (int i=0; i<3;i++){
		if(token_vectorData[i] == NULL){
			printf ("Data inserita non corretta. Reinserirla (yyyy-mm-hh): ");
	 		goto riprova1;
		}
	}
	
	printf("\nInserisci l'orario di partenza (hh-mm): ");
riprova2:
	getInput(46, buff2, false);
	tokenizer(token_vectorOrario,buff2,1);
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
	
	printf("\nInserisci il veicolo che si vuole assegnare a questa tratta (4 cifre): ");
	getInput(5, veicoloAssociato, false);
	
	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call AssociazioneVeicoli_TrattaReale(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize real route insertion statement\n", false);
	}

	data[0]=(void*)codiceTratta;
	data[1]=(void*)veicoloAssociato;
	data[2]=(void*)orarioPartenza;
	data[3]=(void*)dataPartenza;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_STRING;
	type[2]=MYSQL_TYPE_TIME;
	type[3]=MYSQL_TYPE_DATE;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(4,data,type,param);
	

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


void emissione_biglietto(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	int codiceBiglietto;
	void *data[1];
	enum_field_types type[1];
	
	if(!setup_prepared_stmt(&prepared_stmt, "call EmissioneBiglietto(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize ticket issue statement\n", false);
	}

	data[0]=(void*)&codiceBiglietto;
	
	type[0]=MYSQL_TYPE_LONG;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for ticket issue\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while retrieving the ticket issue.");
		goto out;
	}

	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	
	
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

 void eliminare_biglietto(MYSQL *conn){
	
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	char buff[46];
	int codiceBiglietto;
	void *data[1];
	enum_field_types type[1];
	
	printf("\nInserisci il codice del biglietto che si intende eliminare(int): ");
	getInput(46, buff, false);
	codiceBiglietto=atoi(buff);
	
	if(!setup_prepared_stmt(&prepared_stmt, "call Elimina_BigliettoElettronico(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize ticket delete statement\n", false);
	}

	data[0]=(void*)&codiceBiglietto;
	
	type[0]=MYSQL_TYPE_LONG;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for deleting ticket\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while deleting the ticket.");
		goto out;
	}
	
	printf("Ticket with ID %d is correctly deleted\n", codiceBiglietto);

    out:
	mysql_stmt_close(prepared_stmt);
}




 void emissione_abbonamento(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	int codiceAbbonamento;
	void *data[1];
	enum_field_types type[1];
	
	if(!setup_prepared_stmt(&prepared_stmt, "call EmissioneAbbonamento(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize transport pass statement\n", false);
	}

	data[0]=(void*)&codiceAbbonamento;
	
	type[0]=MYSQL_TYPE_LONG;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for transport pass\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while retrieving the transport pass.");
		goto out;
	}
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	
	
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

 void eliminare_abbonamento(MYSQL *conn){
	
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	char buff[46];
	int codiceAbbonamento;
	void *data[1];
	enum_field_types type[1];
	
	printf("\nInserisci il codice dell'abbonamento che si intende eliminare(int): ");
	getInput(46, buff, false);
	codiceAbbonamento=atoi(buff);
	
	if(!setup_prepared_stmt(&prepared_stmt, "call Elimina_Abbonamento(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize transport pass delete statement\n", false);
	}

	data[0]=(void*)&codiceAbbonamento;
	
	type[0]=MYSQL_TYPE_LONG;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for deleting transport pass\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while deleting the transport pass.");
		goto out;
	}

	printf("Transport pass with ID %d is correctly deleted\n", codiceAbbonamento);

    out:
	mysql_stmt_close(prepared_stmt);
}

static void driver_healed (MYSQL *conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];
	void *data[1];
	enum_field_types type[1];

	char codiceFiscale[17];
	
	printf("\nInserisci il codice fiscale del conducente guarito: ");
	getInput(17, codiceFiscale, false);
	
	if (!setup_prepared_stmt(&prepared_stmt, "call ConducenteGuarito(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize driver healed statements\n", false);
	}

	data[0]=(void*)codiceFiscale;

	type[0]=MYSQL_TYPE_STRING;
	
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	
	
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

static void link_driverWithVehicle (MYSQL *conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];
	void *data[2];
	enum_field_types type[2];

	char codiceFiscale[17];
	char veicolo[5];
	printf("\nInserisci il codice fiscale del conducente che si vuole associare (16 caratteri): ");
	getInput(17, codiceFiscale, false);
	
	printf("\nInserisci il veicolo che si vuole associare (4 caratteri): ");
	getInput(5, veicolo, false);
	
	if (!setup_prepared_stmt(&prepared_stmt, "call AssociareConducente_Veicolo(?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize driver linking statements\n", false);
	}

	data[0]=(void*)codiceFiscale;
	data[1]=(void*)veicolo;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_STRING;
	
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(2,data,type,param);

	
	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for driver linking procedure\n", true);
	}


	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while executing the driver linking procedure.");
		goto out;
	}
	printf("Driver link procedure correctly done\n");

out:
	mysql_stmt_close(prepared_stmt);
}

void look_for_Driver_Shift(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	int status;
	char cf[17];
	char header[64];
	void *data[1];
	enum_field_types type[1];
	
		
	printf("\nInserisci il codice fiscale del conducente del quale si vogliono conoscere i turni(16 caratteri): ");
	getInput(17,cf,false);
		
		
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call CercaTurni_per_Conducente(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to setup look for driver shift statements\n", false);
	}

	// Prepare parameters
	
	data[0]=(void*)cf;
	
	type[0]=MYSQL_TYPE_STRING;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for looking for driver shift statements\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while executing the look for driver shift procedure.");
		goto out;
	}

	do {
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}
		
		sprintf(header, "Turni relativi al conducente: %s",cf);
		dump_result_set(conn, prepared_stmt, header);
		
	    next:
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		
	} while (status == 0);

    out:
	mysql_stmt_close(prepared_stmt);
}

void look_for_info_AboutADriver(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	int status;
	char cf[17];
	char header[64];
	void *data[1];
	enum_field_types type[1];
	
		
	printf("\nInserisci il codice fiscale del conducente del quale si vuole conoscere le informazioni(16 caratteri): ");
	getInput(17,cf,false);
		
		
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call Cerca_InformazioniImportanti_Di_Un_Conducente(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to setup look for info about a Driver statements\n", false);
	}

	// Prepare parameters
	
	data[0]=(void*)cf;
	
	type[0]=MYSQL_TYPE_STRING;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for looking for info about a Driver\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while executing the look for info about a Driver procedure.");
		goto out;
	}

	do {
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}
		
		sprintf(header, "Informazioni relative al conducente %s",cf);
		dump_result_set(conn, prepared_stmt, header);
		
	    next:
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		
	} while (status == 0);

    out:
	mysql_stmt_close(prepared_stmt);
}

void run_as_serviceManager(MYSQL* conn)
{
	const char* options[14] = {"1", "2", "3", "4", "5","6","7","8","9","10","11","12","13","14"};
	int op;

	printf("Switching to service manager role...\n");
	
	if(!parse_config("Users/GestoreServizio.json", &conf)) {
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
		printf("1) Aggiungere nuovo turno ad un conducente \n");
		printf("2) Eliminare turno ad un conducente\n");
		printf("3) Cerca conducenti validi per la sostituzione\n");
		printf("4) Sostituire il turno di un conducente\n");
		printf("5) Associare un veicolo ad una tratta reale\n");
		printf("6) Emettere biglietto\n");
		printf("7) Eliminare biglietto\n");
		printf("8) Emettere abbonamento\n");
		printf("9) Eliminare abbonamento\n");
		printf("10) Conducente guarito\n");
		printf("11) Associare un conducente ad un veicolo\n");
		printf("12) Cerca turni di un conducente\n");
		printf("13) Cerca informazioni importanti di un conducente\n");
		printf("14) Quit\n");

		op = atoi(multiChoice("Select an option", options, 14));

		switch (op) {
		
		case 1:
			add_newWorkShift(conn);
			break;
		case 2:
			delete_workShift(conn);
			break;
		case 3:
			find_driver_shiftReplacement(conn);
			break;
		case 4:
			replace_workShift(conn);
			break;
		case 5:
			link_vehicleWithRealRoute(conn);
			break;
		case 6:
			emissione_biglietto(conn);
			break;
		case 7:
			eliminare_biglietto(conn);
			break;
		case 8:
			emissione_abbonamento(conn);
			break;
		case 9:
			eliminare_abbonamento(conn);
			break;
		case 10:
			driver_healed(conn);
			break;
		case 11:
			link_driverWithVehicle(conn);
			break;
		case 12:
			look_for_Driver_Shift(conn);
			break;
		case 13:
			look_for_info_AboutADriver(conn);
			break;
		case 14:
			return;
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}
		
		getchar();
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "defines.h"


static void add_vehicle(MYSQL* conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];
	char buff[46];
	void *data[2];
	enum_field_types type[2];
	
	// Get the required information
	char veicolo[5];
	MYSQL_TIME dataAcquisto[1];
	
	memset(dataAcquisto, 0, sizeof(dataAcquisto));
	
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

	data[0]=(void*)veicolo;
	data[1]=(void*)dataAcquisto;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_DATE;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(2,data,type,param);


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


static void delete_vehicle(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	char veicolo[5];
	void *data[1];
	enum_field_types type[1];

	printf("\nInserisci la matricola del veicolo che si intende eliminare (4 cifre): ");
	getInput(5, veicolo, false);
	
		
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call Elimina_VeicoloPubblico(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize delete vehicle statements\n", false);
	}

	
	data[0]=(void*)veicolo;
	
	type[0]=MYSQL_TYPE_STRING;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for deleting vehicle\n", true);
	}
	if (mysql_stmt_execute(prepared_stmt) != 0) {

		print_stmt_error(prepared_stmt, "An error occurred while deleting the vehicle.");
		goto out;
	}
	
	printf("Vehicle deleted is %s...\n", veicolo);

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
	char buff3[4];
	char cf[17];
	char username[46];
	char nome[46];
	char cognome[46];
	MYSQL_TIME data_nascita[1];
	MYSQL_TIME scadenza_patente[1];
	char luogo_nascita[46];
	char numero_patente[11];
	char veicolo_Assegnato[5];
	void *data[9];
	enum_field_types type[9];
	int risposta=1;
	
	memset(data_nascita, 0, sizeof(data_nascita));
	memset(scadenza_patente, 0, sizeof(scadenza_patente));
		
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
riprova3:	
	printf("\nSi desidera inserire un veicolo per il conducente(yes/no): ");
	getInput(4,buff3,false);
	if ((risposta=(strcmp(buff3,"no")))==0){
		goto no;
	}
	else if ((strcmp(buff3,"yes"))!=0){
		printf("\nRisposta non valida.\nReinserirla.\n");
		goto riprova3;
	}
	
	printf("\nInserisci il veicolo assegnato(4 cifre): ");
	getInput(5, veicolo_Assegnato, false);
no:	
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

	data[0]=(void*)cf;
	data[1]=(void*)username;
	data[2]=(void*)nome;
	data[3]=(void*)cognome;
	data[4]=(void*)data_nascita;
	data[5]=(void*)luogo_nascita;
	data[6]=(void*)numero_patente;
	data[7]=(void*)scadenza_patente;
	data[8]=(void*)veicolo_Assegnato;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_VAR_STRING;
	type[2]=MYSQL_TYPE_VAR_STRING;
	type[3]=MYSQL_TYPE_VAR_STRING;
	type[4]=MYSQL_TYPE_DATE;
	type[5]=MYSQL_TYPE_VAR_STRING;
	type[6]=MYSQL_TYPE_STRING;
	type[7]=MYSQL_TYPE_DATE;
	if (risposta==0) type[8]=MYSQL_TYPE_NULL;
	else type[8]=MYSQL_TYPE_STRING;
	
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(9,data,type,param);
	
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

static void delete_driver(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	char cf[17];
	void *data[1];
	enum_field_types type[1];

	printf("\nInserisci il codice fiscale del conducente che si intende eliminare: ");
	getInput(17, cf, false);
	
		
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call Elimina_Conducente(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize delete driver statements\n", false);
	}

	
	data[0]=(void*)cf;
	
	type[0]=MYSQL_TYPE_STRING;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for deleting the driver\n", true);
	}
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while deleting the driver.");
		goto out;
	}
	
	printf("driver deleted is %s...\n", cf);

    out:
	mysql_stmt_close(prepared_stmt);
}

static void add_busStop(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	char buff[10];
	void *data[3];
	enum_field_types type[3];

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

	data[0]=(void*)fermata;
	data[1]=(void*)&latitudine;
	data[2]=(void*)&longitudine;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_FLOAT;
	type[2]=MYSQL_TYPE_FLOAT;
	
	memset(param, 0, sizeof(param));
	setup_mysql_bind(3,data,type,param);
	
	

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

static void delete_busStop(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	char fermata[6];
	void *data[1];
	enum_field_types type[1];


	printf("\nInserisci il codice di fermata che si vuole eliminare (5 cifre): ");
	getInput(6, fermata, false);
	
		
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call Elimina_Fermata(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize delete bus stop statements\n", false);
	}

	
	data[0]=(void*)fermata;
	
	type[0]=MYSQL_TYPE_STRING;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for deleting bus stop\n", true);
	}
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while deleting the bus stop.");
		goto out;
	}
	
	printf("Bus stop deleted is %s...\n", fermata);

    out:
	mysql_stmt_close(prepared_stmt);
}


static void add_vehicleMaintenance(MYSQL* conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];
	char buff[46];
	char *token_vector[4];
	char veicolo[5];
	MYSQL_TIME dataManutenzione[1];
	float costo;
	char tipoIntervento[46];
	void *data[4];
	enum_field_types type[4];
	
	memset(dataManutenzione, 0, sizeof(dataManutenzione));
	
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
	dataManutenzione->year = atoi (token_vector[0]);
	dataManutenzione->month = atoi (token_vector[1]);
	dataManutenzione->day = atoi (token_vector[2]);
	
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
	
	data[0]=(void*)veicolo;
	data[1]=(void*)dataManutenzione;
	data[2]=(void*)&costo;
	data[3]=(void*)tipoIntervento;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_DATE;
	type[2]=MYSQL_TYPE_FLOAT;
	type[3]=MYSQL_TYPE_VAR_STRING;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(4,data,type,param);
	
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

static void delete_vehicleMaintenance(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];
	char veicolo[5];
	char buff[46];
	char *token_vector[4];
	MYSQL_TIME dataManutenzione[1];
	void *data[2];
	enum_field_types type[2];


	memset(dataManutenzione, 0, sizeof(dataManutenzione));
	
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
	dataManutenzione->year = atoi (token_vector[0]);
	dataManutenzione->month = atoi (token_vector[1]);
	dataManutenzione->day = atoi (token_vector[2]);
		
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call Elimina_Manutenzione(?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize delete vehicle maintenance statements\n", false);
	}


	data[0]=(void*)veicolo;
	data[1]=(void*)dataManutenzione;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_DATE;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(2,data,type,param);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for deleting vehicle maintenance\n", true);
	}
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while deleting the vehicle maintenance.");
		goto out;
	}
	
	printf("Vehicle maintenance %s dated %d-%d-%d deleted...\n", veicolo, dataManutenzione->year, dataManutenzione->month, dataManutenzione->day);

    out:
	mysql_stmt_close(prepared_stmt);
}

static void add_route(MYSQL* conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	
	char codiceTratta[6];
	char primaFermata[6];
	char ultimaFermata[6];
	void *data[3];
	enum_field_types type[3];
	
	
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

	data[0]=(void*)codiceTratta;
	data[1]=(void*)primaFermata;
	data[2]=(void*)ultimaFermata;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_STRING;
	type[2]=MYSQL_TYPE_STRING;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(3,data,type,param);
	

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

static void delete_route(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	char tratta[6];
	void *data[1];
	enum_field_types type[1];


	printf("\nInserisci il codice della tratta che si intende eliminare(5 cifre): ");
	getInput(6, tratta, false);
	
		
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call Elimina_TrattaStradale(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize delete route statements\n", false);
	}

	
	data[0]=(void*)tratta;
	
	type[0]=MYSQL_TYPE_STRING;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for deleting the route\n", true);
	}
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while deleting the route.");
		goto out;
	}
	
	printf("Route deleted is %s...\n", tratta);

    out:
	mysql_stmt_close(prepared_stmt);
}

static void add_realRoute(MYSQL* conn){
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];
	char buff1[46];
	char buff2[46];
	char *token_vectorData[4];
	char *token_vectorOrario[3];
	char codiceTratta[6];
	MYSQL_TIME dataPartenza[1];
	MYSQL_TIME orarioPartenza[1];
	char veicoloAssociato[5];
	void *data[4];
	enum_field_types type[4];
	
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
	
	
	dataPartenza->year = atoi (token_vectorData[0]);
	dataPartenza->month = atoi (token_vectorData[1]);
	dataPartenza->day = atoi (token_vectorData[2]);
	
	printf("\nInserisci l'orario di partenza (hh:mm): ");
riprova2:
	getInput(46, buff2, false);
	tokenizer(token_vectorOrario,buff2,1);
	for (int i=0; i<2;i++){
		if(token_vectorOrario[i] == NULL){
			printf ("Orario inserito non corretto. Reinserirlo (hh:mm): ");
	 		goto riprova2;
		}
	}
	orarioPartenza->hour = atoi(token_vectorOrario[0]);
	orarioPartenza->minute = atoi(token_vectorOrario[1]);
	
	printf("\nInserisci il veicolo assegnato a questa tratta (4 cifre): ");
	getInput(5, veicoloAssociato, false);
	

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_TrattaReale(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize real route insertion statement\n", false);
	}

	data[0]=(void*)codiceTratta;
	data[1]=(void*)dataPartenza;
	data[2]=(void*)orarioPartenza;
	data[3]=(void*)veicoloAssociato;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_DATE;
	type[2]=MYSQL_TYPE_TIME;
	type[3]=MYSQL_TYPE_STRING;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(4,data,type,param);
	

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

static void delete_realRoute(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];
	char buff[46];
	char *token_vectorData[4];
	char *token_vectorOrario[3];
	char codiceTratta[6];
	MYSQL_TIME dataPartenza[1];
	MYSQL_TIME orarioPartenza[1];
	void *data[3];
	enum_field_types type[3];

	memset(dataPartenza, 0, sizeof(dataPartenza));
	memset(orarioPartenza, 0, sizeof(orarioPartenza));
	
	printf("\nInserisci il codice della tratta che si intende eliminare(5 cifre): ");
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
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call Elimina_TrattaReale(?,?,?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize delete real route statements\n", false);
	}

	data[0]=(void*)codiceTratta;
	data[1]=(void*)dataPartenza;
	data[2]=(void*)orarioPartenza;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_DATE;
	type[2]=MYSQL_TYPE_TIME;
	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(3,data,type,param);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for deleting real route\n", true);
	}
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while deleting the real route.");
		goto out;
	}
	
	printf("Real route deleted is %s dated %d-%d-%d %d:%d:00 \n", codiceTratta, dataPartenza->year, dataPartenza->month, dataPartenza->day, orarioPartenza->hour, orarioPartenza->minute);

    out:
	mysql_stmt_close(prepared_stmt);
}

static void add_waypoint(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[2];
	char buff[20];
	void *data[2];
	enum_field_types type[2];

	// Input for the registration routine
	float latitudine;
	float longitudine;
	

	// Get the required information
	printf("\nInserisci latitudine: ");
	getInput(20, buff, false);
	latitudine = strtof(buff,NULL);
	printf("\nInserisci longitudine: ");
	getInput(20, buff, false);
	longitudine = strtof(buff,NULL);

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_Waypoint(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize waypoint insertion statement\n", false);
	}

	data[0]=(void*)&latitudine;
	data[1]=(void*)&longitudine;
	
	type[0]=MYSQL_TYPE_FLOAT;
	type[1]=MYSQL_TYPE_FLOAT;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(2,data,type,param);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for waypoint insertion\n", true);
	}

	
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the waypoint.");
		goto out;
	}
	printf("Wayoint inserted correctly\n");

out:
	mysql_stmt_close(prepared_stmt);
}

static void delete_waypoint(MYSQL *conn){

	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];
	char buff[46];
	void *data[2];
	enum_field_types type[2];
	
	float latitudine;
	float longitudine;
	
	
	printf("\nInserisci latitudine: ");
	getInput(20, buff, false);
	latitudine = strtof(buff,NULL);
	printf("\nInserisci longitudine: ");
	getInput(20, buff, false);
	longitudine = strtof(buff,NULL);

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Elimina_Waypoint(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize waypoint delete statement\n", false);
	}

	data[0]=(void*)&latitudine;
	data[1]=(void*)&longitudine;
	
	type[0]=MYSQL_TYPE_FLOAT;
	type[1]=MYSQL_TYPE_FLOAT;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(2,data,type,param);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for deleting waypoint\n", true);
	}
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while deleting the waypoint.");
		goto out;
	}
	
	printf("waypoint deleted has lat: %f and long: %f \n",latitudine, longitudine);

    out:
	mysql_stmt_close(prepared_stmt);
}

static void add_waypointToARoute(MYSQL* conn)
{
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[4];
	char buff[10];
	void *data[4];
	enum_field_types type[4];

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
	latitudine = strtof(buff,NULL);
	printf("\nInserisci longitudine: ");
	getInput(10, buff, false);
	longitudine = strtof(buff,NULL);
	printf("\nInserisci il numero che rappresenta l'ordinamento del waypoint all'interno della tratta : ");
	getInput(10, buff, false);
	numeroOrdine=atoi(buff);

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Aggiungi_Waypoint_per_una_tratta(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize waypoint insertion statement\n", false);
	}

	data[0]=(void*)tratta;
	data[1]=(void*)&latitudine;
	data[2]=(void*)&longitudine;
	data[3]=(void*)&numeroOrdine;
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_FLOAT;
	type[2]=MYSQL_TYPE_FLOAT;
	type[3]=MYSQL_TYPE_LONG;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(4,data,type,param);
	

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

static void delete_waypointFromARoute(MYSQL *conn){

	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	char buff[10];
	void *data[3];
	enum_field_types type[3];

	// Input for the registration routine
	char tratta[6];
	float latitudine;
	float longitudine;

	// Get the required information
	printf("\nInserisci il codice di tratta(5 cifre): ");
	getInput(6, tratta, false);
	printf("\nInserisci latitudine: ");
	getInput(10, buff, false);
	latitudine = strtof(buff,NULL);
	printf("\nInserisci longitudine: ");
	getInput(10, buff, false);
	longitudine = strtof(buff,NULL);
	

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Elimina_WaypointInTratta(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize waypoint delete from a route statement\n", false);
	}

	data[0]=(void*)tratta;
	data[1]=(void*)&latitudine;
	data[2]=(void*)&longitudine;
	
	
	type[0]=MYSQL_TYPE_STRING;
	type[1]=MYSQL_TYPE_FLOAT;
	type[2]=MYSQL_TYPE_FLOAT;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(3,data,type,param);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for deleting waypoint from a route\n", true);
	}
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while deleting the waypoint from a route.");
		goto out;
	}
	
	printf("Waypoint from route %s deleted has lat: %f and long: %f  \n",tratta, latitudine, longitudine);

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
	void *data[3];
	enum_field_types type[3];

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

	data[0]=(void*)username;
	data[1]=(void*)password;
	data[2]=(void*)ruolo;
	
	type[0]=MYSQL_TYPE_VAR_STRING;
	type[1]=MYSQL_TYPE_VAR_STRING;
	type[2]=MYSQL_TYPE_VAR_STRING;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(3,data,type,param);


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

static void delete_user(MYSQL *conn){
	
	MYSQL_STMT* prepared_stmt;
	MYSQL_BIND param[3];
	void *data[1];
	enum_field_types type[1];
	
	char username[46];
	
	// Get the required information
	printf("\nUsername: ");
	getInput(46, username, false);
	

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&prepared_stmt, "call Elimina_Utente(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user delete statement\n", false);
	}

	data[0]=(void*)username;
	
	type[0]=MYSQL_TYPE_VAR_STRING;

	
	memset(param, 0, sizeof(param));
	
	setup_mysql_bind(1,data,type,param);
	

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for deleting user\n", true);
	}
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while deleting the user.");
		goto out;
	}
	
	printf("User with username %s is deleted",username);

    out:
	mysql_stmt_close(prepared_stmt);
}
	

void run_as_administrator(MYSQL* conn)
{
	const char* options[27] = {"1", "2", "3", "4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27"};
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
		printf("1) Aggiungere veicolo pubblico\n");
		printf("2) Eliminare veicolo pubblico\n");
		printf("3) Aggiungere nuovo conducente\n");
		printf("4) Eliminare conducente\n");
		printf("5) Aggiungere nuova fermata\n");
		printf("6) Eliminare fermata\n");
		printf("7) Aggiungere Manutenzione\n");
		printf("8) Eliminare Manutenzione\n");
		printf("9) Aggiungere tratta stradale\n");
		printf("10) Eliminare tratta stradale\n");
		printf("11) Aggiungere tratta reale\n");
		printf("12) Eliminare tratta reale\n");
		printf("13) Aggiungere waypoint\n");
		printf("14) Eliminare waypoint\n");
		printf("15) Aggiungere Waypoint in una tratta\n");
		printf("16) Eliminare Waypoint da una tratta\n");
		printf("17) Crea Utente\n");
		printf("18) Eliminare Utente\n");
		printf("19) Cerca turni di un conducente\n");
		printf("20) Cerca informazioni importanti di un conducente\n");
		printf("21) Emettere biglietto\n");
		printf("22) Eliminare biglietto\n");
		printf("23) Emettere abbonamento\n");
		printf("24) Eliminare abbonamento\n");
		printf("25) Aggiungere nuovo turno ad un conducente \n");
		printf("26) Eliminare turno ad un conducente\n");
		printf("27) Quit\n");

		op = atoi(multiChoice("Select an option", options, 27));

		switch (op) {
		case 1:
			add_vehicle(conn);
			break;
		case 2:
			delete_vehicle(conn);
			break;
		case 3:
			add_driver(conn);
			break;
		case 4:
			delete_driver(conn);
			break;
		case 5:
			add_busStop(conn);
			break;
		case 6:
			delete_busStop(conn);
			break;	
		case 7:
			add_vehicleMaintenance(conn);
			break;
		case 8:
			delete_vehicleMaintenance(conn);
			break;
		case 9:
			add_route(conn);
			break;
		case 10:
			delete_route(conn);
			break;
		case 11:
			add_realRoute(conn);
			break;
		case 12:
			delete_realRoute(conn);
			break;
		case 13:
			add_waypoint(conn);
			break;
		case 14:
			delete_waypoint(conn);
			break;
		case 15:
			add_waypointToARoute(conn);
			break;
		case 16:
			delete_waypointFromARoute(conn);
			break;
		case 17:
			create_user(conn);
			break;
		case 18:
			delete_user(conn);
			break;
		case 19:
			look_for_Driver_Shift(conn);
			break;
		case 20:
			look_for_info_AboutADriver(conn);
			break;
			
		case 21:
			emissione_biglietto(conn);
			break;
		case 22:
			eliminare_biglietto(conn);
			break;
		case 23:
			emissione_abbonamento(conn);
			break;
		case 24:
			eliminare_abbonamento(conn);
			break;
		case 25:
			add_newWorkShift(conn);
			break;
		case 26:
			delete_workShift(conn);
			break;
		case 27:
			return;
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}
		
		getchar();
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"



void run_as_driver(MYSQL* conn)
{
	const char* options[2] = { "1","2" };
	int op;

	printf("Switching to driver role...\n");

	
	conf.db_username = "Conducente";
	conf.db_password = "conducente";
	conf.database = "progetto";


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

		/*switch (op) {
		case '1':
			career_report(conn);
			break;

		case '2':
			return;

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
		}
		*/
		getchar();
	}
}

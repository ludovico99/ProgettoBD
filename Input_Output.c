
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>

#include "defines.h"

char* multiChoice(const char* domanda, const char* choices[], int num)
{

	// Genera la stringa delle possibilità
	char* possib = (char*)malloc(3* num * sizeof(char));
	int i,j = 0;

	for (i = 0; i < num; i++) {
		possib[j++] = *choices[i];
		if (i >= 9) {
			possib[j++] = *(choices[i]+1);
		}
		possib[j++] = '/';
	}
	possib[j - 1] = '\0'; // Per eliminare l'ultima '/' // Per eliminare l'ultima '/'

	// Chiede la risposta
	while (true) {
		// Mostra la domanda
		printf("%s [%s]: ", domanda, possib);

		/*char c[3];
		if (scanf_s("%2s",c,sizeof(c)) == NULL) {
			printf("scanf_s error");
			exit(-1);
		};*/
		char* c = (char*)malloc(10* sizeof(char));
		if (fgets(c, 10, stdin)==NULL) {
			printf("fgets error");
			exit(-1);

		}
		// Controlla se è un carattere valido
		for (i = 0; i < num; i++) {
			/*if (strcmp(c,choices[i])==0)
			{
				return c;
			}*/
			if (i >= 9)
			{
				if ((c[0] == choices[i][0]) and (c[1] == choices[i][1]))
					return c;
			}
			else {
				if (c[0] == choices[i][0])
					return c;
			}
		}
	}
}

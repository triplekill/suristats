/*
 * suristats.c
 *
 *  Created on: 20 oct. 2013
 *      Author: david
 */

#include <errno.h>		/* errno		*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/* strncpy()	*/
#include <unistd.h>		/* getopt() 	*/
#include <sys/stat.h>	/* stat()		*/

#include "suristats.h"

#include "ssDb.h"
#include "ssParse.h"

#define USAGE	"Usage: %s [ch] DATABASE SQL-STATEMENT\n"

extern	char	*optarg;
extern	int		optind;

int main(int argc, char **argv){
	struct 	stat	sb;

	int		createDb = FALSE, opt;

	ssDb	*db;

	// Verifier la ligne de commande (nom programme, nom de la database, commande SQL)
	if(argc < 3 ){
		fprintf(stderr, USAGE, argv[0]);
		return EXIT_FAILURE;
	}

	// Recuperer les options de la ligne de commande
	while ((opt = getopt (argc, argv, "chf:")) != -1) {
		switch (opt) {
			case 'h':
				// option 'help'
				fprintf(stdout, USAGE, argv[0]);
				return EXIT_SUCCESS;
				break;
			case 'f':
				// option 'fichier log Suricata'
				ssParseFile(optarg);
				break;
			case 'c':
				// option 'creer la database'
				createDb = TRUE;
				break;
			default :
				return EXIT_FAILURE;
				break;
		}
	}

	// Creer la database ou verifier l etat du fichier database
	if (createDb) {
		// Creer la database
		db = ssDbCreate(argv[optind]);
		ssDbClose(db);
	}
	else {
		// Verifier l etat du fichier database
		if (stat(argv[optind], &sb) == -1) {
			perror("stat()");
			exit(errno);
		}

		if ((sb.st_mode & S_IFMT) != S_IFREG) {
			return EXIT_FAILURE;
		}
	}

	// Ouvrir la database
	if ((db = ssDbOpen(argv[optind])) == NULL) {
		return EXIT_FAILURE;
	}

	// Executer la commande SQL
	ssDbCommand(db, argv[optind + 1]);

	// Fermer la database
	ssDbClose(db);

	return EXIT_SUCCESS;
}

/*
 * suristats.c
 *
 *  Created on: 20 oct. 2013
 *      Author: david
 */

#include <errno.h>		/* errno	*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/* strlen()	*/
#include <unistd.h>		/* getopt() */
#include <sys/stat.h>	/* stat()	*/

#include "suristats.h"

#include "ssDb.h"
#include "ssParse.h"

#define USAGE	"Usage: %s [-h] [-c database] [-f logfile]\n\t-c database\tCreer si elle n'existe pas la base donné SQL\n\t-f logfile\tUtilse logfile comme sortie de log Suricata"
#define	TMP		"/tmp/suristat.db"

extern	char	*optarg;

int main(int argc, char **argv){
	struct 	stat	sb;

	char	*logFile = NULL, *dbFile = NULL;
	int		opt;

	ssDb	*db;

	// Recuperer les options de la ligne de commande
	while ((opt = getopt (argc, argv, "hc:f:")) != -1) {
		switch (opt) {
			case 'h':
				// option 'help'
				fprintf(stdout, USAGE, argv[0]);
				return EXIT_SUCCESS;
				break;

			case 'f':
				// option 'fichier log Suricata'
				logFile = strdup(optarg);
				break;

			case 'c':
				// option 'creer la database'
				dbFile = strdup(optarg);
				break;

			default :
				return EXIT_FAILURE;
				break;
		}
	}

	// Si la database n'est pas fournie
	if (dbFile == NULL) {
		dbFile = strdup(TMP);
	}

	// Creer la database si vide
	db = ssDbCreate(dbFile);
	ssDbClose(db);

	// Verifier l etat du fichier database
	if (stat(dbFile, &sb) == -1) {
		perror("stat()");
		exit(errno);
	}

	if ((sb.st_mode & S_IFMT) != S_IFREG) {
		return EXIT_FAILURE;
	}

	// Ouvrir la database
	if ((db = ssDbOpen(dbFile)) == NULL) {
		return EXIT_FAILURE;
	}

	// Verification de l'option 'fichier log Suricata'
	if (logFile == NULL) {
		return EXIT_FAILURE;
	}

	// Parser le fichier log de Suricata
	fprintf(stdout, "Fichier %s : %d lignes lues\n", logFile, ssParseFile(db, logFile));

	// Requete sur la base de donnees
	ssDbRequete(db);

	// Fermer la base de donnees
	ssDbClose(db);

	// Si la base de donnee n'est pas fournie, effacer le fichier base de donnee
	if (!strcmp(dbFile, TMP)) {
		ssDbDelete(dbFile);
	}

	// Liberer les allocations de dbFile et logFile
	free(dbFile);
	free(logFile);

	return EXIT_SUCCESS;
}

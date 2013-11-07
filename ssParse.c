/*
 * ssParse.c
 *
 *  Created on: 22 oct. 2013
 *      Author: david
 */

#include <errno.h>		/* errno		*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/* strncpy()	*/

#include "ssDb.h"
#include "ssParse.h"

#define TAILLE_BUFFER	255

#define	LIGNE_DATE			"Date"
#define LIGNE_SEPARATION	"----"
#define	LIGNE_TITRE			"Counter"

int ssParseFile(ssDb *db, char *name)
{
	ssDbCounter	counter;
	ssDbDump	dump;

	struct	tm	date;

	FILE *file;

	char 	ligne[TAILLE_BUFFER];
    int 	cpt = 0;
    int		sec, min, hours, days;

	// Initialiser dump
	memset(&dump, 0, sizeof(dump));

	// Ouvrir le fichier log Suricata
	if ((file = fopen(name,"r")) == NULL)
	{
		perror("fopen()");
		exit(errno);
	}

	// Parcourir le fichier ligne par ligne
	while (fgets(ligne, sizeof(ligne), file) != NULL) {
		// Incrémenter le compteur de ligne
		cpt++;

		if (strncmp(ligne, LIGNE_SEPARATION, strlen(LIGNE_SEPARATION))) {
			if (strncmp(ligne, LIGNE_DATE, strlen(LIGNE_DATE))) {
				if (strncmp(ligne, LIGNE_TITRE, strlen(LIGNE_TITRE))) {
					// Nouveau counter
					sscanf(ligne, "%s | %s | %d", counter.cname, counter.tm_name, &counter.value);

					// Inserer le counter dans la base de donnee
					ssDbInsererCounter(db, &dump, &counter);
				}
			}
			else {
				// Nouveau dump
				sscanf(ligne, "Date: %d/%d/%d -- %d:%d:%d (uptime: %dd, %dh %dm %ds)",
						&date.tm_mon, &date.tm_mday, &date.tm_year, &date.tm_hour, &date.tm_min, &date.tm_sec, &days, &hours, &min, &sec);

				// Vérifier si le dump correspond à un nouveau runtime
				if ((days DAY + hours HOUR + min MIN + sec SEC) < (dump.days DAY + dump.hours HOUR + dump.min MIN + dump.sec SEC)) {
					// Inserer le précédent runtime dans la base de donnee
					ssDbInsererRuntime(db, &dump);
				}
				// Sauvegarder la date du dump
				memcpy(&dump.date, &date, sizeof(date));

				// Ajuster les variables de la structure tm de runtime
				dump.date.tm_mon 	-= 1;
				dump.date.tm_year	-= 1900;

				// Sauvegarder le uptime du dump
				dump.days 	= days;
				dump.hours	= hours;
				dump.min	= min;
				dump.sec	= sec;
			}
		}
	}

	// Inserer le dernier runtime dans la base de donnee
	ssDbInsererRuntime(db, &dump);

	// Fermer le fichier
	fclose(file);

	return cpt;
}

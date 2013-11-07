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

#define	SEC		* 1
#define	MIN		* 60 SEC
#define	HOUR	* 60 MIN
#define	DAY		* 24 HOUR

#define TAILLE_BUFFER	255

#define	LIGNE_DATE			"Date"
#define LIGNE_SEPARATION	"----"
#define	LIGNE_TITRE			"Counter"

int ssParseFile(ssDb *db, char *name)
{
	ssDbCounter	counter;
	ssDbRuntime	runtime;

	struct	tm	date;

	FILE *file;

	char 	ligne[TAILLE_BUFFER];
    int 	cpt = 0;
    int		sec, min, hours, days;

	// Initialiser runtime
	runtime.days = -1;

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
					ssDbInsererCounter(db, &runtime, &counter);
				}
			}
			else {
				// Premier ou nouvel enregistrement
				sscanf(ligne, "Date: %d/%d/%d -- %d:%d:%d (uptime: %dd, %dh %dm %ds)",
						&date.tm_mon, &date.tm_mday, &date.tm_year, &date.tm_hour, &date.tm_min, &date.tm_sec, &days, &hours, &min, &sec);

				if ((days DAY + hours HOUR + min MIN + sec SEC) < (runtime.days DAY + runtime.hours HOUR + runtime.min MIN + runtime.sec SEC)) {
					// Vérifier si ce n'est pas le premier enregistrement
					if (runtime.days != -1) {
						// Inserer le précédent runtime dans la base de donnee
						ssDbInsererRuntime(db, &runtime);
					}

					// Sauvegarder la date de début du nouveau runtime
					memcpy(&runtime.date, &date, sizeof(date));

					// Ajuster les variables de la structure tm de runtime
					runtime.date.tm_mon 	-= 1;
					runtime.date.tm_year	-= 1900;
				}

				// Sauvegarder le uptime dans la structure runtime
				runtime.days 	= days;
				runtime.hours	= hours;
				runtime.min		= min;
				runtime.sec		= sec;
			}
		}
	}

	// Inserer le dernier runtime dans la base de donnee
	ssDbInsererRuntime(db, &runtime);

	// Fermer le fichier
	fclose(file);

	return cpt;
}

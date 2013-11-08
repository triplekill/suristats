/*
 * ssDb.c
 *
 *  Created on: 20 oct. 2013
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/* strncpy()	*/
#include <time.h>		/* strftime()	*/

#include "ssDb.h"
#include "ssListe.h"

#define TAILLE_BUFFER	255

#define COUNTER_INT				"CREATE TABLE IF NOT EXISTS counter (cname char(%d), tm_name char(%d), value int, date datetime)"
#define	COUNTER_INS				"INSERT INTO counter VALUES ('%s', '%s', %d, '%s')"
#define	COUNTER_DROP			"SELECT value FROM counter WHERE cname = 'capture.kernel_drops' AND date >= '%s' AND date <= '%s'"
#define	COUNTER_PACKET			"SELECT value FROM counter WHERE cname = 'capture.kernel_packets' AND date >= '%s' AND date <= '%s'"
#define	COUNTER_THREAD			"SELECT tm_name FROM counter WHERE cname = 'capture.kernel_packets' AND date = '%s'"
#define	COUNTER_THREAD_DROP		"SELECT value FROM counter WHERE cname = 'capture.kernel_drops' AND tm_name = '%s' AND date >= '%s' AND date <= '%s'"
#define	COUNTER_THREAD_PACKET	"SELECT value FROM counter WHERE cname = 'capture.kernel_packets' AND tm_name = '%s' AND date >= '%s' AND date <= '%s'"

#define RUNTIME_INIT			"CREATE TABLE IF NOT EXISTS runtime (debut datetime, fin datetime, duree int)"
#define	RUNTIME_INS				"INSERT INTO runtime VALUES (datetime('%s', '-%d seconds'), '%s', %d)"
#define RUNTIME_REQ				"SELECT debut, fin, duree FROM runtime"

static int callback(void *liste, int argc, char **argv, char **azColName)
{
	char	*elt;

	int i;

	// Allouer un tableau pour stocker tous les resultats et l'ajouter à la liste des resultats
	elt = malloc(argc * TAILLE_BUFFER * sizeof(char));
	ssListAjouter((ssListe *) liste, (void *) elt);

	// Remplir les résultats
	for(i = 0; i < argc; i++){
		strncpy(elt, argv[i], TAILLE_BUFFER * sizeof(char));
		elt += TAILLE_BUFFER;
	}

	(void) azColName;

	return EXIT_SUCCESS;
}

/*
 * Exécuter une commande SQL
 */
void ssDbCommand(ssDb *db, char *command, ssListe *liste)
{
	char	*errMsg = 0;
	int		rc;

	// Executer la commande SQLite command
	rc = sqlite3_exec(db, command, callback, (void *) liste, &errMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_free(errMsg);
	}
}

/*
 * Creer la database
 */
ssDb *ssDbCreate(char *filename)
{
	ssDb	*db;

	char	sql[TAILLE_BUFFER];
	int 	rc;

	// Ouvrir le fichier SQLite
	rc = sqlite3_open(filename, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);

		return NULL;
	}

	// Creer la table counter
	sprintf(sql, COUNTER_INT, TAILLE_MOT, TAILLE_MOT);
	ssDbCommand(db, sql, (ssListe *) NULL);

	// Creer la table run
	sprintf(sql, RUNTIME_INIT);
	ssDbCommand(db, sql, (ssListe *) NULL);

	return db;
}

/*
 * Effacer la dataBase
 */
void ssDbDelete(char *filename)
{
	remove(filename);
}

/*
 * Ouvrir la database
 */
ssDb *ssDbOpen(char *filename)
{
	ssDb 	*db;

	int 	rc;

	// Ouvrir la database SQLite
	rc = sqlite3_open(filename, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);

		return NULL;
	}

	return db;
}

/*
 * Insérer un COUNTER dans la table counter de la DataBase
 */
void ssDbInsererCounter(ssDb *db, ssDbDump *dump, ssDbCounter *counter)
{
	char	date[TAILLE_BUFFER], sql[TAILLE_BUFFER];

	// Transformer la date et uptime en format datetime de SQL
	strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", &(dump->date));

	// Insere counterName dans la table counter de la datatbase db
	sprintf(sql, COUNTER_INS, counter->cname, counter->tm_name, counter->value, date);
	ssDbCommand(db, sql, (ssListe *) NULL);
}

/*
 * Insérer un RUNTIME (à partir du dernier dump) dans la table runtime de la DataBase
 */
void ssDbInsererRuntime(ssDb *db, ssDbDump *dump)
{
	char	date[TAILLE_BUFFER], sql[TAILLE_BUFFER];
	int		uptime;

	// Transformer la date en format datetime de SQL et uptime en secondes
	strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", &(dump->date));
	uptime = dump->days DAY + dump->hours HOUR + dump->min MIN + dump->sec SEC;

	// Inserer runtime dans la table runtime de la datatbase db
	sprintf(sql, RUNTIME_INS, date, uptime, date, uptime);

	ssDbCommand(db, sql, (ssListe *) NULL);
}

/*
 * Requete sur la base de donnee runtime
 */
void ssDbRequete(ssDb *db)
{
	struct counter_ {
		char	value[TAILLE_BUFFER];
	};
	struct counter_	*counter, *thread;

	struct runtime_ {
		char	debut[TAILLE_BUFFER];
		char	fin[TAILLE_BUFFER];
		char	uptime[TAILLE_BUFFER];
	};
	struct runtime_	*runtime;

	ssListe *counters, *runtimes, *threads;

	char			sql[TAILLE_BUFFER];
	long long int	drops, packets;

	// Creer la liste pour stocker les resultats et affecter à runtimes (liste de runtimes)
	runtimes = ssListeCreate();

	// Recherche dans la base de donnee
	sprintf(sql, RUNTIME_REQ);
	ssDbCommand(db, sql, runtimes);

	// Afficher les résultats
	while ((runtime = (struct runtime_ *) ssListeElever(runtimes)) != NULL) {
		fprintf(stdout, "Runtime Début <%s> Fin <%s> Durée <%s> secondes\n", runtime->debut, runtime->fin, runtime->uptime);

		// Creer la liste pour stocker les resultats counter drop
		counters = ssListeCreate();

		// Recherche les drops dans la base de donnee
		sprintf(sql, COUNTER_DROP, runtime->debut, runtime->fin);
		ssDbCommand(db, sql, counters);

		// Initialiser le compteur drops
		drops = 0;

		// Compter les drops
		while ((counter = (struct counter_ *) ssListeElever(counters)) != NULL) {
			drops += atoll(counter->value);
		}

		// Recherche les packets dans la base de donnee
		sprintf(sql, COUNTER_PACKET, runtime->debut, runtime->fin);
		ssDbCommand(db, sql, counters);

		// Initialiser le compteur packets
		packets = 0;

		// Compter les packets
		while ((counter = (struct counter_ *) ssListeElever(counters)) != NULL) {
			packets += atoll(counter->value);
		}

		fprintf(stdout, "Ratio drop/packet <%lf>\n", (double) drops/(double) packets);

		// Recherche les threads dans le dernier dump dans la base de donnee
		sprintf(sql, COUNTER_THREAD, runtime->fin);
		ssDbCommand(db, sql, counters);

		// Afficher les différents threads du runtime
		while ((counter = (struct counter_ *) ssListeElever(counters)) != NULL) {
			fprintf(stdout, "Thread <%s>\n", counter->value);

			// Creer la liste pour stocker les resultats counter drop
			threads = ssListeCreate();

			// Recherche les drops dans la base de donnee
			sprintf(sql, COUNTER_THREAD_DROP, counter->value, runtime->debut, runtime->fin);
			ssDbCommand(db, sql, threads);

			// Initialiser le compteur drops
			drops = 0;

			// Compter les drops
			while ((thread = (struct counter_ *) ssListeElever(threads)) != NULL) {
				drops += atoll(thread->value);
			}

			fprintf(stdout, "Moyenne de <%.2lf> drop par seconde\n", (double) drops/(double) atoll(runtime->uptime));

			// Recherche les packets dans la base de donnee
			sprintf(sql, COUNTER_THREAD_PACKET, counter->value, runtime->debut, runtime->fin);
			ssDbCommand(db, sql, threads);

			// Initialiser le compteur packets
			packets = 0;

			// Compter les packets
			while ((thread = (struct counter_ *) ssListeElever(threads)) != NULL) {
				packets += atoll(thread->value);
			}

			fprintf(stdout, "Moyenne de <%.2lf> paquets par seconde\n", (double) packets/(double) atoll(runtime->uptime));

			// Liberer la liste threads
			ssListeDelete(threads);
		}

		// Liberer la liste counters
		ssListeDelete(counters);
	}

	// Liberer la liste runtimes
	ssListeDelete(runtimes);
}

/*
 * Fermer la database
 */
void ssDbClose(ssDb *db)
{
	sqlite3_close(db);
}

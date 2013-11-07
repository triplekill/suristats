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

#define COUNTER_INT		"CREATE TABLE IF NOT EXISTS counter (cname char(%d), tm_name char(%d), value int, date datetime, days int, uptime datetime)"
#define	COUNTER_INS		"INSERT INTO counter VALUES ('%s', '%s', %d, '%s', %d, '%s')"
#define	COUNTER_DROP	"SELECT cname, value FROM counter WHERE cname = 'capture.kernel_drops' AND date = '%s'"

#define RUNTIME_INIT	"CREATE TABLE IF NOT EXISTS runtime (date datetime, days int, uptime datetime)"
#define	RUNTIME_INS		"INSERT INTO runtime VALUES ('%s', %d, '%s')"
#define RUNTIME_REQ		"SELECT date, days, uptime FROM runtime"

static ssListe *liste;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	char	*pt;

	int i;

	// Allouer un tableau pour stocker tous les resultats et l'ajouter à la liste des resultats
	pt = malloc(argc * TAILLE_BUFFER * sizeof(char));
	ssListAjouter(liste, (void *) pt);

	// Remplir les résultats
	for(i = 0; i < argc; i++){
		strncpy(pt, argv[i], TAILLE_BUFFER * sizeof(char));
		pt += TAILLE_BUFFER;
	}
/*
	for(i = 0; i < argc; i++){
		fprintf(stdout, "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
*/
	return EXIT_SUCCESS;
}

/*
 * Exécuter une commande SQL
 */
void ssDbCommand(ssDb *db, char *command)
{
	char	*errMsg = 0;
	int		rc;

	// Executer la commande SQLite command
	rc = sqlite3_exec(db, command, callback, 0, &errMsg);

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
	ssDbCommand(db, sql);

	// Creer la table run
	sprintf(sql, RUNTIME_INIT);
	ssDbCommand(db, sql);

	return db;
}

/*
 * Effacer la dataBase
 */
void ssDbDelete(void)
{

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
void ssDbInsererCounter(ssDb *db, ssDbRuntime *runtime, ssDbCounter *counter)
{
	char	date[TAILLE_BUFFER], sql[TAILLE_BUFFER], uptime[TAILLE_BUFFER];

	// Transformer la date et uptime en format datetime de SQL
	strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", &(runtime->date));
	sprintf(uptime, "%2.2d:%2.2d:%2.2d", runtime->hours, runtime->min, runtime->sec);

	// Insere counterName dans la table counter de la datatbase db
	sprintf(sql, COUNTER_INS, counter->cname, counter->tm_name, counter->value, date, runtime->days, uptime);
	ssDbCommand(db, sql);
}

/*
 * Insérer un RUNTIME dans la table runtime de la DataBase
 */
void ssDbInsererRuntime(ssDb *db, ssDbRuntime *runtime)
{
	char	date[TAILLE_BUFFER], sql[TAILLE_BUFFER], uptime[TAILLE_BUFFER];

	// Transformer la date et uptime en format datetime de SQL
	strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", &(runtime->date));
	sprintf(uptime, "%2.2d:%2.2d:%2.2d", runtime->hours, runtime->min, runtime->sec);

	// Insere runtime dans la table runtime de la datatbase db
	sprintf(sql, RUNTIME_INS, date, runtime->days, uptime);
	ssDbCommand(db, sql);
}

/*
 * Requete sur la base de donnee runtime
 */
void ssDbRequete(ssDb *db)
{
	struct runtime_ {
		char	date[TAILLE_BUFFER];
		char	days[TAILLE_BUFFER];
		char	uptime[TAILLE_BUFFER];
	};
	struct runtime_	*runtime;

	struct counter_ {
		char	cname[TAILLE_BUFFER];
		char	value[TAILLE_BUFFER];
	};
	struct counter_	*counter;

	ssListe *counters, *runtimes;

	char	sql[TAILLE_BUFFER];
	int		drop;

	// Creer la liste pour stocker les resultats et affecter à runtimes (liste de runtimes)
	liste = ssListeCreate();
	runtimes = liste;

	// Recherche dans la base de donnee
	sprintf(sql, RUNTIME_REQ);
	ssDbCommand(db, sql);

	// Afficher les résultats
	while ((runtime = (struct runtime_ *) ssListeElever(runtimes)) != NULL) {
		// Creer la liste pour stocker les resultats counter drop
		liste = ssListeCreate();
		counters = liste;

		// Recherche dans la base de donnee
		sprintf(sql, COUNTER_DROP, runtime->date);
		ssDbCommand(db, sql);

		// Initialiser le compteur drop
		drop = 0;

		// Compter les drops
		while ((counter = (struct counter_ *) ssListeElever(counters)) != NULL) {
			drop += atoi(counter->value);
		}

		// Liberer la liste runtimes
		ssListeDelete(counters);

		fprintf(stdout, "Runtime Date <%s> Uptime <%s> days <%s>\n", runtime->date, runtime->days, runtime->uptime);
		fprintf(stdout, "Total capture.kernel_drops <%d>\n", drop);
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

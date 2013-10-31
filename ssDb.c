/*
 * ssDb.c
 *
 *  Created on: 20 oct. 2013
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>

#include "ssDb.h"

#define SSDB_INIT1 "CREATE TABLE IF NOT EXISTS counter (title text, note text, changed datetime)"
#define SSDB_INIT2 "CREATE TABLE IF NOT EXISTS run (title text, note text, changed datetime)"
#define NOTEZ_DB_LIST_QUERY "SELECT id, strftime(\"%s\", changed) AS changed, title FROM notez ORDER BY id DESC"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;

	for(i=0; i<argc; i++){
		fprintf(stdout, "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");

	return EXIT_SUCCESS;
}

/*
 * Create Suricata Stats (SS) DataBase and log tables
 */
ssDb *ssDbCreate(char *filename)
{
	char	*errMsg = 0;
	int 	rc;

	ssDb *db;

	// Ouvrir le fichier SQLite
	if ((rc = sqlite3_open(filename, &db))) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);

		return NULL;
	}

	// Creer la table counter
	if ((rc = sqlite3_exec(db, SSDB_INIT1, callback, NULL, &errMsg)) != SQLITE_OK) {
		fprintf(stderr, "SQL Error: %s\n", errMsg);
		sqlite3_free(errMsg);
	}

	// Creer la table run
	if ((rc = sqlite3_exec(db, SSDB_INIT2, callback, NULL, &errMsg)) != SQLITE_OK) {
		fprintf(stderr, "SQL Error: %s\n", errMsg);
		sqlite3_free(errMsg);
	}

	return db;
}

/*
 * Delete Suricata Stats (SS) DataBase
 */
void ssDbDelete(void)
{

}

/*
 * Open Suricata Stats (SS) DataBase
 */
ssDb *ssDbOpen(char *filename)
{
	ssDb *db;

	int rc;

	// Ouvrir la database SQLite
	if ((rc = sqlite3_open(filename, &db))) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);

		return NULL;
	}

	return db;
}

/*
 * Suricata Stats (SS) command
 */
void ssDbCommand(ssDb *db, char *command)
{
	char	*ErrMsg = 0;
	int		rc;

	// Executer la commande SQLite command
	if ((rc = sqlite3_exec(db, command, callback, 0, &ErrMsg)) != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", ErrMsg);
		sqlite3_free(ErrMsg);
	}
}

/*
 *
 */
void ssDbClose(ssDb *db)
{
	sqlite3_close(db);
}

/*
 * ssDb.c
 *
 *  Created on: 20 oct. 2013
 *      Author: david
 */

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

//#include "ssDb.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;

	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");

	return EXIT_SUCCESS;
}

/*
 * Create Suricata Stats (SS) DataBase
 */
void ssDbCreate(char *filename)
{

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
void* ssDbOpen(char *filename)
{
	sqlite3 *db;

	int rc;

	rc = sqlite3_open(filename, &db);

	if(rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);

		return NULL;
	}

	return db;
}

/*
 * Suricata Stats (SS) command
 */
void ssDbCommand(void *ssDb, char *command)
{
	sqlite3 *db = (sqlite3 *) ssDb;

	char	*zErrMsg = 0;
	int		rc;

	rc = sqlite3_exec(db, command, callback, 0, &zErrMsg);

	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
}

/*
 *
 */
void ssDbClose(void *ssDb)
{
	sqlite3 *db = (sqlite3 *) ssDb;

	sqlite3_close(db);
}

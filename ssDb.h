/*
 * ssDb.h
 *
 *  Created on: 20 oct. 2013
 *      Author: david
 */
#ifndef SSDB_H_
#define SSDB_H_

#include <sqlite3.h>

typedef sqlite3	ssDb;

extern ssDb*	ssDbCreate(char *filename);
extern void 	ssDbDelete(void);
extern ssDb* 	ssDbOpen(char *filename);
extern void 	ssDbCommand(ssDb *db, char *command);
extern void 	ssDbClose(ssDb *db);

#endif /* SSDB_H_ */

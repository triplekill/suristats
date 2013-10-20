/*
 * ssDb.h
 *
 *  Created on: 20 oct. 2013
 *      Author: david
 */
#ifndef SSDB_H_
#define SSDB_H_

extern void ssDbCreate(char *filename);
extern void ssDbDelete(void);
extern void* ssDbOpen(char *filename);
extern void ssDbCommand(void *ssDb, char *command);
extern void ssDbClose(void *ssDb);

#endif /* SSDB_H_ */

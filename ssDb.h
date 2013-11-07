/*
 * ssDb.h
 *
 *  Created on: 20 oct. 2013
 *      Author: david
 */
#ifndef SSDB_H_
#define SSDB_H_

#include <sqlite3.h>
#include <time.h>		/* struct tm	*/

#define	TAILLE_MOT		25

typedef sqlite3	ssDb;

// Structure relative à counter
typedef struct ssDbCounter_ {
    char	cname[TAILLE_MOT];
    char 	tm_name[TAILLE_MOT];
    int		value;
} ssDbCounter;

// Structure relative à runtime
typedef struct ssDbRuntime_ {
	struct	tm	date;
    int 		sec;
    int			min;
    int			hours;
    int			days;
} ssDbRuntime;

extern ssDb*	ssDbCreate(char *filename);
extern void 	ssDbDelete(void);
extern ssDb* 	ssDbOpen(char *filename);
extern void 	ssDbClose(ssDb *db);
extern void		ssDbInsererCounter(ssDb *db, ssDbRuntime *runtime, ssDbCounter *counter);
extern void		ssDbInsererRuntime(ssDb *db, ssDbRuntime *runtime);
extern void		ssDbRequete(ssDb *db);


#endif /* SSDB_H_ */

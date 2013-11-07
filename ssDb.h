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

#define	SEC		* 1
#define	MIN		* 60 SEC
#define	HOUR	* 60 MIN
#define	DAY		* 24 HOUR

typedef sqlite3	ssDb;

// Structure relative à counter
typedef struct ssDbCounter_ {
	// Counter
    char	cname[TAILLE_MOT];
    // TM Name
    char 	tm_name[TAILLE_MOT];
    // Value
    int		value;
} ssDbCounter;

// Structure relative à Dump
typedef struct ssDbDump_ {
	// Date du dump
	struct	tm	date;

	// Durée en jours, heures, minutes et secondes du délai (ou uptime)
    int 		sec;
    int			min;
    int			hours;
    int			days;
} ssDbDump;

extern ssDb*	ssDbCreate(char *filename);
extern void		ssDbDelete(char *filename);
extern ssDb* 	ssDbOpen(char *filename);
extern void 	ssDbClose(ssDb *db);
extern void		ssDbInsererCounter(ssDb *db, ssDbDump *dump, ssDbCounter *counter);
extern void		ssDbInsererRuntime(ssDb *db, ssDbDump *dump);
extern void		ssDbRequete(ssDb *db);


#endif /* SSDB_H_ */

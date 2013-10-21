/*
 * suristats.c
 *
 *  Created on: 20 oct. 2013
 *      Author: david
 */
#include <stdio.h>
#include <stdlib.h>

#include "ssDb.h"
#include "ssParse.h"

int main(int argc, char **argv){
	ssDb	*db;

	if(argc != 3 ){
		fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
		return EXIT_FAILURE;
	}

	db = ssDbCreate(argv[1]);
	ssDbClose(db);

	if ((db = ssDbOpen(argv[1])) == NULL)
	{
		return EXIT_FAILURE;
	}

	ssDbCommand(db, argv[2]);

	ssDbClose(db);

	return EXIT_SUCCESS;
}

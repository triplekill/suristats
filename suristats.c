/*
 * suristats.c
 *
 *  Created on: 20 oct. 2013
 *      Author: david
 */
#include <stdio.h>
#include <stdlib.h>

#include "ssDb.h"

int main(int argc, char **argv){
	void	*ssDb;

	if(argc != 3 ){
		fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
		return EXIT_FAILURE;
	}

	if ((ssDb = ssDbOpen(argv[1])) == NULL)
	{
		return EXIT_FAILURE;
	}

	ssDbCommand(ssDb, argv[2]);

	ssDbClose(ssDb);

	return EXIT_SUCCESS;
}

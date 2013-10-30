/*
 * suristats.c
 *
 *  Created on: 20 oct. 2013
 *      Author: david
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>		/* getopt() */

#include "suristats.h"

#include "ssDb.h"
#include "ssParse.h"

#define USAGE	"Usage: %s [ch] DATABASE SQL-STATEMENT\n"

//extern	char	*optarg;
extern	int		optind;

int main(int argc, char **argv){
	ssDb	*db;

	int	createDb = FALSE;
	int	opt;

	while ((opt = getopt (argc, argv, "chf:")) != -1) {
		switch (opt) {
			case 'h':
				fprintf(stdout, USAGE, argv[0]);
				return EXIT_SUCCESS;
				break;
			case 'f':
//				optarg;
				break;
			case 'c':
				createDb = TRUE;
				break;
			default :
				return EXIT_FAILURE;
				break;
		}
	}

	if(argc < 3 ){
		fprintf(stderr, USAGE, argv[0]);
		return EXIT_FAILURE;
	}

	if (createDb)
	{
		db = ssDbCreate(argv[optind]);
		ssDbClose(db);
	}

	if ((db = ssDbOpen(argv[optind])) == NULL)
	{
		return EXIT_FAILURE;
	}

	ssDbCommand(db, argv[optind + 1]);

	ssDbClose(db);

	return EXIT_SUCCESS;
}

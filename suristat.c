/*
 * suristat.c
 *
 *  Created on: 20 oct. 2013
 *      Author: david
 */
#include <stdio.h>

int main(int argc, char **argv){
	int rc;

	if( argc!=3 ){
		fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
		return(1);
	}
}

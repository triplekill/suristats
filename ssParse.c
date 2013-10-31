/*
 * ssParse.c
 *
 *  Created on: 22 oct. 2013
 *      Author: david
 */

#include <errno.h>	/* errno	*/
#include <stdio.h>
#include <stdlib.h>

#include "ssParse.h"

#define BUFFER_LENGTH	255

void ssParseFile(char *name)
{
	FILE *file;

	char ligne[BUFFER_LENGTH];

	// Ouvrir le fichier log Suricata
	if ((file = fopen(name,"r")) == NULL)
	{
		perror("fopen()");
		exit(errno);
	}

	// Parcourir le fichier ligne par ligne
	while (fgets(ligne, sizeof(ligne), file) != NULL) {
		fprintf(stdout, "%s", ligne);
/*
	    fprintf(sc_perf_op_ctx->fp, "-------------------------------------------------------------------\n");
	    fprintf(sc_perf_op_ctx->fp, "Date: %d/%D/%04d -- %02d:%02d:%02d (uptime: %dd, %02dh %02dm %02ds)\n", tms->tm_mon + 1, tms->tm_mday, tms->tm_year + 1900, tms->tm_hour, tms->tm_min, tms->tm_sec, days, hours, min, sec);
	    fprintf(sc_perf_op_ctx->fp, "-------------------------------------------------------------------\n");
	    fprintf(sc_perf_op_ctx->fp, "%-25s | %-25s | %-s\n", "Counter", "TM Name","Value");
	    fprintf(sc_perf_op_ctx->fp, "-------------------------------------------------------------------\n");

case SC_PERF_TYPE_UINT64:
		fprintf(sc_perf_op_ctx->fp, "%-25s | %-25s | %-llu\n", pc->name->cname, pc->name->tm_name, ui64_temp);
case SC_PERF_TYPE_DOUBLE:
		fprintf(sc_perf_op_ctx->fp, "%-25s | %-25s | %-lf\n", pc->name->cname, pc->name->tm_name, double_temp);
case SC_PERF_TYPE_UINT64:
		fprintf(sc_perf_op_ctx->fp, "%-25s | %-25s | %-llu\n", pc->name->cname, pctmi->tm_name, ui64_result);
case SC_PERF_TYPE_DOUBLE:
		fprintf(sc_perf_op_ctx->fp, "%-25s | %-25s | %0.0lf\n",cpc->name->cname, pctmi->tm_name, double_result);
*/
	}

	// Fermer le fichier
	fclose(file);
}

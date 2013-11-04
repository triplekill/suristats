/*
 * ssListe.c
 *
 *  Created on: 4 nov. 2013
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>

#include "ssListe.h"

// Structure element
typedef struct ssElt_
{
	struct ssElt_	*next;

	void	*elt;
} ssElt;

ssListe* ssListeCreate(void)
{
	ssListe	*liste;

	// Allouer liste
	liste = malloc(sizeof(ssListe));

	// Initialiser pointeur de tete et de queue à NULL
	liste->head = NULL;
	liste->tail = NULL;

	return liste;
}

void ssListAjouter(ssListe *liste, void* elt)
{
	ssElt	*nouvelElt;

	nouvelElt = malloc(sizeof(ssElt));

	// Initialiser l'element
	nouvelElt->next	= NULL;
	nouvelElt->elt	= elt;

	if (liste->head == NULL) {
		// Liste vide, indiquer la tete de liste
		liste->head = nouvelElt;
	}
	else {
		// Dernier element pointe sur nouvel element
		((ssElt*) liste->tail)->next = nouvelElt;
	}
	// Nouvel element est le dernier element
	liste->tail	= nouvelElt;
}

void* ssListeElever(ssListe *liste)
{
	ssElt	*ancienElt = liste->head;
	void	*elt;

	if (liste->head == NULL) {
		return NULL;
	}

	// la nouvelle tete de liste est l'element suivant
	liste->head = ancienElt->next;

	// Extraire le contenu et liberer l'element
	elt = ancienElt->elt;
	free(ancienElt);

	return elt;
}

int ssListeDelete(ssListe *liste)
{
	if (liste->head != NULL) {
		return EXIT_FAILURE;
	}

	// Liberer la liste
	free(liste);

	return EXIT_SUCCESS;
}

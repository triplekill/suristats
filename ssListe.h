/*
 * ssListe.h
 *
 *  Created on: 4 nov. 2013
 *      Author: david
 */

#ifndef SSLISTE_H_
#define SSLISTE_H_

// Structure liste
typedef struct ssListe_ {
	void	*head;
	void	*tail;
} ssListe;

extern ssListe*	ssListeCreate(void);
extern void		ssListAjouter(ssListe *liste, void* elt);
extern void*	ssListeElever(ssListe *liste);
extern int		ssListeDelete(ssListe *liste);

#endif /* SSLISTE_H_ */

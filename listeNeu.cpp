/*
 * Autoren: Michel Benger, Haris Bojagic, Tim Horten, Bryan Lewicki
 * Gruppe : 3 Mittwochvormittag
 * Thema  : Miniprojekt 1 - Graphen aus Open Source Data der Deutschen Bahn erstellen
 *			Miniprojekt 2 - Abstand von zwei Bahnhöfen
 */

#include "listeNeu.h"
#include <iostream>

template <class T>
struct Element<T>* List_Search(ListeNeu<T> &L, T k)
{
    struct Element<T> *x = L.kopf;
    while(x != nullptr && x->schluessel != k)
        x = x->nachf;
    return x;
}

template <class T>
void List_Insert(ListeNeu<T> &L,struct Element<T> *x)
{
	x->nachf = L.kopf;
	if(L.kopf != nullptr)
		L.kopf->vorg = x;
	L.kopf = x;
	x->vorg = nullptr;
}

template <class T>
void List_Delete(ListeNeu<T> &L,struct Element<T> *x)
 {
	if(x->vorg != nullptr)
 		x->vorg->nachf = x->nachf;
 	else
		L.kopf = x->nachf;
 	if(x->nachf != nullptr)
        x->nachf->vorg = x->vorg;
}

template <class T>
void List_Print(ListeNeu<T> &L)
 {
	struct Element<T> *x = L.kopf;
    while(x != nullptr)
    {
        std::cout << x->schluessel << std::endl;
        x = x->nachf;
    }
}

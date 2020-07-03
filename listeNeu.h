#ifndef LISTE_NEU_H
#define LISTE_NEU_H

template <class T>
struct Element{
    T schluessel;                   //Hier wird ein Objekt des Typs T gespeichert
    struct Element *nachf;          //Nachfolgendes Element
    struct Element *vorg;           //Vorangegangenes Element
};

template <class T>
class ListeNeu{
public:
    struct Element<T> *kopf;        //Zeiger auf den Anfang der Liste
    ListeNeu(){kopf = nullptr;}     //Konstruktor
};

/**
 * @brief List_Search - Sucht ein Element in einer Liste und gibt dazu den Zeiger zurück
 * Kapitel 10.2 Durchsuchen einer verketteten Liste auf Seite 238
 * @param L Liste
 * @param k Element nach dem gesucht wird
 * @return Gefundenes Element oder nullptr
 */
template <class T>
struct Element<T>* List_Search(ListeNeu<T> &L, T k);

/**
 * @brief List_Insert - Fügt ein Element am Anfang der Liste ein
 * Kapitel 10.2 Einfügen in eine verkettete Liste auf Seite 238
 * @param L Liste
 * @param x Element welches eingefügt wird
 */
template <class T>
void List_Insert(ListeNeu<T> &L,struct Element<T> *x);

/**
 * @brief List_Insert - Entfernt ein Element aus der Liste
 * Kapitel 10.2 Entfernen aus einer verketteten Liste auf Seite 239
 * @param L Liste
 * @param x Zeiger auf das Element, welches entfernt wird
 */
template <class T>
void List_Delete(ListeNeu<T> &L,struct Element<T> *x);

/**
 * @brief List_Print - Gibt alle Elemente der Liste auf die Konsole aus
 * @param L Liste
 */
template <class T>
void List_Print(ListeNeu<T> &L);

#endif

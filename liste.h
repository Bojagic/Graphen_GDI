/*
name:         liste.h
author:       Tim Horten
date:         29.05.2020
description:  Aufgabenblatt 6:  Template Liste
*/

#ifndef LISTE_H
#define LISTE_H

template <class TE>
struct element{
  TE wert;
  struct element *prev;
  struct element *next;
};

template <class T>
class Liste{
  private:
    struct element<T> *head; // Zeiger auf den Anfang
    struct element<T> *tail; // Zeiger auf das Ende

  public:
    Liste();    // Konstruktor
    ~Liste();   // Destruktor

    void clear();             // Liste leeren
    bool contains(T wert);    // Prüfen, ob der Wert in der Liste vorkommt
    void ausgabe();           //Alle Elemente der Liste ausgeben

    void add_last(T wert);    // Neues Element am Ende anfügen
    void add_first(T wert);   // Neues Element am Anfang einfügen
    T remove_last();          // Letzten Wert entfernen und zurückgeben
    T remove_first();         // Ersten Wert entfernen und zurückgeben
    T remove(size_t index);   // Element an bestimmter Stelle löschen

    T get(size_t index);      // Element an der Stelle index zurückgeben
    T operator[](size_t index);   //Alternative Weise get zu benutzen

    size_t number_Elements();     //Gibt die Anzahl der Elemente zurück
};


template <class T>
Liste<T>::Liste(){      //Konstruktor
  head = nullptr;       //nullptr anstelle von NULL
  tail = nullptr;
}

template <class T>
Liste<T>::~Liste(){    // Destruktor
  clear();
}

template <class T>
void Liste<T>::clear(){        //Liste leeren (vom Anfang zum ende)

  struct element<T> *temp = head;
  struct element<T> *save;
  while (temp != nullptr){      //Solange noch Element vorhanden sind
    save = temp;                //Vorderstes Element merken
    temp = temp->next;          //Zum nächsten Element gehen
    delete save;                //Vorderstes Element löschen
  }
  head = tail = nullptr;
}

template <class T>
bool Liste<T>::contains(T wert){   // Prüfen, ob der Wert in der Liste vorkommt

  struct element<T> *temp = head;

  while (temp != nullptr){
    if(temp->wert == wert)
      return true;
    temp = temp->next;
  }
  return false;
}

template <class T>
void Liste<T>::ausgabe(){        //Alle Elemente der Liste ausgeben

  struct element<T> *temp = head;                   //temp läuft durch die Liste

  while(temp != nullptr){                   //Bis beim ende angekommen
    std::cout << temp->wert << std::endl;   //Element ausgeben
    temp = temp->next;                      //Zum nächsten Element
  }
}

template <class T>
void Liste<T>::add_last(T wert){    // Neues Element am Ende anfügen  (Blatt3 add)

  if(head == nullptr){        //Liste leer?
    head = new struct element<T>;     //neues Element an den Anfang
    head->prev = nullptr;
    tail = head;              //einziges Element ist Anfang und Ende
  }
  else{
    struct element<T> *save = tail;   //Voriges Ende der Liste in save zwischen speichern

    tail = new struct element<T>;     //Neues Element am Ende anlegen
    tail->prev = save;        //Neues Element zeigt auf älteres Element
    save->next = tail;        //älteres Element zeigt auf neues Element
  }
  tail->wert = wert;          //Wert ans Ende speichern
  tail->next = nullptr;
}

template <class T>
void Liste<T>::add_first(T wert){   // Neues Element am Anfang einfügen

  if(head == nullptr){        //Liste leer?
    head = new struct element<T>;     //neues Element an den Anfang
    head->prev = nullptr;
    tail = head;              //einziges Element ist Anfang und Ende
  }
  else{
    struct element<T> *save = head;   //Vorigen Anfang der Liste in save zwischen speichern

    head = new struct element<T>;     //Neues Element am Anfang anlegen
    head->next = save;        //Neues Element zeigt auf älteres Element
    save->prev = head;        //älteres Element zeigt auf neues Element
  }
  head->wert = wert;          //Wert an den Anfang speichern
  head->prev = nullptr;
}

template <class T>
T Liste<T>::remove_last(){          // Letzten Wert entfernen und zurückgeben

if(head != nullptr){    //Liste nicht leer?
    T wert = tail -> wert;    //Letzten Wert abspeichern
    if(head != tail){             //Wenn head=tail -> ein element
      tail = tail->prev;          //Ende nach vorne ziehen
      delete tail->next;          //Altes tail löschen
      tail->next = nullptr;       //Letztes Element zeigt auf kein nächstes
    }
    else{
      delete head;                //einziges Element löschen
	  head = tail = nullptr;        //leere Liste: head und tail zeigen auf nullptr
	}
    return wert;                //Letzten Wert zurück geben
  }
  else
    throw "Exception: Liste ist leer!\n";
}

template <class T>
T Liste<T>::remove_first(){         // Ersten Wert entfernen und zurückgeben

  if(head != nullptr){          //Liste nicht leer?
    T wert = head -> wert;       //Ersten Wert abspeichern
    if(head != tail){             //Wenn head=tail -> ein element
      head = head->next;          //Anfang nach hinten schieben
      delete head->prev;          //Altes head löschen
      head->prev = nullptr;       //Erstes Element zeigt auf kein vorheriges
    }
    else{
      delete head;                //einziges Element löschen
	  head = tail = nullptr;        //leere Liste: head und tail zeigen auf nullptr
	}

    return wert;                //Ersten Wert zurück geben
  }
  else
    throw "Exception: Liste ist leer!\n";
}

template <class T>
T Liste<T>::remove(size_t index){
  struct element<T> *temp = head;
  struct element<T> *tempPrev;
  struct element<T> *tempNext;
  size_t i;

  while(i<index && temp != nullptr){
    temp = temp->next;
    i++;
  }

  if(temp != nullptr)       //temp nicht über tail gelaufen?
  {
    tempPrev = temp->prev;
    tempNext = temp->next;

    tempPrev->next = temp->next;
    tempNext->prev = temp->prev;

    delete temp;
    return temp->wert;
  }

  else
    throw "Exception: Index nicht vorhanden!\n";
}

template <class T>
T Liste<T>::get(size_t index){      // Element an der Stelle index zurückgeben
  static element<T> *lastTemp = head;
  static size_t lastIndex = 0;
  struct element<T> *temp;
  size_t i;

  if(index >= lastIndex){
    temp = lastTemp;
	i = lastIndex;
  }
  else
  {
	temp = head;
	i = 0;
  }

  while(i<index && temp != nullptr){  //Solange nicht beim gesuchten Index und nicht vorbei am Ende der Liste
    temp = temp->next;                //Zum nächsten Element
    i++;
  }

  lastTemp = temp;
  lastIndex = index;

  if(temp != nullptr)       //temp nicht über tail gelaufen?
    return temp->wert;      //Wert des i-ten Elements zurück geben

  else
    throw "Exception: Index nicht vorhanden!\n";
}

template <class T>
T Liste<T>::operator[](size_t index){    //Alternative Weise get zu benutzen
  return get(index);
}

template <class T>
size_t Liste<T>::number_Elements(){
  if(head == nullptr)
    return 0;
  else
  {
    struct element<T> *temp = head;
    size_t i=1;                         //Erstes Element ist bereits head/temp
    while(temp->next != nullptr){
      temp = temp->next;
      i++;
    }
    return i;
  }
}


#endif

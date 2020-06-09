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
    bool contains(T wert);    // Pr�fen, ob der Wert in der Liste vorkommt
    void ausgabe();           //Alle Elemente der Liste ausgeben

    void add_last(T wert);    // Neues Element am Ende anf�gen
    void add_first(T wert);   // Neues Element am Anfang einf�gen
    T remove_last();          // Letzten Wert entfernen und zur�ckgeben
    T remove_first();         // Ersten Wert entfernen und zur�ckgeben
    T get(size_t index);      // Element an der Stelle index zur�ckgeben

    T operator[](size_t index);   //Alternative Weise get zu benutzen
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
    temp = temp->next;          //Zum n�chsten Element gehen
    delete save;                //Vorderstes Element l�schen
  }
  head = tail = nullptr;
}

template <class T>
bool Liste<T>::contains(T wert){   // Pr�fen, ob der Wert in der Liste vorkommt

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

  struct element<T> *temp = head;                   //temp l�uft durch die Liste

  while(temp != nullptr){                   //Bis beim ende angekommen
    std::cout << temp->wert << std::endl;   //Element ausgeben
    temp = temp->next;                      //Zum n�chsten Element
  }
}

template <class T>
void Liste<T>::add_last(T wert){    // Neues Element am Ende anf�gen  (Blatt3 add)

  if(head == nullptr){        //Liste leer?
    head = new struct element<T>;     //neues Element an den Anfang
    head->prev = nullptr;
    tail = head;              //einziges Element ist Anfang und Ende
  }
  else{
    struct element<T> *save = tail;   //Voriges Ende der Liste in save zwischen speichern

    tail = new struct element<T>;     //Neues Element am Ende anlegen
    tail->prev = save;        //Neues Element zeigt auf �lteres Element
    save->next = tail;        //�lteres Element zeigt auf neues Element
  }
  tail->wert = wert;          //Wert ans Ende speichern
  tail->next = nullptr;
}

template <class T>
void Liste<T>::add_first(T wert){   // Neues Element am Anfang einf�gen

  if(head == nullptr){        //Liste leer?
    head = new struct element<T>;     //neues Element an den Anfang
    head->prev = nullptr;
    tail = head;              //einziges Element ist Anfang und Ende
  }
  else{
    struct element<T> *save = head;   //Vorigen Anfang der Liste in save zwischen speichern

    head = new struct element<T>;     //Neues Element am Anfang anlegen
    head->next = save;        //Neues Element zeigt auf �lteres Element
    save->prev = head;        //�lteres Element zeigt auf neues Element
  }
  head->wert = wert;          //Wert an den Anfang speichern
  head->prev = nullptr;
}

template <class T>
T Liste<T>::remove_last(){          // Letzten Wert entfernen und zur�ckgeben

if(head != nullptr){    //Liste nicht leer?
    T wert = tail -> wert;    //Letzten Wert abspeichern
    if(head != tail){             //Wenn head=tail -> ein element
      tail = tail->prev;          //Ende nach vorne ziehen
      delete tail->next;          //Altes tail l�schen
      tail->next = nullptr;       //Letztes Element zeigt auf kein n�chstes
    }
    else{
      delete head;                //einziges Element l�schen
	  head = tail = nullptr;        //leere Liste: head und tail zeigen auf nullptr
	}
    return wert;                //Letzten Wert zur�ck geben
  }
  else
    throw "Exception: Liste ist leer!\n";
}

template <class T>
T Liste<T>::remove_first(){         // Ersten Wert entfernen und zur�ckgeben

  if(head != nullptr){          //Liste nicht leer?
    T wert = head -> wert;       //Ersten Wert abspeichern
    if(head != tail){             //Wenn head=tail -> ein element
      head = head->next;          //Anfang nach hinten schieben
      delete head->prev;          //Altes head l�schen
      head->prev = nullptr;       //Erstes Element zeigt auf kein vorheriges
    }
    else{
      delete head;                //einziges Element l�schen
	  head = tail = nullptr;        //leere Liste: head und tail zeigen auf nullptr
	}

    return wert;                //Ersten Wert zur�ck geben
  }
  else
    throw "Exception: Liste ist leer!\n";
}

template <class T>
T Liste<T>::get(size_t index){      // Element an der Stelle index zur�ckgeben

  struct element<T> *temp = head;
  size_t i = 0;
  while(i<index && temp != nullptr){  //Solange nicht beim gesuchten Index und nicht vorbei am Ende der Liste
    temp = temp->next;                //Zum n�chsten Element
    i++;
  }

  if(temp != nullptr)       //temp nicht �ber tail gelaufen?
    return temp->wert;      //Wert des i-ten Elements zur�ck geben

  else
    throw "Exception: Index nicht vorhanden!\n";
}

template <class T>
T Liste<T>::operator[](size_t index){    //Alternative Weise get zu benutzen
  return get(index);
}


#endif

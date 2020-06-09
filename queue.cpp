#include "queue.h"
#include <iostream>

/**
 * @brief ENQUEUE - INSERT Operation einer Warteschlange
 * @param Q  Warteschlange
 * @param x  Wert, der in die Warteschlange eingefügt werden soll
 */
void ENQUEUE(Queue &Q, int x){
  Q.Q[Q.ende] = x;
  if(Q.ende == Q.laenge)
    Q.ende = 1;
  else Q.ende = Q.ende + 1;

  ++Q.anzahl;
}

/**
 * @brief DEQEUE - DELETE Operation einer Warteschlange
 * @param Q
 * @return Wert, der aus der Warteschlange herausgenommen wurde
 */
int DEQUEUE(Queue &Q){
  int x = Q.Q[Q.kopf];
  if(Q.kopf == Q.laenge)
    Q.kopf = 1;
  else Q.kopf = Q.kopf + 1;

  --Q.anzahl;
  return x;
}

/**
 * @brief Queue_Empty Gibt zurück, ob die Queue leer ist
 * @param Q
 * @return TRUE / FALSE
 */
int Queue_Empty(Queue &Q){
  if(Q.anzahl == 0)
    return 1;
  else
    return 0;
}

/**
 * @brief Queue_NotEmpty Gibt zurück, ob die Queue nicht leer ist
 * @param Q
 * @return TRUE / FALSE
 */
int Queue_NotEmpty(Queue &Q){
  if(Q.anzahl == 0)
    return 0;
  else
    return 1;
}

/**
 * @brief Queue_Debug Debug Ausgabe der Queue
 * @param Q
 */
void Queue_Debug(Queue &Q){
  std::cout << "|";
  int temp = Q.kopf;
  while(temp != Q.ende){
    std::cout << Q.Q[temp] << "|";
    if(temp == Q.laenge)
      temp = 1;
    else temp = temp + 1;
  }
  std::cout << std::endl;
}

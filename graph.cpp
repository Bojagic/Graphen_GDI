#include "graph.h"
#include "knoten.h"
#include "queue.h"
#include "queue.cpp"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>


void BFS(Graph &G, int s)
{
    int u, v;
    for(u=1; u<G.knotenzahl; u++)    //Für alle Knoten
    {
        G.V[u].farbe = WEISS;
        G.V[u].d = UNENDLICH;
        G.V[u].vorgaenger = NIL;
    }
    G.V[s].farbe = GRAU;            //Startknoten anpassen
    G.V[s].d = 0;
    G.V[s].vorgaenger = NIL;
    Queue Q;
    ENQUEUE(Q, s);
    while(Queue_NotEmpty(Q))
    {
        u = DEQUEUE(Q);
        for(v=1; v<G.knotenzahl; v++)
            if(G.Adj[v][u] == 1)
                if(G.V[v].farbe == WEISS)
                {
                    G.V[v].farbe = GRAU;
                    G.V[v].d = G.V[u].d +1;
                    G.V[v].vorgaenger = u;
                    ENQUEUE(Q,v);
                }
        G.V[u].farbe = SCHWARZ;
    }
}

void PRINT_PATH(Graph &G, int s, int v)
{
    if(v == s)
        cout << s;
    else if(G.V[v].vorgaenger == NIL)
        cout << "es gibt keinen Pfad von " << s << " nach " << v << endl;
    else{
        PRINT_PATH(G, s, G.V[v].vorgaenger);    //Rekursiver Aufruf von v bis zum Startpunkt
        cout << " -> " << v;
    }
}

void SaveGraph(ostream &os, Graph &G)
{

}

int LoadGraph(istream &is, Graph &G)
{
    int error =0;
    char c;
    int intvalue,vertex,edge;
    string tempstring;

    // Erste Zeile: G (Knotenzahl)
    c=is.get();
    if (c!='G') error=10;
    if (error==0){
        is>>intvalue;
        if (is.bad()) error=21;
        else if (intvalue<1) error=22;
        else if (intvalue>MAX_KNOTEN) error=23;
        else G.knotenzahl=intvalue;
    }
    while (error==0 && c!=EOF)
    {
        c=is.get();
        switch (c){
        case 'V':
            is>>intvalue;
            if (is.bad()) error=31;
            if (error==0)
            {
                c=is.get();
                getline(is,tempstring);
                if (intvalue>0 && intvalue<=MAX_KNOTEN)
                {
                    vertex=intvalue;
                    size_t firstQuote=tempstring.find_first_of("\"",0);
                    size_t lastQuote=tempstring.find_last_of("\"",tempstring.size());
                    if ((firstQuote!=string::npos) && (lastQuote!=string::npos))
                    {
                        tempstring=tempstring.substr(firstQuote+1,lastQuote-1);
                    }
                    G.V[vertex].name=tempstring;
                }
            }
            break;
        case 'E':
            is>>intvalue;
            if (is.bad()) error=41;
            if (error==0){
                vertex=intvalue;
                is>>intvalue;
                if (is.bad()) error=42;
            }
            if (error==0){
                edge=intvalue;
                is>>intvalue;
                if (is.bad()) error=43;
            }
            if (error==0){
                G.Adj[vertex][edge]=intvalue;
            }
            break;
        case ' ': break;
        case '\n': break;
        case '\r': break;
        case '\t': break;
        }
    }
    return error;
}

void Insert_Edge(Graph &G,int u, int v)
{
    if(u != v)
    {
        G.Adj[u][v] = 1;
        G.Adj[v][u] = 1;
    }
}

void Delete_Edge(Graph &G,int u, int v)
{
    G.Adj[u][v] = 0;
    G.Adj[v][u] = 0;
}

void Flip_Edge(Graph &G,int u, int v)
{
    if(G.Adj[u][v] == 1)
        Delete_Edge(G, u, v);
    else
        Insert_Edge(G, u, v);
}

void Graph_Debug(Graph &G)
{
    cout << "   |";
    for(int i=1;i<=G.knotenzahl;i++)
        cout << setw(3) << i;
    cout << endl;
    cout << "---|";
    for(int i=1;i<=G.knotenzahl;i++)
        cout << "---";
    cout << endl;
    for(int i=1;i<=G.knotenzahl;i++)
    {
        cout << setw(3) << i << "|";
        for(int j=1;j<=G.knotenzahl;j++)
            cout << setw(3) << G.Adj[i][j];
        cout << endl;
    }
}

void Insert_Random_Edge(Graph &G)
{
    srand(time(NULL));

    int counter = 0;            //Anzahl fehlender Kanten zählen
    int u, v;
    for(u=1; u<=G.knotenzahl-1; u++)
        for(v= u+1; v<=G.knotenzahl; v++)
            if(G.Adj[u][v] == 0)
                counter++;

    if(counter > 0){            //Mindestens eine freie Stelle gefunden?
        int edge = (rand() % counter) +1;   //einen der fehlenden Kanten aussuchen
        //cout << counter << "!!!" << edge << "!!!" << endl;

        u = v = 1;                                //zur ausgesuchten Stelle gehen
        int i=0;

        while(i<edge)
        {
                v++;
                if(v == G.knotenzahl+1)
                {
                    u++;
                    v=u+1;
                }

                if(G.Adj[u][v] == 0)
                    i++;
        }
        Insert_Edge(G, u, v);                 //Kante einfügen

    }
}

void Insert_Random_Edges(Graph &G,int i)
{
    for(int j=0; j<i; j++)
        Insert_Random_Edge(G);
}

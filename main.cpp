/**
a) Nutzen der Header-Dateien ohne Nutzung einer Entwicklungsumgebung mit Projektverwaltung
b) Beispiel zum Aufruf der Funktionen LoadGraph und SaveGraph
*/
// Header-Dateien einbinden
#include "queue.h"
#include "knoten.h"
#include "graph.h"
#include "load_db.h"
#include <fstream>
#include <string>
#include <iostream>

// Eventuelle ausgelagerte Implementationen einbinden
//#include "queue.cpp"
//#include "graph.cpp"
//#include "graph1.cpp"

void testGraphenbearbeitung();      //F�r Lern/Lehreinheit

int main(int argc, char *argv[])
{
/*
	//....
	Graph G;
	string sFileName="ring12.gdi";
	{
		ifstream gdi_stream;
		gdi_stream.open(sFileName,ifstream::in);
		if (gdi_stream.is_open())
		{
			if (LoadGraph(gdi_stream,G)==0)
			{
				Graph_Debug(G);
			}
		}
		gdi_stream.close();
	}
	{
		ofstream gdi_stream;
		gdi_stream.open(sFileName,ofstream::out);
		if (gdi_stream.is_open())
		{
			SaveGraph(gdi_stream,G);
		}
		gdi_stream.close();
	}
*/
    //testGraphenbearbeitung();

    bahn_netz bn;
    string sFileName0="wichtige zeilen.json";
    string sFileName="db_streckennetz.json";
	{
		ifstream gdi_stream;
		gdi_stream.open(sFileName,ifstream::in);
		if (gdi_stream.is_open())
		{
			Load_DB(gdi_stream, bn);

			bn.node.ausgabe();
		}
		gdi_stream.close();
	}

	return 0;
}

void testGraphenbearbeitung()
{
    Graph G;
	string sFileName="ring12.gdi";
	{
		ifstream gdi_stream;
		gdi_stream.open(sFileName,ifstream::in);
		if (gdi_stream.is_open())
		{
			if (LoadGraph(gdi_stream,G)==0)
			{
				Graph_Debug(G);                 //Gelesener Graph wird ausgegeben
				cout << "\n\n";

				Insert_Edge(G, 6, 2);            //Kante zwischen Knoten 6 und 2
				Graph_Debug(G);
				cout << "\n\n";

				Delete_Edge(G, 2, 1);
				Graph_Debug(G);
				cout << "\n\n";

                //Insert_Random_Edges(G,33);      //3 zuf�llige Kanten einf�gen
                //Graph_Debug(G);
				//cout << "\n\n";
			}
		}
		gdi_stream.close();
	}
}
/**
a) Nutzen der Header-Dateien ohne Nutzung einer Entwicklungsumgebung mit Projektverwaltung
b) Beispiel zum Aufruf der Funktionen LoadGraph und SaveGraph
*/
// Header-Dateien einbinden
#include "queue.h"
#include "knoten.h"
#include "graph.h"
#include "bahn_netz.h"
#include <fstream>
#include <string>
#include <iostream>
#include "listeNeu.h"

// Eventuelle ausgelagerte Implementationen einbinden
//#include "queue.cpp"
//#include "graph.cpp"
//#include "bahn_netz.cpp"
#include "listeNeu.cpp"

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
				BFS(G, 12);
                PRINT_PATH(G, 12, 11);
                //Insert_Random_Edges(G, 55);
                //Graph_Debug(G);
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

    bahn_netz bn;
    string sFileName0="wichtige zeilen.json";
    string sFileName="db_streckennetz.json";
    string saveFile="Test.gdi";
    string kurzdatei="Iserlohn-Holzwickede.json";
	{
		ifstream gdi_stream;
		gdi_stream.open(kurzdatei,ifstream::in);
		if (gdi_stream.is_open())
		{
			Load_DB(gdi_stream, bn);

            //List_Print(bn.node);
            List_Print(bn.station);
		}
		gdi_stream.close();
	}
	{
		ofstream gdi_stream;
		gdi_stream.open(saveFile,ofstream::out);
		if (gdi_stream.is_open())
		{
			//Save_DBneu(gdi_stream, bn);
		}
		gdi_stream.close();
	}
	return 0;
}

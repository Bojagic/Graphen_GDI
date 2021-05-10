/*
 * Autoren: Michel Benger, Haris Bojagic, Tim Horten, Bryan Lewicki
 * Gruppe : 3 Mittwochvormittag
 * Thema  : Miniprojekt 1 - Graphen aus Open Source Data der Deutschen Bahn erstellen
 *			Miniprojekt 2 - Abstand von zwei Bahnhöfen
 */

// Header-Dateien einbinden
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "bahn_netz.h"
#include "listeNeu.h"

// Eventuelle ausgelagerte Implementationen einbinden
//#include "bahn_netz.cpp"
#include "listeNeu.cpp"

//Miniprojekt 1
void bahndaten_laden(bahn_netz &bn);
void graph_erzeugen(bahn_netz &bn);

//Miniprojekt 2
void station_entfernung(bahn_netz &bn);

int main()
{
    bahn_netz bn;

	char menuWahl;

	do{
		system("cls");
		cout << "+-------------------------------------------------------+" << endl;
		cout << "|Graph erzeugen aus Open Source Daten der Deutschen Bahn|" << endl;
		cout << "+-------------------------------------------------------+" << endl;
		cout << "|<1> Bahndaten aus Datei laden                          |" << endl;
		cout << "|<2> Graph erzeugen                                     |" << endl;
		cout << "|<3> Entfernung zweier Bahnhoefe berechnen              |" << endl;
		cout << "|<0> Programm beenden                                   |" << endl;
		cout << "+-------------------------------------------------------+" << endl;
        menuWahl = getch();
		switch(menuWahl)
		{
			case '1': bahndaten_laden(bn); 		            break;
			case '2': graph_erzeugen(bn); 		            break;
			case '3': station_entfernung(bn); 				break;
			case '0': 										break;
			default : cout << "Falsche Eingabe" << endl; 	break;
		}
		cout << ".....";
		getch();
	}while(menuWahl != '0');

	cout << "Programm beendet" << endl;

	return 0;
}

void bahndaten_laden(bahn_netz &bn)
{
	char menuWahl;
	string datei;

	system("cls");
	cout << "+-------------------------------------------------------+" << endl;
	cout << "|Bahndaten aus Datei laden | Welche Daten laden?        |" << endl;
	cout << "+-------------------------------------------------------+" << endl;
	cout << "|<1> Gesamte Daten                                      |" << endl;
	cout << "|<2> Iserlohn-Holzwickede                               |" << endl;
	cout << "|<3> Iserlohn Umgebung                                  |" << endl;
	cout << "|<0> Zurueck                                            |" << endl;
	cout << "+-------------------------------------------------------+" << endl;
    menuWahl = getch();

	switch(menuWahl)
	{
		case '1': datei = "db_streckennetz.json"; 				break;
		case '2': datei = "Iserlohn-Holzwickede.json"; 			break;
		case '3': datei = "Kleines Bahnnetz.json";              break;
		case '0': return;										break;
		default : cout << "Falsche Eingabe" << endl; return;	break;
	}

	ifstream gdi_stream;
	gdi_stream.open(datei,ifstream::in);
	if (gdi_stream.is_open())
	{
		Load_DB(gdi_stream, bn);

		//List_Print(bn.node);
		//List_Print(bn.station);
	}
	else
		cout << "Fehler! Datei fehlt" << endl;

	gdi_stream.close();
}

void graph_erzeugen(bahn_netz &bn)
{
	if(bn.node.kopf == nullptr)
	{
		cout << "Fehler! Es wurden keine Daten eingelesen" << endl;
		return;
	}

	string datei;

	system("cls");
	cout << "+-------------------------------------------------------+" << endl;
	cout << "|Graph erzeugen | Dateipfad angeben                     |" << endl;
	cout << "+-------------------------------------------------------+" << endl;
	cout << ">> ";
	cin >> datei;

	ofstream gdi_stream;
	gdi_stream.open(datei,ofstream::out);
	if (gdi_stream.is_open())
	{
	    cout << "Speichere Graph ...";
		Save_DB(gdi_stream, bn);
		//Save_DBWithNNodes(gdi_stream, bn, 30);                                    //<----- Hier für IT-Projektmanagment
		cout << "fertig;" << endl;
	}
	else
		cout << "Fehler! Datei konnte nicht erstellt werden" << endl;
	gdi_stream.close();
}

void station_entfernung(bahn_netz &bn)		//Auswahl von zwei Stations, Stations werden dynamisch aus bn geholt
{
    if(bn.station.kopf == nullptr)
	{
		cout << "Fehler! Es wurden keine Daten eingelesen" << endl;
		return;
	}

	struct Element<Station> *currStation = bn.station.kopf;
	struct Element<Station> *stationA, *stationB;
	string eingA, eingB;

    system("cls");
	cout << "+-------------------------------------------------------+" << endl;
	cout << "|Abstand von zwei Bahnhoefen | Waehlen Sie aus der Liste|" << endl;
	cout << "+-------------------------------------------------------+" << endl;

	currStation = bn.station.kopf;
	while(currStation != nullptr)														//Alle Bahnhöfe mit code und text ausgeben
	{
		cout << "|" << left << setw(5) <<currStation->schluessel.code << " | "<< left << setw(47) << currStation->schluessel.text << "|" <<endl;
		currStation = currStation->nachf;
	}
	cout << "+-------------------------------------------------------+" << endl;

	cout << ">> ";																//Erster Bahnhof eingeben
	cin >> eingA;
	stationA = bn.station.kopf;
	while(stationA != nullptr)
    {
        if(stationA->schluessel.code == eingA || stationA->schluessel.text == eingA)		//Stimmt code oder text mit Eingabe überein?
			break;
        stationA = stationA->nachf;
    }

	if(stationA == nullptr)
	{
		cout << "Fehler! Falsche Eingabe" << endl;;
		return;
	}

	cout << ">> ";																//Zweiter Bahnhof eingeben
	cin >> eingB;
	stationB = bn.station.kopf;
	while(stationB != nullptr)
    {
        if(stationB->schluessel.code == eingB || stationB->schluessel.text == eingB)
			break;
        stationB = stationB->nachf;
    }

	if(stationB == nullptr)
	{
		cout << "Fehler! Falsche Eingabe" << endl;
		return;
	}

	cout << "Der Anstand der Bahnhöfe " << stationA->schluessel.text << " und " <<  stationB->schluessel.text << " ist "
         << abstandStationen(stationA->schluessel, stationB->schluessel) << " Kilometer." << endl;
}


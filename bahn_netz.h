/*
 * Autoren: Michél Benger, Haris Bojagic, Tim Horten, Bryan Lewicki
 * Gruppe : 3 Mittwochvormittag
 * Thema  : Miniprojekt 1 - Graphen aus Open Source Data der Deutschen Bahn erstellen
 *			Miniprojekt 2 - Abstand von zwei Bahnhöfen
 */

#ifndef LOAD_DB_H
#define LOAD_DB_H

#include <ostream>
#include <iostream>

#include "listeNeu.h"

using namespace std;

class RailwayNode       //StationNodes einlesen verbessern  Horten
{
public:
    int nummer;
    double breitengrad;   //latitude
    double hoehengrad;    //longitude
    int spokeEnd[2];      //link
    int spokeStart[2];    //link
    string text;          //Nur bei StationNodes
    string typ;           //Nur bei Nodes

    friend ostream &operator<< (ostream &ostr, const RailwayNode node);     //für Liste::ausgabe Testausgabe RailwayNode
};

class Station
{
public:
    string code;
    ListeNeu<int> spokeEnd;
    ListeNeu<int> spokeStart;
//Erweiterungen für Miniprojekt 2 -> mergeStationNodes angepasst
    double breitengrad;     //Vom ersten SNode (Jeder SNode hat seine eigenen genauen Koordinaten, aber Genauigkeit soll 1 km sein)
    double hoehengrad;
    string text;

    friend ostream &operator<< (ostream &ostr, Station station);
};

class RailwayStationCode{        //Benger;
public:
    int nummer;
    int SNodeNummer;
    string code;

    friend ostream &operator<< (ostream &ostr, const RailwayStationCode code);   //für Liste::ausgabe Testausgabe RailwayStationCode
};

class RailwayLink{
public:
    int nummer;

    int startNodeNummer;
    int endNodeNummer;

    friend ostream &operator<< (ostream &ostr, const RailwayLink link);     //für Liste::ausgabe Testausgabe RailwayLink
};

class RailwayLine{  //Nicht notwendig?
public:
    int nummer;
    int linkNummer[420];
    string text;
    int code;

    friend ostream &operator<< (ostream &ostr, const RailwayLine line);     //für Liste::ausgabe Testausgabe RailwayLine
};


class bahn_netz{
public:
    ListeNeu<RailwayNode> node;
    ListeNeu<RailwayNode> stationNode;
    ListeNeu<RailwayStationCode> stationCode;
    ListeNeu<Station> station;
    ListeNeu<RailwayLink> link;
    ListeNeu<RailwayLine> line;
 };


void Load_DB(istream &is, bahn_netz &netz);

void correctLinks(bahn_netz &netz);
void removePseudoNodes(bahn_netz &netz);
void mergeStationNodes(bahn_netz &netz);

void Save_DB(ostream &os, bahn_netz &netz);

int* findLink(struct Element<RailwayNode> *NodeA, struct Element<RailwayNode> *NodeB);
bool doNodesLink(RailwayNode NodeA, RailwayNode NodeB);
bool doStationsLink(Station stationA, Station stationB);
bool doStationLinkNode(Station station, RailwayNode node);

int abstandStationen(Station bahnhofA, Station bahnhofB);
int abstandNodes(RailwayNode bahnhofA, RailwayNode bahnhofB);
int abstandStationNode(Station bahnhofA, RailwayNode bahnhofB);

//Für das erstellen von gdi Dateien mit beliebiger Konten Anzahl
//Erstellt für IT-Projektmangament
void Save_DBWithNNodes(ostream &os, bahn_netz &netz, int anzNodes, int offset);
//Hilfsfunktionen für Save_DBWithNNodes
struct Element<RailwayNode> * kopiereNode(struct Element<RailwayNode> *sourceNode);
struct Element<Station> * kopiereStation(struct Element<Station> *sourceStation);

#endif // LOAD_DB_H

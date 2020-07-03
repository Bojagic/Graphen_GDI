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
    double xKoordinate;
    double yKoordinate;
    int spokeEnd[2];      //link
    int spokeStart[2];    //link
    string text;          //Nur bei StationNodes
    string typ;           //Nur bei Nodes

    friend ostream &operator<< (ostream &ostr, const RailwayNode node);
};

class Station
{
public:
    string code;
    ListeNeu<int> spokeEnd;
    ListeNeu<int> spokeStart;

    friend ostream &operator<< (ostream &ostr, Station station);
};

class RailwayStationCode{        //Benger;
public:
    int nummer;
    int SNodeNummer;
    string code;

    friend ostream &operator<< (ostream &ostr, const RailwayStationCode code);
};

class RailwayLink{
public:
    int nummer;

    int startNodeNummer;
    int endNodeNummer;

    friend ostream &operator<< (ostream &ostr, const RailwayLink link);
};

class RailwayLine{
public:
    int nummer;
    int linkNummer[420];
    string text;
    int code;

    friend ostream &operator<< (ostream &ostr, const RailwayLine line);
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

/**
 * @brief Load_DB - Liesst die Daten, einer in JSON vorliegenden Datei, ein
 * @param is geöffneter input stream
 * @param netz bahn_netz, welches gespeichert wird
 */
void Load_DB(istream &is, bahn_netz &netz);

/**
 * @brief Load_DB - Erstellt anhand der Bahndaten eine Graph Datei in .gdi Format
 * @param os geöffneter output stream
 * @param netz bahn_netz, aus dem dem die Daten geholt werden
 */
void Save_DB(ostream &os, bahn_netz &netz);

/**
 * @brief correctLinks - Korregiert die von Load_DB gespeicherten, falschen Links
 * Die Links stehts nur zweimal den gleichen Node gespeichert
 * @param netz bahn_netz
 */
void correctLinks(bahn_netz &netz);

/**
 * @brief removePseudoNodes - Entfernt die pseudoNodes aus der node Liste
 * Die Verbindungen werden beibehalten (ähnlich wie beim entfernen eines Elements aus einer doppelt verketteten Liste)
 * Wird am zum Schluss in Load_DB aufgerufen
 * @param netz bahn_netz
 */
void removePseudoNodes(bahn_netz &netz);

/**
 * @brief mergeStationNodes - Fügt StationNodes, welche den gleichen StationCode haben als Station zusammen
 * Wird am zum Schluss in Load_DB aufgerufen
 * @param netz bahn_netz
 */
void mergeStationNodes(bahn_netz &netz);

/**
 * @brief doNodesLink - Überprüft, ob zwei Nodes miteinander verbunden sind
 * Wird in Save_DB gebraucht
 * @param NodeA
 * @param NodeB
 * @return Ergebnis
 */
bool doNodesLink(RailwayNode NodeA, RailwayNode NodeB);

/**
 * @brief findLink - Findet denn Link heraus, über den zwei Nodes verbunden sind
 * Arbeitet ähnlich wie doNodesLink
 * Wird in Save_DB gebraucht
 * @param NodeA
 * @param NodeB
 * @return Link Nummer
 */
int findLink(RailwayNode NodeA, RailwayNode NodeB);

/**
 * @brief doStationsLink - Überprüft, ob zwei Stations miteinander verbunden sind
 * Wird in Save_DB gebraucht
 * @param stationA
 * @param stationB
 * @return Ergebnis
 */
bool doStationsLink(Station stationA, Station stationB);

/**
 * @brief doStationLinkNode - Überprüft, ob eine Station mit einem Nodes verbunden ist
 * Wird in Save_DB gebraucht
 * @param station
 * @param node
 * @return Ergebnis
 */
bool doStationLinkNode(Station station, RailwayNode node);


#endif // LOAD_DB_H

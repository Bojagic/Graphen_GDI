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

    friend ostream &operator<< (ostream &ostr, const RailwayNode node);     //für Liste::ausgabe Testausgabe RailwayNode
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
bool doNodesLink(RailwayNode NodeA, RailwayNode NodeB);
int findLink(RailwayNode NodeA, RailwayNode NodeB);
bool doStationsLink(Station stationA, Station stationB);
bool doStationLinkNode(Station station, RailwayNode node);


#endif // LOAD_DB_H

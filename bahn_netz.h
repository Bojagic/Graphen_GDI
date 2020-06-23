#ifndef LOAD_DB_H
#define LOAD_DB_H

#include <ostream>
#include <iostream>

#include "liste.h"

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

class RailwayStationCode{        //Benger;
public:
    int nummer;
    int SNodeNummer;
    string code;
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
    Liste<RailwayNode> node;
    Liste<RailwayNode> stationNode;
    Liste<RailwayStationCode> stationCode;
    Liste<RailwayLink> link;
    Liste<RailwayLine> line;
 };


void Load_DB(istream &is, bahn_netz &netz);

void Save_DB(ostream &os, bahn_netz &netz, size_t anzNode);

void Save_DB(ostream &os, bahn_netz &netz, string startCode, size_t entfernung);    //Bojagic und Horten


#endif // LOAD_DB_H

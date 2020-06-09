#ifndef LOAD_DB_H
#define LOAD_DB_H

#include <ostream>
#include <iostream>

#include "liste.h"

using namespace std;

class RailwayNode
{
public:
    int nummer;
    double xKoordinate;
    double yKoordinate;
    int spokeEnd[2];      //link
    int spokeStart[2];    //link
    string text;
    string typ;                 //formOfNode

    friend ostream &operator<< (ostream &ostr, const RailwayNode node);     //für Liste::ausgabe Testausgabe RailwayNode
};

class RailwayLink{
public:
    int nummer;

    int startNodeNummer;
    int endNodeNummer;

    friend ostream &operator<< (ostream &ostr, const RailwayLink link);     //für Liste::ausgabe Testausgabe RailwayLink
};

class RailwayLine{
public:
    int linkNummer[100];
    string text;
    int code;

    friend ostream &operator<< (ostream &ostr, const RailwayLine line);     //für Liste::ausgabe Testausgabe RailwayLine
};



class bahn_netz{
public:
    Liste<RailwayNode> node;
    Liste<RailwayNode> stationNode;
    Liste<RailwayLink> link;
    Liste<RailwayLine> line;
 };


 /**
 * @brief Load_DB
 * @param is geöffneter input stream
 * @param G Graph, in den eingelesen wird
 * @return Error Number
 */
void Load_DB(istream &is, bahn_netz &netz);


#endif // LOAD_DB_H

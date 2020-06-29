#include <iostream>
#include <fstream>

#include "bahn_netz.h"

using namespace std;

void Load_DB(istream &is, bahn_netz &netz)
{
    string tempstring;      //Zeile
    string datentyp;        //Datenart
    string wort;            //Eingelesenes Wort

    size_t wordStart;
    size_t wordEnd;
    size_t i;

    size_t zaehler[5];
    zaehler[0] = zaehler[1] = zaehler[2] = zaehler[3] = zaehler[4] = 0;

    while(getline(is, tempstring))
    {
        wordEnd = tempstring.find("\t", 0);     //Erstes Wort bis Tabulator einlesen
        datentyp = tempstring.substr(0,wordEnd);

        if(datentyp == "RailwayLine")           //Benger
        {

            i=0;
            string daten;
            RailwayLine tempLine;

            //------Line-------------------------------------------------------------
            daten=tempstring.substr(tempstring.find("Line-")+5,6);
            tempLine.nummer=stoi(daten);

            //------Text-------------------------------------------------------------------

            wordStart=tempstring.find(":\"")+2;
            wordEnd=tempstring.find("\",\"r")-wordStart;
            tempLine.text=tempstring.substr(wordStart,wordEnd);

            //------Railwaycode------------------------------------------------------------

            wordStart=tempstring.find("Code\":\"")+7;
            wordEnd=tempstring.find("\"}");
            wordEnd=wordEnd-wordStart;
            tempLine.code=stoi(tempstring.substr(wordStart,wordEnd));
            //------Link-code--------------------------------------------------------------
            wordEnd=tempstring.find("]");
            do
            {
                //cout<<tempstring.substr(tempstring.find("Link-")+5,6)<<endl;
                tempstring=tempstring.substr(tempstring.find("Link-")+5,wordEnd);
                tempLine.linkNummer[i]=stoi(tempstring);
                i++;
            }while(tempstring.find("]")!=7);
            tempLine.linkNummer[i] = -1;

            netz.line.add_last(tempLine);
            zaehler[0]++;
        }

        if(datentyp == "RailwayLink")           //Bojagic
        {
            RailwayLink tempLink;

            wordStart = tempstring.find("-", 0);                        //Nummer Speichern
            wordEnd = tempstring.find("\t", wordStart);
            wort = tempstring.substr(wordStart+1,wordEnd-wordStart);
            tempLink.nummer = stoi(wort);

            if(tempstring.find("endNode\":\"Node-", wordEnd) != string::npos)   //Links ohne Start und Ende werden nicht gespeichert
            {
                wordStart = tempstring.find("endNode\":\"Node-", wordEnd);      //endNodeNummer Speichern
                //cout << "wordStart----->" << wordStart << endl;
                wordStart+=15;
                wordEnd = tempstring.find("\"", wordStart);
                //cout << "wordEnd----->" << wordEnd << endl;
                wort = tempstring.substr(wordStart, wordEnd-wordStart);
                //cout << "Wort----->" << wort << endl;
                tempLink.endNodeNummer = stoi(wort);

                wordStart = tempstring.find("startNode\":\"Node-", wordEnd);    //startNodeNummer Speichern
                //cout << "wordStart----->" << wordStart << endl;
                wordStart+=17;
                wordEnd = tempstring.find("\"", wordStart);
                //cout << "wordEnd----->" << wordEnd << endl;
                wort = tempstring.substr(wordStart, wordEnd-wordStart);
                //cout << "Wort----->" << wort << endl;
                tempLink.startNodeNummer = stoi(wort);
            }
            netz.link.add_last(tempLink);
            zaehler[1]++;
        }

        if(datentyp == "RailwayNode")           //Horten
        {
            RailwayNode tempNode;

            wordStart = tempstring.find("-", 0);                        //Nummer Speichern
            wordEnd = tempstring.find("\x9", wordStart);
            wort = tempstring.substr(wordStart+1,wordEnd-wordStart);
            tempNode.nummer = stoi(wort);

            wordStart = tempstring.find(":\"", wordEnd);                //X-Koordinate speichern
            wordEnd = tempstring.find(" ", wordStart);
            wort = tempstring.substr(wordStart+2,wordEnd-wordStart-1);
            tempNode.xKoordinate = stod(wort);

            wordStart = wordEnd+1;                                      //Y-Koordinate speichern
            wordEnd = tempstring.find("\"", wordStart);
            wort = tempstring.substr(wordStart,wordEnd-wordStart);
            tempNode.yKoordinate = stod(wort);

            if( tempstring.substr(wordEnd+12, 1) == "E" )               //spokeEnd speichern, wenn vorhanden
            {
                wordStart = tempstring.find("-", wordEnd);
                wordEnd = tempstring.find("\"",wordStart);
                wort = tempstring.substr(wordStart+1, wordEnd-wordStart);
                tempNode.spokeEnd[0] = stoi(wort);

                if( tempstring.substr(wordEnd+1, 1) != "]" ){       //Zweites spokeEnd?
                    wordStart = tempstring.find("-", wordEnd);
                    wordEnd = tempstring.find("\"",wordStart);
                    wort = tempstring.substr(wordStart+1, wordEnd-wordStart);
                    tempNode.spokeEnd[1] = stoi(wort);
                }
                else tempNode.spokeEnd[1] = -1;         //Kein zweites vorhanden
            }

            if( tempstring.substr(wordEnd+13, 1) == "S" )               //spokeStart speichern, wenn vorhanden
            {
                wordStart = tempstring.find("-", wordEnd);
                wordEnd = tempstring.find("\"",wordStart);
                wort = tempstring.substr(wordStart+1, wordEnd-wordStart);
                tempNode.spokeStart[0] = stoi(wort);

                if( tempstring.substr(wordEnd+1, 1) != "]" )          //Zweites spokeStart?
                {
                    wordStart = tempstring.find("-", wordEnd);
                    wordEnd = tempstring.find("\"",wordStart);
                    wort = tempstring.substr(wordStart+1, wordEnd-wordStart);
                    tempNode.spokeStart[1] = stoi(wort);
                }
                else tempNode.spokeStart[1] = -1;       //Kein zweites vorhanden
            }

            if( tempstring.substr(wordEnd+4, 7) == "gn:text" )          //text speichern, wenn vorhanden
            {
                wordStart = tempstring.find(":\"", wordEnd);
                wordEnd = tempstring.find("\"", wordStart+2);
                wort = tempstring.substr(wordStart+2, wordEnd-wordStart-2);
                tempNode.text = wort;
            }

            wordStart = tempstring.find(":\"", wordEnd);                //formOfNode speichern
            wordEnd = tempstring.find("\"", wordStart+2);
            wort = tempstring.substr(wordStart+2, wordEnd-wordStart-2);
            tempNode.typ = wort;

            netz.node.add_last(tempNode);
            zaehler[2]++;
        }

        if(datentyp == "RailwayStationNode")    //Lewicki
        {
            RailwayNode snode;
            double gmlpos1, gmlpos2;
            string stadt, typ;
            int snodeID;
            int spokeEnd[2];
            int spokeStart[2];

            if(tempstring.find("SNode") != string::npos)
            {
                wordEnd = tempstring.find("SNode");
                snodeID = stoi(tempstring.substr(wordEnd+6,tempstring.substr(wordEnd+6).find("\t")));
            }
            if(tempstring.find("gml:pos") != string::npos)
            {
                wordEnd = tempstring.find("gml:pos");
                gmlpos1 = stod(tempstring.substr(wordEnd+10, tempstring.substr(wordEnd+10).find(" ")));
                gmlpos2 = stod(tempstring.substr(tempstring.find(" ")+1, tempstring.substr(tempstring.find(" ")+1).find(",")-1));
            }
            if(tempstring.find("gn:text") != string::npos)
            {
                wordEnd = tempstring.find("gn:text");
                stadt = tempstring.substr(wordEnd+10, tempstring.substr(wordEnd+10).find(",")-1);
            }
            if(tempstring.find("formOfNode") != string::npos)
            {
                wordEnd = tempstring.find("formOfNode");
                typ = tempstring.substr(wordEnd+13, tempstring.substr(wordEnd+13).find("\""));
            }
            if(tempstring.find("net:spokeEnd") != string::npos)
            {
                wordEnd = tempstring.find("net:spokeEnd");
                spokeEnd[0] = stoi(tempstring.substr(wordEnd+21, tempstring.substr(wordEnd+21).find("\"")));
                if(tempstring.substr(wordEnd+21).find("Link-") != string::npos)
                {
                    spokeEnd[1] = stoi(tempstring.substr(wordEnd+35, tempstring.substr(wordEnd+35).find("\"")));
                    if(spokeEnd[0] == spokeEnd[1])      //Wenn erster und zweiter Link gleich sind
                        spokeEnd[1] = -1;               //Wird der zweite nicht gespeichert
                }
                else
                    spokeEnd[1] = -1;
            }
            if(tempstring.find("net:spokeStart") != string::npos)
            {
                wordEnd = tempstring.find("net:spokeStart");
                spokeStart[0] = stoi(tempstring.substr(wordEnd+23, tempstring.substr(wordEnd+23).find("\"")));
                if(tempstring.substr(wordEnd+23).find("Link-") != string::npos)
                {
                    spokeStart[1] = stoi(tempstring.substr(wordEnd+37, tempstring.substr(wordEnd+37).find("\"")));
                    if(spokeStart[0] == spokeStart[1])      //Wenn erster und zweiter Link gleich sind
                        spokeStart[1] = -1;                 //Wird der zweite nicht gespeichert
                }
                else
                    spokeStart[1] = -1;
            }

            snode.spokeStart[0] = spokeStart[0];
            snode.spokeStart[1] = spokeStart[1];    //Wenn spokeStart[0] == spokeStart[1] dann spokeStart[1] = -1
            snode.spokeEnd[0] = spokeEnd[0];        //selbes hier mit spokeEnd
            snode.spokeEnd[1] = spokeEnd[1];
            snode.nummer = snodeID;
            snode.xKoordinate = gmlpos1;
            snode.yKoordinate = gmlpos2;
            snode.typ = typ;
            snode.text = stadt;
            netz.stationNode.add_last(snode);

            zaehler[3]++;
        }

        if(datentyp == "RailwayStationCode")
        {
            RailwayStationCode code;

            wordStart=tempstring.find("Spd-")+4;
            code.nummer=stoi(tempstring.substr(wordStart,7));

            wordStart=tempstring.find("SNode-")+6;
            code.SNodeNummer=stoi(tempstring.substr(wordStart,6));

            wordStart=99;
            wordEnd=tempstring.find("\"}")-1-wordStart;
            code.code=tempstring.substr(wordStart-2,wordEnd+3);

            zaehler[4]++;
        }
    }
    cout << zaehler[0] <<" RailwayLine gelesen" << endl;
    cout << zaehler[1] <<" RailwayLink gelesen" << endl;
    cout << zaehler[2] <<" RailwayNode gelesen" << endl;
    cout << zaehler[3] <<" RailwayStationNode gelesen" << endl;
    cout << zaehler[4] <<" RailwayStationCode gelesen" << endl;

    //correctLinks(netz);
    //removePseudoNodes(netz);
    //mergeStationNodes(netz);
}

void correctLinks(bahn_netz &netz)
{
    size_t anzNodes = netz.stationNode.number_Elements();
    size_t anzLinks = netz.link.number_Elements();
    int nodeNummer;
    int ende, start;
    RailwayLink tempLink;
    for(size_t i=0; i<anzNodes; i++)
    {
        nodeNummer = netz.stationNode[i].nummer;

        ende = netz.stationNode[i].spokeEnd[0];
        start = netz.stationNode[i].spokeStart[0];
        for(size_t j=0; j<anzLinks; j++)
        {
            tempLink = netz.link.remove_first();
            if(ende == tempLink.nummer)
            {
                tempLink.endNodeNummer = nodeNummer;
            }
            if(start == tempLink.nummer)
            {
                tempLink.startNodeNummer = nodeNummer;
            }
            netz.link.add_last(tempLink);
        }

        ende = netz.stationNode[i].spokeEnd[0];
        start = netz.stationNode[i].spokeStart[1];
        for(size_t j=0; j<anzLinks; j++)
        {
            tempLink = netz.link.remove_first();
            if(ende == tempLink.nummer)
            {
                tempLink.endNodeNummer = nodeNummer;
            }
            if(start == tempLink.nummer)
            {
                tempLink.startNodeNummer = nodeNummer;
            }
            netz.link.add_last(tempLink);
        }

        ende = netz.stationNode[i].spokeEnd[1];
        start = netz.stationNode[i].spokeStart[0];
        for(size_t j=0; j<anzLinks; j++)
        {
            tempLink = netz.link.remove_first();
            if(ende == tempLink.nummer)
            {
                tempLink.endNodeNummer = nodeNummer;
            }
            if(start == tempLink.nummer)
            {
                tempLink.startNodeNummer = nodeNummer;
            }
            netz.link.add_last(tempLink);
        }

        ende = netz.stationNode[i].spokeEnd[1];
        start = netz.stationNode[i].spokeStart[1];
        for(size_t j=0; j<anzLinks; j++)
        {
            tempLink = netz.link.remove_first();
            if(ende == tempLink.nummer)
            {
                tempLink.endNodeNummer = nodeNummer;
            }
            if(start == tempLink.nummer)
            {
                tempLink.startNodeNummer = nodeNummer;
            }
            netz.link.add_last(tempLink);
        }
    }

    anzNodes = netz.node.number_Elements();
    for(size_t i=0; i<anzNodes; i++)
    {
        nodeNummer = netz.node[i].nummer;

        ende = netz.node[i].spokeEnd[0];
        start = netz.node[i].spokeStart[0];
        for(size_t j=0; j<anzLinks; j++)
        {
            tempLink = netz.link.remove_first();
            if(ende == tempLink.nummer)
            {
                tempLink.endNodeNummer = nodeNummer;
            }
            if(start == tempLink.nummer)
            {
                tempLink.startNodeNummer = nodeNummer;
            }
            netz.link.add_last(tempLink);
        }

        ende = netz.node[i].spokeEnd[0];
        start = netz.node[i].spokeStart[1];
        for(size_t j=0; j<anzLinks; j++)
        {
            tempLink = netz.link.remove_first();
            if(ende == tempLink.nummer)
            {
                tempLink.endNodeNummer = nodeNummer;
            }
            if(start == tempLink.nummer)
            {
                tempLink.startNodeNummer = nodeNummer;
            }
            netz.link.add_last(tempLink);
        }

        ende = netz.node[i].spokeEnd[1];
        start = netz.node[i].spokeStart[0];
        for(size_t j=0; j<anzLinks; j++)
        {
            tempLink = netz.link.remove_first();
            if(ende == tempLink.nummer)
            {
                tempLink.endNodeNummer = nodeNummer;
            }
            if(start == tempLink.nummer)
            {
                tempLink.startNodeNummer = nodeNummer;
            }
            netz.link.add_last(tempLink);
        }

        ende = netz.node[i].spokeEnd[1];
        start = netz.node[i].spokeStart[1];
        for(size_t j=0; j<anzLinks; j++)
        {
            tempLink = netz.link.remove_first();
            if(ende == tempLink.nummer)
            {
                tempLink.endNodeNummer = nodeNummer;
            }
            if(start == tempLink.nummer)
            {
                tempLink.startNodeNummer = nodeNummer;
            }
            netz.link.add_last(tempLink);
        }
    }
}

void removePseudoNodes(bahn_netz &netz)
{
    size_t anzNodes = netz.node.number_Elements();
    RailwayNode tempNodeA;
    RailwayNode tempNodeB;
    RailwayNode *startNode;
    RailwayNode *endNode;
    for(size_t i=0; i<anzNodes; i++)
    {cout << "a";
        tempNodeA = netz.node[i];
        if(tempNodeA.typ == "pseudoNode")
        {
            for(size_t j=0; j<anzNodes; j++)
            {
                tempNodeB = netz.node[j];
                if(doNodesLink(tempNodeA,tempNodeB))
                {
                    startNode = &tempNodeA;
                    endNode = &tempNodeB;

                    //for
                }
            }

            startNode->spokeEnd[0] = endNode->nummer;
            endNode->spokeStart[0] = startNode->nummer;

            netz.node.remove(i);
            anzNodes--;
        }
    }
}

void mergeStationNodes(bahn_netz &netz)
{

}


void Save_DB(ostream &os, bahn_netz &netz)
{
	size_t anzSNodes = netz.stationNode.number_Elements();
	size_t anzNodes = netz.node.number_Elements();
	size_t nodesGesamt = anzSNodes + anzNodes;

	RailwayNode *knoten = new RailwayNode[nodesGesamt];

	os << "G " << nodesGesamt << endl;

	for(size_t i=0; i<anzSNodes; i++)
	{
		knoten[i] = netz.stationNode[i];
	}

	for(size_t i=0; i<anzNodes; i++)
	{
		knoten[i+anzSNodes] = netz.node[i];
	}
cout << "!";
	for(size_t i=0; i<nodesGesamt; i++)
	{if(i%100 == 0) cout << i << endl;
		os << "V " << i+1 << " \"" << knoten[i].nummer << "\"" << endl;
		for(size_t j=0; j<nodesGesamt; j++)
		{
            if(doNodesLink(knoten[i], knoten[j]))
            {
                os << "E " << i+1 << " " << j+1 << " " << 1 << endl;
            }
		}
	}
	delete[] knoten;
}

bool doNodesLink(RailwayNode NodeA, RailwayNode NodeB)
{
    if(NodeA.nummer == NodeB.nummer)    //Gleiche Knoten
        return false;

    if(NodeA.spokeStart[0] == NodeB.spokeEnd[0] && NodeA.spokeStart[0] !=-1)
        return true;
    if(NodeA.spokeStart[0] == NodeB.spokeEnd[1] && NodeA.spokeStart[0] !=-1)
        return true;
    if(NodeA.spokeStart[1] == NodeB.spokeEnd[0] && NodeA.spokeStart[1] !=-1)
        return true;
    if(NodeA.spokeStart[1] == NodeB.spokeEnd[1] && NodeA.spokeStart[1] !=-1)
        return true;

    if(NodeA.spokeEnd[0] == NodeB.spokeStart[0] && NodeA.spokeEnd[0] !=-1)
        return true;
    if(NodeA.spokeEnd[0] == NodeB.spokeStart[1] && NodeA.spokeEnd[0] !=-1)
        return true;
    if(NodeA.spokeEnd[1] == NodeB.spokeStart[0] && NodeA.spokeEnd[1] !=-1)
        return true;
    if(NodeA.spokeEnd[1] == NodeB.spokeStart[1] && NodeA.spokeEnd[1] !=-1)
        return true;

    return false;
}


ostream &operator<<(ostream &ostr, const RailwayNode node)
{
    ostr << "Nummer    : " << node.nummer      << endl;
    ostr << "X Koord   : " << node.xKoordinate << endl;
    ostr << "Y Koord   : " << node.yKoordinate << endl;
    ostr << "spokeEnd  : " << node.spokeEnd[0];
    if(node.spokeEnd[1] != -1)
        ostr << ", " << node.spokeEnd[1];
    ostr << endl;
    ostr << "spokeStart: " << node.spokeStart[0];
    if(node.spokeStart[1] != -1)
        ostr << ", " << node.spokeStart[1];
    ostr << endl;
    ostr << "text      : " << node.text         << endl;
    ostr << "formOfNode: " << node.typ          << endl;

    return ostr;
}

ostream &operator<<(ostream &ostr, const RailwayLink link)
{
    ostr << "Nummer    : " << link.nummer          << endl;
    ostr << "Start Node: " << link.startNodeNummer << endl;
    ostr << "End   Node: " << link.endNodeNummer   << endl;

    return ostr;
}

ostream &operator<<(ostream &ostr, const RailwayLine line)
{
    ostr << "Nummer    : " << line.linkNummer[0];
    for(int i=1; line.linkNummer[i] != -1; i++)
        ostr << line.linkNummer[i];
    ostr << endl;
    ostr << "X Koord   : " << line.text << endl;
    ostr << "Y Koord   : " << line.code << endl;


    return ostr;
}

ostream &operator<< (ostream &ostr, const RailwayStationCode code)
{
    ostr << "Nummer        :" << code.nummer      << endl;
    ostr << "StationNummer :" << code.SNodeNummer << endl;
    ostr << "Code          :" << code.code        << endl;

    return ostr;
}

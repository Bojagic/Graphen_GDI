#include <iostream>
#include <fstream>

#include "bahn_netz.h"
#include "listeNeu.h"
#include "listeNeu.cpp"

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
            struct Element<RailwayLine> *tempLine = new struct Element<RailwayLine>;

            //------Line-------------------------------------------------------------
            daten=tempstring.substr(tempstring.find("Line-")+5,6);
            tempLine->schluessel.nummer = stoi(daten);

            //------Text-------------------------------------------------------------------

            wordStart=tempstring.find(":\"")+2;
            wordEnd=tempstring.find("\",\"r")-wordStart;
            tempLine->schluessel.text=tempstring.substr(wordStart,wordEnd);

            //------Railwaycode------------------------------------------------------------

            wordStart=tempstring.find("Code\":\"")+7;
            wordEnd=tempstring.find("\"}");
            wordEnd=wordEnd-wordStart;
            tempLine->schluessel.code=stoi(tempstring.substr(wordStart,wordEnd));
            //------Link-code--------------------------------------------------------------
            wordEnd=tempstring.find("]");
            do
            {
                //cout<<tempstring.substr(tempstring.find("Link-")+5,6)<<endl;
                tempstring=tempstring.substr(tempstring.find("Link-")+5,wordEnd);
                tempLine->schluessel.linkNummer[i]=stoi(tempstring);
                i++;
            }while(tempstring.find("]")!=7);
            tempLine->schluessel.linkNummer[i] = -1;

            List_Insert(netz.line, tempLine);
            zaehler[0]++;
        }

        if(datentyp == "RailwayLink")           //Bojagic
        {
            struct Element<RailwayLink> *tempLink = new struct Element<RailwayLink>;

            wordStart = tempstring.find("-", 0);                        //Nummer Speichern
            wordEnd = tempstring.find("\t", wordStart);
            wort = tempstring.substr(wordStart+1,wordEnd-wordStart);
            tempLink->schluessel.nummer = stoi(wort);

            if(tempstring.find("endNode\":\"Node-", wordEnd) != string::npos)   //Links ohne Start und Ende werden nicht gespeichert
            {
                wordStart = tempstring.find("endNode\":\"Node-", wordEnd);      //endNodeNummer Speichern
                //cout << "wordStart----->" << wordStart << endl;
                wordStart+=15;
                wordEnd = tempstring.find("\"", wordStart);
                //cout << "wordEnd----->" << wordEnd << endl;
                wort = tempstring.substr(wordStart, wordEnd-wordStart);
                //cout << "Wort----->" << wort << endl;
                tempLink->schluessel.endNodeNummer = stoi(wort);

                wordStart = tempstring.find("startNode\":\"Node-", wordEnd);    //startNodeNummer Speichern
                //cout << "wordStart----->" << wordStart << endl;
                wordStart+=17;
                wordEnd = tempstring.find("\"", wordStart);
                //cout << "wordEnd----->" << wordEnd << endl;
                wort = tempstring.substr(wordStart, wordEnd-wordStart);
                //cout << "Wort----->" << wort << endl;
                tempLink->schluessel.startNodeNummer = stoi(wort);
            }
            List_Insert(netz.link, tempLink);
            zaehler[1]++;
        }

        if(datentyp == "RailwayNode")           //Horten
        {
            struct Element<RailwayNode> *tempNode = new struct Element<RailwayNode>;

            wordStart = tempstring.find("-", 0);                        //Nummer Speichern
            wordEnd = tempstring.find("\x9", wordStart);
            wort = tempstring.substr(wordStart+1,wordEnd-wordStart);
            tempNode->schluessel.nummer = stoi(wort);

            wordStart = tempstring.find(":\"", wordEnd);                //X-Koordinate speichern
            wordEnd = tempstring.find(" ", wordStart);
            wort = tempstring.substr(wordStart+2,wordEnd-wordStart-1);
            tempNode->schluessel.xKoordinate = stod(wort);

            wordStart = wordEnd+1;                                      //Y-Koordinate speichern
            wordEnd = tempstring.find("\"", wordStart);
            wort = tempstring.substr(wordStart,wordEnd-wordStart);
            tempNode->schluessel.yKoordinate = stod(wort);

            if( tempstring.substr(wordEnd+12, 1) == "E" )               //spokeEnd speichern, wenn vorhanden
            {
                wordStart = tempstring.find("-", wordEnd);
                wordEnd = tempstring.find("\"",wordStart);
                wort = tempstring.substr(wordStart+1, wordEnd-wordStart);
                tempNode->schluessel.spokeEnd[0] = stoi(wort);

                if( tempstring.substr(wordEnd+1, 1) != "]" ){       //Zweites spokeEnd?
                    wordStart = tempstring.find("-", wordEnd);
                    wordEnd = tempstring.find("\"",wordStart);
                    wort = tempstring.substr(wordStart+1, wordEnd-wordStart);
                    tempNode->schluessel.spokeEnd[1] = stoi(wort);
                }
                else tempNode->schluessel.spokeEnd[1] = -1;         //Kein zweites vorhanden
            }

            if( tempstring.substr(wordEnd+13, 1) == "S" )               //spokeStart speichern, wenn vorhanden
            {
                wordStart = tempstring.find("-", wordEnd);
                wordEnd = tempstring.find("\"",wordStart);
                wort = tempstring.substr(wordStart+1, wordEnd-wordStart);
                tempNode->schluessel.spokeStart[0] = stoi(wort);

                if( tempstring.substr(wordEnd+1, 1) != "]" )          //Zweites spokeStart?
                {
                    wordStart = tempstring.find("-", wordEnd);
                    wordEnd = tempstring.find("\"",wordStart);
                    wort = tempstring.substr(wordStart+1, wordEnd-wordStart);
                    tempNode->schluessel.spokeStart[1] = stoi(wort);
                }
                else tempNode->schluessel.spokeStart[1] = -1;       //Kein zweites vorhanden
            }

            if( tempstring.substr(wordEnd+4, 7) == "gn:text" )          //text speichern, wenn vorhanden
            {
                wordStart = tempstring.find(":\"", wordEnd);
                wordEnd = tempstring.find("\"", wordStart+2);
                wort = tempstring.substr(wordStart+2, wordEnd-wordStart-2);
                tempNode->schluessel.text = wort;
            }

            wordStart = tempstring.find(":\"", wordEnd);                //formOfNode speichern
            wordEnd = tempstring.find("\"", wordStart+2);
            wort = tempstring.substr(wordStart+2, wordEnd-wordStart-2);
            tempNode->schluessel.typ = wort;

            List_Insert(netz.node, tempNode);
            zaehler[2]++;
        }

        if(datentyp == "RailwayStationNode")    //Lewicki
        {
            struct Element<RailwayNode> *snode = new struct Element<RailwayNode>;
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
            else
            {
                spokeEnd[0] = -1;
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
            else
            {
                spokeStart[0] = -1;
                spokeStart[1] = -1;
            }

            snode->schluessel.spokeStart[0] = spokeStart[0];
            snode->schluessel.spokeStart[1] = spokeStart[1];    //Wenn spokeStart[0] == spokeStart[1] dann spokeStart[1] = -1
            snode->schluessel.spokeEnd[0] = spokeEnd[0];        //selbes hier mit spokeEnd
            snode->schluessel.spokeEnd[1] = spokeEnd[1];
            snode->schluessel.nummer = snodeID;
            snode->schluessel.xKoordinate = gmlpos1;
            snode->schluessel.yKoordinate = gmlpos2;
            snode->schluessel.typ = typ;
            snode->schluessel.text = stadt;

            List_Insert(netz.stationNode, snode);

            zaehler[3]++;
        }

        if(datentyp == "RailwayStationCode")
        {
            struct Element<RailwayStationCode> *code = new struct Element<RailwayStationCode>;

            wordStart=tempstring.find("Spd-")+4;
            code->schluessel.nummer=stoi(tempstring.substr(wordStart,7));

            wordStart=tempstring.find("SNode-")+6;
            code->schluessel.SNodeNummer=stoi(tempstring.substr(wordStart,6));

            wordStart=99;
            wordEnd=tempstring.find("\"}")-1-wordStart;
            code->schluessel.code=tempstring.substr(wordStart-2,wordEnd+3);

            List_Insert(netz.stationCode, code);
            zaehler[4]++;
        }
    }
    cout << zaehler[0] <<" RailwayLine gelesen" << endl;
    cout << zaehler[1] <<" RailwayLink gelesen" << endl;
    cout << zaehler[2] <<" RailwayNode gelesen" << endl;
    cout << zaehler[3] <<" RailwayStationNode gelesen" << endl;
    cout << zaehler[4] <<" RailwayStationCode gelesen" << endl;

    //removePseudoNodes(netz);
    mergeStationNodes(netz);
    //correctLinks(netz);

}
/*
void correctLinks(bahn_netz &netz)
{
    size_t anzNodes = netz.stationNode.number_Elements();
    size_t anzLinks = netz.link.number_Elements();
    int nodeNummer;
    int ende, start;
    RailwayLink tempLink;
    //int proz = 0;
    for(size_t i=0; i<anzNodes; i++)
    {
        //if(i%(anzNodes/50) == 0) cout << proz++ << "%"<< endl;

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
        //if(i%(anzNodes/50) == 0) cout << proz++ << "%"<< endl;

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
*/
/*
void removePseudoNodes(bahn_netz &netz)                                                     //Bojagic
{
    size_t anzNodes = netz.node.number_Elements();
    size_t anzStationNodes = netz.stationNode.number_Elements();
    RailwayNode tempNodeA;
    RailwayNode tempNodeB;
    RailwayNode tempNodeC;
    int indexB, linkB;
    int indexC, linkC;
    for(size_t i=0; i<anzNodes; i++)
    {
        indexB = indexC = -1;
        tempNodeA = netz.node[i];
        if(tempNodeA.typ == "pseudoNode")
        {
            for(size_t j=0; j<anzNodes; j++)
            {
                tempNodeB = netz.node[j];

                if(doNodesLink(tempNodeA,tempNodeB))
                {
                    indexB = j;
                    linkB = findLink(tempNodeA,tempNodeB);
                }
            }
            for(size_t k=indexB+1; k<anzNodes; k++)
            {
                tempNodeC = netz.node[k];

                if(doNodesLink(tempNodeA,tempNodeC))
                {
                    indexC = k;
                    linkC = findLink(tempNodeA,tempNodeC);
                }
            }

            if(indexB == -1 && indexC == -1)
            {
                for(size_t j=0; j<anzStationNodes; j++)
                {
                    tempNodeB = netz.stationNode[j];

                    if(doNodesLink(tempNodeA,tempNodeB))
                    {
                        indexB = j;
                        linkB = findLink(tempNodeA,tempNodeB);
                    }
                }

                for(size_t k=indexB+1; k<anzStationNodes; k++)
                {
                    tempNodeC = netz.stationNode[k];

                    if(doNodesLink(tempNodeA,tempNodeC))
                    {
                        indexC = k;
                        linkC = findLink(tempNodeA,tempNodeC);
                    }
                }
            }
            else if(indexC == -1)
            {
                for(size_t j=0; j<anzStationNodes; j++)
                {
                    tempNodeC = netz.stationNode[j];

                    if(doNodesLink(tempNodeA,tempNodeC))
                    {
                        indexC = j;
                        linkC = findLink(tempNodeA,tempNodeC);
                    }
                }
            }

            if(indexB != -1 && indexC != -1)
            {
                tempNodeB = netz.node.remove(indexB);
                tempNodeC = netz.node.remove(indexC);

                if(tempNodeB.spokeStart[0] == linkB && tempNodeC.spokeStart[0] == linkC)
                {
                    tempNodeB.spokeStart[0] = linkB;
                    tempNodeC.spokeStart[0] = linkB;
                }
                else if(tempNodeB.spokeStart[0] == linkB && tempNodeC.spokeStart[1] == linkC)
                {
                    tempNodeB.spokeStart[0] = linkB;
                    tempNodeC.spokeStart[1] = linkB;
                }
                else if(tempNodeB.spokeStart[0] == linkB && tempNodeC.spokeEnd[0] == linkC)
                {
                    tempNodeB.spokeStart[0] = linkB;
                    tempNodeC.spokeEnd[0] = linkB;
                }
                else if(tempNodeB.spokeStart[0] == linkB && tempNodeC.spokeEnd[1] == linkC)
                {
                    tempNodeB.spokeStart[0] = linkB;
                    tempNodeC.spokeEnd[1] = linkB;
                }

                else if(tempNodeB.spokeStart[1] == linkB && tempNodeC.spokeStart[0] == linkC)
                {
                    tempNodeB.spokeStart[1] = linkB;
                    tempNodeC.spokeStart[0] = linkB;
                }
                else if(tempNodeB.spokeStart[1] == linkB && tempNodeC.spokeStart[1] == linkC)
                {
                    tempNodeB.spokeStart[1] = linkB;
                    tempNodeC.spokeStart[1] = linkB;
                }
                else if(tempNodeB.spokeStart[1] == linkB && tempNodeC.spokeEnd[0] == linkC)
                {
                    tempNodeB.spokeStart[1] = linkB;
                    tempNodeC.spokeEnd[0] = linkB;
                }
                else if(tempNodeB.spokeStart[1] == linkB && tempNodeC.spokeEnd[1] == linkC)
                {
                    tempNodeB.spokeStart[1] = linkB;
                    tempNodeC.spokeEnd[1] = linkB;
                }

                else if(tempNodeB.spokeEnd[0] == linkB && tempNodeC.spokeStart[0] == linkC)
                {
                    tempNodeB.spokeEnd[0] = linkB;
                    tempNodeC.spokeStart[0] = linkB;
                }
                else if(tempNodeB.spokeEnd[0] == linkB && tempNodeC.spokeStart[1] == linkC)
                {
                    tempNodeB.spokeEnd[0] = linkB;
                    tempNodeC.spokeStart[1] = linkB;
                }
                else if(tempNodeB.spokeEnd[0] == linkB && tempNodeC.spokeEnd[0] == linkC)
                {
                    tempNodeB.spokeEnd[0] = linkB;
                    tempNodeC.spokeEnd[0] = linkB;
                }
                else if(tempNodeB.spokeEnd[0] == linkB && tempNodeC.spokeEnd[1] == linkC)
                {
                    tempNodeB.spokeEnd[0] = linkB;
                    tempNodeC.spokeEnd[1] = linkB;
                }

                else if(tempNodeB.spokeEnd[1] == linkB && tempNodeC.spokeStart[0] == linkC)
                {
                    tempNodeB.spokeEnd[1] = linkB;
                    tempNodeC.spokeStart[0] = linkB;
                }
                else if(tempNodeB.spokeEnd[1] == linkB && tempNodeC.spokeStart[1] == linkC)
                {
                    tempNodeB.spokeEnd[1] = linkB;
                    tempNodeC.spokeStart[1] = linkB;
                }
                else if(tempNodeB.spokeEnd[1] == linkB && tempNodeC.spokeEnd[0] == linkC)
                {
                    tempNodeB.spokeEnd[1] = linkB;
                    tempNodeC.spokeEnd[0] = linkB;
                }
                else if(tempNodeB.spokeEnd[1] == linkB && tempNodeC.spokeEnd[1] == linkC)
                {
                    tempNodeB.spokeEnd[1] = linkB;
                    tempNodeC.spokeEnd[1] = linkB;
                }

                netz.node.add_last(tempNodeB);
                netz.node.add_last(tempNodeC);
            }

            netz.node.remove(i);
            anzNodes--;
            i=0;
        }
    }
}
*/
void mergeStationNodes(bahn_netz &netz)                                                     //Horten
{
    try
    {
        struct Element<Station> *tempStation;
        struct Element<RailwayNode> *currSNode = netz.stationNode.kopf;
        struct Element<RailwayStationCode> *currCode = netz.stationCode.kopf;
        struct Element<Station> *currStation;

        bool gefunden;
        while(currSNode != nullptr)     //laufe durch StationNodes
        {
            gefunden = false;
            currStation =netz.station.kopf;
            while(currStation != nullptr)       //laufe durch alle Stations
            {
                if(currCode->schluessel.code == currStation->schluessel.code)
                {
                    gefunden = true;
                    if(currSNode->schluessel.spokeEnd[0] != -1)
                    {
                        struct Element<int> *spokeEnd0 = new Element<int>;              //Neues Element anlegen
                        spokeEnd0->schluessel = currSNode->schluessel.spokeEnd[0];      //Element den Wert von spokeEnd[0] geben
                        List_Insert(currStation->schluessel.spokeEnd, spokeEnd0);       //Element an Liste anhängen
                    }

                    if(currSNode->schluessel.spokeStart[0] != -1)
                    {
                        struct Element<int> *spokeStart0 = new Element<int>;
                        spokeStart0->schluessel = currSNode->schluessel.spokeStart[0];
                        List_Insert(currStation->schluessel.spokeStart, spokeStart0);
                    }

                    if(currSNode->schluessel.spokeEnd[1] != -1)
                    {
                        struct Element<int> *spokeEnd1 = new Element<int>;
                        spokeEnd1->schluessel = currSNode->schluessel.spokeEnd[1];
                        List_Insert(currStation->schluessel.spokeEnd, spokeEnd1);
                    }

                    if(currSNode->schluessel.spokeStart[1] != -1)
                    {
                        struct Element<int> *spokeStart1 = new Element<int>;
                        spokeStart1->schluessel = currSNode->schluessel.spokeStart[1];
                        List_Insert(currStation->schluessel.spokeStart, spokeStart1);
                    }
                    break;
                }
                currStation = currStation->nachf;
            }
            if(!gefunden)
            {
                tempStation = new struct Element<Station>;

                tempStation->schluessel.code = currCode->schluessel.code;

                if(currSNode->schluessel.spokeEnd[0] != -1)
                {
                    struct Element<int> *spokeEnd0 = new Element<int>;              //Neues Element anlegen
                    spokeEnd0->schluessel = currSNode->schluessel.spokeEnd[0];      //Element den Wert von spokeEnd[0] geben
                    List_Insert(tempStation->schluessel.spokeEnd, spokeEnd0);       //Element an Liste anhängen
                }

                if(currSNode->schluessel.spokeStart[0] != -1)
                {
                    struct Element<int> *spokeStart0 = new Element<int>;
                    spokeStart0->schluessel = currSNode->schluessel.spokeStart[0];
                    List_Insert(tempStation->schluessel.spokeStart, spokeStart0);
                }

                if(currSNode->schluessel.spokeEnd[1] != -1)
                {
                    struct Element<int> *spokeEnd1 = new Element<int>;
                    spokeEnd1->schluessel = currSNode->schluessel.spokeEnd[1];
                    List_Insert(tempStation->schluessel.spokeEnd, spokeEnd1);
                }

                if(currSNode->schluessel.spokeStart[1] != -1)
                {
                    struct Element<int> *spokeStart1 = new Element<int>;
                    spokeStart1->schluessel = currSNode->schluessel.spokeStart[1];
                    List_Insert(tempStation->schluessel.spokeStart, spokeStart1);
                }

                List_Insert(netz.station, tempStation);
            }

            currSNode = currSNode->nachf;
            currCode = currCode->nachf;
        }
    }
    catch(const char* msg)
    {
        cerr << msg;
    }
}

void Save_DB(ostream &os, bahn_netz &netz)  //Lewicki
{
	struct Element<Station> *currStation = netz.station.kopf;
	struct Element<RailwayNode> *currNode = netz.node.kopf;
	struct Element<Station> *currStation2 = netz.station.kopf;
	struct Element<RailwayNode> *currNode2 = netz.node.kopf;
	int anzStations;
	int anzNodes;
	int i,j;

    anzStations=0;
    while(currStation != nullptr)       // Zählt die Stationen
    {
        currStation = currStation->nachf;
        anzStations++;
    }

    anzNodes=0;
    while(currNode != nullptr)          // Zählt die Nodes
    {
        currNode = currNode->nachf;
        anzNodes++;
    }

	os << "G " << anzStations+anzNodes << endl;  // Schreiben des Kopfes mit Anzahl aller Knoten

	i = 0;
	currNode = netz.node.kopf;
	while(currNode != nullptr)                                                      // Geht die ganze Liste der Railwaynodes durch mit currNode
	{
		os << "V " << i+1 << " \"" << currNode->schluessel.nummer << "\"" << endl;  // Und schreibt den aktuellen Node jeweils direkt als Knoten in die Datei

		j=0;
		currNode2 = netz.node.kopf;
		while(currNode2 != nullptr)                                     // Geht die Liste der Railwaynodes durch mit currNode2
		{
            if(doNodesLink(currNode2, currNode)                         // Und prüft ob mit currNode eine verbindung besteht (Selbe Node Nummern ausgeschlossen)
            {
                os << "E " << i+1 << " " << j+1 << " " << 1 << endl;    // Falls eine verbindung besteht, wird die Kante aufgeschrieben
            }
            j++;
            currNode2 = currNode2->nachf;
		}

		j=0;
		currStation2 = netz.station.kopf;
		while(currStation2 != nullptr)                                          // Geht die Liste der Stations durch mit currStation2
        {
            if(doStationLinkNode(currStation2, currNode))                       // Und prüft ob mit currNode eine Verbindung besteht
            {
                os << "E " << i+1 << " " << j+1+anzNodes << " " << 1 << endl;   // Falls eine Verbindung besteht, wird die Kante aufgeschrieben
            }
            j++;
            currStation2 = currStation2->nachf;
        }
        i++;
        currNode = currNode->nachf;
	}

    i = 0;
	currStation = netz.station.kopf;
	while(currStation != nullptr)                                                               // Geht die ganze Liste der Stationen durch mit currStation
	{
		os << "V " << i+1+anzNodes << " \"" << currStation->schluessel.code << "\"" << endl;    // Und schreibt die aktuelle Station jeweils direkt als Knoten in die Datei

		j=0;
		currNode2 = netz.node.kopf;
		while(currNode2 != nullptr)                                             // Geht die Liste der Railwaynodes durch mit currNode2
		{
            //if(doStationLinkNode(currNode2, currStation))                     // Und prüft ob mit currStation eine Verbindung besteht
            if(true)
            {
                os << "E " << i+1+anzNodes << " " << j+1 << " " << 1 << endl;   // Falls eine Verbindung besteht, wird die Kante aufgeschrieben
            }
            j++;
            currNode2 = currNode2->nachf;
		}

		j=0;
		currStation2 = netz.station.kopf;
		while(currStation2 != nullptr)                                                  // Geht die Liste der Stations durch mit currStation2
        {
            //if(doStationLinkNode(currStation2, currStation))                          // Und prüft ob mit currStation eine Verbindung besteht (Selbe Station Nummern ausgeschlossen)
            if(true)
            {
                os << "E " << i+1+anzNodes << " " << j+1+anzNodes << " " << 1 << endl;  // Falls eine Verbindung besteht, wird die Kante aufgeschrieben
            }
            j++;
            currStation2 = currStation2->nachf;
        }
        i++;
        currStation = currStation->nachf;
	}
}


/*
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

int findLink(RailwayNode NodeA, RailwayNode NodeB)      //für removePseudoNodes
{
    if(NodeA.nummer == NodeB.nummer)    //Gleiche Knoten
        return -1;

    if(NodeA.spokeStart[0] == NodeB.spokeEnd[0] && NodeA.spokeStart[0] !=-1)
        return NodeA.spokeStart[0];
    if(NodeA.spokeStart[0] == NodeB.spokeEnd[1] && NodeA.spokeStart[0] !=-1)
        return NodeA.spokeStart[0];
    if(NodeA.spokeStart[1] == NodeB.spokeEnd[0] && NodeA.spokeStart[1] !=-1)
        return NodeA.spokeStart[1];
    if(NodeA.spokeStart[1] == NodeB.spokeEnd[1] && NodeA.spokeStart[1] !=-1)
        return NodeA.spokeStart[1];

    if(NodeA.spokeEnd[0] == NodeB.spokeStart[0] && NodeA.spokeEnd[0] !=-1)
        return NodeA.spokeEnd[0];
    if(NodeA.spokeEnd[0] == NodeB.spokeStart[1] && NodeA.spokeEnd[0] !=-1)
        return NodeA.spokeEnd[0];
    if(NodeA.spokeEnd[1] == NodeB.spokeStart[0] && NodeA.spokeEnd[1] !=-1)
        return NodeA.spokeEnd[1];
    if(NodeA.spokeEnd[1] == NodeB.spokeStart[1] && NodeA.spokeEnd[1] !=-1)
        return NodeA.spokeEnd[1];

    return -1;
}

bool doStationsLink(Station stationA, Station stationB)
{
    if(stationA.code == stationB.code)
        return false;

    size_t anzStationASpokeEnd = stationA.spokeEnd.number_Elements();
    size_t anzStationASpokeStart = stationA.spokeStart.number_Elements();
    size_t anzStationBSpokeEnd = stationB.spokeEnd.number_Elements();
    size_t anzStationBSpokeStart = stationB.spokeStart.number_Elements();

    for(size_t i=0; i<anzStationASpokeEnd; i++)
    {
        for(size_t j=0; i<anzStationBSpokeEnd; j++)
            if(stationA.spokeEnd[i] == stationB.spokeEnd[j] && stationA.spokeEnd[i]  != -1)
                return true;
    }
    for(size_t i=0; i<anzStationASpokeEnd; i++)
    {
        for(size_t j=0; i<anzStationBSpokeStart; j++)
            if(stationA.spokeEnd[i] == stationB.spokeStart[j] && stationA.spokeEnd[i] != -1)
                return true;
    }
    for(size_t i=0; i<anzStationASpokeStart; i++)
    {
        for(size_t j=0; i<anzStationBSpokeEnd; j++)
            if(stationA.spokeStart[i] == stationB.spokeEnd[j] && stationA.spokeStart[i]  != -1)
                return true;
    }
    for(size_t i=0; i<anzStationASpokeStart; i++)
    {
        for(size_t j=0; i<anzStationBSpokeStart; j++)
            if(stationA.spokeStart[i] == stationB.spokeStart[j] && stationA.spokeStart[i]  != -1)
                return true;
    }

    return false;
}

bool doStationLinkNode(Station station, RailwayNode node)                                   //Benger
{
    size_t anzStationSpokeEnd = station.spokeEnd.number_Elements();
    size_t anzStationSpokeStart = station.spokeStart.number_Elements();

    int tempStationSpoke;

    for(size_t i=0; i<anzStationSpokeEnd; i++)
    {
        tempStationSpoke = station.spokeEnd[i];
        if(tempStationSpoke != -1)
        {
            if(tempStationSpoke == node.spokeEnd[0])
                return true;
            if(tempStationSpoke == node.spokeEnd[1])
                return true;
            if(tempStationSpoke == node.spokeStart[0])
                return true;
            if(tempStationSpoke == node.spokeStart[1])
                return true;
        }
    }
    for(size_t i=0; i<anzStationSpokeStart; i++)
    {
        tempStationSpoke = station.spokeStart[i];
        if(tempStationSpoke != -1)
        {
            if(tempStationSpoke == node.spokeEnd[0])
                return true;
            if(tempStationSpoke == node.spokeEnd[1])
                return true;
            if(tempStationSpoke == node.spokeStart[0])
                return true;
            if(tempStationSpoke == node.spokeStart[1])
                return true;
        }
    }
    return false;
}
*/
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

ostream &operator<<(ostream &ostr,Station station)
{
    ostr << "Nummer    : " << station.code         << endl;
    ostr << "Start Node: ";
    List_Print(station.spokeStart);
    ostr << "End   Node: ";
    List_Print(station.spokeEnd);

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

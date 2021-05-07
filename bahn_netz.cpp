/*
 * Autoren: Michel Benger, Haris Bojagic, Tim Horten, Bryan Lewicki
 * Gruppe : 3 Mittwochvormittag
 * Thema  : Miniprojekt 1 - Graphen aus Open Source Data der Deutschen Bahn erstellen
 *			Miniprojekt 2 - Abstand von zwei Bahnhöfen
 */

#include <iostream>
#include <fstream>

#include "bahn_netz.h"
#include "listeNeu.h"
#include "listeNeu.cpp"

//Für abstandStationen
#include <math.h>
#ifndef M_PI      //definiert M_PI wenn nicht definiert ist
    #define M_PI 3.14159265358979323846
#endif
#define E_RAD 6371  // Erdradius

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
        wordEnd = tempstring.find("\t", 0);         //Erstes Wort bis Tabulator einlesen
        datentyp = tempstring.substr(0,wordEnd);    //Art der Daten steht am Anfang der Zeile

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

            List_Insert(netz.line, tempLine);                  //Beschriebenes line-Objekt zur Liste hinzufügen
            zaehler[0]++;
        }

        if(datentyp == "RailwayLink")           //Bojagic
        {
            struct Element<RailwayLink> *tempLink = new struct Element<RailwayLink>;

            wordStart = tempstring.find("-", 0);                                //Nummer Speichern
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
            List_Insert(netz.link, tempLink);                   //Beschriebenes link-Objekt zur Liste hinzufügen
            zaehler[1]++;
        }

        if(datentyp == "RailwayNode")           //Horten
        {
            struct Element<RailwayNode> *tempNode = new struct Element<RailwayNode>;

            wordStart = tempstring.find("-", 0);                        //Nummer Speichern
            wordEnd = tempstring.find("\x9", wordStart);
            wort = tempstring.substr(wordStart+1,wordEnd-wordStart);
            tempNode->schluessel.nummer = stoi(wort);

            wordStart = tempstring.find(":\"", wordEnd);                //Breitengrad speichern
            wordEnd = tempstring.find(" ", wordStart);
            wort = tempstring.substr(wordStart+2,wordEnd-wordStart-1);
            tempNode->schluessel.breitengrad = stod(wort);

            wordStart = wordEnd+1;                                      //Höhengrad speichern
            wordEnd = tempstring.find("\"", wordStart);
            wort = tempstring.substr(wordStart,wordEnd-wordStart);
            tempNode->schluessel.hoehengrad = stod(wort);

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
            else
            {
                tempNode->schluessel.spokeEnd[0] = -1;
                tempNode->schluessel.spokeEnd[1] = -1;
                wordEnd -= 1;
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

            List_Insert(netz.node, tempNode);                   //Beschriebenes node-Objekt zur Liste hinzufügen
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
            snode->schluessel.breitengrad = gmlpos1;
            snode->schluessel.hoehengrad = gmlpos2;
            snode->schluessel.typ = typ;
            snode->schluessel.text = stadt;

            List_Insert(netz.stationNode, snode);               //Beschriebenes node-Objekt zur Liste hinzufügen

            zaehler[3]++;
        }

        if(datentyp == "RailwayStationCode")
        {
            struct Element<RailwayStationCode> *code = new struct Element<RailwayStationCode>;

            wordStart=tempstring.find("Spd-")+4;                                //Nummer einlesen
            code->schluessel.nummer=stoi(tempstring.substr(wordStart,7));

            wordStart=tempstring.find("SNode-")+6;                              //SNodeNummer einlesen
            code->schluessel.SNodeNummer=stoi(tempstring.substr(wordStart,6));

            wordStart=99;
            wordEnd=tempstring.find("\"}")-1-wordStart;                         //Code einlesen
            code->schluessel.code=tempstring.substr(wordStart-2,wordEnd+3);

            List_Insert(netz.stationCode, code);                //Beschriebenes code-Objekt zur Liste hinzufügen
            zaehler[4]++;
        }
    }
    cout << zaehler[0] <<" RailwayLine gelesen" << endl;
    cout << zaehler[1] <<" RailwayLink gelesen" << endl;
    cout << zaehler[2] <<" RailwayNode gelesen" << endl;
    cout << zaehler[3] <<" RailwayStationNode gelesen" << endl;
    cout << zaehler[4] <<" RailwayStationCode gelesen" << endl;

    cout << "entferne pseudoNodes ... ";
    removePseudoNodes(netz);
    cout << "fertig" << endl;

    cout << "berechne Stations ... ";
    mergeStationNodes(netz);
    cout << "fertig" << endl;
}

void removePseudoNodes(bahn_netz &netz)                                                     //Bojagic
{
    struct Element<RailwayNode> *currNode = netz.node.kopf;
    struct Element<RailwayNode> *deleteNode;
    struct Element<RailwayNode> *linkedNodeA;
    struct Element<RailwayNode> *linkedNodeB;
    int *linkA;
    int *linkB;
    while(currNode != nullptr)          //laufe durch alle Nodes
    {
        if(currNode->schluessel.typ == "pseudoNode")        //pseudoNode gefunden?
        {
            linkedNodeA = netz.node.kopf;
            while(linkedNodeA != nullptr)                   //In allen Nodes nach Verbindungen suchen
            {
                linkA = findLink(currNode, linkedNodeA);
                if(linkA != nullptr)
                    break;

                linkedNodeA = linkedNodeA->nachf;           //nächster Node
            }

            if(linkedNodeA == nullptr)
            {
                linkedNodeA = netz.stationNode.kopf;
                while(linkedNodeA != nullptr)                 //Nach Verbindung mit StationNodes suchen
                {
                    linkA = findLink(currNode, linkedNodeA);
                    if(linkA != nullptr)
                    break;

                    linkedNodeA = linkedNodeA->nachf;           //nächster StationNode
                }
            }

            if(linkedNodeA != nullptr)              //Wenn erste Verbindung gefunden wurde
            {                                       //Wird nach zweiter Verbindung gesucht
                if(linkedNodeA->nachf == nullptr && linkedNodeA->schluessel.typ != "railwayStop")   //linkedNodeA am ende von der Node Liste?
                    linkedNodeB = netz.stationNode.kopf;                //bei StationNodes weiter machen
                else
                    linkedNodeB = linkedNodeA->nachf;                   //sonst nach linkedNodeA ind selber Liste weiter machen

                while(linkedNodeB != nullptr)                   //In allen Nodes nach Verbindungen suchen
                {
                    linkB = findLink(currNode, linkedNodeB);
                    if(linkB != nullptr)
                        break;

                    linkedNodeB = linkedNodeB->nachf;           //nächster Node
                }

                if(linkedNodeB == nullptr && linkedNodeA->schluessel.typ != "railwayStop")      //Mit linkedNodeB erfolglos die Nodes durchsucht?
                {                                                                               //wenn er schon die SNodes durchlaufen ist dann nicht nochmal
                    linkedNodeB = netz.stationNode.kopf;
                    while(linkedNodeB != nullptr)                   //In allen Nodes nach Verbindungen suchen
                    {
                        linkB = findLink(currNode, linkedNodeB);
                        if(linkB != nullptr)
                            break;

                        linkedNodeB = linkedNodeB->nachf;           //nächster Node
                    }
                }
            }

            if(linkedNodeA != nullptr && linkedNodeB != nullptr)    //Zwei Links gefunden?
                *linkB = *linkA;

            deleteNode = currNode;
            currNode = currNode->nachf;         //nächster Node
            List_Delete(netz.node, deleteNode);        //gefundener pseudoNode löschen
        }
        else
            currNode = currNode->nachf;         //nächster Node
    }
}

void mergeStationNodes(bahn_netz &netz)                                                     //Horten
{
    try
    {
        struct Element<Station> *tempStation;
        struct Element<RailwayNode> *currSNode = netz.stationNode.kopf;
        struct Element<RailwayStationCode> *currCode = netz.stationCode.kopf;
        struct Element<Station> *currStation;

        bool gefunden;
        while(currSNode != nullptr)             //laufe durch alle StationNodes
        {
            gefunden = false;
            currStation =netz.station.kopf;
            while(currStation != nullptr)           //laufe durch alle Stations (Am Anfang gibt es noch keine Stations)
            {
                if(currCode->schluessel.code == currStation->schluessel.code)           //Passende Station mit gleichem Code gefunden?
                {
                    gefunden = true;
                    if(currSNode->schluessel.spokeEnd[0] != -1)                         //Wenn spokeEnd vorhanden
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
                    break;                              //Einmaliges finden einer passenden Station reicht
                                                        //Die Schleife muss dann nicht weiter durch alle Stations gehen
                }
                currStation = currStation->nachf;       //Zur nächsten Station
            }
            if(!gefunden)                               //Benötiigte Station nicht in der Liste gefunden?
            {
                tempStation = new struct Element<Station>;                          //Neue Station anlegen

                tempStation->schluessel.code = currCode->schluessel.code;               //Daten neue Station schreiben
                tempStation->schluessel.text = currSNode->schluessel.text;
                tempStation->schluessel.breitengrad = currSNode->schluessel.breitengrad;
                tempStation->schluessel.hoehengrad = currSNode->schluessel.hoehengrad;

                if(currSNode->schluessel.spokeEnd[0] != -1)                         //Wenn spokeEnd vorhanden
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

                List_Insert(netz.station, tempStation);             //Erstelle Station an die Liste anhängen
            }

            currSNode = currSNode->nachf;   //Nächste StationNode und nächster StationCode
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

	i = 1;
	currNode = netz.node.kopf;
	while(currNode != nullptr)                                                      // Geht die ganze Liste der Railwaynodes durch mit currNode
	{
		os << "V " << i << " \"" << currNode->schluessel.nummer << "\"" << endl;  // Und schreibt den aktuellen Node jeweils direkt als Knoten in die Datei

		j=1;
		currNode2 = netz.node.kopf;
		while(currNode2 != nullptr)                                     // Geht die Liste der Railwaynodes durch mit currNode2
		{
            if(doNodesLink(currNode2->schluessel, currNode->schluessel))   // Und prüft ob mit currNode eine verbindung besteht (Selbe Node Nummern ausgeschlossen)
            {
                os << "E " << i << " " << j << " " << abstandNodes(currNode2->schluessel, currNode->schluessel) << endl;        // Falls eine verbindung besteht, wird die Kante aufgeschrieben
            }
            j++;
            currNode2 = currNode2->nachf;
		}

		j=1;
		currStation2 = netz.station.kopf;
		while(currStation2 != nullptr)                                          // Geht die Liste der Stations durch mit currStation2
        {
            if(doStationLinkNode(currStation2->schluessel, currNode->schluessel))    // Und prüft ob mit currNode eine Verbindung besteht
            {
                os << "E " << i << " " << j+anzNodes << " " << abstandStationNode(currStation2->schluessel, currNode->schluessel) << endl;     // Falls eine Verbindung besteht, wird die Kante aufgeschrieben
            }
            j++;
            currStation2 = currStation2->nachf;
        }
        i++;
        currNode = currNode->nachf;
	}

    i = 1;
	currStation = netz.station.kopf;
	while(currStation != nullptr)                                                               // Geht die ganze Liste der Stationen durch mit currStation
	{
		os << "V " << i+anzNodes << " \"" << currStation->schluessel.code << "\"" << endl;    // Und schreibt die aktuelle Station jeweils direkt als Knoten in die Datei

		j=1;
		currNode2 = netz.node.kopf;
		while(currNode2 != nullptr)                                             // Geht die Liste der Railwaynodes durch mit currNode2
		{
                if(doStationLinkNode(currStation->schluessel, currNode2->schluessel))        // Und prüft ob mit currStation eine Verbindung besteht
                {
                    os << "E " << i+anzNodes << " " << j << " " << abstandStationNode(currStation->schluessel, currNode2->schluessel) << endl;   // Falls eine Verbindung besteht, wird die Kante aufgeschrieben
                }
            j++;
            currNode2 = currNode2->nachf;
		}

		j=1;
		currStation2 = netz.station.kopf;
		while(currStation2 != nullptr)                                                  // Geht die Liste der Stations durch mit currStation2
        {
            if(doStationsLink(currStation2->schluessel, currStation->schluessel))       // Und prüft ob mit currStation eine Verbindung besteht (Selbe Station Nummern ausgeschlossen)
            {
                os << "E " << i+anzNodes << " " << j+anzNodes << " " << abstandStationen(currStation2->schluessel, currStation->schluessel) << endl;  // Falls eine Verbindung besteht, wird die Kante aufgeschrieben
            }
            j++;
            currStation2 = currStation2->nachf;
        }
        i++;
        currStation = currStation->nachf;
	}
}

int* findLink(struct Element<RailwayNode> *NodeA, struct Element<RailwayNode> *NodeB)
{
    if(NodeA->schluessel.nummer == NodeB->schluessel.nummer)    //Gleiche Knoten
        return nullptr;

    if(NodeA->schluessel.spokeStart[0] == NodeB->schluessel.spokeEnd[0] && NodeA->schluessel.spokeStart[0] !=-1)
        return &(NodeB->schluessel.spokeEnd[0]);
    if(NodeA->schluessel.spokeStart[0] == NodeB->schluessel.spokeEnd[1] && NodeA->schluessel.spokeStart[0] !=-1)
        return &(NodeB->schluessel.spokeEnd[1]);
    if(NodeA->schluessel.spokeStart[1] == NodeB->schluessel.spokeEnd[0] && NodeA->schluessel.spokeStart[1] !=-1)
        return &(NodeB->schluessel.spokeEnd[0]);
    if(NodeA->schluessel.spokeStart[1] == NodeB->schluessel.spokeEnd[1] && NodeA->schluessel.spokeStart[1] !=-1)
        return &(NodeB->schluessel.spokeEnd[1]);

    if(NodeA->schluessel.spokeEnd[0] == NodeB->schluessel.spokeStart[0] && NodeA->schluessel.spokeEnd[0] !=-1)
        return &(NodeB->schluessel.spokeStart[0]);
    if(NodeA->schluessel.spokeEnd[0] == NodeB->schluessel.spokeStart[1] && NodeA->schluessel.spokeEnd[0] !=-1)
        return &(NodeB->schluessel.spokeStart[1]);
    if(NodeA->schluessel.spokeEnd[1] == NodeB->schluessel.spokeStart[0] && NodeA->schluessel.spokeEnd[1] !=-1)
        return &(NodeB->schluessel.spokeStart[0]);
    if(NodeA->schluessel.spokeEnd[1] == NodeB->schluessel.spokeStart[1] && NodeA->schluessel.spokeEnd[1] !=-1)
        return &(NodeB->schluessel.spokeStart[1]);

    return nullptr;
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

bool doStationsLink(Station stationA, Station stationB)
{
    if(stationA.code == stationB.code)
        return false;

    struct Element<int> *currSpokeA;
    struct Element<int> *currSpokeB;

    currSpokeA = stationA.spokeEnd.kopf;
    while(currSpokeA != nullptr)
    {
        currSpokeB = stationB.spokeStart.kopf;
        while(currSpokeB != nullptr)
        {
            if(currSpokeA->schluessel == currSpokeB->schluessel && currSpokeA->schluessel != -1)
                return true;

            currSpokeB = currSpokeB->nachf;
        }

        currSpokeA = currSpokeA->nachf;
    }

   currSpokeA = stationA.spokeStart.kopf;
   while(currSpokeA != nullptr)
    {
        currSpokeB = stationB.spokeEnd.kopf;
        while(currSpokeB != nullptr)
        {
            if(currSpokeA->schluessel == currSpokeB->schluessel && currSpokeA->schluessel != -1)
                return true;

            currSpokeB = currSpokeB->nachf;
        }

        currSpokeA = currSpokeA->nachf;
    }

    return false;
}

bool doStationLinkNode(Station station, RailwayNode node)                                   //Benger
{
    struct Element<int> *currSpoke;

    currSpoke=station.spokeEnd.kopf;
    while(currSpoke!=nullptr) //Durchläuft alle SpokeEnds einer Station
    {
        if(currSpoke->schluessel == node.spokeStart[0]) //Falls Verbindung gefunden
            return true;
        if(currSpoke->schluessel == node.spokeStart[1])
            return true;

        currSpoke=currSpoke->nachf;
    }

    currSpoke=station.spokeStart.kopf;
    while(currSpoke!=nullptr) //Durchläuft alle SpokeStart einer Station
    {
        if(currSpoke->schluessel == node.spokeEnd[0])
            return true;
        if(currSpoke->schluessel == node.spokeEnd[1])
            return true;

    currSpoke=currSpoke->nachf;
    }
    return false;
}

int abstandStationen(Station bahnhofA, Station bahnhofB)
{
    double phiA = (bahnhofA.breitengrad - 90) / 180 * M_PI;         //Breitengrad/Latidude -> phi
    double lamA = bahnhofA.hoehengrad / 180 * M_PI;                 //Höhengrad/Longitude -> lambda

    double phiB = (bahnhofB.breitengrad - 90) / 180 * M_PI;
    double lamB = bahnhofB.hoehengrad / 180 * M_PI;

    double delta = acos( sin(phiA)*sin(phiB) + cos(phiA)*cos(phiB)*cos(lamA-lamB) );    //Winkel in Bogenmaß

    return(ceil(E_RAD * delta));          //E_RAD = Erdradius
}

int abstandNodes(RailwayNode bahnhofA, RailwayNode bahnhofB)
{
    double phiA = (bahnhofA.breitengrad - 90) / 180 * M_PI;         //Breitengrad/Latidude -> phi
    double lamA = bahnhofA.hoehengrad / 180 * M_PI;                 //Höhengrad/Longitude -> lambda

    double phiB = (bahnhofB.breitengrad - 90) / 180 * M_PI;
    double lamB = bahnhofB.hoehengrad / 180 * M_PI;

    double delta = acos( sin(phiA)*sin(phiB) + cos(phiA)*cos(phiB)*cos(lamA-lamB) );    //Winkel in Bogenmaß

    return(ceil(E_RAD * delta));          //E_RAD = Erdradius
}

int abstandStationNode(Station bahnhofA, RailwayNode bahnhofB)
{
    double phiA = (bahnhofA.breitengrad - 90) / 180 * M_PI;         //Breitengrad/Latidude -> phi
    double lamA = bahnhofA.hoehengrad / 180 * M_PI;                 //Höhengrad/Longitude -> lambda

    double phiB = (bahnhofB.breitengrad - 90) / 180 * M_PI;
    double lamB = bahnhofB.hoehengrad / 180 * M_PI;

    double delta = acos( sin(phiA)*sin(phiB) + cos(phiA)*cos(phiB)*cos(lamA-lamB) );    //Winkel in Bogenmaß

    return(ceil(E_RAD * delta));          //E_RAD = Erdradius
}

//Ausgabe Operatoren zum Testen

ostream &operator<<(ostream &ostr, const RailwayNode node)
{
    ostr << "Nummer        : " << node.nummer      << endl;
    ostr << "Breitengrad   : " << node.breitengrad << endl;
    ostr << "Hoehengrad    : " << node.hoehengrad  << endl;
    ostr << "spokeEnd      : " << node.spokeEnd[0];
    if(node.spokeEnd[1] != -1)
        ostr << ", " << node.spokeEnd[1];
    ostr << endl;
    ostr << "spokeStart: " << node.spokeStart[0];
    if(node.spokeStart[1] != -1)
        ostr << ", " << node.spokeStart[1];
    ostr << endl;
    ostr << "text          : " << node.text         << endl;
    ostr << "formOfNode    : " << node.typ          << endl;

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
    ostr << "Nummer     : " << station.code        << endl;
    ostr << "Start Node : ";
    List_Print(station.spokeStart);
    ostr << "End   Node : ";
    List_Print(station.spokeEnd);
    ostr << "Breitengrad: " << station.breitengrad << endl;
    ostr << "Hoehengrad : " << station.hoehengrad  << endl;
    ostr << "Text       : " << station.text        << endl;

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



void Save_DBWithNNodes(ostream &os, bahn_netz &netz, int anzNodes)
{
    struct Element<RailwayNode> *currNode = netz.node.kopf;
    struct Element<Station> *currStation = netz.station.kopf;

	struct Element<RailwayNode> *tempNode;
    struct Element<Station> *tempStation;

    bahn_netz netzMitNNodes;

    int i = 0;
    while(i < anzNodes && currNode != nullptr)
    {
        tempNode = kopiereNode(currNode);
        List_Insert(netzMitNNodes.node, tempNode);
        currNode = currNode->nachf;
        i++;
    }

    while(i < anzNodes && currStation != nullptr)
    {
        tempStation = kopiereStation(currStation);
        List_Insert(netzMitNNodes.station, tempStation);
        currStation = currStation->nachf;
        i++;
    }

    Save_DB(os, netzMitNNodes);
}

struct Element<RailwayNode> * kopiereNode(struct Element<RailwayNode> *sourceNode)
{
    if(sourceNode == nullptr)
        return nullptr;

    struct Element<RailwayNode> *newNode = new struct Element<RailwayNode>;

    newNode->schluessel.nummer = sourceNode->schluessel.nummer;
    newNode->schluessel.breitengrad = sourceNode->schluessel.breitengrad;
    newNode->schluessel.hoehengrad = sourceNode->schluessel.hoehengrad;
    newNode->schluessel.spokeEnd[0] = sourceNode->schluessel.spokeEnd[0];
    newNode->schluessel.spokeEnd[1] = sourceNode->schluessel.spokeEnd[1];
    newNode->schluessel.spokeStart[0] = sourceNode->schluessel.spokeStart[0];
    newNode->schluessel.spokeStart[1] = sourceNode->schluessel.spokeStart[1];
    newNode->schluessel.text = sourceNode->schluessel.text;
    newNode->schluessel.typ = sourceNode->schluessel.typ;

    return newNode;
}

struct Element<Station> * kopiereStation(struct Element<Station> *sourceStation)
{
    if(sourceStation == nullptr)
        return nullptr;

    struct Element<Station> *newStation = new struct Element<Station>;

    newStation->schluessel.code = sourceStation->schluessel.code;
    newStation->schluessel.spokeEnd = sourceStation->schluessel.spokeEnd;
    newStation->schluessel.spokeStart = sourceStation->schluessel.spokeStart;
    newStation->schluessel.breitengrad = sourceStation->schluessel.breitengrad;
    newStation->schluessel.hoehengrad = sourceStation->schluessel.hoehengrad;
    newStation->schluessel.text = sourceStation->schluessel.text;

    return newStation;
}

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

        //testkommentar

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

            netz.stationCode.add_last(code);
            zaehler[4]++;
        }
    }
    correctLinks(netz);
    cout << zaehler[0] <<" RailwayLine gelesen" << endl;
    cout << zaehler[1] <<" RailwayLink gelesen" << endl;
    cout << zaehler[2] <<" RailwayNode gelesen" << endl;
    cout << zaehler[3] <<" RailwayStationNode gelesen" << endl;
    cout << zaehler[4] <<" RailwayStationCode gelesen" << endl;
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


//funktioniert nicht
/*void Save_DB(ostream &os, bahn_netz &netz, size_t anzNode)
{
    os << "G " << anzNode << endl;
    int nodeNummern[anzNode];
    int start[anzNode][2], ende[anzNode][2];

    for(size_t i=0; i<anzNode; i++)                                     //anzNode viele Konten speichern
    {
        nodeNummern[i] = netz.node[i].nummer;
        start[i][0] = netz.node[i].spokeStart[0];
        start[i][1] = netz.node[i].spokeStart[1];
        ende[i][0] = netz.node[i].spokeEnd[0];
        ende[i][1] = netz.node[i].spokeEnd[1];

        os << "V " << i+1 << " \"" << nodeNummern[i] << "\"" << endl;
    }

    for(size_t i=0; i<anzNode; i++)
    {
        for(size_t j=0; j<anzNode; j++)
        {
            if(i!=j)
            {
                if( (start[i][0] != -1 && (start[i][0] == ende[j][0])) || (ende[i][0] != -1 && (ende[i][0] == start[j][0])) )
                {
                    os << "E " << i+1 << " " << j+1 << " 1" << endl;
                    os << "E " << j+1 << " " << i+1 << " 1" << endl;
                }
                else if( (start[i][0] != -1 && (start[i][0] == ende[j][1])) || (ende[i][1] != -1 && (ende[i][1] == start[j][0])) )
                {
                    os << "E " << i+1 << " " << j+1 << " 1" << endl;
                    os << "E " << j+1 << " " << i+1 << " 1" << endl;
                }
                else if( (start[i][1] != -1 && (start[i][1] == ende[j][0])) || (ende[i][0] != -1 && (ende[i][0] == start[j][1])) )
                {
                    os << "E " << i+1 << " " << j+1 << " 1" << endl;
                    os << "E " << j+1 << " " << i+1 << " 1" << endl;
                }
                else if( (start[i][1] != -1 && (start[i][1] == ende[j][1])) || (ende[i][1] != -1 && (ende[i][1] == start[j][1])) )
                {
                    os << "E " << i+1 << " " << j+1 << " 1" << endl;
                    os << "E " << j+1 << " " << i+1 << " 1" << endl;
                }
            }
        }
    }

}*/
//------------------------------------------------------------------------------------------
void Save_DB(FILE *fp, bahn_netz &netz, string startCode, size_t maxEntfernung)
{

    //todo: Save_DB fertig machen
    verbindungen(fp, netz, netz.node[0], maxEntfernung);
}
//------------------------------------------------------------------------------------------
bool doNodesLink(RailwayNode NodeA, RailwayNode NodeB)
{
    if(NodeA.nummer == NodeB.nummer)    //Gleiche Knoten
        return false;

    if(NodeA.spokeStart[0] == NodeB.spokeEnd[0] && NodeA.spokeStart[0] !=-1)
        return true;
    if(NodeA.spokeStart[0] == NodeB.spokeEnd[1] && NodeA.spokeStart[0] !=-1)
        return true;
    if(NodeA.spokeStart[1] == NodeB.spokeEnd[0] && NodeA.spokeStart[0] !=-1)
        return true;
    if(NodeA.spokeStart[1] == NodeB.spokeEnd[1] && NodeA.spokeStart[0] !=-1)
        return true;

    if(NodeA.spokeEnd[0] == NodeB.spokeStart[0] && NodeA.spokeEnd[0] !=-1)
        return true;
    if(NodeA.spokeEnd[0] == NodeB.spokeStart[1] && NodeA.spokeEnd[0] !=-1)
        return true;
    if(NodeA.spokeEnd[1] == NodeB.spokeStart[0] && NodeA.spokeEnd[0] !=-1)
        return true;
    if(NodeA.spokeEnd[1] == NodeB.spokeStart[1] && NodeA.spokeEnd[0] !=-1)
        return true;

    return false;
}
//------------------------------------------------------------------------------------------
//schreibt verbindungen in .gdi Datei in gegebenes Format und returnt am Ende die Gesamtzahl an Knoten
//Knotenanzahl muss sepperat an den Anfang geschrieben werden
size_t verbindungen(FILE *fp, bahn_netz &netz, RailwayNode currentNode, size_t maxEntfernung, size_t durchlaeufe, size_t altKnotenZahl)
{
    static size_t knotenZahl = 0;
    char zeichen = NULL;
    size_t nummer;
    if(maxEntfernung == durchlaeufe)
       return knotenZahl;

    /*while(getline(fp, tempStr))
        if(tempStr.find("\"" + std::to_string(currentNode.nummer) + "\"") == string::npos)
            return knotenZahl;*/

    fseek(fp, 0, SEEK_SET);
    while(zeichen != EOF)
    {
        zeichen = fgetc(fp);
        if(zeichen == 'V')
        {
            while(fgetc(fp) != '"');
            fscanf(fp, "%d", &nummer);
            if(nummer == currentNode.nummer)
                return knotenZahl;
        }
    }


    //funktioniert noch nicht ganz
    for(int i=knotenZahl+1; i<netz.node.number_Elements(); i++)
        if(doNodesLink(currentNode, netz.node[i]))
        {
            knotenZahl++;
            fprintf(fp, "V %d \"%d\"\n", knotenZahl, netz.node[i].nummer);
            fprintf(fp, "E %d %d\n", altKnotenZahl, knotenZahl);
            verbindungen(fp, netz, netz.node[i], maxEntfernung, ++durchlaeufe, knotenZahl);
        }

    return knotenZahl;
}
//------------------------------------------------------------------------------------------
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

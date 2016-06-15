/*
Deze file voorziet alle methodes die nodig zijn voor de hulpdiensten simulatie. En word simpelweg in Main.cpp opgeroepen.

Authors: Tim Goossens & Aron Cornet
Date: 15/06/2016
Version: 2.1
*/

#include "tinyxml.h"
#include "stadsdelen.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <stdlib.h>
#include <time.h>
#include "DesignByContract.h"
#define TIXML_USE_STL

OutputClass output;
UI ui;

Location::Location()
{
    _initCheck = this;
    //ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
Location::Location(int x, int y)
{
     _initCheck = this;
	X = x;
	Y = y;
    //ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}

int Location::getX()
{
 //REQUIRE(this->properlyInitialized(), "Location wasn't initialized when calling getX");   
	return X;
}
int Location::getY()
{
    //REQUIRE(this->properlyInitialized(), "Location wasn't initialized when calling GetY");
	return Y;
}
void Location::setX(int x)
{
    //REQUIRE(this->properlyInitialized(), "Location wasn't initialized when calling setX");
	X = x;
}
void Location::setY(int y)
{
    //REQUIRE(this->properlyInitialized(), "Location wasn't initialized when calling setY");
	Y = y;
}

bool Location::properlyInitialized()
{
	return _initCheck == this;
}
/*
	METHODES VOOR DE STAD
*/
Stad::Stad(void)
{
	_initCheck = this;
	for (int i = 0; i < yrow; i++) {
		for (int j = 0; j < xrow; j++) {
			grid[i][j] = nullptr;
		}
	}
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "Project Software Engineering - door Aron Cornet en Tim Goossens" );
	output.writeToStatus( "Specificatie 2.0" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );

	output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToConsole("Project Software Engineering - door Aron Cornet en Tim Goossens" );
	output.writeToConsole("Specificatie 2.0" );
	output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Stad::properlyInitialized()
{
	return _initCheck == this;
}
void Stad::setVstadsdeel(Vstadsdeel &s)
{
	//REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling setVstadsdeel");
	if ((dynamic_cast<Mbrandweerkazerne*>(&s)) != nullptr) {
		Location l = s.getLocatie();
		for (int i = l.getY(); i > (l.getY() - 4); i--) {
			for (int j = l.getX(); j < (l.getX() + 4); j++) {
				grid[i][j] = &s;
			}
		}
	}
	else if ((dynamic_cast<Mpolitiebureau*>(&s)) != nullptr) {
		Location l = s.getLocatie();
		Location g = s.getGrootte();
		for (int i = l.getY(); i >(l.getY() - g.getY()); i--) {
			for (int j = l.getX(); j < (l.getX() + g.getX()); j++) {
				grid[i][j] = &s;
			}
		}
	}
	else if ((dynamic_cast<MziekenMhuis*>(&s)) != nullptr) {
		Location l = s.getLocatie();
		Location g = s.getGrootte();
		for (int i = l.getY(); i >(l.getY() - g.getY()); i--) {
			for (int j = l.getX(); j < (l.getX() + g.getX()); j++) {
				grid[i][j] = &s;
			}
		}
	}
	else if ((dynamic_cast<Mstraat*>(&s)) != nullptr) {
		Location b = s.getBegin();  Location e = s.getEinde();
		if (b.getX() == e.getX()) {
			for (int i = (b.getY()); i <= (e.getY()); i++) {
				Vstadsdeel* t = grid[i][b.getX()];
				if (t == nullptr) {
					grid[i][b.getX()] = &s;
				}
				else {
					if (grid[i][b.getX()]->getType() == 6) {
						Mkruispunt* k = new Mkruispunt();
						k->setEersteMstraat(grid[i][b.getX()]->getNaam());
						k->setTweedeMstraat(s.getNaam());
						k->setLocatie(s.getLocatie());
						grid[i][b.getX()] = k;
					}
					else {
						break; // HIER MOET NOG WAT WERK VERRICHT WORDEN VANWEGE DE BREAK-OUT
					}
				}
			}
		}
		else {
			for (int i = (b.getX()); i <= (e.getX()); i++) {
				Vstadsdeel* t = grid[b.getY()][i];
				if (t == nullptr) {
					grid[b.getY()][i] = &s;
				}
				else {
					if (grid[i][b.getX()]->getType() == 3) {
						Mkruispunt* k = new Mkruispunt();
						k->setEersteMstraat(grid[i][b.getX()]->getNaam());
						k->setTweedeMstraat(s.getNaam());
						k->setLocatie(s.getLocatie());
						grid[b.getY()][i] = k;
					}
					else {
						break; // HIER MOET NOG WAT WERK VERRICHT WORDEN VANWEGE DE BREAK-OUT
					}
				}
			}
		}
	}
	else if ((dynamic_cast<Mwinkel*>(&s)) != nullptr) {
		Location l = s.getLocatie();
		Location g = s.getGrootte();
		for (int i = l.getY(); i >(l.getY() - g.getY()); i--) {
			for (int j = l.getX(); j < (l.getX() + g.getX()); j++) {
				grid[i][j] = &s;
			}
		}
	}
	else if ((dynamic_cast<Mhuis*>(&s)) != nullptr) {
		Location l = s.getLocatie();
		for (int i = l.getY(); i >(l.getY() - 2); i--) {
			for (int j = l.getX(); j < (l.getX() + 2); j++) {
				grid[i][j] = &s;
			}
		}
	}
}
Vstadsdeel* Stad::getVstadsdeel(Location p)
{
	//REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling getVstadsdeel");
	if ((p.getX() >= 0 && p.getX() <= xrow) && (p.getY() >= 0 && p.getY() <= yrow)) {
		return grid[p.getY()][p.getX()];
	}
	else {
		return nullptr;
	}
}

bool Stad::checkAangrenzing(Vstadsdeel* s)
{
	//REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling checkAangrenzing");
	int aantal = 0;
	if ((dynamic_cast<Mpolitiebureau*>(s)) != nullptr) {
		aantal = 4;
	}
	else if ((dynamic_cast<Mhuis*>(s)) != nullptr) {
		aantal = 2;
	}
	Location p = s->getLocatie();
	Location l;
	Vstadsdeel* t;
	for (int i = (p.getY() + 1); i > (p.getY() - aantal - 1); i--) {
		for (int j = (p.getX() - 1); j < (p.getX() + aantal + 1); j++) {
			l.setX(j); l.setY(i);
			t = getVstadsdeel(l);
			if ((dynamic_cast<Mstraat*>(t)) != nullptr) {
				return true;
			}
		}
	}
	return false;
}
void Stad::printStad()
{
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling printStad");
	Vstadsdeel* vak;
	output.writeToStatus( "Legenda:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "H = Onderdeel van een Huis" );
	output.writeToStatus( "# = Onderdeel van een brandend Huis");
	output.writeToStatus( "K = Onderdeel van de Kazerne" );
	output.writeToStatus( "Z = Onderdeel van het Ziekenhuis" );
	output.writeToStatus( "P = Onderdeel van het Politiebureau" );
	output.writeToStatus( "W = Onderdeel van de Winkel" );
	output.writeToStatus( "  = Onderdeel van een Straat" );
	output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
	output.writeToStatus( "* = Brandweerwagen" );
	output.writeToStatus( "= = Ambulance");
	output.writeToStatus( "% = Politiewagen");
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = (yrow-1); i >= 0; i--) {
		std::string lijn = "";
		for (int j = 0; j < xrow; j++) {
			bool found = false;
			Location p;
			p.setY(i); p.setX(j);
			Vstadsdeel* t = getVstadsdeel(p);
			if (dynamic_cast<Mhuis*>(t) != nullptr || dynamic_cast<Mwinkel*>(t) != nullptr) {
				if (t->getInBrand()) {
					lijn = lijn + "#" + " ";
					found = true;
				}
			}
			for (int k = 0; k < Mbrandweerwagens_onroad.size(); k++) {
				
				if (i == Mbrandweerwagens_onroad[k].second->getLocatie().getY() && j == Mbrandweerwagens_onroad[k].second->getLocatie().getX()) {
					lijn = lijn + "*" + " ";
					found = true;
				}
			}
			for (int k = 0; k < Mambulances_onroad.size(); k++) {
				if (i == Mambulances_onroad[k].second->getLocatie().getY() && j == Mambulances_onroad[k].second->getLocatie().getX()) {
					lijn = lijn + "=" + " ";
					found = true;
				}
				for (int k = 0; k < Mpolitiewagens_onroad.size(); k++) {
					if (i == Mpolitiewagens_onroad[k].second->getLocatie().getY() && j == Mpolitiewagens_onroad[k].second->getLocatie().getX()) {
						lijn = lijn + "%" + " ";
						found = true;
					}
				}
			}
			if (!found) {
				vak = grid[i][j];
				lijn = lijn + vak->getKarakter() + " ";
			}
		}
		output.writeToStatus( lijn );
	}
	output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
}
bool Stad::checkBestemming(Vstadsdeel* s, Location p)
{
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling checkBestemming");
	Vstadsdeel* temp;
	Location coordinaat;
	// RECHTS
	coordinaat.setX(p.getX() + 1); coordinaat.setY(p.getY());
	temp = getVstadsdeel(coordinaat);
	if (temp == s) {
		//std::cout + "RECHTS" );
		return true;
	}
	// LINKS
	coordinaat.setX(p.getX() - 1); coordinaat.setY(p.getY());
	temp = getVstadsdeel(coordinaat);
	if (temp == s) {
		//std::cout + "LINKS" );
		return true;
	}
	// ONDER
	coordinaat.setX(p.getX()); coordinaat.setY(p.getY() - 1);
	temp = getVstadsdeel(coordinaat);
	if (temp == s) {
		//std::cout + "ONDER" );
		return true;
	}
	// BOVEN
	coordinaat.setX(p.getX()); coordinaat.setY(p.getY() + 1);
	temp = getVstadsdeel(coordinaat);
	if (temp == s) {
		//std::cout + "BOVEN" );
		return true;
	}
	return false;
}
Stad::MstraatType Stad::checkMstraat(Vstadsdeel* s, Location p)
{
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling checkMstraat");
	Vstadsdeel* vak = getVstadsdeel(p);
	Location locatie = s->getLocatie();
	if ((dynamic_cast<Mstraat*>(vak)) != nullptr) {
		Location s1 = vak->getBegin();
		Location s2 = vak->getEinde();
		if ((s1.getX() - s2.getX()) == 0) {
			int x = (locatie.getX() - p.getY());
			if (x <= 1 && x >= -1) {
				return Stad::Verticaal;
			}
		}
		if ((s1.getY() - s2.getY()) == 0) {
			int y = (locatie.getY() - p.getX());
			if (y <= 2 && y >= -1) {
				return Stad::Horizontaal;
			}
			
		} 
	}
	if ((dynamic_cast<Mkruispunt*>(vak)) != nullptr) {
		int x = (locatie.getX() - p.getX());
		if (x <= 1 && x >= -1) {
			return Stad::Verticaal;
		}
		int y = (locatie.getY() - p.getY());
		if (y <= 2 && y >= -1) {
			return Stad::Horizontaal;
		}
	} return Stad::Geen;
}
Vstadsdeel::Richting Stad::checkRichting(Vstadsdeel* s, Location p)
{
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling checkRichting");
	Vstadsdeel* vak = getVstadsdeel(p);
	Location locatie = s->getLocatie();
	if ((dynamic_cast<Mstraat*>(vak)) != nullptr) {
		Location s1 = vak->getBegin();
		Location s2 = vak->getEinde();
		Location bovenlid; int bovenafstand = -1;
		Location onderlid; int onderafstand = -1;
		Location linkerlid; int linkerafstand = -1;
		Location rechterlid; int rechterafstand = -1;
		if ((s1.getX() - s2.getX()) == 0) {
			for (int i = p.getY(); i >= 0; i--) {
				Location p2; p2.setY(i); p2.setX(p.getX());
				Vstadsdeel* tijdelijk = getVstadsdeel(p2);
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr) {
					onderlid = p2;
					onderafstand = abs(onderlid.getY() - s->getLocatie().getY()) + abs(onderlid.getX() - s->getLocatie().getX());
					break;
				}
			}
			for (int i = p.getY(); i < yrow; i++) {
				Location p2; p2.setY(i); p2.setX(p.getX());
				Vstadsdeel* tijdelijk = getVstadsdeel(p2);
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr) {
					bovenlid = p2;
					bovenafstand = abs(bovenlid.getY() - s->getLocatie().getY()) + abs(bovenlid.getX() - s->getLocatie().getX());
					break;
				}
			}
			if (onderafstand == bovenafstand) {
				return Vstadsdeel::Richting::Onder;
			}
			else {
				if (onderafstand < bovenafstand) {
					return Vstadsdeel::Richting::Onder;
				}
				else {
					return Vstadsdeel::Richting::Boven;
				}
			}
			
		}
		if ((s1.getY() - s2.getY()) == 0) {
			for (int i = p.getX(); i >= 0; i--)
			{
				Location p2; p2.setY(p.getY()); p2.setX(i);
				Vstadsdeel* tijdelijk = getVstadsdeel(p2);
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr)
				{
					linkerlid = p2;
					linkerafstand = abs(linkerlid.getY() - s->getLocatie().getY()) + abs(linkerlid.getX() - s->getLocatie().getX());
					break;
				}
			}
			for (int i = p.getX(); i < xrow; i++) {
				Location p2; p2.setY(p.getY()); p2.setX(i);
				Vstadsdeel* tijdelijk = getVstadsdeel(p2);
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr)
				{
					rechterlid = p2;
					rechterafstand = abs(rechterlid.getY() - s->getLocatie().getY()) + abs(rechterlid.getX() - s->getLocatie().getX());
					break;
				}
			}
			if (linkerafstand == rechterafstand) {
				return Vstadsdeel::Richting::Links;
			}
			else {
				if (linkerafstand < rechterafstand) {
					return Vstadsdeel::Richting::Links;
				}
				else {
					return Vstadsdeel::Richting::Rechts;
				}
			}
			//std::cout + linkerafstand + "|" + rechterafstand );
		} 
	}
	if ((dynamic_cast<Mkruispunt*>(vak)) != nullptr) {
		Location onderlid; int bovenafstand = -1;
		Location bovenlid; int onderafstand = -1;
		Location linkerlid; int linkerafstand = -1;
		Location rechterlid; int rechterafstand = -1;
		Vstadsdeel::Richting finale; int finaleafstand = 100;
		for (int i = p.getY() - 1; i >= 0; i--) {
			Location p2; p2.setY(i); p2.setX(p.getX());
			Vstadsdeel* tijdelijk = getVstadsdeel(p2);
			if (tijdelijk != nullptr) {
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr) {
					onderlid = p2;
					onderafstand = abs(onderlid.getY() - s->getLocatie().getY()) + abs(onderlid.getX() - s->getLocatie().getX());
					break;
				}
			}
			else {
				break;
			}
		}
		for (int i = p.getY() + 1; i < yrow; i++) {
			Location p2; p2.setY(i); p2.setX(p.getX());
			Vstadsdeel* tijdelijk = getVstadsdeel(p2);
			if (tijdelijk != nullptr) {
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr) {
					bovenlid = p2;
					bovenafstand = abs(bovenlid.getY() - s->getLocatie().getY()) + abs(bovenlid.getX() - s->getLocatie().getX());
					break;
				}
			}
			else {
				break;
			}
		}
		for (int i = p.getX() - 1; i >= 0; i--) {
			Location p2; p2.setY(p.getY()); p2.setX(i);
			Vstadsdeel* tijdelijk = getVstadsdeel(p2);
			if (tijdelijk != nullptr) {
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr) {
					linkerlid = p2;
					linkerafstand = abs(linkerlid.getY() - s->getLocatie().getY()) + abs(linkerlid.getX() - s->getLocatie().getX());
					break;
				}
			}
			else {
				break;
			}
			
		}
		for (int i = p.getX() + 1; i < xrow; i++) {
			Location p2; p2.setY(p.getY()); p2.setX(i);
			Vstadsdeel* tijdelijk = getVstadsdeel(p2);
			if (tijdelijk != nullptr) {
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr) {
					rechterlid = p2;
					rechterafstand = abs(rechterlid.getY() - s->getLocatie().getY()) + abs(rechterlid.getX() - s->getLocatie().getX());
					break;
				}
			}
			else {
				break;
			}
		}
		if (onderafstand != -1) {
			if (onderafstand < finaleafstand) {
				finaleafstand = onderafstand;
				finale = Vstadsdeel::Richting::Onder;
			}
		}
		if (bovenafstand != -1) {
			if (bovenafstand < finaleafstand) {
				finaleafstand = bovenafstand;
				finale = Vstadsdeel::Richting::Boven;
			}
		}
		if (linkerafstand != -1) {
			if (linkerafstand < finaleafstand) {
				finaleafstand = linkerafstand;
				finale = Vstadsdeel::Richting::Links;
			}
		}
		if (rechterafstand != -1) {
			if (rechterafstand < finaleafstand) {
				finaleafstand = rechterafstand;
				finale = Vstadsdeel::Richting::Rechts;
			}
		}
		MstraatType verificatie = checkMstraat(s, p);
		if (verificatie == Horizontaal && (finale == Vstadsdeel::Boven || finale == Vstadsdeel::Onder)) {
			//std::cout + "Correctie" );
			if ((s->getLocatie().getX() - p.getX()) < 0) {
				return Vstadsdeel::Richting::Links;
			}
			else {
				return Vstadsdeel::Richting::Rechts;
			}
		}
		else if (verificatie == Verticaal && (finale == Vstadsdeel::Links || finale == Vstadsdeel::Rechts)) {
			//std::cout + "Correctie" );
			if ((s->getLocatie().getY() - p.getY()) < 0) {
				return Vstadsdeel::Richting::Onder;
			}
			else {
				return Vstadsdeel::Richting::Boven;
			}
		}
		else {
			return finale;
		}
	}
	return Vstadsdeel::Richting::Rechts;
}

bool Stad::stadInlezen(const char* s)
{	
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling stadInlezen");
	TiXmlDocument doc;
	TiXmlElement* root;
	if (!doc.LoadFile(s)) {
		output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
		output.writeToConsole("Failed to open/load file" );
		output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
		return false;
	}
	else {
		std::string s2 = s;
		output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
		output.writeToConsole("Loading file - " + s2 + " - succesful" );
		output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
		root = doc.FirstChildElement();
		if (root == NULL) {
			output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
			output.writeToConsole("Failed to load file : No root element" );
			output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
			return false;
		}
		else {
			for (TiXmlNode* n = root->FirstChild(); n != NULL; n = n->NextSibling()) {
				std::string nodeName = n->Value();
				std::string naam; std::string basis;
				Location van; Location naar; Location grootte; int i;
				int brandbaarheid; int overvalbaarheid;

				for (TiXmlElement* e = n->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
					std::string elementName = e->Value();
					int counter = 0;
					if (!elementName.compare("Naam")) {
						naam = e->GetText();
					}
					if (!elementName.compare("Basis")) {
						basis = e->GetText();
					}
					if (!elementName.compare("Brandbaarheid")) {
						brandbaarheid = atoi(e->GetText());
					}
					if (!elementName.compare("Overvalbaarheid")) {
						overvalbaarheid = atoi(e->GetText());
					}
					for (TiXmlAttribute* a = e->FirstAttribute(); a != NULL; a = a->Next()) {
						if (a != NULL) {
							if (!elementName.compare("Locatie")) {
								if (counter == 0) {
									try {
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> i;
										van.setX(i);
										counter++;
									}
									catch (...) {
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//van.first = atoi(a->Value());
								}
								else {
									try {
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> i;
										van.setY(i);
									}
									catch (...) {
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//van.second = atoi(a->Value());
								}
							}
							if (!elementName.compare("Ingang")) {
								if (counter == 0) {
									try {
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> i;
										naar.setX(i);
										counter++;
									}
									catch (...) {
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//naar.first = atoi(a->Value());
								}
								else {
									try {
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> i;
										naar.setY(i);
									}
									catch (...) {
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//naar.second = atoi(a->Value());
								}
							}
							/*

							*/
							if (!elementName.compare("Van")) {
								if (counter == 0) {
									try {
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> i;
										van.setX(i);
										counter++;
									}
									catch (...) {
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
								}
								else {
									try {
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> i;
										van.setY(i);
									}
									catch (...) {
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
								}
							}
							if (!elementName.compare("Naar")) {
								if (counter == 0) {
									try {
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> i;
										naar.setX(i);
										counter++;
									}
									catch (...) {
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
								}
								else {
									try {
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> i;
										naar.setY(i);
									}
									catch (...) {
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
								}
							}
							if (!elementName.compare("Grootte")) {
								if (counter == 0) {
									try {
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> i;
										grootte.setY(i);
										counter++;
									}
									catch (...) {
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
								}
								else {
									try{
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> i;
										grootte.setX(i);
									}
									catch (...) {
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//naar.second = atoi(a->Value());
								}
							}
						}
					}
				}
				if (!nodeName.compare("Brandweerkazerne")) {
					Mbrandweerkazerne *bk = new Mbrandweerkazerne();
					bk->setNaam(naam);
					bk->setLocatie(van);
					bk->setIngang(naar);
					setVstadsdeel(*bk);
					kazernes.push_back(bk);

					addGebouwCoord(van);//vector
				}
				if (!nodeName.compare("PolitieBureau")) {
					Mpolitiebureau *pb = new Mpolitiebureau();
					pb->setNaam(naam);
					pb->setLocatie(van);
					pb->setIngang(naar);
					pb->setGrootte(grootte);
					setVstadsdeel(*pb);
					bureaus.push_back(pb);
				}
				if (!nodeName.compare("Ziekenhuis")) {
					MziekenMhuis *zh = new MziekenMhuis();
					zh->setNaam(naam);
					zh->setLocatie(van);
					zh->setIngang(naar);
					zh->setGrootte(grootte);
					setVstadsdeel(*zh);
					ziekenhuizen.push_back(zh);
				}
				if (!nodeName.compare("Brandweerwagen")) {
					Mbrandweerwagen *bw = new Mbrandweerwagen();
					bw->setNaam(naam);
					bw->setBasis(basis);
					Location locatie; locatie.setX(0); locatie.setY(0);
					for (int i = 0; i < kazernes.size(); i++) {
						if (kazernes[i]->getNaam() == basis) {
							locatie = kazernes[i]->getIngang();
						}
					}
					if (locatie.getX() != 0 && locatie.getY() != 0) {
						bw->setLocatie(locatie);
					}
					Mbrandweerwagens.push_back(bw);
					Mbrandweerwagens_onstandby.push_back(bw);
				}
				if (!nodeName.compare("Politiewagen")) {
					Mpolitiewagen *pw = new Mpolitiewagen();
					pw->setNaam(naam);
					pw->setBasis(basis);
					Location locatie; locatie.setX(0); locatie.setY(0);
					for (int i = 0; i < bureaus.size(); i++) {
						if (bureaus[i]->getNaam() == basis) {
							locatie = bureaus[i]->getIngang();
						}
					}
					if (locatie.getX() != 0 && locatie.getY() != 0) {
						pw->setLocatie(locatie);
					}
					Mpolitiewagens.push_back(pw);
					Mpolitiewagens_onstandby.push_back(pw);
				}
				if (!nodeName.compare("Ziekenwagen")) {
					Mambulance *am = new Mambulance();
					am->setNaam(naam);
					am->setBasis(basis);
					Location locatie; locatie.setX(0); locatie.setY(0);
					for (int i = 0; i < ziekenhuizen.size(); i++) {
						if (ziekenhuizen[i]->getNaam() == basis) {
							locatie = ziekenhuizen[i]->getIngang();
							am->setLocatie(locatie);
						}
					}
					Mambulances.push_back(am);
					Mambulances_onstandby.push_back(am);
				}
				if (!nodeName.compare("Straat")) {
					Mstraat *s = new Mstraat();
					s->setNaam(naam);
					s->setBegin(van);
					s->setEinde(naar);
					setVstadsdeel(*s);

					std::pair<Location, Location> coords; //vector
                    coords.first = van; coords.second = naar;					
                    addMstraatCoord(coords);	
				}
				if (!nodeName.compare("Huis")) {
					Mhuis *h = new Mhuis();
					h->setLocatie(van);
					h->setBrandbaarheid(brandbaarheid);
					setVstadsdeel(*h);
					addGebouwCoord(van);
				}
				if (!nodeName.compare("Winkel")) {
					Mwinkel *w = new Mwinkel();
					w->setLocatie(van);
					w->setBrandbaarheid(20);
					w->setOvervalbaarheid(overvalbaarheid);
					w->setGrootte(grootte);
					setVstadsdeel(*w);
					addGebouwCoord(van);
				}
			}
			
			//std::cout + "-=-=-=-=-=-=-=-=-=-=-=-" );
			return true;
		}
	}
}
void Stad::simulatie2()
{
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling simulatie2()");
	srand(time(NULL));
	int ib = brandhuizen.size(); int iw = Mbrandweerwagens_onstandby.size();
	std::vector<int> toRemoveBrandweer; std::vector<int> toRemovePolitie; std::vector<int> toRemoveMambulance;
	for (int i = 0; i < ib; i++) {
		for (int j = 0; j < iw; j++) {
			if (ib != 0) {
				std::pair<Vstadsdeel*, Mbrandweerwagen*> temp;

				Location t = Mbrandweerwagens_onstandby[j]->getLocatie();
				Vstadsdeel::Richting r = checkRichting(brandhuizen[i], t);
				Mbrandweerwagens_onstandby[j]->setRichting(r);

				temp.first = brandhuizen[i]; temp.second = Mbrandweerwagens_onstandby[j];
				Mbrandweerwagens_onroad.push_back(temp);
				Mbrandweerwagens_onstandby.erase(Mbrandweerwagens_onstandby.begin() + j);
				brandhuizen.erase(brandhuizen.begin() + i);
				ib = brandhuizen.size(); iw = Mbrandweerwagens_onstandby.size();
			}
		}
	}
	ib = overvalhuizen.size(); iw = Mpolitiewagens_onstandby.size();
	for (int i = 0; i < ib; i++) {
		for (int j = 0; j < iw; j++) {
			if (ib != 0) {
				std::pair<Vstadsdeel*, Mpolitiewagen*> temp;

				Location t = Mpolitiewagens_onstandby[j]->getLocatie();
				Vstadsdeel::Richting r = checkRichting(overvalhuizen[i], t);
				Mpolitiewagens_onstandby[j]->setRichting(r);

				temp.first = overvalhuizen[i]; temp.second = Mpolitiewagens_onstandby[j];
				Mpolitiewagens_onroad.push_back(temp);
				Mpolitiewagens_onstandby.erase(Mpolitiewagens_onstandby.begin() + j);
				overvalhuizen.erase(overvalhuizen.begin() + i);
				ib = overvalhuizen.size(); iw = Mpolitiewagens_onstandby.size();
			}
		} 
	}
	ib = ongevalhuizen.size(); iw = Mambulances_onstandby.size();
	for (int i = 0; i < ib; i++) {
		for (int j = 0; j < iw; j++) {
			if (ib != 0) {
				std::pair<Vstadsdeel*, Mambulance*> temp;

				Location t = Mambulances_onstandby[j]->getLocatie();
				Vstadsdeel::Richting r = checkRichting(ongevalhuizen[i], t);
				Mambulances_onstandby[j]->setRichting(r);

				temp.first = ongevalhuizen[i]; temp.second = Mambulances_onstandby[j];
				Mambulances_onroad.push_back(temp);
				Mambulances_onstandby.erase(Mambulances_onstandby.begin() + j);
				ongevalhuizen.erase(ongevalhuizen.begin() + i);
				ib = ongevalhuizen.size(); iw = Mambulances_onstandby.size();
			}
		}
	}


	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "Brandweerwagens onderweg:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = 0; i < Mbrandweerwagens_onroad.size(); i++) {
		Location t = Mbrandweerwagens_onroad[i].second->getLocatie();
		Vstadsdeel* vak = getVstadsdeel(t);
		if (!checkBestemming(Mbrandweerwagens_onroad[i].first, t)) {
			if ((dynamic_cast<Mkruispunt*>(vak)) == nullptr) {
				output.writeToStatus(Mbrandweerwagens_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.getX()) + "," + std::to_string(t.getY()) + ") in de " + vak->getNaam() + " op weg naar (" + std::to_string(Mbrandweerwagens_onroad[i].first->getLocatie().getX()) + "," + std::to_string(Mbrandweerwagens_onroad[i].first->getLocatie().getY()) + ")");
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				Mbrandweerwagens_onroad[i].second->setLocatie(veranderLocatie(t, Mbrandweerwagens_onroad[i].second->getRichting()));
			}
			else {
				output.writeToStatus(Mbrandweerwagens_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.getX()) + "," + std::to_string(t.getY()) + ") in de " + vak->getNaam() + " op weg naar (" + std::to_string(Mbrandweerwagens_onroad[i].first->getLocatie().getX()) + "," + std::to_string(Mbrandweerwagens_onroad[i].first->getLocatie().getY()) + ")");
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				Vstadsdeel::Richting r = checkRichting(Mbrandweerwagens_onroad[i].first, t); Mbrandweerwagens_onroad[i].second->setRichting(r);
				Mbrandweerwagens_onroad[i].second->setLocatie(veranderLocatie(t, r));
			}
		}
		else {
			if (dynamic_cast<Mbrandweerkazerne*>(Mbrandweerwagens_onroad[i].first) != nullptr) {
				toRemoveBrandweer.push_back(i);
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				output.writeToStatus( "De Brandweerwagen is weer terug in de kazerne" );
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
			}
			else {
				if (brandhuizen.size() != 0) {
					output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
					output.writeToStatus("De brand is geblust en gaat nu naar de volgende opdracht");
					output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
					Mbrandweerwagens_onroad[i].first->setInBrand(false);
					Location t = Mbrandweerwagens_onroad[i].second->getLocatie();
					Mbrandweerwagens_onroad[i].first = brandhuizen[0];
					Mbrandweerwagens_onroad[i].second->setRichting(checkRichting(brandhuizen[0], t)); Vstadsdeel::Richting r = Mbrandweerwagens_onroad[i].second->getRichting();

					t = Mbrandweerwagens_onroad[i].second->getLocatie();
					Mbrandweerwagens_onroad[i].second->setLocatie(veranderLocatie(t, r));

					brandhuizen.erase(brandhuizen.begin());
				}
				else {
					for (int j = 0; j < kazernes.size(); j++) {
						if (kazernes[j]->getNaam() == Mbrandweerwagens_onroad[i].second->getBasis()) {
							output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
							output.writeToStatus("De brand is geblust en gaat nu terug naar de kazerne");
							output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
							Mbrandweerwagens_onroad[i].first->setInBrand(false);
							Location t = Mbrandweerwagens_onroad[i].second->getLocatie();

							Mbrandweerwagens_onroad[i].second->setRichting(checkRichting(kazernes[j], t)); Vstadsdeel::Richting r = Mbrandweerwagens_onroad[i].second->getRichting();
							Mbrandweerwagens_onroad[i].first = kazernes[j];

							t = Mbrandweerwagens_onroad[i].second->getLocatie();
							Mbrandweerwagens_onroad[i].second->setLocatie(veranderLocatie(t, r));
						}
					}
				}
			}
		}
	}
	output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
	output.writeToStatus("Ambulances onderweg:");
	output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
	for (int i = 0; i < Mambulances_onroad.size(); i++) {
		Location t = Mambulances_onroad[i].second->getLocatie();
		Vstadsdeel* vak = getVstadsdeel(t);
		if (!checkBestemming(Mambulances_onroad[i].first, t)) {
			if ((dynamic_cast<Mkruispunt*>(vak)) == nullptr) {
				output.writeToStatus(Mambulances_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.getX()) + "," + std::to_string(t.getY()) + ") in de " + vak->getNaam() + " op weg naar (" + std::to_string(Mambulances_onroad[i].first->getLocatie().getX()) + "," + std::to_string(Mambulances_onroad[i].first->getLocatie().getY()) + ")");
				output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
				Mambulances_onroad[i].second->setLocatie(veranderLocatie(t, Mambulances_onroad[i].second->getRichting()));
			}
			else {
				output.writeToStatus(Mambulances_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.getX()) + "," + std::to_string(t.getY()) + ") in de " + vak->getNaam() + " op weg naar (" + std::to_string(Mambulances_onroad[i].first->getLocatie().getX()) + "," + std::to_string(Mambulances_onroad[i].first->getLocatie().getY()) + ")");
				output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
				Vstadsdeel::Richting r = checkRichting(Mambulances_onroad[i].first, t); Mambulances_onroad[i].second->setRichting(r);
				Mambulances_onroad[i].second->setLocatie(veranderLocatie(t, r));
			}
		}
		else {
			if (dynamic_cast<MziekenMhuis*>(Mambulances_onroad[i].first) != nullptr) {
				toRemoveMambulance.push_back(i);
				output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
				output.writeToStatus("De Ambulance is weer terug in het MziekenMhuis");
				output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
			}
			else {
				if (ongevalhuizen.size() != 0) {
					output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
					output.writeToStatus("De gewonden zijn ingeladen en gaat nu naar de volgende opdracht");
					output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
					Location t = Mambulances_onroad[i].second->getLocatie();
					Mambulances_onroad[i].first = ongevalhuizen[0];
					Mambulances_onroad[i].second->setRichting(checkRichting(ongevalhuizen[0], t)); Vstadsdeel::Richting r = Mambulances_onroad[i].second->getRichting();

					t = Mambulances_onroad[i].second->getLocatie();
					Mambulances_onroad[i].second->setLocatie(veranderLocatie(t, r));

					ongevalhuizen.erase(ongevalhuizen.begin());
				}
				else {
					for (int j = 0; j < ziekenhuizen.size(); j++) {
						if (ziekenhuizen[j]->getNaam() == Mambulances_onroad[i].second->getBasis()) {
							output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
							output.writeToStatus("De gewonden zijn ingeladen en gaat nu terug naar het Ziekenhuis");
							output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
							Location t = Mambulances_onroad[i].second->getLocatie();

							Mambulances_onroad[i].second->setRichting(checkRichting(ziekenhuizen[j], t)); Vstadsdeel::Richting r = Mambulances_onroad[i].second->getRichting();
							Mambulances_onroad[i].first = ziekenhuizen[j];

							t = Mambulances_onroad[i].second->getLocatie();
							Mambulances_onroad[i].second->setLocatie(veranderLocatie(t, r));
						}
					}
				}
			}
		}
	}
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "Huizen die in brand staan:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );

	for (int i = 0; i < brandhuizen.size(); i++) {
		brandhuizen[i]->setBrandbaarheid(brandhuizen[i]->getBrandbaarheid() - 1);
		output.writeToStatus("Huis op locatie (" + std::to_string(brandhuizen[i]->getLocatie().getX()) + "," + std::to_string(brandhuizen[i]->getLocatie().getY()) + ") staat nog in brand met " + std::to_string(brandhuizen[i]->getBrandbaarheid()) + " brandbaarheid over");
	}

	for (int i = 0; i < Mbrandweerwagens_onroad.size(); i++) {
		
		if (dynamic_cast<Mbrandweerkazerne*>(Mbrandweerwagens_onroad[i].first) == nullptr) {
			output.writeToStatus("Huis op locatie (" + std::to_string(Mbrandweerwagens_onroad[i].first->getLocatie().getX()) + "," + std::to_string(Mbrandweerwagens_onroad[i].first->getLocatie().getY()) + ") staat nog in brand met " + std::to_string(Mbrandweerwagens_onroad[i].first->getBrandbaarheid()) + " brandbaarheid over");
			Mbrandweerwagens_onroad[i].first->setBrandbaarheid(Mbrandweerwagens_onroad[i].first->getBrandbaarheid() - 1);
			if (Mbrandweerwagens_onroad[i].first->getBrandbaarheid() == 17) {
				int amount = 0; int stopper = 0;
				int richting = rand() % 4 + 1;
				if (dynamic_cast<Mhuis*>(Mbrandweerwagens_onroad[i].first) != nullptr) { amount = 1; }
				else { amount = 2; }
				while (true) {
					if (amount != 0 && stopper < 5) {
						
						std::pair<int, int> p;
						bool found = false;
						if (richting == 1) {
							Location gebouw;
							gebouw = Mbrandweerwagens_onroad[i].first->getLocatie();
							gebouw.setY(gebouw.getY() + 1);
							Vstadsdeel* t = getVstadsdeel(gebouw);
							if (t != nullptr) {
								for (int l = 0; l < brandhuizen.size(); l++) {
									if (brandhuizen[l] == t)
									{
										found = true;
									}
								}
								if (!found) {
									for (int k = 0; k < Mbrandweerwagens_onroad.size(); k++)
									{
										if (Mbrandweerwagens_onroad[k].first == t)
										{
											found = true;
										}
									}
									if (!found) {
										if (dynamic_cast<Mhuis*>(t) != nullptr || dynamic_cast<Mwinkel*>(t) != nullptr) {
											if (!(t->getInBrand())) {
												t->setInBrand(true);
												brandhuizen.push_back(t);
												ongevalhuizen.push_back(t);
												amount--;
											}
										}
									}
								}
							}
						}
						if (richting == 2) {
							Location gebouw;
							gebouw = Mbrandweerwagens_onroad[i].first->getLocatie();
							gebouw.setY(gebouw.getY() - 2);
							Vstadsdeel* t = getVstadsdeel(gebouw);
							if (t != nullptr) {
								for (int l = 0; l < brandhuizen.size(); l++) {
									if (brandhuizen[l] == t) {
										found = true;
									}
								}
								if (!found) {
									for (int k = 0; k < Mbrandweerwagens_onroad.size(); k++) {
										if (Mbrandweerwagens_onroad[k].first == t) {
											found = true;
										}
									}
									if (!found) {
										if (dynamic_cast<Mhuis*>(t) != nullptr || dynamic_cast<Mwinkel*>(t) != nullptr) {
											if (!(t->getInBrand())) {
												t->setInBrand(true);
												brandhuizen.push_back(t);
												ongevalhuizen.push_back(t);
												amount--;
											}
										}
									}
								}
							}
						}
						if (richting == 3) {
							Location gebouw;
							gebouw = Mbrandweerwagens_onroad[i].first->getLocatie();
							gebouw.setX(gebouw.getX() + 1);
							Vstadsdeel* t = getVstadsdeel(gebouw);
							if (t != nullptr) {
								for (int l = 0; l < brandhuizen.size(); l++) {
									if (brandhuizen[l] == t)
									{
										found = true;
									}
								}
								if (!found) {
									for (int k = 0; k < Mbrandweerwagens_onroad.size(); k++) {
										if (Mbrandweerwagens_onroad[k].first == t) {
											found = true;
										}
									}
									if (!found) {
										if (dynamic_cast<Mhuis*>(t) != nullptr || dynamic_cast<Mwinkel*>(t) != nullptr) {
											if (!(t->getInBrand())) {
												t->setInBrand(true);
												brandhuizen.push_back(t);
												ongevalhuizen.push_back(t);
												amount--;
											}
										}
									}
								}
							}
						}
						if (richting == 4) {
							Location gebouw;
							gebouw = Mbrandweerwagens_onroad[i].first->getLocatie();
							gebouw.setX(gebouw.getX() + 2);
							Vstadsdeel* t = getVstadsdeel(gebouw);
							if (t != nullptr) {
								for (int l = 0; l < brandhuizen.size(); l++) {
									if (brandhuizen[l] == t)
									{
										found = true;
									}
								}
								if (!found) {
									for (int k = 0; k < Mbrandweerwagens_onroad.size(); k++)
									{
										if (Mbrandweerwagens_onroad[k].first == t)
										{
											found = true;
										}
									}
									if (!found)
									{
										if (dynamic_cast<Mhuis*>(t) != nullptr || dynamic_cast<Mwinkel*>(t) != nullptr)
										{
											if (!(t->getInBrand()))
											{
												t->setInBrand(true);
												brandhuizen.push_back(t);
												ongevalhuizen.push_back(t);
												amount--;
											}
										}
									}
								}
							}
						}
						if (richting != 4) {
							richting++;
						}
						else {
							richting = 1;
						} stopper++;
					}
					else {
						break;
					}
					
				}
			}
		}
	}
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "Politiewagens onderweg:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = 0; i < Mpolitiewagens_onroad.size(); i++) {
		Location t = Mpolitiewagens_onroad[i].second->getLocatie();
		Vstadsdeel* vak = getVstadsdeel(t);
		if (!checkBestemming(Mpolitiewagens_onroad[i].first, t)) {
			if (dynamic_cast<Mkruispunt*>(vak) == nullptr) {
				output.writeToStatus(Mpolitiewagens_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.getX()) + "," + std::to_string(t.getY()) + ") in de " + vak->getNaam() + " op weg naar (" + std::to_string(Mpolitiewagens_onroad[i].first->getLocatie().getX()) + "," + std::to_string(Mpolitiewagens_onroad[i].first->getLocatie().getY()) + ")");
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				Mpolitiewagens_onroad[i].second->setLocatie(veranderLocatie(t, Mpolitiewagens_onroad[i].second->getRichting()));
			}
			else {
				output.writeToStatus(Mpolitiewagens_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.getX()) + "," + std::to_string(t.getY()) + ") in de " + vak->getNaam() + " op weg naar (" + std::to_string(Mpolitiewagens_onroad[i].first->getLocatie().getX()) + "," + std::to_string(Mpolitiewagens_onroad[i].first->getLocatie().getY()) + ")");
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				Vstadsdeel::Richting r = checkRichting(Mpolitiewagens_onroad[i].first, t); Mpolitiewagens_onroad[i].second->setRichting(r);
				Mpolitiewagens_onroad[i].second->setLocatie(veranderLocatie(t, r));
			}
		}
		else {
			if (dynamic_cast<Mpolitiebureau*>(Mpolitiewagens_onroad[i].first) != nullptr) {
				toRemovePolitie.push_back(i);
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				output.writeToStatus( "De Politiewagen is weer terug in de kazerne" );
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
			}
			else {
				if (overvalhuizen.size() != 0) {
					output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
					output.writeToStatus("De overvaller is gearresteerd en gaat nu naar de volgende opdracht");
					output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
					Location t = Mpolitiewagens_onroad[i].second->getLocatie();
					Mpolitiewagens_onroad[i].first = overvalhuizen[0];
					Mpolitiewagens_onroad[i].second->setRichting(checkRichting(overvalhuizen[0], t)); Vstadsdeel::Richting r = Mpolitiewagens_onroad[i].second->getRichting();

					t = Mpolitiewagens_onroad[i].second->getLocatie();
					Mpolitiewagens_onroad[i].second->setLocatie(veranderLocatie(t, r));

					overvalhuizen.erase(overvalhuizen.begin());
				}
				else {
					for (int j = 0; j < bureaus.size(); j++) {
						if (bureaus[j]->getNaam() == Mpolitiewagens_onroad[i].second->getBasis()) {
							output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
							output.writeToStatus("De overvaller is gearresteerd en gaat nu terug naar het bureau");
							output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
							Location t = Mpolitiewagens_onroad[i].second->getLocatie();

							Mpolitiewagens_onroad[i].second->setRichting(checkRichting(bureaus[j], t)); Vstadsdeel::Richting r = Mpolitiewagens_onroad[i].second->getRichting();
							Mpolitiewagens_onroad[i].first = bureaus[j];

							t = Mpolitiewagens_onroad[i].second->getLocatie();
							Mpolitiewagens_onroad[i].second->setLocatie(veranderLocatie(t, r));
						}
					}
				}
			}
		}
	}
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "Winkels die overvallen worden:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = 0; i < Mpolitiewagens_onroad.size(); i++) {
		if (dynamic_cast<Mpolitiebureau*>(Mpolitiewagens_onroad[i].first) == nullptr) {
			output.writeToStatus("Winkel op locatie (" + std::to_string(Mpolitiewagens_onroad[i].first->getLocatie().getX()) + "," + std::to_string(Mpolitiewagens_onroad[i].first->getLocatie().getY()) + ") wordt nog overvallen met " + std::to_string(Mpolitiewagens_onroad[i].first->getOvervalbaarheid()) + " overvalbaarheid over");
			Mpolitiewagens_onroad[i].first->setOvervalbaarheid(Mpolitiewagens_onroad[i].first->getOvervalbaarheid() - 1);
		}
	}
	std::vector<int> toRemoveBrandweerfinal;
	std::vector<int> toRemovePolitiefinal;
	std::vector<int> toRemoveMambulanceFinal;
	if (toRemoveBrandweer.size() != 0) {
		for (int i = toRemoveBrandweer.size() - 1; i > -1; i--) {
			toRemoveBrandweerfinal.push_back(toRemoveBrandweer[i]);
		}
		//std::reverse(toRemoveBrandweer.begin(), toRemoveBrandweer.end());
		for (int i = 0; i < toRemoveBrandweerfinal.size(); i++) {
			Mbrandweerwagens_onstandby.push_back(Mbrandweerwagens_onroad[i].second);
			Mbrandweerwagens_onroad.erase(Mbrandweerwagens_onroad.begin() + i);
		}
	}
	if (toRemovePolitie.size() != 0) {
		for (int i = toRemovePolitie.size() - 1; i > -1; i--) {
			toRemovePolitiefinal.push_back(toRemovePolitie[i]);
		}
		//std::reverse(toRemovePolitie.begin(), toRemovePolitie.end());
		for (int i = 0; i < toRemovePolitiefinal.size(); i++) {
			Mpolitiewagens_onstandby.push_back(Mpolitiewagens_onroad[i].second);
			Mpolitiewagens_onroad.erase(Mpolitiewagens_onroad.begin() + i);
		}
	}
	if (toRemoveMambulance.size() != 0) {
		for (int i = toRemoveMambulance.size() - 1; i > -1; i--) {
			toRemoveMambulanceFinal.push_back(toRemoveMambulance[i]);
		}
		//std::reverse(toRemovePolitie.begin(), toRemovePolitie.end());
		for (int i = 0; i < toRemoveMambulanceFinal.size(); i++) {
			Mambulances_onstandby.push_back(Mambulances_onroad[i].second);
			Mambulances_onroad.erase(Mambulances_onroad.begin() + i);
		}
	}
	
}

Location Stad::veranderLocatie(Location l, Vstadsdeel::Richting r)
{
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling veranderLocatie");
	Location t;
	if (r == Vstadsdeel::Links) {
		t.setX(l.getX() - 1);
		t.setY(l.getY());
	}
	if (r == Vstadsdeel::Rechts) {
		t.setX(l.getX() + 1);
		t.setY(l.getY());
	}
	if (r == Vstadsdeel::Onder) {
		t.setY(l.getY() - 1);
		t.setX(l.getX());
	}
	if (r == Vstadsdeel::Boven) {
		t.setY(l.getY() + 1);
		t.setX(l.getX());
	}
	return t;
}
bool Stad::checkStad()
{   
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling checkStad");
	bool success = true;
	for (int i = 0; i < yrow; i++) {	
		for (int j = 0; j < xrow; j++) {
			Location loc; loc.setY(i); loc.setX(j); 
			if (getVstadsdeel(loc) == nullptr) {
				success = false;
				output.writeToConsole("Inconsistente Stad: Stad bevat nullptr(s)!" );
				return success;
			}
            else {
            	continue;
            }
		}
	}
	Location newLoc;
	Vstadsdeel* other;
	for (int i = 0; i < getGebouwCoordsLength(); i++) {
		Vstadsdeel* current = getVstadsdeel(getGebouwCoord(i));
		if (dynamic_cast<Mhuis*>(current) != nullptr) {	
			if (checkAangrenzing(current) == false) {
				success = false;
				output.writeToConsole("Inconsistente Stad: er grenst geen Mstraat aan het Mhuis op locatie" + std::to_string(current->getLocatie().getX()) + "|" + std::to_string(current->getLocatie().getY()));
				return success;
			}			
			for (int j = getGebouwCoord(i).getY(); j >= (getGebouwCoord(j).getY() - 1); j--) {
				for (int k = getGebouwCoord(i).getX(); k <= (getGebouwCoord(i).getX() + 1); k++) {
					newLoc.setX(k); newLoc.setY(j);
					other = getVstadsdeel(newLoc);
					
					if (other != current) {
						success = false;
						output.writeToConsole("Inconsistente Stad: Mhuis op locatie " + std::to_string(current->getLocatie().getX()) + "|" + std::to_string(current->getLocatie().getY()));
						return success;
					}
					
				}
			}
		}
		else if (dynamic_cast<Mhuis*>(current) != nullptr) {
			if (checkAangrenzing(current) == false) {
				success = false;
				output.writeToConsole("Inconsistente Stad: er grenst geen Mstraat aan de kazerne op locatie" + std::to_string(current->getLocatie().getX()) + "|" + std::to_string(current->getLocatie().getY()));
				return success;
			}
			for (int j = getGebouwCoord(i).getY(); j >= (getGebouwCoord(j).getY() - 3); j--) {
				for (int k = getGebouwCoord(i).getX(); k <= (getGebouwCoord(i).getX() + 3); k++) {
					newLoc.setX(k); newLoc.setY(j);
					other = getVstadsdeel(newLoc);
					
					if (other != current) {
						success = false;
						output.writeToConsole("Inconsistente Stad: Kazerne op locatie " + std::to_string(current->getLocatie().getX()) + "|" + std::to_string(current->getLocatie().getY()));
						return success;
					}
				}
			}
		}
	}
	Location begin;
	Location end;
    for (int i = 0; i < getMstraatCoordsLength(); i++) {
       	begin.setX(getMstraatCoord(i).first.getX());
		begin.setY(getMstraatCoord(i).first.getY());
		
		end.setX(getMstraatCoord(i).second.getX());
		end.setY(getMstraatCoord(i).second.getY());
		
		Vstadsdeel* currentBegin = getVstadsdeel(begin);
		Vstadsdeel* currentEnd = getVstadsdeel(end);		
		
		if ((begin.getX() == 0) && (end.getY() == (yrow - 1))) {
			std::string stNaam;			
			if (currentBegin->getEersteMstraat() == currentEnd->getEersteMstraat()) {			
				stNaam = currentBegin->getEersteMstraat();
			}
			else {
				success = false;
				output.writeToConsole("Inconsistente Stad: Mstraat \t" + stNaam );
				return success;
			}
			for (int i = 0; i <= xrow - 1; i++) {
				newLoc.setX(i);
				newLoc.setY(begin.getY());
				other = getVstadsdeel(newLoc);

				if (other->getEersteMstraat() == stNaam || other->getNaam() == stNaam) {
					continue;
				}
				else {
					success = false;
					output.writeToConsole("Inconsistente Stad: Mstraat \t" + currentBegin->getEersteMstraat() );
					return success;
				}
			}	
		}
		else if ((begin.getY() == 0) && (end.getX() == (xrow - 1))) {
			std::string stNaam;
			if (currentBegin->getTweedeMstraat() == currentEnd->getTweedeMstraat()) {
				stNaam = currentBegin->getTweedeMstraat();
			}
			else {
				success = false;
				output.writeToConsole("Inconsistente Stad: Mstraat \t" + currentBegin->getTweedeMstraat() );
				return success;
			}
			for (int i = 0; i <= yrow - 1; i++) {
				newLoc.setY(i);
				newLoc.setX(begin.getX());
				other = getVstadsdeel(newLoc);
				
				if (other->getTweedeMstraat() == stNaam || other->getNaam() == stNaam) {
					continue;
				}
				else {
					success = false;
					output.writeToConsole("Inconsistente Stad: Mstraat \t" + stNaam );
					return success;
				}
			}
		}
    }
	return success;
}
Location Stad::getGebouwCoord(int i)
{
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling getGebouwCoord");
	return gebouwCoords[i];
}
std::pair<Location, Location> Stad::getMstraatCoord(int i)
{
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling getMstraatCoord");
	return MstraatCoords[i];
}
void Stad::addGebouwCoord(Location g)
{
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling addGebouwCoord");
	gebouwCoords.push_back(g);
}
void Stad::addMstraatCoord(std::pair<Location, Location> s)
{
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling addMstraatCoord");
	MstraatCoords.push_back(s);
}
int Stad::getGebouwCoordsLength()
{
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling getGebouwCoordsLength");
	return gebouwCoords.size();
}
int Stad::getMstraatCoordsLength()
{
    //REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling getMstraatCoordsLength");
	return MstraatCoords.size();
}


/*
	NOG TE OVERLOADEN METHODES VOOR Vstadsdeel
*/
Vstadsdeel::Vstadsdeel(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Vstadsdeel::properlyInitialized()
{
	return _initCheck == this;
}
Location Vstadsdeel::getLocatie()
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling getLocatie");
	Location t = Location(0, 0);
	return t;
}
Location Vstadsdeel::getBegin()
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling getBegin");
	Location t = Location(0, 0);
	return t;
}
Location Vstadsdeel::getEinde()
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling getEinde");
	Location t = Location(0, 0);
	return t;
}
Location Vstadsdeel::getGrootte()
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling getGrootte");
	Location t = Location(0, 0);
	return t;
}
bool Vstadsdeel::getInBrand()
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling getInBrand");
	return false;
}
char Vstadsdeel::getKarakter()
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling getKarakter");
	return '0';
}
Vstadsdeel::Type Vstadsdeel::getType()
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling getType");
	return Station;
}
std::string Vstadsdeel::getNaam()
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling getNaam");
	return "";
}
void Vstadsdeel::setInBrand(bool i)
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling setInBrand");
    
	inBrand = i;
}
void Vstadsdeel::setBrandbaarheid(int i)
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling setBrandbaarheid");
	brandbaarheid = 0;
}
void Vstadsdeel::setOvervalbaarheid(int i)
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling setOvervalbaarheid");
	overvalbaarheid = 0;
}
int Vstadsdeel::getBrandbaarheid()
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling getBrandbaarheid");
	return brandbaarheid;
}
int Vstadsdeel::getOvervalbaarheid()
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling getOvervalbaarheid");
	return overvalbaarheid;
}
std::string Vstadsdeel::getEersteMstraat()
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling getEersteMstraat");
	return "";
}

std::string Vstadsdeel::getTweedeMstraat()
{
    //REQUIRE(this->properlyInitialized(), "Vstadsdeel wasn't initialized when calling getTweedeMstraat");
	return "";
}




/*
	METHODES VOOR DE Mbrandweerkazerne
*/
Mbrandweerkazerne::Mbrandweerkazerne(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Mbrandweerkazerne::properlyInitialized()
{
	return _initCheck == this;
}
void Mbrandweerkazerne::setNaam(std::string s)
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerkazerne wasn't initialized when calling setNaam");
	naam = s;
}
void Mbrandweerkazerne::setLocatie(Location p)
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerkazerne wasn't initialized when calling setLocatie");
	locatie = p;
}
void Mbrandweerkazerne::setIngang(Location p)
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerkazerne wasn't initialized when calling setIngang");
	ingang = p;
}
Location Mbrandweerkazerne::getLocatie()
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerkazerne wasn't initialized when calling getLocatie");
	return locatie;
}
Location Mbrandweerkazerne::getIngang()
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerkazerne wasn't initialized when calling getIngang");
	return ingang;
}
std::string Mbrandweerkazerne::getNaam()
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerkazerne wasn't initialized when calling getNaam");
	return naam;
}
char Mbrandweerkazerne::getKarakter()
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerkazerne wasn't initialized when calling getKarakter");
	return karakter;
}
Vstadsdeel::Type Mbrandweerkazerne::getType()
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerkazerne wasn't initialized when calling getType");
	return type;
}

/*
METHODES VOOR DE POLITIE STATION
*/
Mpolitiebureau::Mpolitiebureau(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Mpolitiebureau::properlyInitialized()
{
	return _initCheck == this;
}
void Mpolitiebureau::setNaam(std::string s)
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiebureau wasn't initialized when calling setNaam");
	naam = s;
}
void Mpolitiebureau::setLocatie(Location p)
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiebureau wasn't initialized when calling setLocatie");
	locatie = p;
}
void Mpolitiebureau::setIngang(Location p)
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiebureau wasn't initialized when calling setIngang");
	ingang = p;
}
void Mpolitiebureau::setGrootte(Location p)
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiebureau wasn't initialized when calling setGrootte");
	grootte = p;
}
Location Mpolitiebureau::getLocatie()
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiebureau wasn't initialized when calling getLocatie");
	return locatie;
}
Location Mpolitiebureau::getIngang()
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiebureau wasn't initialized when calling getIngang");
	return ingang;
}
Location Mpolitiebureau::getGrootte()
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiebureau wasn't initialized when calling getGrootte");
	return grootte;
}
std::string Mpolitiebureau::getNaam()
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiebureau wasn't initialized when calling getNaam");
	return naam;
}
char Mpolitiebureau::getKarakter()
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiebureau wasn't initialized when calling getKarakter");
	return karakter;
}
Vstadsdeel::Type Mpolitiebureau::getType()
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiebureau wasn't initialized when calling getType");
	return type;
}

/*
METHODES VOOR DE MziekenMhuis
*/
MziekenMhuis::MziekenMhuis(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool MziekenMhuis::properlyInitialized()
{
	return _initCheck == this;
}
void MziekenMhuis::setNaam(std::string s)
{
    //REQUIRE(this->properlyInitialized(), "MziekenMhuis wasn't initialized when calling setNaam");
	naam = s;
}
void MziekenMhuis::setLocatie(Location p)
{
    //REQUIRE(this->properlyInitialized(), "MziekenMhuis wasn't initialized when calling setLocatie");
	locatie = p;
}
void MziekenMhuis::setIngang(Location p)
{
    //REQUIRE(this->properlyInitialized(), "MziekenMhuis wasn't initialized when calling setIngang");
	ingang = p;
}
void MziekenMhuis::setGrootte(Location p)
{
    //REQUIRE(this->properlyInitialized(), "MziekenMhuis wasn't initialized when calling setGrootte");
	grootte = p;
}
Location MziekenMhuis::getLocatie()
{
    //REQUIRE(this->properlyInitialized(), "MziekenMhuis wasn't initialized when calling getLocatie");
	return locatie;
}
Location MziekenMhuis::getIngang()
{
    //REQUIRE(this->properlyInitialized(), "MziekenMhuis wasn't initialized when calling getIngang");
	return ingang;
}
Location MziekenMhuis::getGrootte()
{
    //REQUIRE(this->properlyInitialized(), "MziekenMhuis wasn't initialized when calling getGrootte");
	return grootte;
}
std::string MziekenMhuis::getNaam()
{
    //REQUIRE(this->properlyInitialized(), "MziekenMhuis wasn't initialized when calling getNaam");
	return naam;
}
char MziekenMhuis::getKarakter()
{
    //REQUIRE(this->properlyInitialized(), "MziekenMhuis wasn't initialized when calling getKarakter");
	return karakter;
}
Vstadsdeel::Type MziekenMhuis::getType()
{
    //REQUIRE(this->properlyInitialized(), "MziekenMhuis wasn't initialized when calling getType");
	return type;
}


/*
	METHODES VOOR DE Mbrandweerwagen
*/
Mbrandweerwagen::Mbrandweerwagen(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Mbrandweerwagen::properlyInitialized()
{
	return _initCheck == this;
}
void Mbrandweerwagen::setNaam(std::string n)
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerwagen wasn't initialized when calling setNaam");
	naam = n;
}
void Mbrandweerwagen::setBasis(std::string b)
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerwagen wasn't initialized when calling setBasis");
	basis = b;
}
void Mbrandweerwagen::setLocatie(Location p)
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerwagen wasn't initialized when calling setLocatie");
	locatie = p;
}
void Mbrandweerwagen::setRichting(Vstadsdeel::Richting r)
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerwagen wasn't initialized when calling setRichting");
	richting = r;
}
Location Mbrandweerwagen::getLocatie()
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerwagen wasn't initialized when calling getLocatie");
	return locatie;
}
Vstadsdeel::Richting Mbrandweerwagen::getRichting()
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerwagen wasn't initialized when calling getRichting");
	return richting;
}
std::string Mbrandweerwagen::getBasis()
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerwagen wasn't initialized when calling getBasis");
	return basis;
}
std::string Mbrandweerwagen::getNaam()
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerwagen wasn't initialized when calling getNaam");
	return naam;
}
char Mbrandweerwagen::getKarakter()
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerwagen wasn't initialized when calling getKarakter");
	return karakter;
}
Vstadsdeel::Type Mbrandweerwagen::getType()
{
    //REQUIRE(this->properlyInitialized(), "Mbrandweerwagen wasn't initialized when calling getType");
	return type;
}

/*
METHODES VOOR DE Mbrandweerwagen
*/
Mpolitiewagen::Mpolitiewagen(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Mpolitiewagen::properlyInitialized()
{
	return _initCheck == this;
}
void Mpolitiewagen::setNaam(std::string n)
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiewagen wasn't initialized when calling setNaam");
	naam = n;
}
void Mpolitiewagen::setBasis(std::string b)
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiewagen wasn't initialized when calling setBasis");
	basis = b;
}
void Mpolitiewagen::setRichting(Vstadsdeel::Richting r)
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiewagen wasn't initialized when calling setRichting");
	richting = r;
}
void Mpolitiewagen::setLocatie(Location p)
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiewagen wasn't initialized when calling setLocatie");
	locatie = p;
}
Vstadsdeel::Richting Mpolitiewagen::getRichting()
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiewagen wasn't initialized when calling getRichting");
	return richting;
}
Location Mpolitiewagen::getLocatie()
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiewagen wasn't initialized when calling getLocatie");
	return locatie;
}
std::string Mpolitiewagen::getBasis()
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiewagen wasn't initialized when calling getBasis");
	return basis;
}
std::string Mpolitiewagen::getNaam()
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiewagen wasn't initialized when calling getNaam");
	return naam;
}
char Mpolitiewagen::getKarakter()
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiewagen wasn't initialized when calling getKarakter");
	return karakter;
}
Vstadsdeel::Type Mpolitiewagen::getType()
{
    //REQUIRE(this->properlyInitialized(), "Mpolitiewagen wasn't initialized when calling getType");
	return type;
}

/*
METHODES VOOR DE Mbrandweerwagen
*/
Mambulance::Mambulance(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Mambulance::properlyInitialized()
{
	return _initCheck == this;
}
void Mambulance::setNaam(std::string n)
{
    //REQUIRE(this->properlyInitialized(), "Mambulance wasn't initialized when calling setNaam");
	naam = n;
}
void Mambulance::setBasis(std::string b)
{
    //REQUIRE(this->properlyInitialized(), "Mambulance wasn't initialized when calling setBasis");
	basis = b;
}
void Mambulance::setRichting(Vstadsdeel::Richting r)
{
    //REQUIRE(this->properlyInitialized(), "Mambulance wasn't initialized when calling setRichting");
	richting = r;
}
void Mambulance::setLocatie(Location p)
{
    //REQUIRE(this->properlyInitialized(), "Mambulance wasn't initialized when calling setLocatie");
	locatie = p;
}
Vstadsdeel::Richting Mambulance::getRichting()
{
    //REQUIRE(this->properlyInitialized(), "Mambulance wasn't initialized when calling getRichting");
	return richting;
}
Location Mambulance::getLocatie()
{
    //REQUIRE(this->properlyInitialized(), "Mambulance wasn't initialized when calling getLocatie");
	return locatie;
}
std::string Mambulance::getBasis()
{
    //REQUIRE(this->properlyInitialized(), "Mambulance wasn't initialized when calling getBasis");
	return basis;
}
std::string Mambulance::getNaam()
{
    //REQUIRE(this->properlyInitialized(), "Mambulance wasn't initialized when calling getNaam");
	return naam;
}
char Mambulance::getKarakter()
{
    //REQUIRE(this->properlyInitialized(), "Mambulance wasn't initialized when calling getKarakter");
	return karakter;
}
Vstadsdeel::Type Mambulance::getType()
{
    //REQUIRE(this->properlyInitialized(), "Mambulance wasn't initialized when calling getType");
	return type;
}



/*
	METHODES VOOR DE Mstraat
*/
Mstraat::Mstraat(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Mstraat::properlyInitialized()
{
	return _initCheck == this;
}
void Mstraat::setNaam(std::string n)
{
    //REQUIRE(this->properlyInitialized(), "Mstraat wasn't initialized when calling setNaam");
	naam = n;
}
void Mstraat::setBegin(Location b)
{
    //REQUIRE(this->properlyInitialized(), "Mstraat wasn't initialized when calling setBegin");
	begin = b;
}
void Mstraat::setEinde(Location e)
{
    //REQUIRE(this->properlyInitialized(), "Mstraat wasn't initialized when calling setEinde");
	einde = e;
}
std::string Mstraat::getNaam()
{
    //REQUIRE(this->properlyInitialized(), "Mstraat wasn't initialized when calling getNaam");
	return naam;
}
Location Mstraat::getBegin()
{
    //REQUIRE(this->properlyInitialized(), "Mstraat wasn't initialized when calling getBegin");
	return begin;
}
Location Mstraat::getEinde()
{
    //REQUIRE(this->properlyInitialized(), "Mstraat wasn't initialized when calling getEinde");
	return einde;
}
char Mstraat::getKarakter()
{
    //REQUIRE(this->properlyInitialized(), "Mstraat wasn't initialized when calling getKarakter");
	return karakter;
}
Vstadsdeel::Type Mstraat::getType()
{
    //REQUIRE(this->properlyInitialized(), "Mstraat wasn't initialized when calling getType");
	return type;
}
/*






	METHODES VOOR EEN Mhuis
*/
Mhuis::Mhuis(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Mhuis::properlyInitialized()
{
	return _initCheck == this;
}
void Mhuis::setInBrand(bool i)
{
    //REQUIRE(this->properlyInitialized(), "Mhuis wasn't initialized when calling setInBrand");
	inBrand = i;
}
bool Mhuis::getInBrand()
{
    //REQUIRE(this->properlyInitialized(), "Mhuis wasn't initialized when calling getInBrand");
	return inBrand;
}
void Mhuis::setLocatie(Location l)
{
    //REQUIRE(this->properlyInitialized(), "Mhuis wasn't initialized when calling setLocatie");
	locatie = l;
}
void Mhuis::setBrandbaarheid(int b)
{
    //REQUIRE(this->properlyInitialized(), "Mhuis wasn't initialized when calling setBrandbaarheid");
	brandbaarheid = b;
}
int Mhuis::getBrandbaarheid()
{
    //REQUIRE(this->properlyInitialized(), "Mhuis wasn't initialized when calling getBrandbaarheid");
	return brandbaarheid;
}
Location Mhuis::getLocatie()
{
    //REQUIRE(this->properlyInitialized(), "Mhuis wasn't initialized when calling getLocatie");
	return locatie;
}
char Mhuis::getKarakter()
{
    //REQUIRE(this->properlyInitialized(), "Mhuis wasn't initialized when calling getKarakter");
	return karakter;
}
Vstadsdeel::Type Mhuis::getType()
{
    //REQUIRE(this->properlyInitialized(), "Mhuis wasn't initialized when calling getType");
	return type;
}

/*






METHODES VOOR EEN Mwinkel
*/
Mwinkel::Mwinkel(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Mwinkel::properlyInitialized()
{
	return _initCheck == this;
}
void Mwinkel::setInBrand(bool i)
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling setInBrand");
	inBrand = i;
}
bool Mwinkel::getInBrand()
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling getInBrand");
	return inBrand;
}
void Mwinkel::setLocatie(Location l)
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling setLocatie");
	locatie = l;
}
void Mwinkel::setGrootte(Location p)
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling setGrootte");
	grootte = p;
}
void Mwinkel::setBrandbaarheid(int b)
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling setBrandbaarheid");
	brandbaarheid = b;
}
void Mwinkel::setOvervalbaarheid(int b)
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling setOvervalbaarheid");
	overvalbaarheid = b;
}
int Mwinkel::getBrandbaarheid()
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling getBrandbaarheid");
	return brandbaarheid;
}
int Mwinkel::getOvervalbaarheid()
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling getOvervalbaarheid");
	return overvalbaarheid;
}
Location Mwinkel::getLocatie()
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling getLocatie");
	return locatie;
}
Location Mwinkel::getGrootte()
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling getGrootte");
	return grootte;
}
char Mwinkel::getKarakter()
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling getKarakter");
	return karakter;
}
Vstadsdeel::Type Mwinkel::getType()
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling getType");
	return type;
}



/*
	METHODES VOOR EEN Mkruispunt
*/
Mkruispunt::Mkruispunt(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Mkruispunt::properlyInitialized()
{
	return _initCheck == this;
}
void Mkruispunt::setEersteMstraat(std::string s)
{
    //REQUIRE(this->properlyInitialized(), "Mkruispunt wasn't initialized when calling setEersteMstraat");
	eersteMstraat = s;
}
void Mkruispunt::setTweedeMstraat(std::string s)
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling setTweedeMstraat");
	tweedeMstraat = s;
}
void Mkruispunt::setLocatie(Location l)
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling setLocatie");
	locatie = l;
}
std::string Mkruispunt::getEersteMstraat()
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling getEersteMstraat");
	return eersteMstraat;
}
std::string Mkruispunt::getTweedeMstraat()
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling getTweedeMstraat");
	return tweedeMstraat;
}
char Mkruispunt::getKarakter()
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling setKarakter");
	return karakter;
}
std::string Mkruispunt::getNaam()
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling getNaam");
	return eersteMstraat;
}
Vstadsdeel::Type Mkruispunt::getType()
{
    //REQUIRE(this->properlyInitialized(), "Mwinkel wasn't initialized when calling getType");
	return type;
}
/*
METHODES VOOR OUTPUTCLASS
*/
OutputClass::OutputClass(void)
{
    _initCheck = this;
	status = new std::ofstream("status.txt", std::ofstream::out);
	console = new std::ofstream("console.txt", std::ofstream::out);
    //ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool OutputClass::properlyInitialized()
{
    return _initCheck == this;
}
void OutputClass::writeToConsole(std::string s)
{
    //REQUIRE(this->properlyInitialized(), "OutputClass wasn't initialized when calling writeToConsole");
	*console << s << std::endl;
}
void OutputClass::writeToConsole()
{
    //REQUIRE(this->properlyInitialized(), "OutputClass wasn't initialized when calling writeToConsole");
	*console << std::endl;
}
void OutputClass::writeToStatus(std::string s)
{
    //REQUIRE(this->properlyInitialized(), "OutputClass wasn't initialized when calling writeToStatus");
	if (ui.getSU())
	{
		*status << s << std::endl;
	}
}void OutputClass::writeToStatus()
{
    //REQUIRE(this->properlyInitialized(), "OutputClass wasn't initialized when calling writeToStatus");
	if (ui.getSU())
	{
		*status << std::endl;
	}
}

/*
METHODES VOOR UI
*/
UI::UI()
{
	_initCheck = this;
    //ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool UI::properlyInitialized()
{
    return _initCheck == this;
}

void UI::showMenu()
{
    //REQUIRE(this->properlyInitialized(), "UI wasn't initialized when calling showMenu");
	std::cout << "(1) Invoerbestand met stad inlezen" << std::endl;
	std::cout << "(2) Invoerbestand met voertuigen inlezen" << std::endl;
	std::cout << "(3) Mhuis in brand steken" << std::endl;
	std::cout << "(4) Mwinkel laten overvallen" << std::endl;
	std::cout << "(5) Aantal stappen in simulatie aanpassen" << std::endl;
	std::cout << "(6) Simpele grafische impressie tonen" << std::endl;
	std::cout << "(7) Simulatie starten" << std::endl;
	std::cout << "(0) Sluit dit programma" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Uw keuze: ";
	std::cin >> choice;
	switch (choice) {
		case 1:
        {
			leesBestandStad();
			break;
		}
		case 2:
		{
			leesBestandVoertuigen();
			break;
		}
		case 3:
		{
			startBrand();
			break;
		}
		case 4:
		{
			startOverval();
			break;
		}
		case 5:
		{
			setSteps();
			break;
		}
		case 6:
		{
			showMap();
			break;
		}
		case 7:
		{
			runSimulatie();
			break;
		}
		case 0:
		{
			break;
		}
	}
}

void UI::leesBestandStad()
{
    //REQUIRE(this->properlyInitialized(), "UI wasn't initialized when calling leesBestandStad");
	std::string s = "";
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	std::cout << "Geef het bestandnaam op: ";
	std::cin >> s;
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl << std::endl;
	stad->stadInlezen(s.c_str());
	showMenu();
}

void UI::leesBestandVoertuigen()
{
    //REQUIRE(this->properlyInitialized(), "UI wasn't initialized when calling LeesBestandVoertuigen");
	std::string s = "";
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	std::cout << "Geef het bestandnaam op: ";
	std::cin >> s;
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl << std::endl;
	stad->stadInlezen(s.c_str());
	showMenu();
}

void UI::startBrand()
{
    //REQUIRE(this->properlyInitialized(), "UI wasn't initialized when calling startBrand");
	int x = 0; int y = 0; Location coord;
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	std::cout << "Geef de x coordinaat van het Mhuis op: ";
	std::cin >> x;
	std::cout << "Geef de y coordinaat van het Mhuis op: ";
	std::cin >> y;
	coord.setX(x); coord.setY(y);
	if (dynamic_cast<Mhuis*>(stad->getVstadsdeel(coord)) != nullptr || dynamic_cast<Mwinkel*>(stad->getVstadsdeel(coord)) != nullptr)
	{
		stad->brandhuizen.push_back(stad->getVstadsdeel(coord));
		stad->ongevalhuizen.push_back(stad->getVstadsdeel(coord));
		stad->getVstadsdeel(coord)->setInBrand(true);
	}
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl << std::endl;
	showMenu();
}
void UI::startOverval()
{
    //REQUIRE(this->properlyInitialized(), "UI wasn't initialized when calling startOverval");
	int x = 0; int y = 0; Location coord;
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	std::cout << "Geef de x coordinaat van de Mwinkel op: ";
	std::cin >> x;
	std::cout << "Geef de y coordinaat van de Mwinkel op: ";
	std::cin >> y;
	coord.setX(x); coord.setY(y);
	if (dynamic_cast<Mwinkel*>(stad->getVstadsdeel(coord)) != nullptr)
	{
		stad->overvalhuizen.push_back(stad->getVstadsdeel(coord));
	}
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl << std::endl;
	showMenu();
}
bool UI::startMbrandweerwagen()
{
    //REQUIRE(this->properlyInitialized(), "UI wasn't initialized when calling startMbrandweerwagen");
	return true;
}
void UI::setSteps()
{
	int s = 0; 
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	std::cout << "Geef het aantal stappen voor elke simulatieronde: ";
	std::cin >> s;
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl << std::endl;
	steps = s;
	showMenu();
}
void UI::setChoice(int c)
{
    //REQUIRE(this->properlyInitialized(), "UI wasn't initialized when calling setChoice");
	int choice = c;
}
void UI::setSU(bool s) //Simpele Uitvoer
{
    //REQUIRE(this->properlyInitialized(), "UI wasn't initialized when calling setSU");
	SU = s;
}
void UI::setStad(Stad* s)
{
    //REQUIRE(this->properlyInitialized(), "UI wasn't initialized when calling setStad");
	stad = s;
}
void UI::runSimulatie()
{
    //REQUIRE(this->properlyInitialized(), "UI wasn't initialized when calling runSimulatie");
	for (int i = 0; i < steps; i++) {
		stad->simulatie2();
	}
	showMenu();
}
bool UI::getSU()
{
    //REQUIRE(this->properlyInitialized(), "UI wasn't initialized when calling getSU");
	return SU;
}
void UI::showMap()
{
    //REQUIRE(this->properlyInitialized(), "UI wasn't initialized when calling showMap");
	stad->printStad();
	showMenu();
}


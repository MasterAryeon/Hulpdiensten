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


/*
	METHODES VOOR DE STAD
*/
Stad::Stad(void)
{
	_initCheck = this;
	for (int i = 0; i < xrow; i++) {
		for (int j = 0; j < yrow; j++) {
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
	//REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling stVstadsdeel");
	if ((dynamic_cast<Mbrandweerkazerne*>(&s)) != nullptr) {
		std::pair<int, int> l = s.getLocatie();
		for (int i = l.second; i > (l.second - 4); i--) {
			for (int j = l.first; j < (l.first + 4); j++) {
				grid[i][j] = &s;
			}
		}
	}
	else if ((dynamic_cast<Mpolitiebureau*>(&s)) != nullptr) {
		std::pair<int, int> l = s.getLocatie();
		std::pair<int, int> g = s.getGrootte();
		for (int i = l.second; i >(l.second - g.first); i--) {
			for (int j = l.first; j < (l.first + g.second); j++) {
				grid[i][j] = &s;
			}
		}
	}
	else if ((dynamic_cast<MziekenMhuis*>(&s)) != nullptr) {
		std::pair<int, int> l = s.getLocatie();
		std::pair<int, int> g = s.getGrootte();
		for (int i = l.second; i >(l.second - g.first); i--) {
			for (int j = l.first; j < (l.first + g.second); j++) {
				grid[i][j] = &s;
			}
		}
	}
	else if ((dynamic_cast<Mstraat*>(&s)) != nullptr) {
		std::pair<int, int> b = s.getBegin();  std::pair<int, int> e = s.getEinde();
		if (b.first == e.first) {
			for (int i = (b.second); i <= (e.second); i++) {
				Vstadsdeel* t = grid[i][b.first];
				if (t == nullptr) {
					grid[i][b.first] = &s;
				}
				else {
					if (grid[i][b.first]->getType() == 6) {
						Mkruispunt* k = new Mkruispunt();
						k->setEersteMstraat(grid[i][b.first]->getNaam());
						k->setTweedeMstraat(s.getNaam());
						k->setLocatie(s.getLocatie());
						grid[i][b.first] = k;
					}
					else {
						break; // HIER MOET NOG WAT WERK VERRICHT WORDEN VANWEGE DE BREAK-OUT
					}
				}
			}
		}
		else {
			for (int i = (b.first); i <= (e.first); i++) {
				Vstadsdeel* t = grid[b.second][i];
				if (t == nullptr) {
					grid[b.second][i] = &s;
				}
				else {
					if (grid[i][b.first]->getType() == 3) {
						Mkruispunt* k = new Mkruispunt();
						k->setEersteMstraat(grid[i][b.first]->getNaam());
						k->setTweedeMstraat(s.getNaam());
						k->setLocatie(s.getLocatie());
						grid[b.second][i] = k;
					}
					else {
						break; // HIER MOET NOG WAT WERK VERRICHT WORDEN VANWEGE DE BREAK-OUT
					}
				}
			}
		}
	}
	else if ((dynamic_cast<Mwinkel*>(&s)) != nullptr) {
		std::pair<int, int> l = s.getLocatie();
		std::pair<int, int> g = s.getGrootte();
		for (int i = l.second; i >(l.second - g.first); i--) {
			for (int j = l.first; j < (l.first + g.second); j++) {
				grid[i][j] = &s;
			}
		}
	}
	else if ((dynamic_cast<Mhuis*>(&s)) != nullptr) {
		std::pair<int, int> l = s.getLocatie();
		for (int i = l.second; i >(l.second - 2); i--) {
			for (int j = l.first; j < (l.first + 2); j++) {
				grid[i][j] = &s;
			}
		}
	}
}
Vstadsdeel* Stad::getVstadsdeel(std::pair<int, int> p)
{
	//REQUIRE(this->properlyInitialized(), "Vstadsdeel not properly initialized");
	if ((p.first >= 0 && p.first <= xrow) && (p.second >= 0 && p.second <= yrow)) {
		return grid[p.first][p.second];
	}
	else {
		return nullptr;
	}
}

bool Stad::checkAangrenzing(Vstadsdeel* s)
{
	//REQUIRE(this->properlyInitialized(), "Stad not properly initialized");
	int aantal = 0;
	if ((dynamic_cast<Mpolitiebureau*>(s)) != nullptr) {
		aantal = 4;
	}
	else if ((dynamic_cast<Mhuis*>(s)) != nullptr) {
		aantal = 2;
	}
	std::pair<int, int> p = s->getLocatie();
	std::pair<int, int> l;
	Vstadsdeel* t;
	for (int i = (p.second + 1); i > (p.second - aantal - 1); i--) {
		for (int j = (p.first - 1); j < (p.first + aantal + 1); j++) {
			l.first = i; l.second = j;
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
	Vstadsdeel* vak;
	output.writeToStatus( "Legenda:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "H = Onderdeel van een Mhuis" );
	output.writeToStatus( "# = Onderdeel van een brandend Mhuis");
	output.writeToStatus( "K = Onderdeel van de kazerne" );
	output.writeToStatus( "Z = Onderdeel van het MziekenMhuis" );
	output.writeToStatus( "P = Onderdeel van het Mpolitiebureau" );
	output.writeToStatus( "W = Onderdeel van de Mwinkel" );
	output.writeToStatus( "  = Onderdeel van een Mstraat" );
	output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
	output.writeToStatus( "* = Mbrandweerwagen" );
	output.writeToStatus( "= = Mambulance");
	output.writeToStatus( "% = Mpolitiewagen");
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = (xrow-1); i >= 0; i--) {
		std::string lijn = "";
		for (int j = 0; j < yrow; j++) {
			bool found = false;
			std::pair<int, int> p;
			p.first = i; p.second = j;
			Vstadsdeel* t = getVstadsdeel(p);
			if (dynamic_cast<Mhuis*>(t) != nullptr || dynamic_cast<Mwinkel*>(t) != nullptr) {
				if (t->getInBrand()) {
					lijn = lijn + "#" + " ";
					found = true;
				}
			}
			for (int k = 0; k < Mbrandweerwagens_onroad.size(); k++) {
				
				if (i == Mbrandweerwagens_onroad[k].second->getLocatie().second && j == Mbrandweerwagens_onroad[k].second->getLocatie().first) {
					lijn = lijn + "*" + " ";
					found = true;
				}
			}
			for (int k = 0; k < Mambulances_onroad.size(); k++) {
				if (i == Mambulances_onroad[k].second->getLocatie().second && j == Mambulances_onroad[k].second->getLocatie().first) {
					lijn = lijn + "=" + " ";
					found = true;
				}
				for (int k = 0; k < Mpolitiewagens_onroad.size(); k++) {
					if (i == Mpolitiewagens_onroad[k].second->getLocatie().second && j == Mpolitiewagens_onroad[k].second->getLocatie().first) {
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
bool Stad::checkBestemming(Vstadsdeel* s, std::pair<int, int> p)
{
	Vstadsdeel* temp;
	std::pair<int, int> coordinaat;
	// RECHTS
	coordinaat.first = p.first; coordinaat.second = p.second + 1;
	temp = getVstadsdeel(coordinaat);
	if (temp == s) {
		//std::cout + "RECHTS" );
		return true;
	}
	// LINKS
	coordinaat.first = p.first; coordinaat.second = p.second - 1;
	temp = getVstadsdeel(coordinaat);
	if (temp == s) {
		//std::cout + "LINKS" );
		return true;
	}
	// ONDER
	coordinaat.first = p.first - 1; coordinaat.second = p.second;
	temp = getVstadsdeel(coordinaat);
	if (temp == s) {
		//std::cout + "ONDER" );
		return true;
	}
	// BOVEN
	coordinaat.first = p.first + 1; coordinaat.second = p.second;
	temp = getVstadsdeel(coordinaat);
	if (temp == s) {
		//std::cout + "BOVEN" );
		return true;
	}
	return false;
}
Stad::MstraatType Stad::checkMstraat(Vstadsdeel* s, std::pair<int, int> p)
{
	Vstadsdeel* vak = getVstadsdeel(p);
	std::pair<int, int> locatie = s->getLocatie();
	if ((dynamic_cast<Mstraat*>(vak)) != nullptr) {
		std::pair<int, int> s1 = vak->getBegin();
		std::pair<int, int> s2 = vak->getEinde();
		if ((s1.first - s2.first) == 0) {
			int x = (locatie.first - p.second);
			if (x <= 1 && x >= -1) {
				return Stad::Verticaal;
			}
		}
		if ((s1.second - s2.second) == 0) {
			int y = (locatie.second - p.first);
			if (y <= 2 && y >= -1) {
				return Stad::Horizontaal;
			}
			
		} 
	}
	if ((dynamic_cast<Mkruispunt*>(vak)) != nullptr) {
		int x = (locatie.first - p.second);
		if (x <= 1 && x >= -1) {
			return Stad::Verticaal;
		}
		int y = (locatie.second - p.first);
		if (y <= 2 && y >= -1) {
			return Stad::Horizontaal;
		}
	} return Stad::Geen;
}
Vstadsdeel::Richting Stad::checkRichting(Vstadsdeel* s, std::pair<int, int> p)
{
	Vstadsdeel* vak = getVstadsdeel(p);
	std::pair<int, int> locatie = s->getLocatie();
	if ((dynamic_cast<Mstraat*>(vak)) != nullptr) {
		std::pair<int, int> s1 = vak->getBegin();
		std::pair<int, int> s2 = vak->getEinde();
		std::pair<int, int> bovenlid; int bovenafstand = -1;
		std::pair<int, int> onderlid; int onderafstand = -1;
		std::pair<int, int> linkerlid; int linkerafstand = -1;
		std::pair<int, int> rechterlid; int rechterafstand = -1;
		if ((s1.first - s2.first) == 0) {
			for (int i = p.first; i >= 0; i--) {
				std::pair<int, int> p2; p2.first = i; p2.second = p.second;
				Vstadsdeel* tijdelijk = getVstadsdeel(p2);
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr) {
					onderlid = p2;
					onderafstand = abs(onderlid.first - s->getLocatie().second) + abs(onderlid.second - s->getLocatie().first);
					break;
				}
			}
			for (int i = p.first; i < xrow; i++) {
				std::pair<int, int> p2; p2.first = i; p2.second = p.second;
				Vstadsdeel* tijdelijk = getVstadsdeel(p2);
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr) {
					bovenlid = p2;
					bovenafstand = abs(bovenlid.first - s->getLocatie().second) + abs(bovenlid.second - s->getLocatie().first);
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
		if ((s1.second - s2.second) == 0) {
			for (int i = p.second; i >= 0; i--)
			{
				std::pair<int, int> p2; p2.first = p.first; p2.second = i;
				Vstadsdeel* tijdelijk = getVstadsdeel(p2);
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr)
				{
					linkerlid = p2;
					linkerafstand = abs(linkerlid.first - s->getLocatie().second) + abs(linkerlid.second - s->getLocatie().first);
					break;
				}
			}
			for (int i = p.second; i < yrow; i++) {
				std::pair<int, int> p2; p2.first = p.first; p2.second = i;
				Vstadsdeel* tijdelijk = getVstadsdeel(p2);
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr)
				{
					rechterlid = p2;
					rechterafstand = abs(rechterlid.first - s->getLocatie().second) + abs(rechterlid.second - s->getLocatie().first);
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
		std::pair<int, int> bovenlid; int bovenafstand = -1;
		std::pair<int, int> onderlid; int onderafstand = -1;
		std::pair<int, int> linkerlid; int linkerafstand = -1;
		std::pair<int, int> rechterlid; int rechterafstand = -1;
		Vstadsdeel::Richting finale; int finaleafstand = 100;
		for (int i = p.first - 1; i >= 0; i--) {
			std::pair<int, int> p2; p2.first = i; p2.second = p.second;
			Vstadsdeel* tijdelijk = getVstadsdeel(p2);
			if (tijdelijk != nullptr) {
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr) {
					onderlid = p2;
					onderafstand = abs(onderlid.first - s->getLocatie().second) + abs(onderlid.second - s->getLocatie().first);
					break;
				}
			}
			else {
				break;
			}
		}
		for (int i = p.first + 1; i < xrow; i++) {
			std::pair<int, int> p2; p2.first = i; p2.second = p.second;
			Vstadsdeel* tijdelijk = getVstadsdeel(p2);
			if (tijdelijk != nullptr) {
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr) {
					bovenlid = p2;
					bovenafstand = abs(bovenlid.first - s->getLocatie().second) + abs(bovenlid.second - s->getLocatie().first);
					break;
				}
			}
			else {
				break;
			}
		}
		for (int i = p.second - 1; i >= 0; i--) {
			std::pair<int, int> p2; p2.first = p.first; p2.second = i;
			Vstadsdeel* tijdelijk = getVstadsdeel(p2);
			if (tijdelijk != nullptr) {
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr) {
					linkerlid = p2;
					int t1 = abs(linkerlid.first - s->getLocatie().second);
					int t2 = abs(linkerlid.second - s->getLocatie().first);
					linkerafstand = abs(linkerlid.first - s->getLocatie().second) + abs(linkerlid.second - s->getLocatie().first);
					break;
				}
			}
			else {
				break;
			}
			
		}
		for (int i = p.second + 1; i < yrow; i++) {
			std::pair<int, int> p2; p2.first = p.first; p2.second = i;
			Vstadsdeel* tijdelijk = getVstadsdeel(p2);
			if (tijdelijk != nullptr) {
				if ((dynamic_cast<Mkruispunt*>(tijdelijk)) != nullptr) {
					rechterlid = p2;
					rechterafstand = abs(rechterlid.first - s->getLocatie().second) + abs(rechterlid.second - s->getLocatie().first);
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
			if ((s->getLocatie().first - p.second) < 0) {
				return Vstadsdeel::Richting::Links;
			}
			else {
				return Vstadsdeel::Richting::Rechts;
			}
		}
		else if (verificatie == Verticaal && (finale == Vstadsdeel::Links || finale == Vstadsdeel::Rechts)) {
			//std::cout + "Correctie" );
			if ((s->getLocatie().second - p.first) < 0) {
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
				std::pair<int, int> van; std::pair<int, int> naar; std::pair<int, int> grootte;
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
										ss >> van.first;
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
										ss >> van.second;
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
										ss >> naar.first;
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
										ss >> naar.second;
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
										ss >> van.first;
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
										ss >> van.second;
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
										ss >> naar.first;
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
										ss >> naar.second;
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
										ss >> grootte.first;
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
										ss >> grootte.second;
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
				if (!nodeName.compare("Mbrandweerkazerne")) {
					Mbrandweerkazerne *bk = new Mbrandweerkazerne();
					bk->setNaam(naam);
					bk->setLocatie(van);
					bk->setIngang(naar);
					setVstadsdeel(*bk);
					kazernes.push_back(bk);

					addGebouwCoord(van);//vector
				}
				if (!nodeName.compare("Mpolitiebureau")) {
					Mpolitiebureau *pb = new Mpolitiebureau();
					pb->setNaam(naam);
					pb->setLocatie(van);
					pb->setIngang(naar);
					pb->setGrootte(grootte);
					setVstadsdeel(*pb);
					bureaus.push_back(pb);
				}
				if (!nodeName.compare("MziekenMhuis")) {
					MziekenMhuis *zh = new MziekenMhuis();
					zh->setNaam(naam);
					zh->setLocatie(van);
					zh->setIngang(naar);
					zh->setGrootte(grootte);
					setVstadsdeel(*zh);
					ziekenhuizen.push_back(zh);
				}
				if (!nodeName.compare("Mbrandweerwagen")) {
					Mbrandweerwagen *bw = new Mbrandweerwagen();
					bw->setNaam(naam);
					bw->setBasis(basis);
					std::pair<int, int> locatie; locatie.first = 0; locatie.second = 0;
					for (int i = 0; i < kazernes.size(); i++) {
						if (kazernes[i]->getNaam() == basis) {
							locatie = kazernes[i]->getIngang();
						}
					}
					if (locatie.first != 0 && locatie.second != 0) {
						bw->setLocatie(locatie);
					}
					Mbrandweerwagens.push_back(bw);
					Mbrandweerwagens_onstandby.push_back(bw);
				}
				if (!nodeName.compare("Mpolitiewagen")) {
					Mpolitiewagen *pw = new Mpolitiewagen();
					pw->setNaam(naam);
					pw->setBasis(basis);
					std::pair<int, int> locatie; locatie.first = 0; locatie.second = 0;
					for (int i = 0; i < bureaus.size(); i++) {
						if (bureaus[i]->getNaam() == basis) {
							locatie = bureaus[i]->getIngang();
						}
					}
					if (locatie.first != 0 && locatie.second != 0) {
						pw->setLocatie(locatie);
					}
					Mpolitiewagens.push_back(pw);
					Mpolitiewagens_onstandby.push_back(pw);
				}
				if (!nodeName.compare("Ziekenwagen")) {
					Mambulance *am = new Mambulance();
					am->setNaam(naam);
					am->setBasis(basis);
					std::pair<int, int> locatie; locatie.first = 0; locatie.second = 0;
					for (int i = 0; i < ziekenhuizen.size(); i++) {
						if (ziekenhuizen[i]->getNaam() == basis) {
							locatie = ziekenhuizen[i]->getIngang();
							am->setLocatie(locatie);
						}
					}
					Mambulances.push_back(am);
					Mambulances_onstandby.push_back(am);
				}
				if (!nodeName.compare("Mstraat")) {
					Mstraat *s = new Mstraat();
					s->setNaam(naam);
					s->setBegin(van);
					s->setEinde(naar);
					setVstadsdeel(*s);

                    std::pair<std::pair<int, int>, std::pair<int, int>> coords; //vector
                    coords.first = van; coords.second = naar;					
                    addMstraatCoord(coords);	
				}
				if (!nodeName.compare("Mhuis")) {
					Mhuis *h = new Mhuis();
					h->setLocatie(van);
					h->setBrandbaarheid(brandbaarheid);
					setVstadsdeel(*h);
					addGebouwCoord(van);
				}
				if (!nodeName.compare("Mwinkel")) {
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
	srand(time(NULL));
	int ib = brandhuizen.size(); int iw = Mbrandweerwagens_onstandby.size();
	std::vector<int> toRemoveBrandweer; std::vector<int> toRemovePolitie; std::vector<int> toRemoveMambulance;
	for (int i = 0; i < ib; i++) {
		for (int j = 0; j < iw; j++) {
			if (ib != 0) {
				std::pair<Vstadsdeel*, Mbrandweerwagen*> temp;

				std::pair<int, int> t = Mbrandweerwagens_onstandby[j]->getLocatie();
				std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
				Vstadsdeel::Richting r = checkRichting(brandhuizen[i], locatie);
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

				std::pair<int, int> t = Mpolitiewagens_onstandby[j]->getLocatie();
				std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
				Vstadsdeel::Richting r = checkRichting(overvalhuizen[i], locatie);
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

				std::pair<int, int> t = Mambulances_onstandby[j]->getLocatie();
				std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
				Vstadsdeel::Richting r = checkRichting(ongevalhuizen[i], locatie);
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
	output.writeToStatus( "Mbrandweerwagens onderweg:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = 0; i < Mbrandweerwagens_onroad.size(); i++) {
		std::pair<int, int> t = Mbrandweerwagens_onroad[i].second->getLocatie();
		std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
		Vstadsdeel* vak = getVstadsdeel(locatie);
		if (!checkBestemming(Mbrandweerwagens_onroad[i].first, locatie)) {
			if ((dynamic_cast<Mkruispunt*>(vak)) == nullptr) {
				output.writeToStatus(Mbrandweerwagens_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.second) + "," + std::to_string(t.first) + ") in de " + vak->getNaam());
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				Mbrandweerwagens_onroad[i].second->setLocatie(veranderLocatie(locatie, Mbrandweerwagens_onroad[i].second->getRichting()));
			}
			else {
				output.writeToStatus(Mbrandweerwagens_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.second) + "," + std::to_string(t.first) + ") in de " + vak->getNaam());
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				Vstadsdeel::Richting r = checkRichting(Mbrandweerwagens_onroad[i].first, locatie); Mbrandweerwagens_onroad[i].second->setRichting(r);
				Mbrandweerwagens_onroad[i].second->setLocatie(veranderLocatie(locatie, r));
			}
		}
		else {
			if (dynamic_cast<Mbrandweerkazerne*>(Mbrandweerwagens_onroad[i].first) != nullptr) {
				toRemoveBrandweer.push_back(i);
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				output.writeToStatus( "De Mbrandweerwagen is weer terug in de kazerne" );
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
			}
			else {
				if (brandhuizen.size() != 0) {
					output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
					output.writeToStatus("De brand is geblust en gaat nu naar de volgende opdracht");
					output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
					Mbrandweerwagens_onroad[i].first->setInBrand(false);
					std::pair<int, int> t = Mbrandweerwagens_onroad[i].second->getLocatie();
					std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
					Mbrandweerwagens_onroad[i].first = brandhuizen[0];
					Mbrandweerwagens_onroad[i].second->setRichting(checkRichting(brandhuizen[0], locatie)); Vstadsdeel::Richting r = Mbrandweerwagens_onroad[i].second->getRichting();

					t = Mbrandweerwagens_onroad[i].second->getLocatie();
					locatie; locatie.first = t.second; locatie.second = t.first;
					Mbrandweerwagens_onroad[i].second->setLocatie(veranderLocatie(locatie, r));

					brandhuizen.erase(brandhuizen.begin());
				}
				else {
					for (int j = 0; j < kazernes.size(); j++) {
						if (kazernes[j]->getNaam() == Mbrandweerwagens_onroad[i].second->getBasis()) {
							output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
							output.writeToStatus("De brand is geblust en gaat nu terug naar de kazerne");
							output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
							Mbrandweerwagens_onroad[i].first->setInBrand(false);
							std::pair<int, int> t = Mbrandweerwagens_onroad[i].second->getLocatie();
							std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;

							Mbrandweerwagens_onroad[i].second->setRichting(checkRichting(kazernes[j], locatie)); Vstadsdeel::Richting r = Mbrandweerwagens_onroad[i].second->getRichting();
							Mbrandweerwagens_onroad[i].first = kazernes[j];

							t = Mbrandweerwagens_onroad[i].second->getLocatie();
							locatie; locatie.first = t.second; locatie.second = t.first;
							Mbrandweerwagens_onroad[i].second->setLocatie(veranderLocatie(locatie, r));
						}
					}
				}
			}
		}
	}
	output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
	output.writeToStatus("Mambulances onderweg:");
	output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
	for (int i = 0; i < Mambulances_onroad.size(); i++) {
		std::pair<int, int> t = Mambulances_onroad[i].second->getLocatie();
		std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
		Vstadsdeel* vak = getVstadsdeel(locatie);
		if (!checkBestemming(Mambulances_onroad[i].first, locatie)) {
			if ((dynamic_cast<Mkruispunt*>(vak)) == nullptr) {
				output.writeToStatus(Mambulances_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.second) + "," + std::to_string(t.first) + ") in de " + vak->getNaam());
				output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
				Mambulances_onroad[i].second->setLocatie(veranderLocatie(locatie, Mambulances_onroad[i].second->getRichting()));
			}
			else {
				output.writeToStatus(Mambulances_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.second) + "," + std::to_string(t.first) + ") in de " + vak->getNaam());
				output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
				Vstadsdeel::Richting r = checkRichting(Mambulances_onroad[i].first, locatie); Mambulances_onroad[i].second->setRichting(r);
				Mambulances_onroad[i].second->setLocatie(veranderLocatie(locatie, r));
			}
		}
		else {
			if (dynamic_cast<MziekenMhuis*>(Mambulances_onroad[i].first) != nullptr) {
				toRemoveMambulance.push_back(i);
				output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
				output.writeToStatus("De Mambulance is weer terug in het MziekenMhuis");
				output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
			}
			else {
				if (ongevalhuizen.size() != 0) {
					output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
					output.writeToStatus("De gewonden zijn ingeladen en gaat nu naar de volgende opdracht");
					output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
					std::pair<int, int> t = Mambulances_onroad[i].second->getLocatie();
					std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
					Mambulances_onroad[i].first = ongevalhuizen[0];
					Mambulances_onroad[i].second->setRichting(checkRichting(ongevalhuizen[0], locatie)); Vstadsdeel::Richting r = Mambulances_onroad[i].second->getRichting();

					t = Mambulances_onroad[i].second->getLocatie();
					locatie; locatie.first = t.second; locatie.second = t.first;
					Mambulances_onroad[i].second->setLocatie(veranderLocatie(locatie, r));

					ongevalhuizen.erase(ongevalhuizen.begin());
				}
				else {
					for (int j = 0; j < ziekenhuizen.size(); j++) {
						if (ziekenhuizen[j]->getNaam() == Mambulances_onroad[i].second->getBasis()) {
							output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
							output.writeToStatus("De gewonden zijn ingeladen en gaat nu terug naar het MziekenMhuis");
							output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
							std::pair<int, int> t = Mambulances_onroad[i].second->getLocatie();
							std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;

							Mambulances_onroad[i].second->setRichting(checkRichting(ziekenhuizen[j], locatie)); Vstadsdeel::Richting r = Mambulances_onroad[i].second->getRichting();
							Mambulances_onroad[i].first = ziekenhuizen[j];

							t = Mambulances_onroad[i].second->getLocatie();
							locatie; locatie.first = t.second; locatie.second = t.first;
							Mambulances_onroad[i].second->setLocatie(veranderLocatie(locatie, r));
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
		output.writeToStatus("Mhuis op locatie (" + std::to_string(brandhuizen[i]->getLocatie().second) + "," + std::to_string(brandhuizen[i]->getLocatie().first) + ") staat nog in brand met " + std::to_string(brandhuizen[i]->getBrandbaarheid()) + " brandbaarheid over");
	}

	for (int i = 0; i < Mbrandweerwagens_onroad.size(); i++) {
		
		if (dynamic_cast<Mbrandweerkazerne*>(Mbrandweerwagens_onroad[i].first) == nullptr) {
			output.writeToStatus("Mhuis op locatie (" + std::to_string(Mbrandweerwagens_onroad[i].first->getLocatie().second) + "," + std::to_string(Mbrandweerwagens_onroad[i].first->getLocatie().first) + ") staat nog in brand met " + std::to_string(Mbrandweerwagens_onroad[i].first->getBrandbaarheid()) + " brandbaarheid over");
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
							std::pair<int, int> gebouw;
							gebouw = Mbrandweerwagens_onroad[i].first->getLocatie();
							p.first = gebouw.second; p.second = gebouw.first;
							p.first += 1;
							Vstadsdeel* t = getVstadsdeel(p);
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
							std::pair<int, int> gebouw;
							gebouw = Mbrandweerwagens_onroad[i].first->getLocatie();
							p.first = gebouw.second; p.second = gebouw.first;
							p.first -= 2;
							Vstadsdeel* t = getVstadsdeel(p);
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
							std::pair<int, int> gebouw;
							gebouw = Mbrandweerwagens_onroad[i].first->getLocatie();
							p.first = gebouw.second; p.second = gebouw.first;
							p.second -= 1;
							Vstadsdeel* t = getVstadsdeel(p);
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
							std::pair<int, int> gebouw;
							gebouw = Mbrandweerwagens_onroad[i].first->getLocatie();
							p.first = gebouw.second; p.second = gebouw.first;
							p.second += 2;
							Vstadsdeel* t = getVstadsdeel(p);
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
	output.writeToStatus( "Mpolitiewagens onderweg:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = 0; i < Mpolitiewagens_onroad.size(); i++) {
		std::pair<int, int> t = Mpolitiewagens_onroad[i].second->getLocatie();
		std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
		Vstadsdeel* vak = getVstadsdeel(locatie);
		if (!checkBestemming(Mpolitiewagens_onroad[i].first, locatie)) {
			if (dynamic_cast<Mkruispunt*>(vak) == nullptr) {
				output.writeToStatus(Mpolitiewagens_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.second) + "," + std::to_string(t.first) + ") in de " + vak->getNaam());
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				Mpolitiewagens_onroad[i].second->setLocatie(veranderLocatie(locatie, Mpolitiewagens_onroad[i].second->getRichting()));
			}
			else {
				output.writeToStatus(Mpolitiewagens_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.second) + "," + std::to_string(t.first) + ") in de " + vak->getNaam());
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				Vstadsdeel::Richting r = checkRichting(Mpolitiewagens_onroad[i].first, locatie); Mpolitiewagens_onroad[i].second->setRichting(r);
				Mpolitiewagens_onroad[i].second->setLocatie(veranderLocatie(locatie, r));
			}
		}
		else {
			if (dynamic_cast<Mpolitiebureau*>(Mpolitiewagens_onroad[i].first) != nullptr) {
				toRemovePolitie.push_back(i);
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				output.writeToStatus( "De Mpolitiewagen is weer terug in de kazerne" );
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
			}
			else {
				if (overvalhuizen.size() != 0) {
					output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
					output.writeToStatus("De overvaller is gearresteerd en gaat nu naar de volgende opdracht");
					output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
					std::pair<int, int> t = Mpolitiewagens_onroad[i].second->getLocatie();
					std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
					Mpolitiewagens_onroad[i].first = overvalhuizen[0];
					Mpolitiewagens_onroad[i].second->setRichting(checkRichting(overvalhuizen[0], locatie)); Vstadsdeel::Richting r = Mpolitiewagens_onroad[i].second->getRichting();

					t = Mpolitiewagens_onroad[i].second->getLocatie();
					locatie; locatie.first = t.second; locatie.second = t.first;
					Mpolitiewagens_onroad[i].second->setLocatie(veranderLocatie(locatie, r));

					overvalhuizen.erase(overvalhuizen.begin());
				}
				else {
					for (int j = 0; j < bureaus.size(); j++) {
						if (bureaus[j]->getNaam() == Mpolitiewagens_onroad[i].second->getBasis()) {
							output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
							output.writeToStatus("De overvaller is gearresteerd en gaat nu terug naar het bureau");
							output.writeToStatus("-=-=-=-=-=-=-=-=-=-=-=-");
							std::pair<int, int> t = Mpolitiewagens_onroad[i].second->getLocatie();
							std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;

							Mpolitiewagens_onroad[i].second->setRichting(checkRichting(bureaus[j], locatie)); Vstadsdeel::Richting r = Mpolitiewagens_onroad[i].second->getRichting();
							Mpolitiewagens_onroad[i].first = bureaus[j];

							t = Mpolitiewagens_onroad[i].second->getLocatie();
							locatie; locatie.first = t.second; locatie.second = t.first;
							Mpolitiewagens_onroad[i].second->setLocatie(veranderLocatie(locatie, r));
						}
					}
				}
			}
		}
	}
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "Mwinkels die overvallen worden:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = 0; i < Mpolitiewagens_onroad.size(); i++) {
		if (dynamic_cast<Mpolitiebureau*>(Mpolitiewagens_onroad[i].first) == nullptr) {
			output.writeToStatus("Mwinkel op locatie (" + std::to_string(Mpolitiewagens_onroad[i].first->getLocatie().second) + "," + std::to_string(Mpolitiewagens_onroad[i].first->getLocatie().first) + ") wordt nog overvallen met " + std::to_string(Mpolitiewagens_onroad[i].first->getOvervalbaarheid()) + " overvalbaarheid over");
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

std::pair<int, int> Stad::veranderLocatie(std::pair<int, int> l, Vstadsdeel::Richting r)
{
	std::pair<int, int> t;
	if (r == Vstadsdeel::Links) {
		l.second -= 1;
	}
	if (r == Vstadsdeel::Rechts) {
		l.second += 1;
	}
	if (r == Vstadsdeel::Onder) {
		l.first -= 1;
	}
	if (r == Vstadsdeel::Boven) {
		l.first += 1;
	}
	t.first = l.second; t.second = l.first;
	return t;
}
bool Stad::checkStad()
{   
	bool success = true;
	for (int i = 0; i < xrow; i++) {	
		for (int j = 0; j < yrow; j++) {
			std::pair<int, int> loc; loc.first = i; loc.second =j; 
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
	std::pair<int, int> newLoc;	
	Vstadsdeel* other;
	for (int i = 0; i < getGebouwCoordsLength(); i++) {
        std::pair<int, int> loc;  loc.first = getGebouwCoord(i).second; loc.second =getGebouwCoord(i).first;
		Vstadsdeel* current = getVstadsdeel(loc);
		if (dynamic_cast<Mhuis*>(current) != nullptr) {	
			if (checkAangrenzing(current) == false) {
				success = false;
				output.writeToConsole("Inconsistente Stad: er grenst geen Mstraat aan het Mhuis op locatie" + std::to_string(current->getLocatie().first) + "|" + std::to_string(current->getLocatie().second));
				return success;
			}			
			for (int i = loc.first; i >= (loc.first-1); i--) {	
				for (int j = loc.second; j <= (loc.second+1); j++) {
					newLoc.first = i; newLoc.second = j;
					other = getVstadsdeel(newLoc);
					
					if (other != current) {
						success = false;
						output.writeToConsole("Inconsistente Stad: Mhuis op locatie " + std::to_string(current->getLocatie().first) + "|" + std::to_string(current->getLocatie().second));
						return success;
					}
					
				}
			}
		}
		else if (dynamic_cast<Mhuis*>(current) != nullptr) {
			if (checkAangrenzing(current) == false) {
				success = false;
				output.writeToConsole("Inconsistente Stad: er grenst geen Mstraat aan de kazerne op locatie" + std::to_string(current->getLocatie().first) + "|" + std::to_string(current->getLocatie().second));
				return success;
			}
			for (int i = loc.first; i >= (loc.first - 3); i--) {
				for (int j = loc.second; j <= (loc.second + 3); j++) {
					newLoc.first = i; newLoc.second = j;
					other = getVstadsdeel(newLoc);
					
					if (other != current) {
						success = false;
						output.writeToConsole("Inconsistente Stad: Kazerne op locatie " + std::to_string(current->getLocatie().first) + "|" + std::to_string(current->getLocatie().second));
						return success;
					}
				}
			}
		}
	}
	std::pair<int, int> begin;
	std::pair<int, int> end;
    for (int i = 0; i < getMstraatCoordsLength(); i++) {
       	begin.first = getMstraatCoord(i).first.second;
		begin.second = getMstraatCoord(i).first.first;
		
		end.first = getMstraatCoord(i).second.second;
		end.second = getMstraatCoord(i).second.first;
		
		Vstadsdeel* currentBegin = getVstadsdeel(begin);
		Vstadsdeel* currentEnd = getVstadsdeel(end);		
		
		if ((begin.second == 0) && (end.second == (yrow - 1))) {
			std::string stNaam;			
			if (currentBegin->getEersteMstraat() == currentEnd->getEersteMstraat()) {			
				stNaam = currentBegin->getEersteMstraat();
			}
			else {
				success = false;
				output.writeToConsole("Inconsistente Stad: Mstraat \t" + stNaam );
				return success;
			}
			for (int i = 0; i <= yrow - 1; i++) {
				newLoc.second = i;
				newLoc.first = begin.first;
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
		else if ((begin.first == 0) && (end.first == (xrow - 1))) {
			std::string stNaam;
			if (currentBegin->getTweedeMstraat() == currentEnd->getTweedeMstraat()) {
				stNaam = currentBegin->getTweedeMstraat();
			}
			else {
				success = false;
				output.writeToConsole("Inconsistente Stad: Mstraat \t" + currentBegin->getTweedeMstraat() );
				return success;
			}
			for (int i = 0; i <= xrow - 1; i++) {
				newLoc.first = i;
				newLoc.second = begin.second;
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
std::pair<int, int> Stad::getGebouwCoord(int i)
{
	return gebouwCoords[i];
}
std::pair<std::pair<int, int>, std::pair<int, int>> Stad::getMstraatCoord(int i)
{
	return MstraatCoords[i];
}
void Stad::addGebouwCoord(std::pair<int, int> g)
{
	gebouwCoords.push_back(g);
}
void Stad::addMstraatCoord(std::pair<std::pair<int, int>, std::pair<int, int>> s)
{
	MstraatCoords.push_back(s);
}
int Stad::getGebouwCoordsLength()
{
	return gebouwCoords.size();
}
int Stad::getMstraatCoordsLength()
{
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
std::pair<int, int> Vstadsdeel::getLocatie()
{
	std::pair<int, int> t; t.first = 0; t.second = 0;
	return t;
}
std::pair<int, int> Vstadsdeel::getBegin()
{
	std::pair<int, int> t; t.first = 0; t.second = 0;
	return t;
}
std::pair<int, int> Vstadsdeel::getEinde()
{
	std::pair<int, int> t; t.first = 0; t.second = 0;
	return t;
}
std::pair<int, int> Vstadsdeel::getGrootte()
{
	std::pair<int, int> t; t.first = 0; t.second = 0;
	return t;
}
bool Vstadsdeel::getInBrand()
{
	return false;
}
char Vstadsdeel::getKarakter()
{
	return '0';
}
Vstadsdeel::Type Vstadsdeel::getType()
{
	return Station;
}
std::string Vstadsdeel::getNaam()
{
	return "";
}
void Vstadsdeel::setInBrand(bool i)
{
	inBrand = i;
}
void Vstadsdeel::setBrandbaarheid(int i)
{
	brandbaarheid = 0;
}
void Vstadsdeel::setOvervalbaarheid(int i)
{
	overvalbaarheid = 0;
}
int Vstadsdeel::getBrandbaarheid()
{
	return brandbaarheid;
}
int Vstadsdeel::getOvervalbaarheid()
{
	return overvalbaarheid;
}
std::string Vstadsdeel::getEersteMstraat()
{
	return "";
}

std::string Vstadsdeel::getTweedeMstraat()
{
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
	naam = s;
}
void Mbrandweerkazerne::setLocatie(std::pair<int, int> p)
{
	locatie = p;
}
void Mbrandweerkazerne::setIngang(std::pair<int, int> p)
{
	ingang = p;
}
std::pair<int, int> Mbrandweerkazerne::getLocatie()
{
	return locatie;
}
std::pair<int, int> Mbrandweerkazerne::getIngang()
{
	return ingang;
}
std::string Mbrandweerkazerne::getNaam()
{
	return naam;
}
char Mbrandweerkazerne::getKarakter()
{
	return karakter;
}
Vstadsdeel::Type Mbrandweerkazerne::getType()
{
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
	naam = s;
}
void Mpolitiebureau::setLocatie(std::pair<int, int> p)
{
	locatie = p;
}
void Mpolitiebureau::setIngang(std::pair<int, int> p)
{
	ingang = p;
}
void Mpolitiebureau::setGrootte(std::pair<int, int> p)
{
	grootte = p;
}
std::pair<int, int> Mpolitiebureau::getLocatie()
{
	return locatie;
}
std::pair<int, int> Mpolitiebureau::getIngang()
{
	return ingang;
}
std::pair<int, int> Mpolitiebureau::getGrootte()
{
	return grootte;
}
std::string Mpolitiebureau::getNaam()
{
	return naam;
}
char Mpolitiebureau::getKarakter()
{
	return karakter;
}
Vstadsdeel::Type Mpolitiebureau::getType()
{
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
	naam = s;
}
void MziekenMhuis::setLocatie(std::pair<int, int> p)
{
	locatie = p;
}
void MziekenMhuis::setIngang(std::pair<int, int> p)
{
	ingang = p;
}
void MziekenMhuis::setGrootte(std::pair<int, int> p)
{
	grootte = p;
}
std::pair<int, int> MziekenMhuis::getLocatie()
{
	return locatie;
}
std::pair<int, int> MziekenMhuis::getIngang()
{
	return ingang;
}
std::pair<int, int> MziekenMhuis::getGrootte()
{
	return grootte;
}
std::string MziekenMhuis::getNaam()
{
	return naam;
}
char MziekenMhuis::getKarakter()
{
	return karakter;
}
Vstadsdeel::Type MziekenMhuis::getType()
{
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
	naam = n;
}
void Mbrandweerwagen::setBasis(std::string b)
{
	basis = b;
}
void Mbrandweerwagen::setLocatie(std::pair<int, int> p)
{
	locatie = p;
}
void Mbrandweerwagen::setRichting(Vstadsdeel::Richting r)
{
	richting = r;
}
std::pair<int, int> Mbrandweerwagen::getLocatie()
{
	return locatie;
}
Vstadsdeel::Richting Mbrandweerwagen::getRichting()
{
	return richting;
}
std::string Mbrandweerwagen::getBasis()
{
	return basis;
}
std::string Mbrandweerwagen::getNaam()
{
	return naam;
}
char Mbrandweerwagen::getKarakter()
{
	return karakter;
}
Vstadsdeel::Type Mbrandweerwagen::getType()
{
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
	naam = n;
}
void Mpolitiewagen::setBasis(std::string b)
{
	basis = b;
}
void Mpolitiewagen::setRichting(Vstadsdeel::Richting r)
{
	richting = r;
}
void Mpolitiewagen::setLocatie(std::pair<int, int> p)
{
	locatie = p;
}
Vstadsdeel::Richting Mpolitiewagen::getRichting()
{
	return richting;
}
std::pair<int, int> Mpolitiewagen::getLocatie()
{
	return locatie;
}
std::string Mpolitiewagen::getBasis()
{
	return basis;
}
std::string Mpolitiewagen::getNaam()
{
	return naam;
}
char Mpolitiewagen::getKarakter()
{
	return karakter;
}
Vstadsdeel::Type Mpolitiewagen::getType()
{
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
	naam = n;
}
void Mambulance::setBasis(std::string b)
{
	basis = b;
}
void Mambulance::setRichting(Vstadsdeel::Richting r)
{
	richting = r;
}
void Mambulance::setLocatie(std::pair<int, int> p)
{
	locatie = p;
}
Vstadsdeel::Richting Mambulance::getRichting()
{
	return richting;
}
std::pair<int, int> Mambulance::getLocatie()
{
	return locatie;
}
std::string Mambulance::getBasis()
{
	return basis;
}
std::string Mambulance::getNaam()
{
	return naam;
}
char Mambulance::getKarakter()
{
	return karakter;
}
Vstadsdeel::Type Mambulance::getType()
{
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
	naam = n;
}
void Mstraat::setBegin(std::pair<int, int> b)
{
	begin = b;
}
void Mstraat::setEinde(std::pair<int, int> e)
{
	einde = e;
}
std::string Mstraat::getNaam()
{
	return naam;
}
std::pair<int, int> Mstraat::getBegin()
{
	return begin;
}
std::pair<int, int> Mstraat::getEinde()
{
	return einde;
}
char Mstraat::getKarakter()
{
	return karakter;
}
Vstadsdeel::Type Mstraat::getType()
{
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
	inBrand = i;
}
bool Mhuis::getInBrand()
{
	return inBrand;
}
void Mhuis::setLocatie(std::pair<int, int> l)
{
	locatie = l;
}
void Mhuis::setBrandbaarheid(int b)
{
	brandbaarheid = b;
}
int Mhuis::getBrandbaarheid()
{
	return brandbaarheid;
}
std::pair<int, int> Mhuis::getLocatie()
{
	return locatie;
}
char Mhuis::getKarakter()
{
	return karakter;
}
Vstadsdeel::Type Mhuis::getType()
{
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
	inBrand = i;
}
bool Mwinkel::getInBrand()
{
	return inBrand;
}
void Mwinkel::setLocatie(std::pair<int, int> l)
{
	locatie = l;
}
void Mwinkel::setGrootte(std::pair<int, int> p)
{
	grootte = p;
}
void Mwinkel::setBrandbaarheid(int b)
{
	brandbaarheid = b;
}
void Mwinkel::setOvervalbaarheid(int b)
{
	overvalbaarheid = b;
}
int Mwinkel::getBrandbaarheid()
{
	return brandbaarheid;
}
int Mwinkel::getOvervalbaarheid()
{
	return overvalbaarheid;
}
std::pair<int, int> Mwinkel::getLocatie()
{
	return locatie;
}
std::pair<int, int> Mwinkel::getGrootte()
{
	return grootte;
}
char Mwinkel::getKarakter()
{
	return karakter;
}
Vstadsdeel::Type Mwinkel::getType()
{
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
	eersteMstraat = s;
}
void Mkruispunt::setTweedeMstraat(std::string s)
{
	tweedeMstraat = s;
}
void Mkruispunt::setLocatie(std::pair<int, int> l)
{
	locatie = l;
}
std::string Mkruispunt::getEersteMstraat()
{
	return eersteMstraat;
}
std::string Mkruispunt::getTweedeMstraat()
{
	return tweedeMstraat;
}
char Mkruispunt::getKarakter()
{
	return karakter;
}
std::string Mkruispunt::getNaam()
{
	return eersteMstraat;
}
Vstadsdeel::Type Mkruispunt::getType()
{
	return type;
}
/*
METHODES VOOR OUTPUTCLASS
*/
OutputClass::OutputClass(void)
{
	status = new std::ofstream("status.txt", std::ofstream::out);
	console = new std::ofstream("console.txt", std::ofstream::out);
}
void OutputClass::writeToConsole(std::string s)
{
	*console << s << std::endl;
}
void OutputClass::writeToConsole()
{
	*console << std::endl;
}
void OutputClass::writeToStatus(std::string s)
{
	if (ui.getSU())
	{
		*status << s << std::endl;
	}
}void OutputClass::writeToStatus()
{
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
}

void UI::showMenu()
{
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
	int x = 0; int y = 0; std::pair<int, int> coord;
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	std::cout << "Geef de x coordinaat van het Mhuis op: ";
	std::cin >> x;
	std::cout << "Geef de y coordinaat van het Mhuis op: ";
	std::cin >> y;
	coord.first = y; coord.second = x; 
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
	int x = 0; int y = 0; std::pair<int, int> coord;
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	std::cout << "Geef de x coordinaat van de Mwinkel op: ";
	std::cin >> x;
	std::cout << "Geef de y coordinaat van de Mwinkel op: ";
	std::cin >> y;
	coord.first = y; coord.second = x;
	if (dynamic_cast<Mwinkel*>(stad->getVstadsdeel(coord)) != nullptr)
	{
		stad->overvalhuizen.push_back(stad->getVstadsdeel(coord));
	}
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl << std::endl;
	showMenu();
}
bool UI::startMbrandweerwagen()
{
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
	int choice = c;
}
void UI::setSU(bool s) //Simpele Uitvoer
{
	SU = s;
}
void UI::setStad(Stad* s)
{
	stad = s;
}
void UI::runSimulatie()
{
	for (int i = 0; i < steps; i++) {
		stad->simulatie2();
	}
	showMenu();
}
bool UI::getSU()
{
	return SU;
}
void UI::showMap()
{
	stad->printStad();
	showMenu();
}


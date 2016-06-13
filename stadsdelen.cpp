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

	//memset(grid,,sizeof(grid));
	for (int i = 0; i < xrow; i++)
	{
		for (int j = 0; j < yrow; j++)
		{
			grid[i][j] = nullptr;
		}
	}
	//status = new std::ofstream("status.txt", std::ofstream::out);
	//console = new std::ofstream("console.txt", std::ofstream::out);
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
void Stad::setStadsdeel(Stadsdeel &s)
{
	//REQUIRE(this->properlyInitialized(), "Stad wasn't initialized when calling stStadsdeel");
	if ((dynamic_cast<Brandweerkazerne*>(&s)) != nullptr)
	{
		std::pair<int, int> l = s.getLocatie();
		for (int i = l.second; i > (l.second - 4); i--)
		{
			for (int j = l.first; j < (l.first + 4); j++)
			{
				grid[i][j] = &s;
			}
		}
	}
	else if ((dynamic_cast<Politiebureau*>(&s)) != nullptr)
	{
		std::pair<int, int> l = s.getLocatie();
		std::pair<int, int> g = s.getGrootte();
		for (int i = l.second; i >(l.second - g.first); i--)
		{
			for (int j = l.first; j < (l.first + g.second); j++)
			{
				grid[i][j] = &s;
			}
		}
	}
	else if ((dynamic_cast<Ziekenhuis*>(&s)) != nullptr)
	{
		std::pair<int, int> l = s.getLocatie();
		std::pair<int, int> g = s.getGrootte();
		for (int i = l.second; i >(l.second - g.first); i--)
		{
			for (int j = l.first; j < (l.first + g.second); j++)
			{
				grid[i][j] = &s;
			}
		}
	}
	else if ((dynamic_cast<Straat*>(&s)) != nullptr)
	{
		std::pair<int, int> b = s.getBegin();  std::pair<int, int> e = s.getEinde();
		if (b.first == e.first)
		{
			for (int i = (b.second); i <= (e.second); i++)
			{
				Stadsdeel* t = grid[i][b.first];
				if (t == nullptr)
				{
					grid[i][b.first] = &s;
				}
				else
				{
					if (grid[i][b.first]->getType() == 6)
					{
						Kruispunt* k = new Kruispunt();
						k->setEersteStraat(grid[i][b.first]->getNaam());
						k->setTweedeStraat(s.getNaam());
						k->setLocatie(s.getLocatie());
						grid[i][b.first] = k;
					}
					else
					{
						break; // HIER MOET NOG WAT WERK VERRICHT WORDEN VANWEGE DE BREAK-OUT
					}
				}
			}
		}
		else
		{
			for (int i = (b.first); i <= (e.first); i++)
			{
				Stadsdeel* t = grid[b.second][i];
				if (t == nullptr)
				{
					grid[b.second][i] = &s;
				}
				else
				{
					if (grid[i][b.first]->getType() == 3)
					{
						Kruispunt* k = new Kruispunt();
						k->setEersteStraat(grid[i][b.first]->getNaam());
						k->setTweedeStraat(s.getNaam());
						k->setLocatie(s.getLocatie());
						grid[b.second][i] = k;
					}
					else
					{
						break; // HIER MOET NOG WAT WERK VERRICHT WORDEN VANWEGE DE BREAK-OUT
					}
				}
			}
		}
	}
	else if ((dynamic_cast<Winkel*>(&s)) != nullptr)
	{
		std::pair<int, int> l = s.getLocatie();
		std::pair<int, int> g = s.getGrootte();
		for (int i = l.second; i >(l.second - g.first); i--)
		{
			for (int j = l.first; j < (l.first + g.second); j++)
			{
				grid[i][j] = &s;
			}
		}
	}
	else if ((dynamic_cast<Huis*>(&s)) != nullptr)
	{
		std::pair<int, int> l = s.getLocatie();
		for (int i = l.second; i >(l.second - 2); i--)
		{
			for (int j = l.first; j < (l.first + 2); j++)
			{
				grid[i][j] = &s;
			}
		}
	}
}
Stadsdeel* Stad::getStadsdeel(std::pair<int, int> p)
{
	//REQUIRE(this->properlyInitialized(), "Stadsdeel not properly initialized");
	if ((p.first >= 0 && p.first <= xrow) && (p.second >= 0 && p.second <= yrow))
	{
		return grid[p.first][p.second];
	}
	else
	{
		return nullptr;
	}
}

bool Stad::checkAangrenzing(Stadsdeel* s)
{
	//REQUIRE(this->properlyInitialized(), "Stad not properly initialized");
	int aantal = 0;
	if ((dynamic_cast<Politiebureau*>(s)) != nullptr)
	{
		aantal = 4;
	}
	else if ((dynamic_cast<Huis*>(s)) != nullptr)
	{
		aantal = 2;
	}
	std::pair<int, int> p = s->getLocatie();
	std::pair<int, int> l;
	Stadsdeel* t;
	for (int i = (p.second + 1); i > (p.second - aantal - 1); i--)
	{
		for (int j = (p.first - 1); j < (p.first + aantal + 1); j++)
		{
			l.first = i; l.second = j;
			t = getStadsdeel(l);
			if ((dynamic_cast<Straat*>(t)) != nullptr)
			{
				return true;
			}
		}
	}
	return false;
}
void Stad::printStad()
{
	Stadsdeel* vak;
	output.writeToStatus( "Legenda:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "H = Onderdeel van een huis" );
	output.writeToStatus( "X = Onderdeel van de kazerne" );
	output.writeToStatus( "Z = Onderdeel van het ziekenhuis" );
	output.writeToStatus( "P = Onderdeel van het politiebureau" );
	output.writeToStatus( "W = Onderdeel van de winkel" );
	output.writeToStatus( "_ = Onderdeel van een straat" );
	output.writeToStatus( "O = Een kruispunt" );
	output.writeToStatus( "B = Startlocatie van een brandweerwagen" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = (xrow-1); i >= 0; i--)
	{
		std::string lijn = "";
		for (int j = 0; j < yrow; j++)
		{
			bool found = false;
			std::pair<int, int> remember;
			for (int k = 0; k < brandweerwagens.size(); k++)
			{
				
				if (i == brandweerwagens[k]->getLocatie().second && j == brandweerwagens[k]->getLocatie().first && remember != brandweerwagens[k]->getLocatie())
				{
					lijn = lijn + "B" + " ";
					remember = brandweerwagens[k]->getLocatie();
					found = true;
				}
			}
			if (!found)
			{
				vak = grid[i][j];
				lijn = lijn + vak->getKarakter() + " ";
			}
		}
		output.writeToStatus( lijn );
	}
}
bool Stad::checkBestemming(Stadsdeel* s, std::pair<int, int> p)
{
	Stadsdeel* temp;
	std::pair<int, int> coordinaat;
	// RECHTS
	coordinaat.first = p.first; coordinaat.second = p.second + 1;
	temp = getStadsdeel(coordinaat);
	if (temp == s)
	{
		//std::cout + "RECHTS" );
		return true;
	}
	// LINKS
	coordinaat.first = p.first; coordinaat.second = p.second - 1;
	temp = getStadsdeel(coordinaat);
	if (temp == s)
	{
		//std::cout + "LINKS" );
		return true;
	}
	// ONDER
	coordinaat.first = p.first - 1; coordinaat.second = p.second;
	temp = getStadsdeel(coordinaat);
	if (temp == s)
	{
		//std::cout + "ONDER" );
		return true;
	}
	// BOVEN
	coordinaat.first = p.first + 1; coordinaat.second = p.second;
	temp = getStadsdeel(coordinaat);
	if (temp == s)
	{
		//std::cout + "BOVEN" );
		return true;
	}
	return false;
}
Stad::StraatType Stad::checkStraat(Stadsdeel* s, std::pair<int, int> p)
{
	Stadsdeel* vak = getStadsdeel(p);
	std::pair<int, int> locatie = s->getLocatie();
	if ((dynamic_cast<Straat*>(vak)) != nullptr)
	{
		std::pair<int, int> s1 = vak->getBegin();
		std::pair<int, int> s2 = vak->getEinde();
		if ((s1.first - s2.first) == 0)
		{
			int x = (locatie.first - p.second);
			if (x <= 1 && x >= -1)
			{
				return Stad::Verticaal;
			}
		}
		if ((s1.second - s2.second) == 0)
		{
			int y = (locatie.second - p.first);
			if (y <= 2 && y >= -1)
			{
				return Stad::Horizontaal;
			}
			
		} 
	}
	if ((dynamic_cast<Kruispunt*>(vak)) != nullptr)
	{
		int x = (locatie.first - p.second);
		if (x <= 1 && x >= -1)
		{
			return Stad::Verticaal;
		}
		int y = (locatie.second - p.first);
		if (y <= 2 && y >= -1)
		{
			return Stad::Horizontaal;
		}
	} return Stad::Geen;
}
Stadsdeel::Richting Stad::checkRichting(Stadsdeel* s, std::pair<int, int> p)
{
	Stadsdeel* vak = getStadsdeel(p);
	std::pair<int, int> locatie = s->getLocatie();
	if ((dynamic_cast<Straat*>(vak)) != nullptr)
	{
		std::pair<int, int> s1 = vak->getBegin();
		std::pair<int, int> s2 = vak->getEinde();
		std::pair<int, int> bovenlid; int bovenafstand = -1;
		std::pair<int, int> onderlid; int onderafstand = -1;
		std::pair<int, int> linkerlid; int linkerafstand = -1;
		std::pair<int, int> rechterlid; int rechterafstand = -1;
		if ((s1.first - s2.first) == 0)
		{
			for (int i = p.first; i >= 0; i--)
			{
				std::pair<int, int> p2; p2.first = i; p2.second = p.second;
				Stadsdeel* tijdelijk = getStadsdeel(p2);
				if ((dynamic_cast<Kruispunt*>(tijdelijk)) != nullptr)
				{
					onderlid = p2;
					onderafstand = abs(onderlid.first - s->getLocatie().second) + abs(onderlid.second - s->getLocatie().first);
					break;
				}
			}
			for (int i = p.first; i < xrow; i++)
			{
				std::pair<int, int> p2; p2.first = i; p2.second = p.second;
				Stadsdeel* tijdelijk = getStadsdeel(p2);
				if ((dynamic_cast<Kruispunt*>(tijdelijk)) != nullptr)
				{
					bovenlid = p2;
					bovenafstand = abs(bovenlid.first - s->getLocatie().second) + abs(bovenlid.second - s->getLocatie().first);
					break;
				}
			}
			if (onderafstand == bovenafstand)
			{
				return Stadsdeel::Richting::Onder;
			}
			else
			{
				if (onderafstand < bovenafstand)
				{
					return Stadsdeel::Richting::Onder;
				}
				else
				{
					return Stadsdeel::Richting::Boven;
				}
			}
			
		}
		if ((s1.second - s2.second) == 0)
		{
			for (int i = p.second; i >= 0; i--)
			{
				std::pair<int, int> p2; p2.first = p.first; p2.second = i;
				Stadsdeel* tijdelijk = getStadsdeel(p2);
				if ((dynamic_cast<Kruispunt*>(tijdelijk)) != nullptr)
				{
					linkerlid = p2;
					linkerafstand = abs(linkerlid.first - s->getLocatie().second) + abs(linkerlid.second - s->getLocatie().first);
					break;
				}
			}
			for (int i = p.second; i < yrow; i++)
			{
				std::pair<int, int> p2; p2.first = p.first; p2.second = i;
				Stadsdeel* tijdelijk = getStadsdeel(p2);
				if ((dynamic_cast<Kruispunt*>(tijdelijk)) != nullptr)
				{
					rechterlid = p2;
					rechterafstand = abs(rechterlid.first - s->getLocatie().second) + abs(rechterlid.second - s->getLocatie().first);
					break;
				}
			}
			if (linkerafstand == rechterafstand)
			{
				return Stadsdeel::Richting::Links;
			}
			else
			{
				if (linkerafstand < rechterafstand)
				{
					return Stadsdeel::Richting::Links;
				}
				else
				{
					return Stadsdeel::Richting::Rechts;
				}
			}
			//std::cout + linkerafstand + "|" + rechterafstand );
		} 
	}
	if ((dynamic_cast<Kruispunt*>(vak)) != nullptr)
	{
		std::pair<int, int> bovenlid; int bovenafstand = -1;
		std::pair<int, int> onderlid; int onderafstand = -1;
		std::pair<int, int> linkerlid; int linkerafstand = -1;
		std::pair<int, int> rechterlid; int rechterafstand = -1;
		Stadsdeel::Richting finale; int finaleafstand = 100;
		for (int i = p.first - 1; i >= 0; i--)
		{
			std::pair<int, int> p2; p2.first = i; p2.second = p.second;
			Stadsdeel* tijdelijk = getStadsdeel(p2);
			if (tijdelijk != nullptr)
			{
				if ((dynamic_cast<Kruispunt*>(tijdelijk)) != nullptr)
				{
					onderlid = p2;
					onderafstand = abs(onderlid.first - s->getLocatie().second) + abs(onderlid.second - s->getLocatie().first);
					break;
				}
			}
			else
			{
				break;
			}
		}
		for (int i = p.first + 1; i < xrow; i++)
		{
			std::pair<int, int> p2; p2.first = i; p2.second = p.second;
			Stadsdeel* tijdelijk = getStadsdeel(p2);
			if (tijdelijk != nullptr)
			{
				if ((dynamic_cast<Kruispunt*>(tijdelijk)) != nullptr)
				{
					bovenlid = p2;
					bovenafstand = abs(bovenlid.first - s->getLocatie().second) + abs(bovenlid.second - s->getLocatie().first);
					break;
				}
			}
			else
			{
				break;
			}
		}
		for (int i = p.second - 1; i >= 0; i--)
		{
			std::pair<int, int> p2; p2.first = p.first; p2.second = i;
			Stadsdeel* tijdelijk = getStadsdeel(p2);
			if (tijdelijk != nullptr)
			{
				if ((dynamic_cast<Kruispunt*>(tijdelijk)) != nullptr)
				{
					linkerlid = p2;
					int t1 = abs(linkerlid.first - s->getLocatie().second);
					int t2 = abs(linkerlid.second - s->getLocatie().first);
					linkerafstand = abs(linkerlid.first - s->getLocatie().second) + abs(linkerlid.second - s->getLocatie().first);
					break;
				}
			}
			else
			{
				break;
			}
			
		}
		for (int i = p.second + 1; i < yrow; i++)
		{
			std::pair<int, int> p2; p2.first = p.first; p2.second = i;
			Stadsdeel* tijdelijk = getStadsdeel(p2);
			if (tijdelijk != nullptr)
			{
				if ((dynamic_cast<Kruispunt*>(tijdelijk)) != nullptr)
				{
					rechterlid = p2;
					rechterafstand = abs(rechterlid.first - s->getLocatie().second) + abs(rechterlid.second - s->getLocatie().first);
					break;
				}
			}
			else
			{
				break;
			}
		}
		if (onderafstand != -1)
		{
			if (onderafstand < finaleafstand)
			{
				finaleafstand = onderafstand;
				finale = Stadsdeel::Richting::Onder;
			}
		}
		if (bovenafstand != -1)
		{
			if (bovenafstand < finaleafstand)
			{
				finaleafstand = bovenafstand;
				finale = Stadsdeel::Richting::Boven;
			}
		}
		if (linkerafstand != -1)
		{
			if (linkerafstand < finaleafstand)
			{
				finaleafstand = linkerafstand;
				finale = Stadsdeel::Richting::Links;
			}
		}
		if (rechterafstand != -1)
		{
			if (rechterafstand < finaleafstand)
			{
				finaleafstand = rechterafstand;
				finale = Stadsdeel::Richting::Rechts;
			}
		}
		StraatType verificatie = checkStraat(s, p);
		if (verificatie == Horizontaal && (finale == Stadsdeel::Boven || finale == Stadsdeel::Onder))
		{
			//std::cout + "Correctie" );
			if ((s->getLocatie().first - p.second) < 0)
			{
				return Stadsdeel::Richting::Links;
			}
			else
			{
				return Stadsdeel::Richting::Rechts;
			}
		}
		else if (verificatie == Verticaal && (finale == Stadsdeel::Links || finale == Stadsdeel::Rechts))
		{
			//std::cout + "Correctie" );
			if ((s->getLocatie().second - p.first) < 0)
			{
				return Stadsdeel::Richting::Onder;
			}
			else
			{
				return Stadsdeel::Richting::Boven;
			}
		}
		else
		{
			return finale;
		}
	}
	return Stadsdeel::Richting::Rechts;
}

bool Stad::stadInlezen(const char* s)
{	//Omvormen naar pointer
	TiXmlDocument doc;
	TiXmlElement* root;
	if (!doc.LoadFile(s))
	{
		//std::cerr + doc.ErrorDesc() );
		output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
		output.writeToConsole("Failed to open/load file" );
		output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
		return false;
	}
	else
	{
		//std::cout + "-=-=-=-=-=-=-=-=-=-=-=-" );
		//std::cout + "Loading file succesful" );
		//std::cout + "-=-=-=-=-=-=-=-=-=-=-=-" );
		std::string s2 = s;
		output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
		output.writeToConsole("Loading file - " + s2 + " - succesful" );
		output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
		root = doc.FirstChildElement();
		if (root == NULL)
		{
			//std::cout + "-=-=-=-=-=-=-=-=-=-=-=-" );
			//std::cerr + "Failed to load file: No root element" );
			//std::cout + "-=-=-=-=-=-=-=-=-=-=-=-" );

			output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
			output.writeToConsole("Failed to load file : No root element" );
			output.writeToConsole("-=-=-=-=-=-=-=-=-=-=-=-" );
			return false;
		}
		else
		{
			for (TiXmlNode* n = root->FirstChild(); n != NULL; n = n->NextSibling())
			{
				std::string nodeName = n->Value();
				std::string naam; std::string basis;
				std::pair<int, int> van; std::pair<int, int> naar; std::pair<int, int> grootte;
				int brandbaarheid; int overvalbaarheid;

				for (TiXmlElement* e = n->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
				{
					std::string elementName = e->Value();
					int counter = 0;
					if (!elementName.compare("Naam"))
					{
						naam = e->GetText();
					}
					if (!elementName.compare("Basis"))
					{
						basis = e->GetText();
					}
					if (!elementName.compare("Brandbaarheid"))
					{
						brandbaarheid = atoi(e->GetText());
					}
					if (!elementName.compare("Overvalbaarheid"))
					{
						overvalbaarheid = atoi(e->GetText());
					}
					for (TiXmlAttribute* a = e->FirstAttribute(); a != NULL; a = a->Next())
					{
						if (a != NULL)
						{
							if (!elementName.compare("Locatie"))
							{
								if (counter == 0)
								{
									try
									{
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> van.first;
										counter++;
									}
									catch (...)
									{
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//van.first = atoi(a->Value());
								}
								else
								{
									try
									{
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> van.second;
									}
									catch (...)
									{
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//van.second = atoi(a->Value());
								}
							}
							if (!elementName.compare("Ingang"))
							{
								if (counter == 0)
								{
									try
									{
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> naar.first;
										counter++;
									}
									catch (...)
									{
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//naar.first = atoi(a->Value());
								}
								else
								{
									try
									{
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> naar.second;
									}
									catch (...)
									{
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//naar.second = atoi(a->Value());
								}
							}
							/*

							*/
							if (!elementName.compare("Van"))
							{
								if (counter == 0)
								{
									try
									{
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> van.first;
										counter++;
									}
									catch (...)
									{
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//van.first = atoi(a->Value());
								}
								else
								{
									try
									{
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> van.second;
									}
									catch (...)
									{
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//van.second = atoi(a->Value());
								}
							}
							if (!elementName.compare("Naar"))
							{
								if (counter == 0)
								{
									try
									{
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> naar.first;
										counter++;
									}
									catch (...)
									{
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//naar.first = atoi(a->Value());
								}
								else
								{
									try
									{
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> naar.second;
									}
									catch (...)
									{
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//naar.second = atoi(a->Value());
								}
							}
							if (!elementName.compare("Grootte"))
							{
								if (counter == 0)
								{
									try
									{
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> grootte.first;
										counter++;
									}
									catch (...)
									{
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//naar.first = atoi(a->Value());
								}
								else
								{
									try
									{
										std::stringstream ss; ss.exceptions(std::ios::failbit | std::ios::badbit);
										ss << a->Value();
										ss >> grootte.second;
									}
									catch (...)
									{
										output.writeToConsole("Failed to convert string to int. Exiting parser" );
										return false;
									}
									//naar.second = atoi(a->Value());
								}
							}
						}
					}
				}
				if (!nodeName.compare("Brandweerkazerne"))
				{
					Brandweerkazerne *bk = new Brandweerkazerne();
					bk->setNaam(naam);
					bk->setLocatie(van);
					bk->setIngang(naar);
					setStadsdeel(*bk);
					kazernes.push_back(bk);

					addGebouwCoord(van);//vector
				}
				if (!nodeName.compare("PolitieBureau"))
				{
					Politiebureau *pb = new Politiebureau();
					pb->setNaam(naam);
					pb->setLocatie(van);
					pb->setIngang(naar);
					pb->setGrootte(grootte);
					setStadsdeel(*pb);
					bureaus.push_back(pb);
				}
				if (!nodeName.compare("Ziekenhuis"))
				{
					Ziekenhuis *zh = new Ziekenhuis();
					zh->setNaam(naam);
					zh->setLocatie(van);
					zh->setIngang(naar);
					zh->setGrootte(grootte);
					setStadsdeel(*zh);
					ziekenhuizen.push_back(zh);
				}
				if (!nodeName.compare("Brandweerwagen"))
				{
					Brandweerwagen *bw = new Brandweerwagen();
					bw->setNaam(naam);
					bw->setBasis(basis);
					std::pair<int, int> locatie; locatie.first = 0; locatie.second = 0;
					for (int i = 0; i < kazernes.size(); i++)
					{
						if (kazernes[i]->getNaam() == basis)
						{
							locatie = kazernes[i]->getIngang();
						}
					}
					if (locatie.first != 0 && locatie.second != 0)
					{
						bw->setLocatie(locatie);
					}
					brandweerwagens.push_back(bw);
					brandweerwagens_onstandby.push_back(bw);
				}
				if (!nodeName.compare("Politiewagen"))
				{
					Politiewagen *pw = new Politiewagen();
					pw->setNaam(naam);
					pw->setBasis(basis);
					std::pair<int, int> locatie; locatie.first = 0; locatie.second = 0;
					for (int i = 0; i < bureaus.size(); i++)
					{
						if (bureaus[i]->getNaam() == basis)
						{
							locatie = bureaus[i]->getIngang();
						}
					}
					if (locatie.first != 0 && locatie.second != 0)
					{
						pw->setLocatie(locatie);
					}
					politiewagens.push_back(pw);
					politiewagens_onstandby.push_back(pw);
				}
				if (!nodeName.compare("Ziekenwagen"))
				{
					Ambulance *am = new Ambulance();
					am->setNaam(naam);
					am->setBasis(basis);
					std::pair<int, int> locatie; locatie.first = 0; locatie.second = 0;
					for (int i = 0; i < ziekenhuizen.size(); i++)
					{
						if (ziekenhuizen[i]->getNaam() == basis)
						{
							locatie = ziekenhuizen[i]->getIngang();
							am->setLocatie(locatie);
						}
					}
					ambulances.push_back(am);
				}
				if (!nodeName.compare("Straat"))
				{
					Straat *s = new Straat();
					s->setNaam(naam);
					s->setBegin(van);
					s->setEinde(naar);
					setStadsdeel(*s);

                    std::pair<std::pair<int, int>, std::pair<int, int>> coords; //vector
                    coords.first = van; coords.second = naar;					
                    addStraatCoord(coords);	
				}
				if (!nodeName.compare("Huis"))
				{
					Huis *h = new Huis();
					h->setLocatie(van);
					h->setBrandbaarheid(brandbaarheid);
					setStadsdeel(*h);
					addGebouwCoord(van);
				}
				if (!nodeName.compare("Winkel"))
				{
					Winkel *w = new Winkel();
					w->setLocatie(van);
					w->setOvervalbaarheid(overvalbaarheid);
					w->setGrootte(grootte);
					setStadsdeel(*w);
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
	int ib = brandhuizen.size(); int iw = brandweerwagens_onstandby.size();
	std::vector<int> toRemoveBrandweer; std::vector<int> toRemovePolitie;
	for (int i = 0; i < ib; i++)
	{
		for (int j = 0; j < iw; j++)
		{
			if (ib != 0)
			{
				std::pair<Stadsdeel*, Brandweerwagen*> temp;

				std::pair<int, int> t = brandweerwagens_onstandby[j]->getLocatie();
				std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
				Stadsdeel::Richting r = checkRichting(brandhuizen[i], locatie);
				brandweerwagens_onstandby[j]->setRichting(r);

				temp.first = brandhuizen[i]; temp.second = brandweerwagens_onstandby[j];
				brandweerwagens_onroad.push_back(temp);
				brandweerwagens_onstandby.erase(brandweerwagens_onstandby.begin() + j);
				brandhuizen.erase(brandhuizen.begin() + i);
				iw -= 1; ib -= 1; j -= 1;
			}
		} i -= 1;
	}
	for (int i = 0; i < brandhuizen.size(); i++)
	{
		brandhuizen[i]->setBrandbaarheid(brandhuizen[i]->getBrandbaarheid() - 1);
		output.writeToStatus("Huis op locatie (" + std::to_string(brandhuizen[i]->getLocatie().second) + "," + std::to_string(brandhuizen[i]->getLocatie().first) + ") staat nog in brand met " + std::to_string(brandhuizen[i]->getBrandbaarheid()) + " brandbaarheid over");
	}
	ib = overvalhuizen.size(); iw = politiewagens_onstandby.size();
	for (int i = 0; i < ib; i++)
	{
		for (int j = 0; j < iw; j++)
		{
			if (ib != 0)
			{
				std::pair<Stadsdeel*, Politiewagen*> temp;

				std::pair<int, int> t = politiewagens_onstandby[j]->getLocatie();
				std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
				Stadsdeel::Richting r = checkRichting(overvalhuizen[i], locatie);
				politiewagens_onstandby[j]->setRichting(r);

				temp.first = overvalhuizen[i]; temp.second = politiewagens_onstandby[j];
				politiewagens_onroad.push_back(temp);
				politiewagens_onstandby.erase(politiewagens_onstandby.begin() + j);
				overvalhuizen.erase(overvalhuizen.begin() + i);
				iw -= 1; ib -= 1; j -= 1;
			}
		} i -= 1;
	}



	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "Brandweerwagens onderweg:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = 0; i < brandweerwagens_onroad.size(); i++)
	{
		std::pair<int, int> t = brandweerwagens_onroad[i].second->getLocatie();
		std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
		Stadsdeel* vak = getStadsdeel(locatie);
		if (!checkBestemming(brandweerwagens_onroad[i].first, locatie))
		{
			if ((dynamic_cast<Kruispunt*>(vak)) == nullptr)
			{
				output.writeToStatus(brandweerwagens_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.second) + "," + std::to_string(t.first) + ") in de " + vak->getNaam());
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				brandweerwagens_onroad[i].second->setLocatie(veranderLocatie(locatie, brandweerwagens_onroad[i].second->getRichting()));
			}
			else
			{
				output.writeToStatus(brandweerwagens_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.second) + "," + std::to_string(t.first) + ") in de " + vak->getNaam());
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				Stadsdeel::Richting r = checkRichting(brandweerwagens_onroad[i].first, locatie); brandweerwagens_onroad[i].second->setRichting(r);
				brandweerwagens_onroad[i].second->setLocatie(veranderLocatie(locatie, r));
			}
		}
		else
		{
			if (dynamic_cast<Brandweerkazerne*>(brandweerwagens_onroad[i].first) != nullptr)
			{
				toRemoveBrandweer.push_back(i);
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				output.writeToStatus( "De brandweerwagen is weer terug in de kazerne" );
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
			}
			else
			{
				for (int j = 0; j < kazernes.size(); j++)
				{
					if (kazernes[j]->getNaam() == brandweerwagens_onroad[i].second->getBasis())
					{
						output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
						output.writeToStatus( "De brand is geblust en gaat nu terug naar de kazerne" );
						output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
						std::pair<int, int> t = brandweerwagens_onroad[i].second->getLocatie();
						std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;

						brandweerwagens_onroad[i].first->setBrandbaarheid(20);
						brandweerwagens_onroad[i].second->setRichting(checkRichting(kazernes[j], locatie)); Stadsdeel::Richting r = brandweerwagens_onroad[i].second->getRichting();
						brandweerwagens_onroad[i].first = kazernes[j];

						t = brandweerwagens_onroad[i].second->getLocatie();
						locatie; locatie.first = t.second; locatie.second = t.first;
						brandweerwagens_onroad[i].second->setLocatie(veranderLocatie(locatie, r));
					}
				}
			}
		}
	}
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "Huizen die in brand staan:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = 0; i < brandweerwagens_onroad.size(); i++)
	{
		if (dynamic_cast<Brandweerkazerne*>(brandweerwagens_onroad[i].first) == nullptr)
		{
			output.writeToStatus("Huis op locatie (" + std::to_string(brandweerwagens_onroad[i].first->getLocatie().second) + "," + std::to_string(brandweerwagens_onroad[i].first->getLocatie().first) + ") staat nog in brand met " + std::to_string(brandweerwagens_onroad[i].first->getBrandbaarheid()) + " brandbaarheid over");
			brandweerwagens_onroad[i].first->setBrandbaarheid(brandweerwagens_onroad[i].first->getBrandbaarheid() - 1);
			/*if (brandweerwagens_onroad[i].first->getBrandbaarheid() < (20 - 3))
			{
				int richting = rand() % 4 + 1;
				if (richting == 1)
				{
					std::pair<int, int> gebouw;
					gebouw = brandweerwagens_onroad[i].first->getLocatie();
					gebouw.first += 1;
					Stadsdeel* t = getStadsdeel(gebouw);
					if (t != nullptr)
					{
						if (t->getType() == Stadsdeel::House)
						{
							brandhuizen.push_back(t);
						}
					}
				}
				if (richting == 2)
				{
					std::pair<int, int> gebouw;
					gebouw = brandweerwagens_onroad[i].first->getLocatie();
					gebouw.first -= 2;
					Stadsdeel* t = getStadsdeel(gebouw);
					if (t != nullptr)
					{
						if (t->getType() == Stadsdeel::House)
						{
							brandhuizen.push_back(t);
						}
					}
				}
				if (richting == 3)
				{
					std::pair<int, int> gebouw;
					gebouw = brandweerwagens_onroad[i].first->getLocatie();
					gebouw.second -= 1;
					Stadsdeel* t = getStadsdeel(gebouw);
					if (t != nullptr)
					{
						if (t->getType() == Stadsdeel::House)
						{
							brandhuizen.push_back(t);
						}
					}
				}
				if (richting == 3)
				{
					std::pair<int, int> gebouw;
					gebouw = brandweerwagens_onroad[i].first->getLocatie();
					gebouw.second += 2;
					Stadsdeel* t = getStadsdeel(gebouw);
					if (t != nullptr)
					{
						if (t->getType() == Stadsdeel::House)
						{
							brandhuizen.push_back(t);
						}
					}
				}
			}*/
		}
	}
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "Politiewagens onderweg:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = 0; i < politiewagens_onroad.size(); i++)
	{
		std::pair<int, int> t = politiewagens_onroad[i].second->getLocatie();
		std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
		Stadsdeel* vak = getStadsdeel(locatie);
		if (!checkBestemming(politiewagens_onroad[i].first, locatie))
		{
			if (dynamic_cast<Kruispunt*>(vak) == nullptr)
			{
				output.writeToStatus(politiewagens_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.second) + "," + std::to_string(t.first) + ") in de " + vak->getNaam());
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				politiewagens_onroad[i].second->setLocatie(veranderLocatie(locatie, politiewagens_onroad[i].second->getRichting()));
			}
			else
			{
				output.writeToStatus(politiewagens_onroad[i].second->getNaam() + " op locatie (" + std::to_string(t.second) + "," + std::to_string(t.first) + ") in de " + vak->getNaam());
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				Stadsdeel::Richting r = checkRichting(politiewagens_onroad[i].first, locatie); politiewagens_onroad[i].second->setRichting(r);
				politiewagens_onroad[i].second->setLocatie(veranderLocatie(locatie, r));
			}
		}
		else
		{
			if (dynamic_cast<Politiebureau*>(politiewagens_onroad[i].first) != nullptr)
			{
				toRemovePolitie.push_back(i);
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
				output.writeToStatus( "De politiewagen is weer terug in de kazerne" );
				output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
			}
			else
			{
				for (int j = 0; j < bureaus.size(); j++)
				{
					if (bureaus[j]->getNaam() == politiewagens_onroad[i].second->getBasis())
					{
						output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
						output.writeToStatus( "De overvaller is gearresteerd en gaat nu terug naar het bureau" );
						output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
						std::pair<int, int> t = politiewagens_onroad[i].second->getLocatie();
						std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;

						politiewagens_onroad[i].first->setOvervalbaarheid(16);
						politiewagens_onroad[i].second->setRichting(checkRichting(bureaus[j], locatie)); Stadsdeel::Richting r = politiewagens_onroad[i].second->getRichting();
						politiewagens_onroad[i].first = bureaus[j];

						t = politiewagens_onroad[i].second->getLocatie();
						locatie; locatie.first = t.second; locatie.second = t.first;
						politiewagens_onroad[i].second->setLocatie(veranderLocatie(locatie, r));
					}
				}
			}
		}
	}
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	output.writeToStatus( "winkels die overvallen worden:" );
	output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
	for (int i = 0; i < politiewagens_onroad.size(); i++)
	{
		if (dynamic_cast<Politiebureau*>(politiewagens_onroad[i].first) == nullptr)
		{
			output.writeToStatus("Winkel op locatie (" + std::to_string(politiewagens_onroad[i].first->getLocatie().second) + "," + std::to_string(politiewagens_onroad[i].first->getLocatie().first) + ") wordt nog overvallen met " + std::to_string(politiewagens_onroad[i].first->getOvervalbaarheid()) + " overvalbaarheid over");
			politiewagens_onroad[i].first->setOvervalbaarheid(politiewagens_onroad[i].first->getOvervalbaarheid() - 1);
		}
	}
	std::vector<int> toRemoveBrandweerfinal;
	std::vector<int> toRemovePolitiefinal;
	if (toRemoveBrandweer.size() != 0)
	{
		for (int i = toRemoveBrandweer.size() - 1; i > -1; i--)
		{
			toRemoveBrandweerfinal.push_back(toRemoveBrandweer[i]);
		}
		//std::reverse(toRemoveBrandweer.begin(), toRemoveBrandweer.end());
		for (int i = 0; i < toRemoveBrandweerfinal.size(); i++)
		{
			brandweerwagens_onstandby.push_back(brandweerwagens_onroad[i].second);
			brandweerwagens_onroad.erase(brandweerwagens_onroad.begin() + i);
		}
	}
	
	if (toRemovePolitie.size() != 0)
	{
		for (int i = toRemovePolitie.size() - 1; i > -1; i--)
		{
			toRemovePolitiefinal.push_back(toRemovePolitie[i]);
		}
		//std::reverse(toRemovePolitie.begin(), toRemovePolitie.end());
		for (int i = 0; i < toRemovePolitiefinal.size(); i++)
		{
			politiewagens_onstandby.push_back(politiewagens_onroad[i].second);
			politiewagens_onroad.erase(politiewagens_onroad.begin() + i);
		}
	}
	
}
/*void Stad::simulatie()
{
	//std::pair<int,int> test; test.first = 4; test.second = 10;

	printStad();
	srand(time(NULL));
	Stadsdeel* doel;
	bool stopper = true; bool first = true; bool returning = false;
	while (true)
	{
		int x = rand() % (yrow-1) + 1;
		int y = rand() % (xrow-1) + 1;
		std::pair<int, int> p; p.first = y; p.second = x;
		doel = getStadsdeel(p);
		if (doel->getType() == Stadsdeel::House)
		{
			std::pair<Stadsdeel*, Brandweerwagen*> huis;
			huis.first = doel; huis.second = wagens[0];
			brandhuizen.push_back(huis);
			break;
		}
	}
	std::vector<Richting> startrichting;
	std::vector<Brandweerwagen*> onstandby;
	std::vector<std::pair<Brandweerwagen*,Stadsdeel*>> onroad;
	//Richting richting = checkRichting(doel, wagen);
	while (stopper)
	{
		if (first)
		{
			for (int i = 0; i < wagens.size(); i++)
			{
				bool standby = true;
				for (int j = 0; j < brandhuizen.size(); j++)
				{
					if (wagens[i] == brandhuizen[j].second)
					{
						standby = false;
						std::pair<Brandweerwagen*, Stadsdeel*> t; t.first = wagens[i]; t.second = brandhuizen[j].first; //t.second = getStadsdeel(test);
						onroad.push_back(t);
						output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
						output.writeToStatus( "Huis op (y,x) coordinaat (" + brandhuizen[j].first->getLocatie().second + "," + brandhuizen[j].first->getLocatie().first + ") staat in brand." );
						output.writeToStatus( "Brandweerwagen " + wagens[i]->getNaam() + " zal ernaar uitrukken" );
						output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
						output.writeToStatus( std::endl;
						std::pair<int, int> p = onroad[i].first->getLocatie();
						std::pair<int, int> locatie; locatie.first = p.second; locatie.second = p.first;
						Richting r = checkRichting(onroad[i].second, locatie);
						startrichting.push_back(r);
					}
				} 
				if (standby)
				{
					onstandby.push_back(wagens[i]);
				}
			} first = false;
		}
		for (int i = 0; i < onroad.size(); i++)
		{
			output.writeToStatus( "Statusupdate:" );
			output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
			output.writeToStatus( "Brandweerwagens onderweg:" );
			output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
			std::pair<int, int> t = onroad[i].first->getLocatie();
			std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
			Stadsdeel* vak = getStadsdeel(locatie);
			if (returning)
			{
				if (checkBestemming(onroad[i].second, locatie))
				{
					output.writeToStatus( onroad[i].first->getNaam() + " is weer terug in de kazerne (" + onroad[i].first->getBasis() + ")" );
					output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
					//std::cout + "(" + locatie.first + "," + locatie.second + ") Terug op de kazerne" );
					stopper = false;
					break;
				}
			}
			if (!checkBestemming(onroad[i].second, locatie))
			{
				if (vak->getType() != Stadsdeel::Crossroad)
				{
					output.writeToStatus( onroad[i].first->getNaam() + " op locatie (" + t.second + "," + t.first + ") in de " + vak->getNaam() );
					output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
					if (startrichting[i] == Links)
					{
						locatie.second -= 1;
						t; t.first = locatie.second; t.second = locatie.first;
						onroad[i].first->setLocatie(t);
					}
					if (startrichting[i] == Rechts)
					{
						locatie.second += 1;
						t; t.first = locatie.second; t.second = locatie.first;
						onroad[i].first->setLocatie(t);
					}
					if (startrichting[i] == Onder)
					{
						locatie.first -= 1;
						t; t.first = locatie.second; t.second = locatie.first;
						onroad[i].first->setLocatie(t);
					}
					if (startrichting[i] == Boven)
					{
						locatie.first += 1;
						t; t.first = locatie.second; t.second = locatie.first;
						onroad[i].first->setLocatie(t);
					}
					
				}
				else
				{
					output.writeToStatus( onroad[i].first->getNaam() + " op kruispunt (" + t.second + "," + t.first + ") in de " + vak->getNaam() );
					output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
					std::pair<int, int> p = onroad[i].first->getLocatie();
					std::pair<int, int> locatie; locatie.first = p.second; locatie.second = p.first;
					std::pair<int, int> t;
					startrichting[i] = checkRichting(onroad[i].second, locatie);
					if (startrichting[i] == Links)
					{
						locatie.second -= 1;
						t.first = locatie.second; t.second = locatie.first;
						onroad[i].first->setLocatie(t);
					}
					if (startrichting[i] == Rechts)
					{
						locatie.second += 1;
						t.first = locatie.second; t.second = locatie.first;
						onroad[i].first->setLocatie(t);
					}
					if (startrichting[i] == Onder)
					{
						locatie.first -= 1;
						t.first = locatie.second; t.second = locatie.first;
						onroad[i].first->setLocatie(t);
					}
					if (startrichting[i] == Boven)
					{
						locatie.first += 1;
						t.first = locatie.second; t.second = locatie.first;
						onroad[i].first->setLocatie(t);
					}
					
				}
				
				//std::cout + t.second + "|" + t.first );
			}
			else
			{
				returning = true;
				for (int j = 0; j < kazernes.size(); j++)
				{
					if (onroad[i].first->getBasis() == kazernes[j]->getNaam())
					{
						output.writeToStatus( onroad[i].first->getNaam() + " gearriveerd locatie (" + locatie.first + "," + locatie.second + ") en de brand is geblust" );
						output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
						for (int k = 0; k < brandhuizen.size(); k++)
						{
							if (brandhuizen[k].second == wagens[i])
							{
								brandhuizen.pop_back();
							}
						}
						onroad[i].second = kazernes[j];
						returning = true;
					}
				}
				startrichting[i] = checkRichting(onroad[i].second, locatie);
			}
		}
		output.writeToStatus( "Huizen in brand:" );
		output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
		for (int i = 0; i < onstandby.size(); i++)
		{
			output.writeToStatus( "Brandweerwagen " + onstandby[i]->getNaam() + " in kazerne (" + onstandby[i]->getBasis() + ")" );
		}
		output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
		output.writeToStatus( "Huizen in brand:" );
		output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
		for (int i = 0; i < brandhuizen.size(); i++)
		{
			output.writeToStatus( "Huis op locatie (" + brandhuizen[i].first->getLocatie().second + "," + brandhuizen[i].first->getLocatie().first + ") staat nog in brand met " + brandhuizen[i].first->getBrandbaarheid() + " brandbaarheid over" );
			brandhuizen[i].first->setBrandbaarheid(brandhuizen[i].first->getBrandbaarheid() - 1);
		}
		output.writeToStatus( "-=-=-=-=-=-=-=-=-=-=-=-" );
		output.writeToStatus( std::endl;
	}
} */
std::pair<int, int> Stad::veranderLocatie(std::pair<int, int> l, Stadsdeel::Richting r)
{
	std::pair<int, int> t;
	if (r == Stadsdeel::Links)
	{
		l.second -= 1;
	}
	if (r == Stadsdeel::Rechts)
	{
		l.second += 1;
	}
	if (r == Stadsdeel::Onder)
	{
		l.first -= 1;
	}
	if (r == Stadsdeel::Boven)
	{
		l.first += 1;
	}
	t.first = l.second; t.second = l.first;
	return t;
}
bool Stad::checkStad()
{   
	bool success = true;
	for (int i = 0; i < xrow; i++)
	{	
		for (int j = 0; j < yrow; j++)
		{
			std::pair<int, int> loc; loc.first = i; loc.second =j; 
			if (getStadsdeel(loc) == nullptr)
			{
				success = false;
				output.writeToConsole("Inconsistente Stad: Stad bevat nullptr(s)!" );
				return success;
			}
            else
            {
            	continue;
            }
		}
	}
	std::pair<int, int> newLoc;	
	Stadsdeel* other;
	for (int i = 0; i < getGebouwCoordsLength(); i++)
    {
        std::pair<int, int> loc;  loc.first = getGebouwCoord(i).second; loc.second =getGebouwCoord(i).first;
		Stadsdeel* current = getStadsdeel(loc);
		
		
		
		if (dynamic_cast<Huis*>(current) != nullptr)
		{	
			if (checkAangrenzing(current) == false)
			{
				success = false;
				output.writeToConsole("Inconsistente Stad: er grenst geen straat aan het huis op locatie" + std::to_string(current->getLocatie().first) + "|" + std::to_string(current->getLocatie().second));
				return success;
			}			
			for (int i = loc.first; i >= (loc.first-1); i--)
			{	
				for (int j = loc.second; j <= (loc.second+1); j++)
				{
					newLoc.first = i; newLoc.second = j;
					other = getStadsdeel(newLoc);
					
					if (other != current)
					{
						success = false;
						output.writeToConsole("Inconsistente Stad: Huis op locatie " + std::to_string(current->getLocatie().first) + "|" + std::to_string(current->getLocatie().second));
						return success;
					}
					
				}
			}
		}
		else if (dynamic_cast<Huis*>(current) != nullptr)
		{
			if (checkAangrenzing(current) == false)
			{
				success = false;
				output.writeToConsole("Inconsistente Stad: er grenst geen straat aan de kazerne op locatie" + std::to_string(current->getLocatie().first) + "|" + std::to_string(current->getLocatie().second));
				return success;
			}
			for (int i = loc.first; i >= (loc.first - 3); i--)
			{
				for (int j = loc.second; j <= (loc.second + 3); j++)
				{
					newLoc.first = i; newLoc.second = j;
					other = getStadsdeel(newLoc);
					
					if (other != current)
					{
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
    for (int i = 0; i < getStraatCoordsLength(); i++)
    {
       	begin.first = getStraatCoord(i).first.second;
		begin.second = getStraatCoord(i).first.first;
		
		end.first = getStraatCoord(i).second.second;
		end.second = getStraatCoord(i).second.first;
		
		Stadsdeel* currentBegin = getStadsdeel(begin);
		Stadsdeel* currentEnd = getStadsdeel(end);		
		
		if ((begin.second == 0) && (end.second == (yrow - 1)))
		{//HORIZONTAAL
			std::string stNaam;			
			if (currentBegin->getEersteStraat() == currentEnd->getEersteStraat())
			{			
				stNaam = currentBegin->getEersteStraat();
			}
			else
			{
				success = false;
				output.writeToConsole("Inconsistente Stad: Straat \t" + stNaam );
				return success;
			}
			for (int i = 0; i <= yrow - 1; i++)
			{
				newLoc.second = i;
				newLoc.first = begin.first;
				other = getStadsdeel(newLoc);

				if (other->getEersteStraat() == stNaam || other->getNaam() == stNaam)
				{
					continue;
				}
				else
				{
					success = false;
					output.writeToConsole("Inconsistente Stad: Straat \t" + currentBegin->getEersteStraat() );
					return success;
				}
			}	
		}
		else if ((begin.first == 0) && (end.first == (xrow - 1)))
		{//VERTICAAL
			std::string stNaam;
			if (currentBegin->getTweedeStraat() == currentEnd->getTweedeStraat())
			{
				stNaam = currentBegin->getTweedeStraat();
			}
			else
			{
				success = false;
				output.writeToConsole("Inconsistente Stad: Straat \t" + currentBegin->getTweedeStraat() );
				return success;
			}
			for (int i = 0; i <= xrow - 1; i++)
			{
				newLoc.first = i;
				newLoc.second = begin.second;
				other = getStadsdeel(newLoc);
				
				if (other->getTweedeStraat() == stNaam || other->getNaam() == stNaam)
				{
					continue;
				}
				else
				{
					success = false;
					output.writeToConsole("Inconsistente Stad: Straat \t" + stNaam );
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
std::pair<std::pair<int, int>, std::pair<int, int>> Stad::getStraatCoord(int i)
{
	return straatCoords[i];
}
void Stad::addGebouwCoord(std::pair<int, int> g)
{
	gebouwCoords.push_back(g);
}
void Stad::addStraatCoord(std::pair<std::pair<int, int>, std::pair<int, int>> s)
{
	straatCoords.push_back(s);
}
int Stad::getGebouwCoordsLength()
{
	return gebouwCoords.size();
}
int Stad::getStraatCoordsLength()
{
	return straatCoords.size();
}


/*
	NOG TE OVERLOADEN METHODES VOOR STADSDEEL
*/
Stadsdeel::Stadsdeel(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Stadsdeel::properlyInitialized()
{
	return _initCheck == this;
}
std::pair<int, int> Stadsdeel::getLocatie()
{
	std::pair<int, int> t; t.first = 0; t.second = 0;
	return t;
}
std::pair<int, int> Stadsdeel::getBegin()
{
	std::pair<int, int> t; t.first = 0; t.second = 0;
	return t;
}
std::pair<int, int> Stadsdeel::getEinde()
{
	std::pair<int, int> t; t.first = 0; t.second = 0;
	return t;
}
std::pair<int, int> Stadsdeel::getGrootte()
{
	std::pair<int, int> t; t.first = 0; t.second = 0;
	return t;
}
char Stadsdeel::getKarakter()
{
	return '0';
}
Stadsdeel::Type Stadsdeel::getType()
{
	return Station;
}
std::string Stadsdeel::getNaam()
{
	return "";
}
void Stadsdeel::setBrandbaarheid(int i)
{
	brandbaarheid = 0;
}
void Stadsdeel::setOvervalbaarheid(int i)
{
	overvalbaarheid = 0;
}
int Stadsdeel::getBrandbaarheid()
{
	return brandbaarheid;
}
int Stadsdeel::getOvervalbaarheid()
{
	return overvalbaarheid;
}
std::string Stadsdeel::getEersteStraat()
{
	return "";
}

std::string Stadsdeel::getTweedeStraat()
{
	return "";
}




/*
	METHODES VOOR DE BRANDWEERKAZERNE
*/
Brandweerkazerne::Brandweerkazerne(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Brandweerkazerne::properlyInitialized()
{
	return _initCheck == this;
}
void Brandweerkazerne::setNaam(std::string s)
{
	naam = s;
}
void Brandweerkazerne::setLocatie(std::pair<int, int> p)
{
	locatie = p;
}
void Brandweerkazerne::setIngang(std::pair<int, int> p)
{
	ingang = p;
}
std::pair<int, int> Brandweerkazerne::getLocatie()
{
	return locatie;
}
std::pair<int, int> Brandweerkazerne::getIngang()
{
	return ingang;
}
std::string Brandweerkazerne::getNaam()
{
	return naam;
}
char Brandweerkazerne::getKarakter()
{
	return karakter;
}
Stadsdeel::Type Brandweerkazerne::getType()
{
	return type;
}

/*
METHODES VOOR DE POLITIE STATION
*/
Politiebureau::Politiebureau(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Politiebureau::properlyInitialized()
{
	return _initCheck == this;
}
void Politiebureau::setNaam(std::string s)
{
	naam = s;
}
void Politiebureau::setLocatie(std::pair<int, int> p)
{
	locatie = p;
}
void Politiebureau::setIngang(std::pair<int, int> p)
{
	ingang = p;
}
void Politiebureau::setGrootte(std::pair<int, int> p)
{
	grootte = p;
}
std::pair<int, int> Politiebureau::getLocatie()
{
	return locatie;
}
std::pair<int, int> Politiebureau::getIngang()
{
	return ingang;
}
std::pair<int, int> Politiebureau::getGrootte()
{
	return grootte;
}
std::string Politiebureau::getNaam()
{
	return naam;
}
char Politiebureau::getKarakter()
{
	return karakter;
}
Stadsdeel::Type Politiebureau::getType()
{
	return type;
}

/*
METHODES VOOR DE ZIEKENHUIS
*/
Ziekenhuis::Ziekenhuis(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Ziekenhuis::properlyInitialized()
{
	return _initCheck == this;
}
void Ziekenhuis::setNaam(std::string s)
{
	naam = s;
}
void Ziekenhuis::setLocatie(std::pair<int, int> p)
{
	locatie = p;
}
void Ziekenhuis::setIngang(std::pair<int, int> p)
{
	ingang = p;
}
void Ziekenhuis::setGrootte(std::pair<int, int> p)
{
	grootte = p;
}
std::pair<int, int> Ziekenhuis::getLocatie()
{
	return locatie;
}
std::pair<int, int> Ziekenhuis::getIngang()
{
	return ingang;
}
std::pair<int, int> Ziekenhuis::getGrootte()
{
	return grootte;
}
std::string Ziekenhuis::getNaam()
{
	return naam;
}
char Ziekenhuis::getKarakter()
{
	return karakter;
}
Stadsdeel::Type Ziekenhuis::getType()
{
	return type;
}


/*
	METHODES VOOR DE BRANDWEERWAGEN
*/
Brandweerwagen::Brandweerwagen(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Brandweerwagen::properlyInitialized()
{
	return _initCheck == this;
}
void Brandweerwagen::setNaam(std::string n)
{
	naam = n;
}
void Brandweerwagen::setBasis(std::string b)
{
	basis = b;
}
void Brandweerwagen::setLocatie(std::pair<int, int> p)
{
	locatie = p;
}
void Brandweerwagen::setRichting(Stadsdeel::Richting r)
{
	richting = r;
}
std::pair<int, int> Brandweerwagen::getLocatie()
{
	return locatie;
}
Stadsdeel::Richting Brandweerwagen::getRichting()
{
	return richting;
}
std::string Brandweerwagen::getBasis()
{
	return basis;
}
std::string Brandweerwagen::getNaam()
{
	return naam;
}
char Brandweerwagen::getKarakter()
{
	return karakter;
}
Stadsdeel::Type Brandweerwagen::getType()
{
	return type;
}

/*
METHODES VOOR DE BRANDWEERWAGEN
*/
Politiewagen::Politiewagen(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Politiewagen::properlyInitialized()
{
	return _initCheck == this;
}
void Politiewagen::setNaam(std::string n)
{
	naam = n;
}
void Politiewagen::setBasis(std::string b)
{
	basis = b;
}
void Politiewagen::setRichting(Stadsdeel::Richting r)
{
	richting = r;
}
void Politiewagen::setLocatie(std::pair<int, int> p)
{
	locatie = p;
}
Stadsdeel::Richting Politiewagen::getRichting()
{
	return richting;
}
std::pair<int, int> Politiewagen::getLocatie()
{
	return locatie;
}
std::string Politiewagen::getBasis()
{
	return basis;
}
std::string Politiewagen::getNaam()
{
	return naam;
}
char Politiewagen::getKarakter()
{
	return karakter;
}
Stadsdeel::Type Politiewagen::getType()
{
	return type;
}

/*
METHODES VOOR DE BRANDWEERWAGEN
*/
Ambulance::Ambulance(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Ambulance::properlyInitialized()
{
	return _initCheck == this;
}
void Ambulance::setNaam(std::string n)
{
	naam = n;
}
void Ambulance::setBasis(std::string b)
{
	basis = b;
}
void Ambulance::setLocatie(std::pair<int, int> p)
{
	locatie = p;
}
std::pair<int, int> Ambulance::getLocatie()
{
	return locatie;
}
std::string Ambulance::getBasis()
{
	return basis;
}
std::string Ambulance::getNaam()
{
	return naam;
}
char Ambulance::getKarakter()
{
	return karakter;
}
Stadsdeel::Type Ambulance::getType()
{
	return type;
}



/*
	METHODES VOOR DE STRAAT
*/
Straat::Straat(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Straat::properlyInitialized()
{
	return _initCheck == this;
}
void Straat::setNaam(std::string n)
{
	naam = n;
}
void Straat::setBegin(std::pair<int, int> b)
{
	begin = b;
}
void Straat::setEinde(std::pair<int, int> e)
{
	einde = e;
}
std::string Straat::getNaam()
{
	return naam;
}
std::pair<int, int> Straat::getBegin()
{
	return begin;
}
std::pair<int, int> Straat::getEinde()
{
	return einde;
}
char Straat::getKarakter()
{
	return karakter;
}
Stadsdeel::Type Straat::getType()
{
	return type;
}
/*






	METHODES VOOR EEN HUIS
*/
Huis::Huis(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Huis::properlyInitialized()
{
	return _initCheck == this;
}
void Huis::setLocatie(std::pair<int, int> l)
{
	locatie = l;
}
void Huis::setBrandbaarheid(int b)
{
	brandbaarheid = b;
}
int Huis::getBrandbaarheid()
{
	return brandbaarheid;
}
std::pair<int, int> Huis::getLocatie()
{
	return locatie;
}
char Huis::getKarakter()
{
	return karakter;
}
Stadsdeel::Type Huis::getType()
{
	return type;
}

/*






METHODES VOOR EEN WINKEL
*/
Winkel::Winkel(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Winkel::properlyInitialized()
{
	return _initCheck == this;
}
void Winkel::setLocatie(std::pair<int, int> l)
{
	locatie = l;
}
void Winkel::setGrootte(std::pair<int, int> p)
{
	grootte = p;
}
void Winkel::setBrandbaarheid(int b)
{
	brandbaarheid = b;
}
void Winkel::setOvervalbaarheid(int b)
{
	overvalbaarheid = b;
}
int Winkel::getBrandbaarheid()
{
	return brandbaarheid;
}
int Winkel::getOvervalbaarheid()
{
	return overvalbaarheid;
}
std::pair<int, int> Winkel::getLocatie()
{
	return locatie;
}
std::pair<int, int> Winkel::getGrootte()
{
	return grootte;
}
char Winkel::getKarakter()
{
	return karakter;
}
Stadsdeel::Type Winkel::getType()
{
	return type;
}



/*
	METHODES VOOR EEN KRUISPUNT
*/
Kruispunt::Kruispunt(void)
{
	_initCheck = this;
	//ENSURE(properlyInitialized(), "Constructor must end in properlyInitialized state");
}
bool Kruispunt::properlyInitialized()
{
	return _initCheck == this;
}
void Kruispunt::setEersteStraat(std::string s)
{
	eersteStraat = s;
}
void Kruispunt::setTweedeStraat(std::string s)
{
	tweedeStraat = s;
}
void Kruispunt::setLocatie(std::pair<int, int> l)
{
	locatie = l;
}
std::string Kruispunt::getEersteStraat()
{
	return eersteStraat;
}
std::string Kruispunt::getTweedeStraat()
{
	return tweedeStraat;
}
char Kruispunt::getKarakter()
{
	return karakter;
}
std::string Kruispunt::getNaam()
{
	return eersteStraat;
}
Stadsdeel::Type Kruispunt::getType()
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
	std::cout << "(2) Huis in brand steken" << std::endl;
	std::cout << "(3) Invoerbestand met voertuigen inlezen" << std::endl;
	std::cout << "(4) Brandweerwagen laten uitrukken" << std::endl;
	std::cout << "(5) Aantal stappen in simulatie aanpassen" << std::endl;
	std::cout << "(6) Simpele uitvoer laten uitvoeren" << std::endl;
	std::cout << "(7) Simpele grafische impressie tonen" << std::endl;
	std::cout << "(8) Simulatie starten" << std::endl;
	std::cout << "(0) Sluit dit programma" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Uw keuze: ";
	std::cin >> choice;
	switch (choice)
	{
		case 1:
		{
			leesBestandStad();
			break;
		}
		case 2:
		{
			startBrand();
			break;
		}
		case 3:
		{
			leesBestandVoertuigen();
			break;
		}
		case 5:
		{
			setSteps();
			break;
		}
		case 7:
		{
			showMap();
			break;
		}
		case 8:
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
	std::cout << "Geef de x coordinaat van het huis op: ";
	std::cin >> x;
	std::cout << "Geef de y coordinaat van het huis op: ";
	std::cin >> y;
	coord.first = y; coord.second = x; 
	if (dynamic_cast<Huis*>(stad->getStadsdeel(coord)) != nullptr)
	{
		stad->brandhuizen.push_back(stad->getStadsdeel(coord));
	}
	std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl << std::endl;
	showMenu();
}
bool UI::startBrandweerwagen()
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
	for (int i = 0; i < steps; i++)
	{
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


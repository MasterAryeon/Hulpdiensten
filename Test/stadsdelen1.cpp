#include "tinyxml.h"
#include "stadsdelen.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>
#include <stdlib.h>
#include <time.h>

#include "DesignByContract.h"

#define TIXML_USE_STL




/*
	METHODES VOOR DE STAD
*/
Stad::Stad(void)
{
	//memset(grid,,sizeof(grid));
	for (int i = 0; i < xrow; i++)
	{
		for (int j = 0; j < yrow; j++)
		{
			grid[i][j] = nullptr;
		} 
	}
	status = new std::ofstream ("status.txt", std::ofstream::out);
	console = new std::ofstream ("console.txt", std::ofstream::out);
	*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	*status << "Project Software Engineering - door Aron Cornet en Tim Goossens" << std::endl;
	*status << "Specificatie 1.0" << std::endl;
	*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;

	*console << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	*console << "Project Software Engineering - door Aron Cornet en Tim Goossens" << std::endl;
	*console << "Specificatie 1.0" << std::endl;
	*console << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	//ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
}
bool Stad::properlyInitialized()
{
	return _initCheck == this;
}
void Stad::setStadsdeel(Stadsdeel &s)
{
	switch (s.getType())
	{
		case Stadsdeel::Station:
		{
			std::pair<int, int> l = s.getLocatie();
			for (int i = l.second; i > (l.second - 4); i--)
			{
				for (int j = l.first; j < (l.first + 4); j++)
				{
					grid[i][j] = &s;
				}
			}
		break;
		}
		case Stadsdeel::Street:
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
						if (grid[i][b.first]->getType() == 3)
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
			break;
		}
		case Stadsdeel::House:
		{
			std::pair<int, int> l = s.getLocatie();
			for (int i = l.second; i > (l.second - 2); i--)
			{
				for (int j = l.first; j < (l.first + 2); j++)
				{
					grid[i][j] = &s;
				}
			}
			break;
		}
	}
}
Stadsdeel* Stad::getStadsdeel(std::pair<int, int> p)
{
	if ((p.first >= 0 && p.first <= xrow) && (p.second >= 0 && p.second <= yrow))
	{
		return grid[p.first][p.second];
	} else 
	{
		return nullptr;
	}
	
}

bool Stad::checkAangrenzing(Stadsdeel* s)
{
	int aantal = 0;
	if (s->getType() == Stadsdeel::Station)
	{
		aantal = 4;
	}
	else if (s->getType() == Stadsdeel::House)
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
			if (t->getType() == Stadsdeel::Street)
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
	*status << "Legenda:" << std::endl;
	*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	*status << "H = Onderdeel van een huis" << std::endl;
	*status << "X = Onderdeel van de kazerne" << std::endl;
	*status << "_ = Onderdeel van een straat" << std::endl;
	*status << "O = Een kruispunt" << std::endl;
	*status << "B = Startlocatie van een brandweerwagen" << std::endl;
	*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	for (int i = (xrow-1); i >= 0; i--)
	{
		std::string lijn = "";
		for (int j = 0; j < yrow; j++)
		{
			bool found = false;
			std::pair<int, int> remember;
			for (int k = 0; k < wagens.size(); k++)
			{
				
				if (i == wagens[k]->getLocatie().second && j == wagens[k]->getLocatie().first && remember != wagens[k]->getLocatie())
				{
					lijn = lijn + "B" + " ";
					remember = wagens[k]->getLocatie();
					found = true;
				}
			}
			//if (i == 7 && j == 8)
			//{
			//	lijn = lijn + "!" + " ";
			//	found = true;
			//}
			if (!found)
			{
				vak = grid[i][j];
				lijn = lijn + vak->getKarakter() + " ";
			}
		}
		//std::cout << lijn << std::endl;
		
		*status << lijn << std::endl;
	}
	//*status << "~~~~~~~~~~~~~~~~~~~" << std::endl;
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
		//std::cout << "RECHTS" << std::endl;
		return true;
	}
	// LINKS
	coordinaat.first = p.first; coordinaat.second = p.second - 1;
	temp = getStadsdeel(coordinaat);
	if (temp == s)
	{
		//std::cout << "LINKS" << std::endl;
		return true;
	}
	// ONDER
	coordinaat.first = p.first - 1; coordinaat.second = p.second;
	temp = getStadsdeel(coordinaat);
	if (temp == s)
	{
		//std::cout << "ONDER" << std::endl;
		return true;
	}
	// BOVEN
	coordinaat.first = p.first + 1; coordinaat.second = p.second;
	temp = getStadsdeel(coordinaat);
	if (temp == s)
	{
		//std::cout << "BOVEN" << std::endl;
		return true;
	}
	return false;
}
Stad::StraatType Stad::checkStraat(Stadsdeel* s, std::pair<int, int> p)
{
	Stadsdeel* vak = getStadsdeel(p);
	std::pair<int, int> locatie = s->getLocatie();
	if (vak->getType() == Stadsdeel::Street)
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
	if (vak->getType() == Stadsdeel::Crossroad)
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
Stad::Richting Stad::checkRichting(Stadsdeel* s, std::pair<int, int> p)
{
	Stadsdeel* vak = getStadsdeel(p);
	std::pair<int, int> locatie = s->getLocatie();
	if (vak->getType() == Stadsdeel::Street)
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
				if (tijdelijk->getType() == Stadsdeel::Crossroad)
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
				if (tijdelijk->getType() == Stadsdeel::Crossroad)
				{
					bovenlid = p2;
					bovenafstand = abs(bovenlid.first - s->getLocatie().second) + abs(bovenlid.second - s->getLocatie().first);
					break;
				}
			}
			if (onderafstand == bovenafstand)
			{
				return Richting::Onder;
			}
			else
			{
				if (onderafstand < bovenafstand)
				{
					return Richting::Onder;
				}
				else
				{
					return Richting::Boven;
				}
			}
			
		}
		if ((s1.second - s2.second) == 0)
		{
			for (int i = p.second; i >= 0; i--)
			{
				std::pair<int, int> p2; p2.first = p.first; p2.second = i;
				Stadsdeel* tijdelijk = getStadsdeel(p2);
				if (tijdelijk->getType() == Stadsdeel::Crossroad)
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
				if (tijdelijk->getType() == Stadsdeel::Crossroad)
				{
					rechterlid = p2;
					rechterafstand = abs(rechterlid.first - s->getLocatie().second) + abs(rechterlid.second - s->getLocatie().first);
					break;
				}
			}
			if (linkerafstand == rechterafstand)
			{
				return Richting::Links;
			}
			else
			{
				if (linkerafstand < rechterafstand)
				{
					return Richting::Links;
				}
				else
				{
					return Richting::Rechts;
				}
			}
			//std::cout << linkerafstand << "|" << rechterafstand << std::endl;
		} 
	}
	if (vak->getType() == Stadsdeel::Crossroad)
	{
		std::pair<int, int> bovenlid; int bovenafstand = -1;
		std::pair<int, int> onderlid; int onderafstand = -1;
		std::pair<int, int> linkerlid; int linkerafstand = -1;
		std::pair<int, int> rechterlid; int rechterafstand = -1;
		Richting finale; int finaleafstand = 100;
		for (int i = p.first - 1; i >= 0; i--)
		{
			std::pair<int, int> p2; p2.first = i; p2.second = p.second;
			Stadsdeel* tijdelijk = getStadsdeel(p2);
			if (tijdelijk != nullptr)
			{
				if (tijdelijk->getType() == Stadsdeel::Crossroad)
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
				if (tijdelijk->getType() == Stadsdeel::Crossroad)
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
				if (tijdelijk->getType() == Stadsdeel::Crossroad)
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
				if (tijdelijk->getType() == Stadsdeel::Crossroad)
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
				finale = Richting::Onder;
			}
		}
		if (bovenafstand != -1)
		{
			if (bovenafstand < finaleafstand)
			{
				finaleafstand = bovenafstand;
				finale = Richting::Boven;
			}
		}
		if (linkerafstand != -1)
		{
			if (linkerafstand < finaleafstand)
			{
				finaleafstand = linkerafstand;
				finale = Richting::Links;
			}
		}
		if (rechterafstand != -1)
		{
			if (rechterafstand < finaleafstand)
			{
				finaleafstand = rechterafstand;
				finale = Richting::Rechts;
			}
		}
		StraatType verificatie = checkStraat(s, p);
		if (verificatie == Horizontaal && (finale == Boven || finale == Onder))
		{
			//std::cout << "Correctie" << std::endl;
			if ((s->getLocatie().first - p.second) < 0)
			{
				return Richting::Links;
			}
			else
			{
				return Richting::Rechts;
			}
		}
		else if (verificatie == Verticaal && (finale == Links || finale == Rechts))
		{
			//std::cout << "Correctie" << std::endl;
			if ((s->getLocatie().second - p.first) < 0)
			{
				return Richting::Onder;
			}
			else
			{
				return Richting::Boven;
			}
		}
		else
		{
			return finale;
		}
	}
	return Richting::Rechts;
}

bool Stad::stadInlezen(char* s)
{	//Omvormen naar pointer
	TiXmlDocument doc;
	TiXmlElement* root;
	if (!doc.LoadFile(s))
	{
		std::cerr << doc.ErrorDesc() << std::endl;
		return false;
	}
	else
	{
		//std::cout << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
		//std::cout << "Loading file succesful" << std::endl;
		//std::cout << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;

		*console << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
		*console << "Loading file succesful" << std::endl;
		*console << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
		root = doc.FirstChildElement();
		if (root == NULL)
		{
			//std::cout << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
			//std::cerr << "Failed to load file: No root element" << std::endl;
			//std::cout << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;

			*console << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
			*console << "Failed to load file : No root element" << std::endl;
			*console << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
			return false;
		}
		else
		{
			for (TiXmlNode* n = root->FirstChild(); n != NULL; n = n->NextSibling())
			{
				std::string nodeName = n->Value();
				std::string naam; std::string basis;
				std::pair<int, int> van; std::pair<int, int> naar;
				int brandbaarheid;
				std::string::size_type sz;
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
					for (TiXmlAttribute* a = e->FirstAttribute(); a != NULL; a = a->Next())
					{
						if (a != NULL)
						{
							if (!elementName.compare("Locatie"))
							{
								if (counter == 0)
								{
									van.first = atoi(a->Value());
									counter++;
								}
								else
								{
									van.second = atoi(a->Value());
								}
							}
							if (!elementName.compare("Ingang"))
							{
								if (counter == 0)
								{
									naar.first = atoi(a->Value());
									counter++;
								}
								else
								{

									naar.second = atoi(a->Value());
								}
							}
							/*

							*/
							if (!elementName.compare("Van"))
							{
								if (counter == 0)
								{
									van.first = atoi(a->Value());
									counter++;
								}
								else
								{
									van.second = atoi(a->Value());
								}
							}
							if (!elementName.compare("Naar"))
							{
								if (counter == 0)
								{
									naar.first = atoi(a->Value());
									counter++;
								}
								else
								{
									naar.second = atoi(a->Value());
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
					basissen.push_back(bk);

					addGebouwCoord(van);//vector
				}
				if (!nodeName.compare("Brandweerwagen"))
				{
					Brandweerwagen *bw = new Brandweerwagen();
					bw->setNaam(naam);
					bw->setBasis(basis);
					std::pair<int, int> locatie; locatie.first = 0; locatie.second = 0;
					for (int i = 0; i < basissen.size(); i++)
					{
						if (basissen[i]->getNaam() == basis)
						{
							locatie = basissen[i]->getIngang();
						}
					}
					if (locatie.first != 0 && locatie.second != 0)
					{
						bw->setLocatie(locatie);
					}
					wagens.push_back(bw);
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
			}
			
			//std::cout << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
			return true;
		}
	}
}

void Stad::simulatie()
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
						*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
						*status << "Huis op (y,x) coordinaat (" << brandhuizen[j].first->getLocatie().second << "," << brandhuizen[j].first->getLocatie().first << ") staat in brand." << std::endl;
						*status << "Brandweerwagen " << wagens[i]->getNaam() << " zal ernaar uitrukken" << std::endl;
						*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
						*status << std::endl;
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
			*status << "Statusupdate:" << std::endl;
			*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
			*status << "Brandweerwagens onderweg:" << std::endl;
			*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
			std::pair<int, int> t = onroad[i].first->getLocatie();
			std::pair<int, int> locatie; locatie.first = t.second; locatie.second = t.first;
			Stadsdeel* vak = getStadsdeel(locatie);
			if (returning)
			{
				if (checkBestemming(onroad[i].second, locatie))
				{
					*status << onroad[i].first->getNaam() << " is weer terug in de kazerne (" << onroad[i].first->getBasis() << ")" << std::endl;
					*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
					//std::cout << "(" << locatie.first << "," << locatie.second << ") Terug op de kazerne" << std::endl;
					stopper = false;
					break;
				}
			}
			if (!checkBestemming(onroad[i].second, locatie))
			{
				if (vak->getType() != Stadsdeel::Crossroad)
				{
					*status << onroad[i].first->getNaam() << " op locatie (" << t.second << "," << t.first << ") in de " << vak->getNaam() << std::endl;
					*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
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
					*status << onroad[i].first->getNaam() << " op kruispunt (" << t.second << "," << t.first << ") in de " << vak->getNaam() << std::endl;
					*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
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
				
				//std::cout << t.second << "|" << t.first << std::endl;
			}
			else
			{
				returning = true;
				for (int j = 0; j < basissen.size(); j++)
				{
					if (onroad[i].first->getBasis() == basissen[j]->getNaam())
					{
						*status << onroad[i].first->getNaam() << " gearriveerd locatie (" << locatie.first << "," << locatie.second << ") en de brand is geblust" << std::endl;
						*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
						for (int k = 0; k < brandhuizen.size(); k++)
						{
							if (brandhuizen[k].second == wagens[i])
							{
								brandhuizen.pop_back();
							}
						}
						onroad[i].second = basissen[j];
						returning = true;
					}
				}
				startrichting[i] = checkRichting(onroad[i].second, locatie);
			}
		}
		*status << "Huizen in brand:" << std::endl;
		*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
		for (int i = 0; i < onstandby.size(); i++)
		{
			*status << "Brandweerwagen " << onstandby[i]->getNaam() << " in kazerne (" << onstandby[i]->getBasis() << ")" << std::endl;
		}
		*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
		*status << "Huizen in brand:" << std::endl;
		*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
		for (int i = 0; i < brandhuizen.size(); i++)
		{
			*status << "Huis op locatie (" << brandhuizen[i].first->getLocatie().second << "," << brandhuizen[i].first->getLocatie().first << ") staat nog in brand met " << brandhuizen[i].first->getBrandbaarheid() << " brandbaarheid over" << std::endl;
			brandhuizen[i].first->setBrandbaarheid(brandhuizen[i].first->getBrandbaarheid() - 1);
		}
		*status << "-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
		*status << std::endl;
	}
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
		
		
		
		if (current->getType() == Stadsdeel::House)
		{				
			for (int i = loc.first; i >= (loc.first-1); i--)
			{	
				for (int j = loc.second; j <= (loc.second+1); j++)
				{
					newLoc.first = i; newLoc.second = j;
					other = getStadsdeel(newLoc);
					if (other != current)
					{
						success = false;
						return success;
					}
					
				}
			}
		}
		else if (current->getType() == Stadsdeel::Station)
		{
			
			for (int i = loc.first; i >= (loc.first - 3); i--)
			{
				for (int j = loc.second; j <= (loc.second + 3); j++)
				{
					newLoc.first = i; newLoc.second = j;
					other = getStadsdeel(newLoc);
					
					if (other != current)
					{
						success = false;
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
int Stadsdeel::getBrandbaarheid()
{
	return brandbaarheid;
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
	METHODES VOOR DE BRANDWEERWAGEN
*/
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
std::pair<int, int> Brandweerwagen::getLocatie()
{
	return locatie;
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
	METHODES VOOR DE STRAAT
*/
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
	METHODES VOOR EEN KRUISPUNT
*/
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

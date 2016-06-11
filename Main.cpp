#include "tinyxml.h"
#include "stadsdelen.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>
#define TIXML_USE_STL

int main()
{
	Stad stad;
	UI ui;
	ui.setStad(&stad);
	ui.showMenu();

	//stad.stadInlezen("voertuigen2.0_bis.xml");
	//stad.printStad();
	/*if (stad.checkStad())
	{
		std::pair<int, int> p; p.first = 7; p.second = 3;
		std::pair<Stadsdeel*, Brandweerwagen*> huis;
		Stadsdeel* t = stad.getStadsdeel(p);
		stad.brandhuizen.push_back(t);
		p.first = 2; p.second = 17;
		t = stad.getStadsdeel(p);
		stad.brandhuizen.push_back(t);
		p.first = 9; p.second = 14;
		t = stad.getStadsdeel(p);
		stad.overvalhuizen.push_back(t);
		for (int i = 0; i < stad.brandweerwagens.size(); i++)
		{
			stad.brandweerwagens_onstandby.push_back(stad.brandweerwagens[i]);
		}
		for (int i = 0; i < stad.politiewagens.size(); i++)
		{
			stad.politiewagens_onstandby.push_back(stad.politiewagens[i]);
		}
		for (int i = 0; i < 40; i++)
		{
			stad.simulatie2();
		}
		
	}*/
	return 0;
}

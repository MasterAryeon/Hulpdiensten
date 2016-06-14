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
	return 0;
}

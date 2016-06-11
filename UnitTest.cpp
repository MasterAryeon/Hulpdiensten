#include "gtest/gtest.h"
#include "stadsdelen.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/stat.h>

bool FileCompare(const std::string leftFileName, const std::string rightFileName) {
	std::ifstream leftFile, rightFile;
	char leftRead, rightRead;
	bool result;

	// Open the two files.
	leftFile.open(leftFileName);
	if (!leftFile.is_open()) {
		return false;
	};
	rightFile.open(rightFileName);
	if (!rightFile.is_open()) {
		leftFile.close();
		return false;
	};

	result = true; // files exist and are open; assume equality unless a counterexamples shows up.
	while (result && leftFile.good() && rightFile.good()) {
		leftFile.get(leftRead);
		rightFile.get(rightRead);
		result = (leftRead == rightRead);
	};
	if (result) {
		// last read was still equal; are we at the end of both files ?
		result = (!leftFile.good()) && (!rightFile.good());
	};

	leftFile.close();
	rightFile.close();
	return result;
}

TEST(Stad_Inlezen, Stadsdelen)
{
	//Inlezen van de de foutieve stad
	Stad* stad = new Stad();
	EXPECT_EQ(false, stad->stadInlezen("stad2.0_bis_fout.xml"));
	EXPECT_EQ(false, stad->checkStad());

	//Inlezen van de de juiste stad
	EXPECT_EQ(true, stad->stadInlezen("stad2.0_bis.xml"));
	EXPECT_EQ(true, stad->checkStad());

	//Opvragen van een stadsdeel buiten de stad
	std::pair<int, int> p; p.first = -1; p.second = -1;
	std::pair<int, int> q; q.first = -1; q.second = -1;
	Stadsdeel* test = stad->getStadsdeel(p);

	EXPECT_EQ(nullptr, test);

}
TEST(Stad_Methodes, Stadsdelen)
{
	Stad* stad = new Stad();
	stad->stadInlezen("stad2.0_bis.xml");
	std::pair<int, int> p; p.first = -1; p.second = -1;
	std::pair<int, int> q; q.first = -1; q.second = -1;
	Stadsdeel* test = stad->getStadsdeel(p);
	p; p.first = 12; p.second = 5;
	test = stad->getStadsdeel(p);

	//Checken of het huis aan een straat zit
	EXPECT_TRUE(stad->checkAangrenzing(test));

	//Checken van een stadsdeel buiten de stad
	p; p.first = -1; p.second = -1;
	test = stad->getStadsdeel(p);
	EXPECT_DEATH(stad->checkAangrenzing(test), "");

	//Checken of de wagen bij de bestemming is
	p; p.first = 10; p.second = 5;
	q; q.first = 10; q.second = 4;
	test = stad->getStadsdeel(p);
	EXPECT_TRUE(stad->checkBestemming(test, q));

	//Checken of het huis aan een straat zit (niet waar)
	q; q.first = 8; q.second = 0;
	EXPECT_FALSE(stad->checkBestemming(test, q));

	//Checken of men in de straat van de bestemming zit (Horizontaal)
	//q; q.first = 10; q.second = 1;
	//EXPECT_EQ(Stad::StraatType::Horizontaal, stad->checkStraat(test, q));

	//Checken of men in de straat van de bestemming zit (Verticaal)
	//q; q.first = 13; q.second = 7;
	//EXPECT_EQ(Stad::StraatType::Verticaal, stad->checkStraat(test, q));

	//Checken of men in de straat van de bestemming zit (Geen)
	//q; q.first = 5; q.second = 4;
	//EXPECT_EQ(Stad::StraatType::Geen, stad->checkStraat(test, q));
}

TEST(Stad_Richting, Stadsdelen)
{
	Stad* stad = new Stad();
	stad->stadInlezen("stad2.0_bis.xml");
	std::pair<int, int> p; p.first = -1; p.second = -1;
	std::pair<int, int> q; q.first = -1; q.second = -1;
	Stadsdeel* test = stad->getStadsdeel(p);
	

	// Check de richting die de wagen vanaf zijn positie moet nemen (Onder)
	p; p.first = 4; p.second = 10;
	q; q.first = 10; q.second = 7;
	test = stad->getStadsdeel(p);
	EXPECT_EQ(Stadsdeel::Onder, stad->checkRichting(test, q));

	// Check de richting die de wagen vanaf zijn positie moet nemen (Boven)
	p; p.first = 14; p.second = 10;
	test = stad->getStadsdeel(p);
	EXPECT_EQ(Stadsdeel::Boven, stad->checkRichting(test, q));

	// Check de richting die de wagen vanaf zijn positie moet nemen (Links)
	p; p.first = 12; p.second = 1;
	test = stad->getStadsdeel(p);
	EXPECT_EQ(Stadsdeel::Links, stad->checkRichting(test, q));

	// Check de richting die de wagen vanaf zijn positie moet nemen (Rechts)
	p; p.first = 9; p.second = 17;
	test = stad->getStadsdeel(p);
	EXPECT_EQ(Stadsdeel::Rechts, stad->checkRichting(test, q));

	//// Check de richting die de wagen vanaf zijn positie moet nemen (Rechts met Correctie)
	//p; p.first = 4; p.second = 10;
	//q; q.first = 5; q.second = 7;
	//test = stad->getStadsdeel(p);
	//EXPECT_EQ(Stadsdeel::Rechts, stad->checkRichting(test, q));
}

TEST(Stadsdelen_Setters, Stadsdelen)
{
	// Check de getters en setters van de brandweerkazerne
	Brandweerkazerne *bk = new Brandweerkazerne();
	std::string s1 = "Test123";
	std::string s2 = "BW1337";
	std::pair<int, int> p; p.first = 10; p.second = 8;
	std::pair<int, int> q; p.first = 9; p.second = 8;
	int b = 20;
	bk->setNaam(s1);
	bk->setIngang(p);
	bk->setLocatie(q);
	
	EXPECT_EQ(s1, bk->getNaam());
	EXPECT_EQ(p, bk->getIngang());
	EXPECT_EQ(q, bk->getLocatie());
	EXPECT_EQ('X', bk->getKarakter());
	EXPECT_EQ(Stadsdeel::Station, bk->getType());

	//Check de getters en setters van de brandweerwagen
	Brandweerwagen *bw = new Brandweerwagen();
	bw->setNaam(s1);
	bw->setBasis(s2);
	bw->setLocatie(p);

	EXPECT_EQ(s1, bw->getNaam());
	EXPECT_EQ(s2, bw->getBasis());
	EXPECT_EQ(p, bw->getLocatie());
	EXPECT_EQ('B', bw->getKarakter());
	EXPECT_EQ(Stadsdeel::Wagon, bw->getType());

	//Check de getters en setters van de straten
	Straat *s = new Straat();
	p; p.first = 0; p.second = 0;
	q; q.first = 0; q.second = 15;
	s->setBegin(p);
	s->setEinde(q);
	s->setNaam(s1);

	EXPECT_EQ(s1, s->getNaam());
	EXPECT_EQ(p, s->getBegin());
	EXPECT_EQ(q, s->getEinde());
	EXPECT_EQ('_', s->getKarakter());
	EXPECT_EQ(Stadsdeel::Street, s->getType());

	//Check de getters en setters van de huizen
	Huis *h = new Huis();
	p; p.first = 9; p.second = 3;
	h->setLocatie(p);
	h->setBrandbaarheid(b);

	EXPECT_EQ(p, h->getLocatie());
	EXPECT_EQ(b, h->getBrandbaarheid());
	EXPECT_EQ('H', h->getKarakter());
	EXPECT_EQ(Stadsdeel::House, h->getType());

	//Check de getters en setters van de kruispunt
	Kruispunt *k = new Kruispunt();
	k->setEersteStraat(s1);
	k->setTweedeStraat(s2);

	EXPECT_EQ(s1, k->getEersteStraat());
	EXPECT_EQ(s2, k->getTweedeStraat());
	EXPECT_EQ('O', k->getKarakter());
	EXPECT_EQ(Stadsdeel::Crossroad, k->getType());

}

TEST(Stadsdelen_Output, Stadsdelen)
{
	//Inlezen van de de juiste stad
	Stad* stad = new Stad();
	stad->stadInlezen("stad2.0_bis.xml");
	//EXPECT_TRUE(FileCompare("console.txt","console_standaard.txt"));
}
TEST(end, ending)
{
	system("pause");
}


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
TEST(Stad_Outputs, Stadsdelen)
{
	Stad* stad = new Stad();
	stad->stadInlezen("s.xml");
	EXPECT_TRUE(FileCompare("testfiles/sample1.txt", "console.txt"));
	EXPECT_FALSE(FileCompare("testfiles/sample1-1.txt", "console.txt"));

	EXPECT_TRUE(FileCompare("testfiles/sample2.txt", "status.txt"));
	EXPECT_FALSE(FileCompare("testfiles/sample2-2.txt", "status.txt"));

	stad->printStad();

	EXPECT_TRUE(FileCompare("testfiles/sample3.txt", "status.txt"));
	EXPECT_FALSE(FileCompare("testfiles/sample3-3.txt", "status.txt"));

	stad->simulatie2();

	EXPECT_TRUE(FileCompare("testfiles/sample4.txt", "status.txt"));
	EXPECT_FALSE(FileCompare("testfiles/sample4-4.txt", "status.txt"));

}
TEST(Stad_Inlezen, Stadsdelen)
{
	//Inlezen van de de foutieve stad
	Stad* stad = new Stad();
	EXPECT_EQ(false, stad->stadInlezen("stad2.0_bis_fout.xml"));
	EXPECT_EQ(false, stad->checkStad());

	//Inlezen van de de juiste stad
	EXPECT_EQ(true, stad->stadInlezen("s.xml"));
	EXPECT_EQ(true, stad->checkStad());

	//Opvragen van een stadsdeel buiten de stad
	Location p; p.setX(-1); p.setY(-1);
	Location q; q.setX(-1); q.setY(-1);
	Vstadsdeel* test = stad->getVstadsdeel(p);

	EXPECT_EQ(nullptr, test);

}

TEST(Stad_Methodes, Stadsdelen)
{
	Stad* stad = new Stad();
	stad->stadInlezen("s.xml");
	Location p; p.setX(-1); p.setY(-1);
	Location q; q.setX(-1); q.setY(-1);
	Vstadsdeel* test = stad->getVstadsdeel(p);
	p; p.setX(5); p.setY(12);
	test = stad->getVstadsdeel(p);

	//Checken of het huis aan een straat zit
	EXPECT_TRUE(stad->checkAangrenzing(test));

	//Checken van een stadsdeel buiten de stad
	p; p.setX(-1); p.setY(-1);
	test = stad->getVstadsdeel(p);
	EXPECT_DEATH(stad->checkAangrenzing(test), "");

	//Checken of de wagen bij de bestemming is
	p; p.setX(5); p.setY(10);
	q; q.setX(4); q.setY(10);
	test = stad->getVstadsdeel(p);
	EXPECT_TRUE(stad->checkBestemming(test, q));

	//Checken of het huis aan een straat zit (niet waar)
	q; q.setX(0); q.setY(8);
	EXPECT_FALSE(stad->checkBestemming(test, q));

	//Checken of men in de straat van de bestemming zit (Horizontaal)
	p; p.setX(5); p.setY(12);
	test = stad->getVstadsdeel(p);

	q; q.setX(1); q.setY(10);
	EXPECT_EQ(Stad::MstraatType::Horizontaal, stad->checkMstraat(test, q));

	//Checken of men in de straat van de bestemming zit (Verticaal)
	q; q.setX(7); q.setY(13);
	EXPECT_EQ(Stad::MstraatType::Verticaal, stad->checkMstraat(test, q));

	//Checken of men in de straat van de bestemming zit (Geen)
	q; q.setX(4); q.setY(5);
	EXPECT_EQ(Stad::MstraatType::Geen, stad->checkMstraat(test, q));
}

TEST(Stad_Richting, Stadsdelen)
{
	Stad* stad = new Stad();
	stad->stadInlezen("s.xml");
	Location p; p.setX(-1); p.setY(-1);
	Location q; q.setX(-1); q.setY(-1);
	Vstadsdeel* test = stad->getVstadsdeel(p);
	

	// Check de richting die de wagen vanaf zijn positie moet nemen (Onder)
	p.setX(3); p.setY(7);
	q.setX(0); q.setY(14);
	test = stad->getVstadsdeel(p);
	EXPECT_EQ(Vstadsdeel::Onder, stad->checkRichting(test, q)); 

	// Check de richting die de wagen vanaf zijn positie moet nemen (Boven)
	q.setX(0); q.setY(4);
	test = stad->getVstadsdeel(p);
	EXPECT_EQ(Vstadsdeel::Boven, stad->checkRichting(test, q));

	// Check de richting die de wagen vanaf zijn positie moet nemen (Links)
	q.setX(10); q.setY(5);
	test = stad->getVstadsdeel(p);
	EXPECT_EQ(Vstadsdeel::Links, stad->checkRichting(test, q));

	// Check de richting die de wagen vanaf zijn positie moet nemen (Rechts)
	q.setX(0); q.setY(5);
	test = stad->getVstadsdeel(p);
	EXPECT_EQ(Vstadsdeel::Rechts, stad->checkRichting(test, q));

}

TEST(Stadsdelen_Setters, Stadsdelen)
{
	// Check de getters en setters van de brandweerkazerne
	Mbrandweerkazerne *bk = new Mbrandweerkazerne();
	std::string s1 = "Test123";
	std::string s2 = "BW1337";
	Location p; p.setX(10); p.setX(8);
	Location q; q.setX(9); q.setY(8);
	int b = 20;
	bk->setNaam(s1);
	bk->setIngang(p);
	bk->setLocatie(q);
	
	EXPECT_EQ(s1, bk->getNaam());
	EXPECT_EQ(p.getX(), bk->getIngang().getX());
	EXPECT_EQ(p.getY(), bk->getIngang().getY());
	EXPECT_EQ(q.getX(), bk->getLocatie().getX());
	EXPECT_EQ(q.getY(), bk->getLocatie().getY());
	
	//Check de getters en setters van de brandweerwagen
	Mbrandweerwagen *bw = new Mbrandweerwagen();
	p; p.setX(0); p.setX(5);
	bw->setNaam(s1);
	bw->setBasis(s2);
	bw->setLocatie(p);

	EXPECT_EQ(s1, bw->getNaam());
	EXPECT_EQ(s2, bw->getBasis());
	EXPECT_EQ(p.getX(), bw->getLocatie().getX());
	EXPECT_EQ(p.getY(), bw->getLocatie().getY());
	EXPECT_EQ(Vstadsdeel::Wagon, bw->getType());

	//Check de getters en setters van de straten
	Mstraat *s = new Mstraat();
	p; p.setX(0); p.setX(0);
	q; q; q.setX(0); q.setY(15);
	s->setBegin(p);
	s->setEinde(q);
	s->setNaam(s1);

	EXPECT_EQ(s1, s->getNaam());
	EXPECT_EQ(p.getX(), s->getBegin().getX());
	EXPECT_EQ(p.getY(), s->getBegin().getY());
	EXPECT_EQ(q.getX(), s->getEinde().getX());
	EXPECT_EQ(q.getY(), s->getEinde().getY());
	EXPECT_EQ(Vstadsdeel::Street, s->getType());

	//Check de getters en setters van de huizen
	Mhuis *h = new Mhuis();
	p; q.setX(9); p.setY(3);
	h->setLocatie(p);
	h->setBrandbaarheid(b);

	EXPECT_EQ(p.getX(), h->getLocatie().getX());
	EXPECT_EQ(p.getY(), h->getLocatie().getY());
	EXPECT_EQ(b, h->getBrandbaarheid());
	EXPECT_EQ(Vstadsdeel::House, h->getType());

	//Check de getters en setters van de kruispunt
	Mkruispunt *k = new Mkruispunt();
	k->setEersteMstraat(s1);
	k->setTweedeMstraat(s2);

	EXPECT_EQ(s1, k->getEersteMstraat());
	EXPECT_EQ(s2, k->getTweedeMstraat());
	EXPECT_EQ(Vstadsdeel::Crossroad, k->getType());
	system("pause");
}



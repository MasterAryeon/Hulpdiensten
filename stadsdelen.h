#ifndef YOUR_HEADER_H
#define YOUR_HEADER_H

#include <iostream>
#include <string>
#include <vector>

class Location
{
public: 
	Location();
	Location(int,int);
	bool properlyInitialized();
	int getX();
	int getY();
	void setX(int);
	void setY(int);

private:
	int X = 0;
	int Y = 0;
	Location* _initCheck;
};
/*
CLASSE VOOR DE BASE
*/
class Vstadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Vstadsdeel();
	bool properlyInitialized();
	enum Type { Station, Police, Hospital, Wagon, House, Store, Street, Crossroad, Policewagon, Mambulancewagon };
	enum Richting { Links, Rechts, Onder, Boven };
	virtual bool getInBrand();
	virtual Location getLocatie();
	virtual Location getBegin();
	virtual Location getEinde();
	virtual Location getGrootte();
	virtual int getBrandbaarheid();
	virtual int getOvervalbaarheid();
	virtual void setInBrand(bool);
	virtual void setBrandbaarheid(int);
	virtual void setOvervalbaarheid(int);
	virtual char getKarakter();
	virtual std::string getNaam();
	virtual Type getType();
	virtual std::string getEersteMstraat();
	virtual std::string getTweedeMstraat();
private:
	bool inBrand = false;
	int brandbaarheid;
	int overvalbaarheid;
	Vstadsdeel* _initCheck;

};

/*
CLASSE VOOR DE Mbrandweerkazerne
*/
class Mbrandweerkazerne : public Vstadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Mbrandweerkazerne();
	bool properlyInitialized();
	void setNaam(std::string);
	void setLocatie(Location);
	void setIngang(Location);
	Location getLocatie();
	Location getIngang();
	char getKarakter();
	std::string getNaam();
	Type getType();
private:
	std::string naam;
	Location locatie;
	Location ingang;
	char karakter = 'K';
	Type type = Station;
	Mbrandweerkazerne* _initCheck;
};
/*
CLASSE VOOR DE Mpolitiebureau
*/
class Mpolitiebureau : public Vstadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Mpolitiebureau();
	bool properlyInitialized();
	void setNaam(std::string);
	void setLocatie(Location);
	void setIngang(Location);
	void setGrootte(Location);
	Location getLocatie();
	Location getIngang();
	Location getGrootte();
	char getKarakter();
	std::string getNaam();
	Type getType();
private:
	std::string naam;
	Location locatie;
	Location ingang;
	Location grootte;
	char karakter = 'P';
	Type type = Police;
	Mpolitiebureau* _initCheck;
};
/*
CLASSE VOOR DE MziekenMhuis
*/
class MziekenMhuis : public Vstadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	MziekenMhuis();
	bool properlyInitialized();
	void setNaam(std::string);
	void setLocatie(Location);
	void setIngang(Location);
	void setGrootte(Location);
	Location getLocatie();
	Location getIngang();
	Location getGrootte();
	char getKarakter();
	std::string getNaam();
	Type getType();
private:
	std::string naam;
	Location locatie;
	Location ingang;
	Location grootte;
	char karakter = 'Z';
	Type type = Hospital;
	MziekenMhuis* _initCheck;
};
/*
CLASSE VOOR DE Mbrandweerwagen
*/
class Mbrandweerwagen : Vstadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Mbrandweerwagen();
	bool properlyInitialized();
	void setNaam(std::string);
	void setBasis(std::string);
	void setLocatie(Location);
	void setRichting(Vstadsdeel::Richting);
	Location getLocatie();
	Vstadsdeel::Richting getRichting();
	std::string getBasis();
	std::string getNaam();
	char getKarakter();
	Vstadsdeel::Type getType();
private:
	std::string naam;
	std::string basis;
	Location locatie;
	char karakter = 'B';
	Vstadsdeel::Type type = Vstadsdeel::Wagon;
	Vstadsdeel::Richting richting;
	Mbrandweerwagen* _initCheck;
};
class Mpolitiewagen : Vstadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Mpolitiewagen();
	bool properlyInitialized();
	void setNaam(std::string);
	void setBasis(std::string);
	void setLocatie(Location);
	void setRichting(Vstadsdeel::Richting);
	Location getLocatie();
	Vstadsdeel::Richting getRichting();
	std::string getBasis();
	std::string getNaam();
	char getKarakter();
	Vstadsdeel::Type getType();
private:
	std::string naam;
	std::string basis;
	Location locatie;
	char karakter = 'B';
	Vstadsdeel::Type type = Vstadsdeel::Policewagon;
	Vstadsdeel::Richting richting;
	Mpolitiewagen* _initCheck;
};
class Mambulance : Vstadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Mambulance();
	bool properlyInitialized();
	void setNaam(std::string);
	void setBasis(std::string);
	void setLocatie(Location);
	void setRichting(Vstadsdeel::Richting);
	Location getLocatie();
	Vstadsdeel::Richting getRichting();
	std::string getBasis();
	std::string getNaam();
	char getKarakter();
	Vstadsdeel::Type getType();
private:
	std::string naam;
	std::string basis;
	Location locatie;
	Vstadsdeel::Richting richting;
	char karakter = 'B';
	Vstadsdeel::Type type = Vstadsdeel::Mambulancewagon;
	Mambulance* _initCheck;
};
/*
CLASSE VOOR DE Mstraat
*/
class Mstraat : public Vstadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Mstraat();
	bool properlyInitialized();
	void setNaam(std::string);
	void setBegin(Location);
	void setEinde(Location);
	std::string getNaam();
	Location getBegin();
	Location getEinde();
	char getKarakter();
	Type getType();
private:
	std::string naam;
	Location begin;
	Location einde;
	char karakter = ' ';
	Type type = Street;
	Mstraat* _initCheck;
};

/*
CLASSE VOOR EEN Mhuis
*/
class Mhuis : public Vstadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Mhuis();
	bool properlyInitialized();
	bool getInBrand();
	void setInBrand(bool);
	void setLocatie(Location);
	int getBrandbaarheid();
	void setBrandbaarheid(int);
	Location getLocatie();
	char getKarakter();
	Type getType();
private:
	Location locatie;
	bool inBrand = false;
	int brandbaarheid;
	char karakter = 'H';
	Type type = House;
	Mhuis* _initCheck;
};
/*
CLASSE VOOR EEN Mwinkel
*/
class Mwinkel : public Vstadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Mwinkel();
	bool properlyInitialized();
	bool getInBrand();
	void setInBrand(bool);
	void setLocatie(Location);
	int getBrandbaarheid();
	int getOvervalbaarheid();
	void setBrandbaarheid(int);
	void setOvervalbaarheid(int);
	void setGrootte(Location);
	Location getLocatie();
	Location getGrootte();
	char getKarakter();
	Type getType();
private:
	Location locatie;
	Location grootte;
	bool inBrand = false;
	int brandbaarheid; int overvalbaarheid;
	char karakter = 'W';
	Type type = Store;
	Mwinkel* _initCheck;
};
/*
CLASSE VOOR EEN Mkruispunt
*/
class Mkruispunt : public Vstadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Mkruispunt();
	void setEersteMstraat(std::string);
	void setTweedeMstraat(std::string);
	void setLocatie(Location);
	std::string getEersteMstraat();
	std::string getTweedeMstraat();
	std::string getNaam();
	char getKarakter();
	Type getType();
	bool properlyInitialized();
private:
	std::string eersteMstraat = "";
	std::string tweedeMstraat = "";
	Location locatie;
	char karakter = ' ';
	Type type = Crossroad;
	Mkruispunt* _initCheck;
};



/*
CLASSE VOOR DE STAD
*/
class Stad
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	
	enum MstraatType { Horizontaal, Verticaal, Geen };

	void setVstadsdeel(Vstadsdeel &);
	Vstadsdeel* getVstadsdeel(Location);

	bool checkAangrenzing(Vstadsdeel*);

	bool checkBestemming(Vstadsdeel*, Location);
	MstraatType checkMstraat(Vstadsdeel*, Location);
	Vstadsdeel::Richting checkRichting(Vstadsdeel*, Location);
	Location veranderLocatie(Location, Vstadsdeel::Richting);

	
	void simulatie();
	void simulatie2();

	std::vector<Mbrandweerkazerne*> kazernes;
	std::vector<Mpolitiebureau*> bureaus;
	std::vector<MziekenMhuis*> ziekenhuizen;

	std::vector<Mbrandweerwagen*> Mbrandweerwagens;
	std::vector<std::pair<Vstadsdeel*,Mbrandweerwagen*>> Mbrandweerwagens_onroad;
	std::vector<Mbrandweerwagen*> Mbrandweerwagens_onstandby;

	std::vector<Mpolitiewagen*> Mpolitiewagens;
	std::vector<std::pair<Vstadsdeel*, Mpolitiewagen*>> Mpolitiewagens_onroad;
	std::vector<Mpolitiewagen*> Mpolitiewagens_onstandby;

	std::vector<Mambulance*> Mambulances;
	std::vector<std::pair<Vstadsdeel*, Mambulance*>> Mambulances_onroad;
	std::vector<Mambulance*> Mambulances_onstandby;

	std::vector<Vstadsdeel*> brandhuizen;
	std::vector<Vstadsdeel*> overvalhuizen;
	std::vector<Vstadsdeel*> ongevalhuizen;
	
	Location getGebouwCoord(int);//
	std::pair<Location, Location> getMstraatCoord(int);//

	void addGebouwCoord(Location);//
	void addMstraatCoord(std::pair<Location, Location>);//

	int getGebouwCoordsLength();//
	int getMstraatCoordsLength();//

	bool checkStad();//
	void printStad();
	bool stadInlezen(const char*);
	Stad();
	bool properlyInitialized();
protected:
	static const int xrow = 22;
	static const int yrow = 16;
private:
	Stad* _initCheck;
	Vstadsdeel* grid[yrow][xrow];
	std::ofstream* status;
	std::ofstream* console;
	std::vector<Location> gebouwCoords;
	std::vector<std::pair<Location, Location>> MstraatCoords;
};
class OutputClass
{
public:
	OutputClass();
	void writeToConsole(std::string);
	void writeToConsole();
	void writeToStatus(std::string);
	void writeToStatus();
	bool properlyInitialized();
private:
	std::ofstream* status;
	std::ofstream* console;
	OutputClass* _initCheck;
};

/*
CLASS UI
*/

class UI
{
public:
	bool properlyInitialized();
	UI();
	void showMenu();
	void setSteps();
	void setChoice(int);
	void setSU(bool);
	void setStad(Stad*);
	void leesBestandStad();
	void leesBestandVoertuigen();
	void startBrand();
	void startOverval();
	void runSimulatie();
	bool startMbrandweerwagen();
	bool getSU();
	void showMap();

private:
	Stad* stad;
	int steps = 10;
	int choice;
	bool SU = true;
	UI* _initCheck;
};
#endif

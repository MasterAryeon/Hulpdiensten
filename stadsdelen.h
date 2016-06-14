#ifndef YOUR_HEADER_H
#define YOUR_HEADER_H

#include <iostream>
#include <string>
#include <vector>


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
	virtual std::pair<int, int> getLocatie();
	virtual std::pair<int, int> getBegin();
	virtual std::pair<int, int> getEinde();
	virtual std::pair<int, int> getGrootte();
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
	bool inBrand;
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
	void setLocatie(std::pair<int, int>);
	void setIngang(std::pair<int, int>);
	std::pair<int, int> getLocatie();
	std::pair<int, int> getIngang();
	char getKarakter();
	std::string getNaam();
	Type getType();
private:
	std::string naam;
	std::pair<int, int> locatie;
	std::pair<int, int> ingang;
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
	void setLocatie(std::pair<int, int>);
	void setIngang(std::pair<int, int>);
	void setGrootte(std::pair<int, int>);
	std::pair<int, int> getLocatie();
	std::pair<int, int> getIngang();
	std::pair<int, int> getGrootte();
	char getKarakter();
	std::string getNaam();
	Type getType();
private:
	std::string naam;
	std::pair<int, int> locatie;
	std::pair<int, int> ingang;
	std::pair<int, int> grootte;
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
	void setLocatie(std::pair<int, int>);
	void setIngang(std::pair<int, int>);
	void setGrootte(std::pair<int, int>);
	std::pair<int, int> getLocatie();
	std::pair<int, int> getIngang();
	std::pair<int, int> getGrootte();
	char getKarakter();
	std::string getNaam();
	Type getType();
private:
	std::string naam;
	std::pair<int, int> locatie;
	std::pair<int, int> ingang;
	std::pair<int, int> grootte;
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
	void setLocatie(std::pair<int, int>);
	void setRichting(Vstadsdeel::Richting);
	std::pair<int, int> getLocatie();
	Vstadsdeel::Richting getRichting();
	std::string getBasis();
	std::string getNaam();
	char getKarakter();
	Vstadsdeel::Type getType();
private:
	std::string naam;
	std::string basis;
	std::pair<int, int> locatie;
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
	void setLocatie(std::pair<int, int>);
	void setRichting(Vstadsdeel::Richting);
	std::pair<int, int> getLocatie();
	Vstadsdeel::Richting getRichting();
	std::string getBasis();
	std::string getNaam();
	char getKarakter();
	Vstadsdeel::Type getType();
private:
	std::string naam;
	std::string basis;
	std::pair<int, int> locatie;
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
	void setLocatie(std::pair<int, int>);
	void setRichting(Vstadsdeel::Richting);
	std::pair<int, int> getLocatie();
	Vstadsdeel::Richting getRichting();
	std::string getBasis();
	std::string getNaam();
	char getKarakter();
	Vstadsdeel::Type getType();
private:
	std::string naam;
	std::string basis;
	std::pair<int, int> locatie;
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
	void setBegin(std::pair<int, int>);
	void setEinde(std::pair<int, int>);
	std::string getNaam();
	std::pair<int, int> getBegin();
	std::pair<int, int> getEinde();
	char getKarakter();
	Type getType();
private:
	std::string naam;
	std::pair<int, int> begin;
	std::pair<int, int> einde;
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
	void setLocatie(std::pair<int, int>);
	int getBrandbaarheid();
	void setBrandbaarheid(int);
	std::pair<int, int> getLocatie();
	char getKarakter();
	Type getType();
private:
	std::pair<int, int> locatie;
	bool inBrand;
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
	void setLocatie(std::pair<int, int>);
	int getBrandbaarheid();
	int getOvervalbaarheid();
	void setBrandbaarheid(int);
	void setOvervalbaarheid(int);
	void setGrootte(std::pair<int, int>);
	std::pair<int, int> getLocatie();
	std::pair<int, int> getGrootte();
	char getKarakter();
	Type getType();
private:
	std::pair<int, int> locatie;
	std::pair<int, int> grootte;
	bool inBrand;
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
	void setLocatie(std::pair<int, int>);
	std::string getEersteMstraat();
	std::string getTweedeMstraat();
	std::string getNaam();
	char getKarakter();
	Type getType();
	bool properlyInitialized();
private:
	std::string eersteMstraat = "";
	std::string tweedeMstraat = "";
	std::pair<int, int> locatie;
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
	Vstadsdeel* getVstadsdeel(std::pair<int, int>);

	bool checkAangrenzing(Vstadsdeel*);

	bool checkBestemming(Vstadsdeel*, std::pair<int, int>);
	MstraatType checkMstraat(Vstadsdeel*, std::pair<int, int>);
	Vstadsdeel::Richting checkRichting(Vstadsdeel*, std::pair<int, int>);
	std::pair<int, int> veranderLocatie(std::pair<int, int>, Vstadsdeel::Richting);

	
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
	
	std::pair<int, int> getGebouwCoord(int);//
	std::pair<std::pair<int, int>, std::pair<int, int>> getMstraatCoord(int);//

	void addGebouwCoord(std::pair<int, int>);//
	void addMstraatCoord(std::pair<std::pair<int, int>, std::pair<int, int>>);//

	int getGebouwCoordsLength();//
	int getMstraatCoordsLength();//

	bool checkStad();//
	void printStad();
	bool stadInlezen(const char*);
	Stad();
	bool properlyInitialized();
protected:
	static const int xrow = 16;
	static const int yrow = 22;
private:
	Stad* _initCheck;
	Vstadsdeel* grid[xrow][yrow];
	std::ofstream* status;
	std::ofstream* console;
	std::vector<std::pair<int, int>> gebouwCoords;
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> MstraatCoords;
};
class OutputClass
{
public:
	OutputClass();
	void writeToConsole(std::string);
	void writeToConsole();
	void writeToStatus(std::string);
	void writeToStatus();
private:
	std::ofstream* status;
	std::ofstream* console;
};

/*
CLASS UI
*/

class UI
{
public:
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
	UI();

private:
	Stad* stad;
	int steps = 10;
	int choice;
	bool SU = true;
	UI* _initCheck;
};
#endif
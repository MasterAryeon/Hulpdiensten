#ifndef YOUR_HEADER_H
#define YOUR_HEADER_H

#include <iostream>
#include <string>
#include <vector>


/*
CLASSE VOOR DE BASE
*/
class Stadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Stadsdeel();
	bool properlyInitialized();
	enum Type { Station, Police, Hospital, Wagon, House, Store, Street, Crossroad, Policewagon, Ambulancewagon };
	enum Richting { Links, Rechts, Onder, Boven };
	virtual std::pair<int, int> getLocatie();
	virtual std::pair<int, int> getBegin();
	virtual std::pair<int, int> getEinde();
	virtual std::pair<int, int> getGrootte();
	virtual int getBrandbaarheid();
	virtual int getOvervalbaarheid();
	virtual void setBrandbaarheid(int);
	virtual void setOvervalbaarheid(int);
	virtual char getKarakter();
	virtual std::string getNaam();
	virtual Type getType();
	virtual std::string getEersteStraat();
	virtual std::string getTweedeStraat();
private:
	int brandbaarheid;
	int overvalbaarheid;
	Stadsdeel* _initCheck;

};

/*
CLASSE VOOR DE BRANDWEERKAZERNE
*/
class Brandweerkazerne : public Stadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Brandweerkazerne();
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
	char karakter = 'X';
	Type type = Station;
	Brandweerkazerne* _initCheck;
};
/*
CLASSE VOOR DE POLITIEBUREAU
*/
class Politiebureau : public Stadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Politiebureau();
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
	Politiebureau* _initCheck;
};
/*
CLASSE VOOR DE ZIEKENHUIS
*/
class Ziekenhuis : public Stadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Ziekenhuis();
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
	Ziekenhuis* _initCheck;
};
/*
CLASSE VOOR DE BRANDWEERWAGEN
*/
class Brandweerwagen : Stadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Brandweerwagen();
	bool properlyInitialized();
	void setNaam(std::string);
	void setBasis(std::string);
	void setLocatie(std::pair<int, int>);
	void setRichting(Stadsdeel::Richting);
	std::pair<int, int> getLocatie();
	Stadsdeel::Richting getRichting();
	std::string getBasis();
	std::string getNaam();
	char getKarakter();
	Stadsdeel::Type getType();
private:
	std::string naam;
	std::string basis;
	std::pair<int, int> locatie;
	char karakter = 'B';
	Stadsdeel::Type type = Stadsdeel::Wagon;
	Stadsdeel::Richting richting;
	Brandweerwagen* _initCheck;
};
class Politiewagen : Stadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Politiewagen();
	bool properlyInitialized();
	void setNaam(std::string);
	void setBasis(std::string);
	void setLocatie(std::pair<int, int>);
	void setRichting(Stadsdeel::Richting);
	std::pair<int, int> getLocatie();
	Stadsdeel::Richting getRichting();
	std::string getBasis();
	std::string getNaam();
	char getKarakter();
	Stadsdeel::Type getType();
private:
	std::string naam;
	std::string basis;
	std::pair<int, int> locatie;
	char karakter = 'B';
	Stadsdeel::Type type = Stadsdeel::Policewagon;
	Stadsdeel::Richting richting;
	Politiewagen* _initCheck;
};
class Ambulance : Stadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Ambulance();
	bool properlyInitialized();
	void setNaam(std::string);
	void setBasis(std::string);
	void setLocatie(std::pair<int, int>);
	std::pair<int, int> getLocatie();
	std::string getBasis();
	std::string getNaam();
	char getKarakter();
	Stadsdeel::Type getType();
private:
	std::string naam;
	std::string basis;
	std::pair<int, int> locatie;
	char karakter = 'B';
	Stadsdeel::Type type = Stadsdeel::Ambulancewagon;
	Ambulance* _initCheck;
};
/*
CLASSE VOOR DE STRAAT
*/
class Straat : public Stadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Straat();
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
	char karakter = '_';
	Type type = Street;
	Straat* _initCheck;
};

/*
CLASSE VOOR EEN HUIS
*/
class Huis : public Stadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Huis();
	bool properlyInitialized();
	void setLocatie(std::pair<int, int>);
	int getBrandbaarheid();
	void setBrandbaarheid(int);
	std::pair<int, int> getLocatie();
	char getKarakter();
	Type getType();
private:
	std::pair<int, int> locatie;
	int brandbaarheid;
	char karakter = 'H';
	Type type = House;
	Huis* _initCheck;
};
/*
CLASSE VOOR EEN WINKEL
*/
class Winkel : public Stadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Winkel();
	bool properlyInitialized();
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
	int brandbaarheid; int overvalbaarheid;
	char karakter = 'W';
	Type type = Store;
	Winkel* _initCheck;
};
/*
CLASSE VOOR EEN KRUISPUNT
*/
class Kruispunt : public Stadsdeel
{
public:
	/**
	\n ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
	*/
	Kruispunt();
	void setEersteStraat(std::string);
	void setTweedeStraat(std::string);
	void setLocatie(std::pair<int, int>);
	std::string getEersteStraat();
	std::string getTweedeStraat();
	std::string getNaam();
	char getKarakter();
	Type getType();
	bool properlyInitialized();
private:
	std::string eersteStraat = "";
	std::string tweedeStraat = "";
	std::pair<int, int> locatie;
	char karakter = 'O';
	Type type = Crossroad;
	Kruispunt* _initCheck;
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
	
	enum StraatType { Horizontaal, Verticaal, Geen };

	void setStadsdeel(Stadsdeel &);
	Stadsdeel* getStadsdeel(std::pair<int, int>);

	bool checkAangrenzing(Stadsdeel*);

	bool checkBestemming(Stadsdeel*, std::pair<int, int>);
	StraatType checkStraat(Stadsdeel*, std::pair<int, int>);
	Stadsdeel::Richting checkRichting(Stadsdeel*, std::pair<int, int>);
	std::pair<int, int> veranderLocatie(std::pair<int, int>, Stadsdeel::Richting);

	
	void simulatie();
	void simulatie2();

	std::vector<Brandweerkazerne*> kazernes;
	std::vector<Politiebureau*> bureaus;
	std::vector<Ziekenhuis*> ziekenhuizen;

	std::vector<Brandweerwagen*> brandweerwagens;
	std::vector<std::pair<Stadsdeel*,Brandweerwagen*>> brandweerwagens_onroad;
	std::vector<Brandweerwagen*> brandweerwagens_onstandby;

	std::vector<Politiewagen*> politiewagens;
	std::vector<std::pair<Stadsdeel*, Politiewagen*>> politiewagens_onroad;
	std::vector<Politiewagen*> politiewagens_onstandby;

	std::vector<Ambulance*> ambulances;
	std::vector<std::pair<Stadsdeel*, Ambulance*>> ambulances_onroad;
	std::vector<Ambulance*> ambulances_onstandby;

	std::vector<Stadsdeel*> brandhuizen;
	std::vector<Stadsdeel*> overvalhuizen;
	
	std::pair<int, int> getGebouwCoord(int);//
	std::pair<std::pair<int, int>, std::pair<int, int>> getStraatCoord(int);//

	void addGebouwCoord(std::pair<int, int>);//
	void addStraatCoord(std::pair<std::pair<int, int>, std::pair<int, int>>);//

	int getGebouwCoordsLength();//
	int getStraatCoordsLength();//

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
	Stadsdeel* grid[xrow][yrow];
	std::ofstream* status;
	std::ofstream* console;
	std::vector<std::pair<int, int>> gebouwCoords;
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> straatCoords;
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
	void runSimulatie();
	bool startBrandweerwagen();
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
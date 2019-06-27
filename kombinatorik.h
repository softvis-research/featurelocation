#ifndef KOMBINATORIK_H
#define KOMBINATORIK_H

#include <iostream>
using namespace std;

/* Wichtiger Hinweis:
   ==================
   Bei den folgenden Funktionen und Klassen kommt es eigentlich auf hoechste Performanz an. 
   Daher wuerde man ueblicherweise alle Funktionen inline implementieren und keine
   rekursiven Funktionsimplementierungen wie beispielsweise bei fakultaet() verwenden,
   sondern iterative.
*/

// Hilfsfunktionen in Form freier Funktionen
long unsigned int produkt(const long unsigned int& von,const long unsigned int& bis);
long unsigned int fakultaet(const long unsigned int& n);
long unsigned int potenz(const long unsigned int& basis,const long unsigned int& exponent);

class UrneMWMR {
public:
	// Die Klasse hat dynamische Anteile, daher die orthodoxe-kanonische Form!
	UrneMWMR(const unsigned& _n,const unsigned& _k);
	UrneMWMR(const UrneMWMR& urne);
	UrneMWMR& operator=(const UrneMWMR& urne); // Sollte fuer MV geeignet sein
	virtual ~UrneMWMR(); // Der Destruktor virtuell und zur Vererbung geeignet!
	virtual void assign(const UrneMWMR& urne); // virtuelle Zuweisungsfunktion
	virtual void zurueckSetzen();
	const unsigned int& n() const;
	const unsigned int& k() const;
	const unsigned int& element(const unsigned int& stelle) const;
	virtual bool naechsteZiehung();
	virtual long unsigned int ziehungen() const;
protected:
	unsigned n_,k_;
	unsigned int * ziehung_;
};

ostream& operator<<(ostream& os,const UrneMWMR& urne);

//-------------------------------------------------//

class UrneMWOR: virtual public UrneMWMR {
public:
	UrneMWOR(const unsigned int& n,const unsigned int& k);
	UrneMWOR(const UrneMWOR& urne);
	UrneMWOR operator=(const UrneMWOR& urne); // Sollte fuer MV geeignet sein
	bool naechsteZiehung();
	long unsigned int ziehungen() const;
 protected:
	 bool nichtSortiert() const;
};

//-------------------------------------------------//

class UrneOWMR: virtual public UrneMWMR {
public:
	UrneOWMR(const unsigned int& n,const unsigned int& k);
	UrneOWMR(const UrneOWMR& urne);
	UrneOWMR operator=(const UrneOWMR& urne); // Sollte fuer MV geeignet sein
	bool naechsteZiehung();
	long unsigned int ziehungen() const;
	void zurueckSetzen();
protected:
	bool duplikate() const;
};

//-------------------------------------------------//

class UrneOWOR: public UrneMWOR, public UrneOWMR {
public:
	UrneOWOR(const unsigned int& n,const unsigned int& k);
	UrneOWOR(const UrneOWOR& urne);
	UrneOWOR operator=(const UrneOWOR& urne); // Sollte fuer MV geeignet sein
	bool naechsteZiehung();
	long unsigned int ziehungen() const;
	void zurueckSetzen();
};

#endif // KOMBINATORIK_H

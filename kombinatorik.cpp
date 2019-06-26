#include "kombinatorik.h"
#include <typeinfo>
#include <cassert>
using namespace std;

/* Wichtiger Hinweis:
   ==================
   Bei den folgenden Funktionen und Klassen kommt es eigentlich auf hoechste Performanz an. 
   Daher wuerde man ueblicherweise alle Funktionen inline implementieren und keine
   rekursiven Funktionsimplementierungen wie beispielsweise bei fakultaet() verwenden,
   sondern iterative.
*/

// Hilfsfunktionen in Form freier Funktionen

long unsigned int produkt(const long unsigned int& von,const long unsigned int& bis) { 
	assert(von > 0);
	assert(von <= bis);
	long unsigned int ergebnis = von;
	for (long unsigned int i = von + 1; i <= bis; ++i)
		ergebnis *= i;
	return ergebnis;
}

long unsigned int fakultaet(const long unsigned int& n) {
	if (n == 0)
		return 1;
	else return produkt(1,n);
}

long unsigned int potenz(const long unsigned int& basis,const long unsigned int& exponent) {
	if (exponent == 0)
		return 1;
	if (basis == 0)
		return 0;
	long unsigned int ergebnis = basis;
	for (long unsigned int i = 2; i <= exponent; ++i)
		ergebnis *= basis;
	return ergebnis;
}

//-Memberfunktionen UrneMWMR-----------------------//

UrneMWMR::UrneMWMR(const unsigned& _n,const unsigned& _k):n_(_n),k_(_k),ziehung_(NULL) {
	assert(k() >= 2);
	ziehung_ = new unsigned int [k()];
	zurueckSetzen();
}

UrneMWMR::UrneMWMR(const UrneMWMR& urne):n_(urne.n()),k_(urne.k()),ziehung_(NULL) {
	ziehung_ = new unsigned int [k()];
	for (int i = 0; i < k(); ++i)
		ziehung_[i] = urne.element(i);
}

UrneMWMR& UrneMWMR::operator=(const UrneMWMR& urne) {
    assign(urne);
	return *this;
}

UrneMWMR::~UrneMWMR() {
	delete[] ziehung_;
}

void UrneMWMR::assign(const UrneMWMR& urne) {
	if (this != &urne) {
        assert(typeid(*this) == typeid(urne));
		delete[] ziehung_;
		n_ = urne.n();
		k_ = urne.k();
		ziehung_ = new unsigned int [k()];
		for (int i = 0; i < k(); ++i)
			ziehung_[i] = urne.element(i);
	}
}

void UrneMWMR::zurueckSetzen() {
	for (unsigned int i =0; i < k(); ++i)
	ziehung_[i] = 0;
}

const unsigned int& UrneMWMR::n() const {
	return n_;
}

const unsigned int& UrneMWMR::k() const {
	return k_;
}

const unsigned int& UrneMWMR::element(const unsigned int& stelle) const {
	assert(0 <= stelle);
	assert(stelle < k());
	return ziehung_[stelle];
}

bool UrneMWMR::naechsteZiehung() {
	for (int i = k() - 1; i >= 0; --i) {
		if (element(i) + 1 < n()) {
			++ziehung_[i];
			return true;
		}
		else ziehung_[i] = 0;
	}
	return false;
}

long unsigned int UrneMWMR::ziehungen() const { 
	return potenz(n(),k());
}

ostream& operator<<(ostream& os,const UrneMWMR& urne) {
  for (int i = 0; i < urne.k(); ++i)
  os << urne.element(i) << ' ';
  return os;
}

//-Memberfunktionen UrneMWOR-----------------------//

UrneMWOR::UrneMWOR(const unsigned int& n,const unsigned int& k): UrneMWMR(n,k) {
}

UrneMWOR::UrneMWOR(const UrneMWOR& urne):UrneMWMR(urne) {
}

UrneMWOR UrneMWOR::operator=(const UrneMWOR& urne) { 
    assign(urne);
	return *this;
}

bool UrneMWOR::naechsteZiehung() {
	do
		if (!UrneMWMR::naechsteZiehung())
		return false;
	while (nichtSortiert());
	return true;
}

long unsigned int UrneMWOR::ziehungen() const {  
	return produkt(n(),n() + k() - 1)/fakultaet(k());
}

bool UrneMWOR::nichtSortiert() const {
	for (int i = 0; i < k() - 1; ++i)
	if (element(i+1) < element(i))
	return true;
	return false;
}

//-Memberfunktionen UrneOWMR-----------------------//

UrneOWMR::UrneOWMR(const unsigned int& n,const unsigned int& k):UrneMWMR(n,k) {
	zurueckSetzen();
}

UrneOWMR::UrneOWMR(const UrneOWMR& urne):UrneMWMR(urne) {
}

UrneOWMR UrneOWMR::operator=(const UrneOWMR& urne) { 
	assign(urne);
	return *this;
}

bool UrneOWMR::naechsteZiehung() {
	do
		if (!UrneMWMR::naechsteZiehung())
		return false;
	while (duplikate());
	return true;
}

long unsigned int UrneOWMR::ziehungen() const {  
	return produkt(n() - k() + 1,n());
}

void UrneOWMR::zurueckSetzen() {
	UrneMWMR::zurueckSetzen();
	naechsteZiehung();
}

bool UrneOWMR::duplikate() const {
	for (int i = 0; i < k() - 1; ++i)
		for (int j = i + 1; j < k(); ++j)
			if (element(i) == element(j))
			return true;
	return false;
}

//-Memberfunktionen UrneOWOR-----------------------//

UrneOWOR::UrneOWOR(const unsigned int& n,const unsigned int& k):UrneMWMR(n,k),UrneMWOR(n,k),UrneOWMR(n,k) {
}

UrneOWOR::UrneOWOR(const UrneOWOR& urne):UrneMWMR(urne),UrneMWOR(urne),UrneOWMR(urne) {
}

UrneOWOR UrneOWOR::operator=(const UrneOWOR& urne) { 
	UrneMWMR::assign(urne);
	return *this;
}

bool UrneOWOR::naechsteZiehung() {
	do
		if (!UrneMWMR::naechsteZiehung())
		return false;
	while (duplikate() || nichtSortiert());
	return true;
}

long unsigned int UrneOWOR::ziehungen() const {  
	return n() == k() ? 1 : produkt(k() + 1,n())/fakultaet(n() - k());
}

void UrneOWOR::zurueckSetzen() {
	UrneOWMR::zurueckSetzen();
}
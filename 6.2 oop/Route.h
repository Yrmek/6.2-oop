#pragma once
#pragma once
#include <iostream>
#include <string>

using namespace std;

struct GPSLocation {
	float longitude;
	float latitude;
	friend istream& operator>>(istream& in, GPSLocation& me)
	{
		me.fill(in);
		return in;
	}

	void fill(istream& in)
	{
		cout << "> Enter Longitude and Latitide: " << endl;
		in >> longitude >> latitude;
	}
	void gpsprint(ostream& out) const {
		out << "Longitude " << longitude << endl;
		out << "Latitude " << latitude << endl;
	}
};

class Route {
protected:
	GPSLocation start;
	GPSLocation end;
public:
	void setLocation(GPSLocation st, GPSLocation en) {
		if (st.latitude < 0 || st.latitude > 90 || en.latitude < 0 || en.latitude > 90 ||
			st.longitude < -180 || st.longitude > 180 || en.longitude < -180 || en.longitude > 180)
			throw "Longitude or Latitude is out of bounds";
		start = st;
		end = en;
	}
	GPSLocation getStart() {
		return  start;
	}
	GPSLocation getEnd() {
		return end;
	}
	virtual ~Route() {
		cout << " Destructor Route" << endl;
	}
	virtual Route* clone() const = 0;
protected:
	Route() {
		start = { 0 , 0 };
		end = { 0 , 0 };
	}
	Route(GPSLocation st, GPSLocation en) {
		start = st;
		end = en;
	}

	Route(const Route& other) {
		start = other.start;
		end = other.end;
	}
	Route(const Route&& other) noexcept {
		start = other.start;
		end = other.end;
	}

	Route& operator= (Route&& other) {
		if (this != &other) {
			start = other.start;
			end = other.end;
		}
		return *this;
	}

	Route& operator= (const Route& other) {
		if (this != &other) {
			start = other.start;
			end = other.end;
		}
		return *this;
	}

	virtual void fill(istream& in)
	{
		cout << "> Enter coordinates of start and end: " << endl;
		in >> start >> end;
	}
	virtual void print(ostream& out) const
	{
		out << "__________________________________" << endl;
		out << "Type: " << get_type() << endl;
		out << "Start: " << endl;
		start.gpsprint(out);
		out << "End: " << endl;
		end.gpsprint(out);
	}

	virtual const char* get_type() const = 0;

	friend istream& operator>>(istream& in, Route& me)
	{
		me.fill(in);
		return in;
	}
	friend ostream& operator<<(ostream& out, const Route& me)
	{
		me.print(out);
		return out;
	}
};

class Vehicle : public Route {
protected:
	unsigned int time;
	unsigned int cost;
public:
	Vehicle& operator= (Vehicle&& other) {
		if (this != &other) {
			Route::operator=(move(other));
			time = other.time;
			cost = other.cost;
		}
		return *this;
	}
	Vehicle& operator= (const Vehicle& other) {
		if (this != &other) {
			Route::operator=(other);
			time = other.time;
			cost = other.cost;
		}
		return *this;
	}
	unsigned int getTime() {
		return time;
	}
	unsigned int getCost() {
		return cost;
	}
	virtual Vehicle* clone() const override {
		return new Vehicle(*this);
	}
	Vehicle() : Route() {
		time = 0;
		cost = 0;
	}
	Vehicle(unsigned int tme, unsigned int cst, GPSLocation st, GPSLocation en) : Route(st, en) {
		time = tme;
		cost = cst;
	}
	Vehicle(const Vehicle& other) : Route(other) {
		time = other.time;
		cost = other.cost;
	}
	Vehicle(Vehicle&& other) noexcept : Route(other) {
		time = other.time;
		cost = other.cost;
	}
	void setTimeAndCost(unsigned int tme, unsigned int cst) {
		if (time < 0 || cost < 0)
			throw "Time and cost cant be < 0";
		time = tme;
		cost = cst;
	}

	virtual ~Vehicle() override {
		cout << "Destructor Vehicle" << endl;
	}
protected:
	virtual void fill(istream& in) override
	{
		Route::fill(in);
		cout << "Enter time and cost :" << endl;
		in >> time >> cost;
	}
	virtual void print(ostream& out) const override
	{
		Route::print(out);
		out << "Cost :" << cost << endl;
		out << "Time :" << time << endl;
	}

	virtual const char* get_type() const override
	{
		return "Vehicle";
	}
};

class Car : public Vehicle {
protected:
	char siteLink[100];
public:
	Car& operator=(const Car& other) {
		if (this != &other) {
			Vehicle::operator=(other);
			strncpy_s(siteLink, other.siteLink, sizeof(siteLink) - 1);
			siteLink[sizeof(siteLink) - 1] = '\0';
		}
		return *this;
	}

	Car& operator=(Car&& other) noexcept {
		if (this != &other) {
			Vehicle::operator=(std::move(other));
			strncpy_s(siteLink, other.siteLink, sizeof(siteLink) - 1);
			siteLink[sizeof(siteLink) - 1] = '\0';
			other.siteLink[0] = '\0';
		}
		return *this;
	}
	virtual Car* clone() const override {
		return new Car(*this);
	}
	virtual ~Car() { cout << "Car destructor" << endl; }
	char* getSiteLink() {
		return siteLink;
	}
	Car() :Vehicle() {
		strcpy_s(siteLink, 100, "");
	}
	Car(unsigned int tme, unsigned int cst, GPSLocation st, GPSLocation en, const char* link) : Vehicle(tme, cst, st, en) {
		strcpy_s(siteLink, 100, link);
	}
	Car(const Car& other) : Vehicle(other) {
		strcpy_s(siteLink, 100, other.siteLink);
	}
	Car(Car&& other) noexcept : Vehicle(other) {
		strncpy_s(siteLink, other.siteLink, sizeof(siteLink) - 1);
		siteLink[sizeof(siteLink) - 1] = '\0';
		other.siteLink[0] = '\0';
	}
	void setSiteLink(const char* link) {
		if (sizeof(link) / sizeof(link[0]) >= 100)
			throw "Size of link > 100";
		strcpy_s(siteLink, link);
	}
protected:
	virtual void fill(istream& in) override
	{
		Vehicle::fill(in);
		cout << "Enter site link :" << endl;
		in >> siteLink;
	}
	virtual void print(ostream& out) const override
	{
		Vehicle::print(out);
		out << "Site link : " << siteLink << endl;
		out << "__________________________________" << endl;
	}

	virtual const char* get_type() const override
	{
		return "Car";
	}
};

class Bus : public Vehicle {
public:
	Bus() :Vehicle() {}
	Bus(unsigned int tme, unsigned int cst, GPSLocation st, GPSLocation en) : Vehicle(tme, cst, st, en) {}
	Bus(const Bus& other) : Vehicle(other) {}
	Bus(Bus&& other) : Vehicle(other) {}
	virtual ~Bus() { cout << "Bus destructor" << endl; }
	Bus& operator=(const Bus& other) {
		if (this != &other) {
			Vehicle::operator=(other);
		}
		return *this;
	}
	Bus& operator=(Bus&& other) noexcept {
		if (this != &other) {
			Vehicle::operator=(move(other));
		}
		return *this;
	}
	virtual Bus* clone() const override {
		return new Bus(*this);
	}
protected:
	virtual void fill(istream& in) override
	{
		Vehicle::fill(in);
	}
	virtual void print(ostream& out) const override
	{
		Vehicle::print(out);
		out << "__________________________________" << endl;
	}

	virtual const char* get_type() const override
	{
		return "Bus";
	}
};

class Trolleybus : public Vehicle {
public:
	Trolleybus() :Vehicle() {}
	Trolleybus(unsigned int tme, unsigned int cst, GPSLocation st, GPSLocation en) : Vehicle(tme, cst, st, en) {}
	Trolleybus(const Trolleybus& other) : Vehicle(other) {}
	Trolleybus(Trolleybus&& other) : Vehicle(other) {}
	virtual ~Trolleybus() { cout << " Trolleybus destructor" << endl; }
	Trolleybus& operator=(const Trolleybus& other) {
		if (this != &other) {
			Vehicle::operator=(other);
		}
		return *this;
	}
	Trolleybus& operator=(Trolleybus&& other) noexcept {
		if (this != &other) {
			Vehicle::operator=(move(other));
		}
		return *this;
	}
	virtual Trolleybus* clone() const override {
		return new Trolleybus(*this);
	}
protected:
	virtual void fill(istream& in) override
	{
		Vehicle::fill(in);
	}
	virtual void print(ostream& out) const override
	{
		Vehicle::print(out);
		out << "__________________________________" << endl;
	}

	virtual const char* get_type() const override
	{
		return "Trolleybus";
	}
};

class Metro : public Vehicle {
public:
	Metro() :Vehicle() {}
	Metro(unsigned int tme, unsigned int cst, GPSLocation st, GPSLocation en) : Vehicle(tme, cst, st, en) {}
	Metro(const Metro& other) : Vehicle(other) {}
	Metro(Metro&& other) : Vehicle(other) {}
	virtual ~Metro() { cout << " Metro destructor" << endl; }
	Metro& operator=(const Metro& other) {
		if (this != &other) {
			Vehicle::operator=(other);
		}
		return *this;
	}
	Metro& operator=(Metro&& other) noexcept {
		if (this != &other) {
			Vehicle::operator=(move(other));
		}
		return *this;
	}
	virtual Metro* clone() const override {
		return new Metro(*this);
	}
protected:
	virtual void fill(istream& in) override
	{
		Vehicle::fill(in);
	}
	virtual void print(ostream& out) const override
	{
		Vehicle::print(out);
		out << "__________________________________" << endl;
	}

	virtual const char* get_type() const override
	{
		return "Metro";
	}

};

class Taxi : public Vehicle {
public:
	Taxi() :Vehicle() {}
	Taxi(unsigned int tme, unsigned int cst, GPSLocation st, GPSLocation en) : Vehicle(tme, cst, st, en) {}
	Taxi(const Taxi& other) : Vehicle(other) {}
	Taxi(Taxi&& other) : Vehicle(other) {}
	virtual ~Taxi() { cout << " Taxi destructor" << endl; }
	Taxi& operator=(const Taxi& other) {
		if (this != &other) {
			Vehicle::operator=(other);
		}
		return *this;
	}
	Taxi& operator=(Taxi&& other) noexcept {
		if (this != &other) {
			Vehicle::operator=(move(other));
		}
		return *this;
	}
	virtual Taxi* clone() const override {
		return new Taxi(*this);
	}
protected:
	virtual void fill(istream& in) override
	{
		Vehicle::fill(in);
	}
	virtual void print(ostream& out) const override
	{
		Vehicle::print(out);
		out << "__________________________________" << endl;
	}

	virtual const char* get_type() const override
	{
		return "Taxi";
	}

};

class OnFoot : public Vehicle {
public:
	OnFoot() :Vehicle() {}
	OnFoot(unsigned int tme, GPSLocation st, GPSLocation en) : Vehicle(tme, 0, st, en) {}
	OnFoot(const OnFoot& other) : Vehicle(other) { }
	OnFoot(OnFoot&& other) : Vehicle(other) {}
	virtual ~OnFoot() { cout << " OnFoot destructor" << endl; }
	OnFoot& operator=(const OnFoot& other) {
		if (this != &other) {
			Vehicle::operator=(other);
		}
		return *this;
	}
	OnFoot& operator=(OnFoot&& other) noexcept {
		if (this != &other) {
			Vehicle::operator=(move(other));
		}
		return *this;
	}
	virtual OnFoot* clone() const override {
		return new OnFoot(*this);
	}
protected:
	virtual void fill(istream& in) override
	{
		Vehicle::fill(in);
	}
	virtual void print(ostream& out) const override
	{
		Vehicle::print(out);
		out << "__________________________________" << endl;
	}
	virtual const char* get_type() const override
	{
		return "OnFoot";
	}
};

#pragma once

/*
* 构建器模式的目标是将对象的创建与它的“表示法”分开。这就意味着，创建过程保持原状，但是产生对象的表示法可能不同。
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstddef>

using std::size_t;

class BicyclePart
{
public:
	enum BPart { FRAME, WHEEL, SEAT, DERAILLEUR, HANDLEBAR, SPROCKET, RACK, SHOCK, NPARTS};

private:
	BPart id;
	static std::string names[NPARTS];
	
public:
	BicyclePart(BPart bp) { id = bp; }
	friend std::ostream&
		operator<<(std::ostream& os, const BicyclePart& bp) {
		return os << bp.names[bp.id];
	}
};

class Bicycle {
	std::vector<BicyclePart*> parts;
public:
	~Bicycle() { /* purge(parts); */ }
	void addPart(BicyclePart* bp) { parts.push_back(bp); }
	friend std::ostream &
		operator<<(std::ostream& os, const Bicycle& b) {
		os << "{ ";
		for (size_t i = 0; i < b.parts.size(); ++i)
			os << *b.parts[i] << ' ';
		return os << '}';
	}
};

class BicycleBuilder {
protected:
	Bicycle* product;
public:
	BicycleBuilder() { product = 0; }
	void createProduct() { product = new Bicycle; }
	virtual void buildFrame() = 0;
	virtual void buildWheel() = 0;
	virtual void buildSeat() = 0;
	virtual void buildDerailleur() = 0;
	virtual void buildHandlebar() = 0;
	virtual void buildSprocket() = 0;
	virtual void buildRack() = 0;
	virtual void buildShock() = 0;
	virtual std::string getBikeName() const = 0;
	Bicycle* getProduct() {
		Bicycle* temp = product;
		product = 0;
		return temp;
	}
};

class MountainBikeBuilder : public BicycleBuilder{
public:
	virtual void buildFrame() = 0;
	virtual void buildWheel() = 0;
	virtual void buildSeat() = 0;
	virtual void buildDerailleur() = 0;
	virtual void buildHandlebar() = 0;
	virtual void buildSprocket() = 0;
	virtual void buildRack() = 0;
	virtual void buildShock() = 0;
	virtual std::string getBikeName() const { return "MountainBike"; }
};

class TouringBikeBuilder : public BicycleBuilder {
public:
	virtual void buildFrame() = 0;
	virtual void buildWheel() = 0;
	virtual void buildSeat() = 0;
	virtual void buildDerailleur() = 0;
	virtual void buildHandlebar() = 0;
	virtual void buildSprocket() = 0;
	virtual void buildRack() = 0;
	virtual void buildShock() = 0;
	virtual std::string getBikeName() const { return "TouringBike"; }
};

class RacingBikeBuilder : public BicycleBuilder {
public:
	virtual void buildFrame() = 0;
	virtual void buildWheel() = 0;
	virtual void buildSeat() = 0;
	virtual void buildDerailleur() = 0;
	virtual void buildHandlebar() = 0;
	virtual void buildSprocket() = 0;
	virtual void buildRack() = 0;
	virtual void buildShock() = 0;
	virtual std::string getBikeName() const { return "RacingBike"; }
};

class BicycleTechnician {
	BicycleBuilder* builder;
public:
	BicycleTechnician() { builder = 0; }
	void setBuilder(BicycleBuilder* b) { builder = b; }
	void construct();
};
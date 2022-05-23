#pragma once
#include <iostream>
#include <stdexcept>
#include <cstddef>
#include <string>
#include <vector>

using namespace std;

class CHYShapeFactory
{
public:
	virtual void draw() = 0;
	virtual void erase() = 0;
	virtual ~CHYShapeFactory() {}
	class BadShapeCreation : public logic_error
	{
	public:
		BadShapeCreation(string type) : logic_error("Cannot create type " + type) {}
	};

	static CHYShapeFactory* factory(const string& type) throw(BadShapeCreation);
};

class Circle : public CHYShapeFactory
{
	Circle() {}
	friend class CHYShapeFactory;
public:
	void draw() { cout << "Circle::draw" << endl; }
	void erase() { cout << "Circle::erase" << endl; }
	~Circle() { cout << "Circle::~Circle" << endl; }
};

class Square : public CHYShapeFactory
{
	Square() {}
	friend class CHYShapeFactory;
public:
	void draw() { cout << "Square::draw" << endl; }
	void erase() { cout << "Square::erase" << endl; }
	~Square() { cout << "Square::~Circle" << endl; }
};

CHYShapeFactory* CHYShapeFactory::factory(const string& type) throw(CHYShapeFactory::BadShapeCreation)
{
	if (type == "Circle")
	{
		return new Circle;
	}

	if (type == "Square")
	{
		return new Square;
	}

	throw BadShapeCreation(type);
}


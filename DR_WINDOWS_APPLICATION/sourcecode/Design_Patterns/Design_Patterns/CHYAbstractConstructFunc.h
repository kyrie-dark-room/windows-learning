#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstddef>

using namespace std;

class Shape
{
	Shape* s;
	Shape(Shape&);
	Shape operator=(Shape&);
protected:
	Shape() { s = 0; }
public:
	virtual void draw() { s->draw(); }
	virtual void erase() { s->erase(); }
	virtual void test() { s->test(); }
	virtual ~Shape() {
		cout << "~Shape" << endl;
		if (s) {
			cout << "Making virtual call: ";
			s->erase(); // Virtual call
		}
		cout << "delete s: ";
		delete s;
	}
	class BadShapeCreation : public logic_error {
	public:
		BadShapeCreation(string type) : logic_error("Cannot create type " + type) {}
	};
	Shape(string type) throw(BadShapeCreation);
};

class Circle : public Shape {
	Circle(Circle&);
	Circle operator=(Circle&);
	Circle() {}
	friend class Shape;
public:
	void draw() { cout << "Circle::draw" << endl; }
	void erase() { cout << "Circle::erase" << endl; }
	void test() { draw(); }
	~Circle() { cout << "Circle::~Circle" << endl; }
};

class Square : public Shape {
	Square(Square&);
	Square operator=(Square&);
	Square() {}
	friend class Shape;
public:
	void draw() { cout << "Square::draw" << endl; }
	void erase() { cout << "Square::erase" << endl; }
	void test() { draw(); }
	~Square() { cout << "Square::~Square" << endl; }
};

Shape::Shape(string type) throw(Shape::BadShapeCreation) {
	if (type == "Circle")
		s = new Circle;
	else if(type == "Square")
		s = new Square;
	else throw BadShapeCreation(type);
	draw();
}


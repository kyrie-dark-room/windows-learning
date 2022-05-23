#pragma once
#include <iostream>
using namespace std;

class CHYStrategy
{
public:
	virtual void greet() = 0;
};

class SayHi : public CHYStrategy
{
public:
	void greet()
	{
		cout << "Hi! How's it going?" << endl;
	}
};

class Ignore : public CHYStrategy
{
public:
	void greet()
	{
		cout << "(Pretend I don't see you)" << endl;
	}
};

class Admission : public CHYStrategy
{
public:
	void greet()
	{
		cout << "I'm sorry. I forgot your name." << endl;
	}
};

class Context
{
	CHYStrategy& strategy;
public:
	Context(CHYStrategy& strat) : strategy(strat) {}
	void greet() { strategy.greet(); }
};


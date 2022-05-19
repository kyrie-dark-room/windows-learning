#pragma once
#include <iostream>

using namespace std;

class CHYTemplateMethod
{
protected:
	virtual void customize1() = 0;
	virtual void customize2() = 0;
public:
	void templateMethod()
	{
		for (int i = 0; i < 5; i++)
		{
			customize1();
			customize2();
		}
	}
};

// Create a new "application"
class MyApp : public CHYTemplateMethod
{
protected:
	void customize1() { cout << "Hello "; }
	void customize2() { cout << "World!" << endl; }
};


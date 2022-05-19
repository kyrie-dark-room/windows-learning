#pragma once
#include <iostream>
using namespace std;

/*
* 代理模式的基本思想很简单：代理（Surrogate）类派生自一个基类，由平行地派生自同一个基类的一个或多个类提供实际的实现。
*/

class ProxyBase
{
public:
	virtual void f() = 0;
	virtual void g() = 0;
	virtual void h() = 0;
	virtual ~ProxyBase() {}
};

class Implementation : public ProxyBase
{
public:
	void f() { cout << "Implementation.f()" << endl; }
	void g() { cout << "Implementation.g()" << endl; }
	void h() { cout << "Implementation.h()" << endl; }
};

class Proxy : public ProxyBase
{
	ProxyBase* implementation;
public:
	Proxy() 
	{
		implementation = new Implementation;
	}

	~Proxy()
	{
		delete implementation;
	}

	// Forward calls to the implementation
	void f() { implementation->f(); }
	void g() { implementation->g(); }
	void h() { implementation->h(); }
};


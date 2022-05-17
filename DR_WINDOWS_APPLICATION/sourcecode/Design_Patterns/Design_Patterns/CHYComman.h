#pragma once
#include <iostream>
#include <vector>

using namespace std;

/*
* 1）命令（command）模式的结构很简单，但是对于消除代码间的耦合--清理代码--却有着重要的影响
* 2）从最直观的角度来看，命令模式就是一个函数对象：一个作为对象的函数。通过将函数封装为对象，就能够以参数的形式将其传递给其他函数或者对象，告诉我们在履行请求的过程中执行特定的操作。可以说，命令模式是携带
*   行为信息的信使。
*/

class CHYComman
{
public:
	virtual void execute() = 0;
};

class Hello : public CHYComman
{
	void execute() { cout << "Hello "; }
};

class World : public CHYComman
{
	void execute() { cout << "World! "; }
};

class IAm : public CHYComman
{
	void execute() { cout << "I'm the command pattern!"; }
};

class Macro
{
	vector<CHYComman*> m_vCommonds;

public:
	void add(CHYComman* c) { m_vCommonds.push_back(c); }
	void run()
	{
		vector<CHYComman*>::iterator it = m_vCommonds.begin();
		while (it != m_vCommonds.end())
		{
			(*it++)->execute();
		}
	}
};

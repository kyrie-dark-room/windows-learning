#pragma once
#include <iostream>
#include <string>

using namespace std;

/*
* 状态模式通过一个前端对象来使用后端实现对象履行其职责。然而，在前端对象生存期期间，状态模式从一个实现对象到另一个实现对象进行切换，以实现对于相同的函数调用产生不同的行为。
*/
class Creature
{
	class CHYState
	{
	public:
		virtual string response() = 0;
	};

	class Frog : public CHYState
	{
	public:
		string response() { return "Ribbet!"; }
	};

	class Prince : public CHYState
	{
	public:
		string response() { return "Darling!"; }
	};

	CHYState* state;
public:
	Creature() : state(new Frog()) {}
	void greet()
	{
		cout << state->response() << endl;
	}

	void kiss()
	{
		delete state;
		state = new Prince();
	}
};



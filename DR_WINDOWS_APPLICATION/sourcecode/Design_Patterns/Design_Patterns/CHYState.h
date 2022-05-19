#pragma once
#include <iostream>
#include <string>

using namespace std;

/*
* ״̬ģʽͨ��һ��ǰ�˶�����ʹ�ú��ʵ�ֶ���������ְ��Ȼ������ǰ�˶����������ڼ䣬״̬ģʽ��һ��ʵ�ֶ�����һ��ʵ�ֶ�������л�����ʵ�ֶ�����ͬ�ĺ������ò�����ͬ����Ϊ��
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



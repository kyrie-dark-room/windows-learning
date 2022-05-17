#pragma once
#include <iostream>
#include <vector>

using namespace std;

/*
* 1�����command��ģʽ�Ľṹ�ܼ򵥣����Ƕ����������������--�������--ȴ������Ҫ��Ӱ��
* 2������ֱ�۵ĽǶ�����������ģʽ����һ����������һ����Ϊ����ĺ�����ͨ����������װΪ���󣬾��ܹ��Բ�������ʽ���䴫�ݸ������������߶��󣬸�����������������Ĺ�����ִ���ض��Ĳ���������˵������ģʽ��Я��
*   ��Ϊ��Ϣ����ʹ��
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

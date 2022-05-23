#pragma once
#include <iostream>
#include <vector>

using namespace std;

enum Answer{NO, YES};

class CHYChainOfReponsibility
{
public:
	virtual Answer canIHave() = 0;
	virtual ~CHYChainOfReponsibility() {}
};

class AskMom : public CHYChainOfReponsibility
{
public:
	Answer canIHave()
	{
		cout << "Mooom? Can I have this?" << endl;
		return NO;
	}
};

class AskDad : public CHYChainOfReponsibility
{
public:
	Answer canIHave()
	{
		cout << "Dad? I really need this!" << endl;
		return NO;
	}
};

class AskGrandpa : public CHYChainOfReponsibility
{
public:
	Answer canIHave()
	{
		cout << "Grandpa, is it my birthday yet?" << endl;
		return NO;
	}
};

class AskGrandma : public CHYChainOfReponsibility
{
public:
	Answer canIHave()
	{
		cout << "Grandma, I really love you!" << endl;
		return YES;
	}
};

class Gimme : CHYChainOfReponsibility
{
	vector<CHYChainOfReponsibility*> chain;
public:
	Gimme()
	{
		chain.push_back(new AskMom());
		chain.push_back(new AskDad());
		chain.push_back(new AskGrandpa());
		chain.push_back(new AskGrandma());
	}

	Answer canIHave()
	{
		vector<CHYChainOfReponsibility*>::iterator it = chain.begin();
		while (it != chain.end())
		{
			if ((*it++)->canIHave() == YES)
				return YES;
		}
		cout << "Whiiiiinnne!" << endl;
		return NO;
	}

	~Gimme() { /*purge(chain);*/ }
};


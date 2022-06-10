#pragma once

/*
* �۲���ģʽ��Observer��ģʽ���ڽ��һ���൱���������⣺��ĳЩ��������ı�״̬ʱ�����һ�������Ҫ������Ӧ�ĸ��£���ôӦ����δ����أ�
* �ڹ۲���ģʽ�����������仯���¼��������ڽ��й۲�Ķ���������͸��·����ķ�ʽ�������˵���۲���ģʽ�����޸�����߶���Ӱ����Χ���������롣
* �����úܶ෽����ʵ�ֹ۲���ģʽ������Ĵ��뽫����һ�������ܣ����߿ɸ��������ܹ����Լ��Ĺ۲���ģʽ���롣
*/

#include <set>

class Observable;
class Argument {};

class Observer
{
public:
	// Called by the observed object, whenever
	// the observed object is changed:
	virtual void update(Observable* o, Argument* arg) = 0;
	virtual ~Observer() {}
};

class Observable {
	bool changed;
	std::set<Observer*> observers;
protected:
	virtual void setChanged() { changed = true; }
	virtual void clearChanged() { changed = false; }
public:
	virtual void addObserver(Observer& o) {
		observers.insert(&o);
	}

	virtual void deleteObserver(Observer& o) {
		observers.erase(&o);
	}

	virtual void deleteObserver() {
		observers.clear();
	}

	virtual int countObservers() {
		return (int)observers.size();
	}

	virtual bool hasChanged() {
		return changed;
	}

	// If this object has changed, notify all of its observers
	virtual void notifyObservers(Argument* arg = 0) {
		if (!hasChanged())
			return;
		clearChanged();
		std::set<Observer*>::iterator it;
		for (it = observers.begin(); it != observers.end(); it++)
		{
			(*it)->update(this, arg);
		}
	}
	
	virtual ~Observable() {}
};


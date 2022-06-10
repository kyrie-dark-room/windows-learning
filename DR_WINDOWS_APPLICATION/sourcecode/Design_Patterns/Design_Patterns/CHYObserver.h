#pragma once

/*
* 观察者模式（Observer）模式用于解决一个相当常见的问题：当某些其他对象改变状态时，如果一组对象需要进行相应的更新，那么应该如何处理呢？
* 在观察者模式中有两个“变化的事件”：正在进行观察的对象的数量和更新发生的方式。这就是说，观察者模式允许修改这二者而不影响周围的其他代码。
* 可以用很多方法来实现观察者模式，下面的代码将创建一个程序框架，读者可根据这个框架构建自己的观察者模式代码。
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


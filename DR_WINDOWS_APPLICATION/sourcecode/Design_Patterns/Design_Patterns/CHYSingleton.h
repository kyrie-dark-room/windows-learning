#pragma once
#include "CHYThreadLocker.h"

/*
* 单件（Singleton）是最简单的设计模式，它是允许一个类有且仅有一个实例的方法。
*/

// https://blog.csdn.net/wrt_345177767/article/details/113179299
// 方式一（传统实现方式：类本身照例实现）
class Singleton
{
	Singleton() {}
	Singleton(const Singleton&) {}
	Singleton& operator=(const Singleton&) {}
protected:
	virtual ~Singleton() {}

public:
	static Singleton* Instantialize();

private:
	static Singleton* m_pInstance;
	static CRITICAL_SECTION m_Section;
};

Singleton* Singleton::m_pInstance = 0;
CRITICAL_SECTION Singleton::m_Section;

// 方式二（类模板：单例接口）
template <typename T>
class CHYSingleton
{
public:
	static T* GetPointer()
	{
		if (m_pInstance == NULL)
		{
			m_ThreadLocker.Lock();
			if (m_pInstance == NULL)
			{
				T* pInstance = new T();
				m_pInstance = reinterpret_cast<void*>(pInstance);
			}

			m_ThreadLocker.UnLock();
		}

		return reinterpret_cast<T*>(m_pInstance);
	}

	static T& GetInstance()
	{
		return *GetPointer();
	}

	static void ReleaseInstance()
	{
		m_ThreadLocker.Lock();
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}

		m_ThreadLocker.UnLock();
	}

private:
	static void* m_pInstance;
	static CHYThreadLocker m_ThreadLocker;
};

template<typename T>
void* CHYSingleton<T>::m_pInstance = 0;

template<typename T>
CHYThreadLocker CHYSingleton<T>::m_ThreadLocker;

// C++ 编程思想
// 方式三（类模板：通过继承实现）
template<class T>
class CSingleton
{
	CSingleton(const CSingleton&);
	CSingleton& operator=(const CSingleton&);

protected:
	CSingleton() {}
	virtual ~CSingleton() {}

public:
	static T& GetInstance()
	{
		static T theInstance;
		return theInstance;
	}
};

class MyClass : public CSingleton<MyClass>
{
	int x;
protected:
	friend class CSingleton<MyClass>;
	MyClass() { x = 0; }

public:
	void SetValue(int n) { x = n; }
	int GetValue() const { return x; }
};

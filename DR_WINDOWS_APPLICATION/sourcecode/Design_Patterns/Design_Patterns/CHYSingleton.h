#pragma once
#include "CHYThreadLocker.h"

/*
* ������Singleton������򵥵����ģʽ����������һ�������ҽ���һ��ʵ���ķ�����
*/

// https://blog.csdn.net/wrt_345177767/article/details/113179299
// ��ʽһ����ͳʵ�ַ�ʽ���౾������ʵ�֣�
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

// ��ʽ������ģ�壺�����ӿڣ�
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

// C++ ���˼��
// ��ʽ������ģ�壺ͨ���̳�ʵ�֣�
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

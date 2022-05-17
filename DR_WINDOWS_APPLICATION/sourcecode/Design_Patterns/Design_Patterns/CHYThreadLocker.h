#pragma once
#include <Windows.h>

class CHYThreadLocker
{
public:
	CHYThreadLocker();
	virtual ~CHYThreadLocker();

	void Lock(void);
	void UnLock(void);

private:
	CRITICAL_SECTION m_Section;
};

class CHYThreadLockerEx
{
public:
	CHYThreadLockerEx(CRITICAL_SECTION cs) : m_Section(cs)
	{
		::EnterCriticalSection(&m_Section);
	}

	~CHYThreadLockerEx()
	{
		::LeaveCriticalSection(&m_Section);
	}

private:
	CRITICAL_SECTION m_Section;
};

#pragma once
/*
* 知识：
* 1）对EnterCriticalSection的调用最终会超时并引发异常。导致超时的时间长度由下面这个注册表子项中包含的CriticalSectionTimeout值决定。
*		HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager
*    这个值以秒为单位，它的默认值为2592000秒，也就是大约30天。
*/

#include <Windows.h>

namespace HY_KERNELOBJECT
{
	// 基于关键段的线程锁类
	class CHYThreadMutex
	{
	public:
		CHYThreadMutex();
		virtual ~CHYThreadMutex();

		void Lock();
		BOOL TryLock();
		void Unlock();

	private:
		CRITICAL_SECTION m_csThreadLocker;
	};

	// 基于带自旋锁关键段的线程锁类
	class CHYThreadMutexEx
	{
	public:
		CHYThreadMutexEx();
		virtual ~CHYThreadMutexEx();

		void Lock();
		BOOL TryLock();
		void Unlock();
		DWORD SetSpinCount(DWORD dwSpinCount);

	private:
		CRITICAL_SECTION m_csThreadLocker;
		DWORD m_dwSpinCount;
	};

	// 基于读写锁的线程锁
	class CHYThreadSRWLock
	{
	public:
		CHYThreadSRWLock();
		virtual ~CHYThreadSRWLock();
		void RLock();
		void RUnLock();
		void WLock();
		void WUnlock();
	private:
		SRWLOCK m_sRWLock;
	};
}



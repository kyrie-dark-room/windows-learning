#pragma once
/*
* ֪ʶ��
* 1����EnterCriticalSection�ĵ������ջᳬʱ�������쳣�����³�ʱ��ʱ�䳤�����������ע��������а�����CriticalSectionTimeoutֵ������
*		HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager
*    ���ֵ����Ϊ��λ������Ĭ��ֵΪ2592000�룬Ҳ���Ǵ�Լ30�졣
*/

#include <Windows.h>

namespace HY_KERNELOBJECT
{
	// ���ڹؼ��ε��߳�����
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

	// ���ڴ��������ؼ��ε��߳�����
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

	// ���ڶ�д�����߳���
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



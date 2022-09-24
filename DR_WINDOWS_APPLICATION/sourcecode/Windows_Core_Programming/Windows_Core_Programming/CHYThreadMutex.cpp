#include "CHYThreadMutex.h"

namespace HY_KERNELOBJECT
{
	CHYThreadMutex::CHYThreadMutex()
	{
		::InitializeCriticalSection(&m_csThreadLocker);
	}

	CHYThreadMutex::~CHYThreadMutex()
	{
		::DeleteCriticalSection(&m_csThreadLocker);
	}

	void CHYThreadMutex::Lock()
	{
		::EnterCriticalSection(&m_csThreadLocker);
	}

	void CHYThreadMutex::Unlock()
	{
		::LeaveCriticalSection(&m_csThreadLocker);
	}

	BOOL CHYThreadMutex::TryLock()
	{
		return ::TryEnterCriticalSection(&m_csThreadLocker);
	}

	CHYThreadMutexEx::CHYThreadMutexEx():
		m_dwSpinCount(4000)
	{
		::InitializeCriticalSectionAndSpinCount(&m_csThreadLocker, m_dwSpinCount);
	}

	CHYThreadMutexEx::~CHYThreadMutexEx()
	{
		::DeleteCriticalSection(&m_csThreadLocker);
	}

	void CHYThreadMutexEx::Lock()
	{
		::EnterCriticalSection(&m_csThreadLocker);
	}

	void CHYThreadMutexEx::Unlock()
	{
		::LeaveCriticalSection(&m_csThreadLocker);
	}

	BOOL CHYThreadMutexEx::TryLock()
	{
		return ::TryEnterCriticalSection(&m_csThreadLocker);
	}

	DWORD CHYThreadMutexEx::SetSpinCount(DWORD dwSpinCount)
	{
		m_dwSpinCount = dwSpinCount;
		return ::SetCriticalSectionSpinCount(&m_csThreadLocker, m_dwSpinCount);
	}

	CHYThreadSRWLock::CHYThreadSRWLock()
	{
		::InitializeSRWLock(&m_sRWLock);
	}

	CHYThreadSRWLock::~CHYThreadSRWLock()
	{

	}

	void CHYThreadSRWLock::RLock()
	{
		::AcquireSRWLockShared(&m_sRWLock);
	}

	void CHYThreadSRWLock::RUnLock()
	{
		::ReleaseSRWLockShared(&m_sRWLock);
	}

	void CHYThreadSRWLock::WLock()
	{
		::AcquireSRWLockExclusive(&m_sRWLock);
	}

	void CHYThreadSRWLock::WUnlock()
	{
		::ReleaseSRWLockExclusive(&m_sRWLock);
	}
}
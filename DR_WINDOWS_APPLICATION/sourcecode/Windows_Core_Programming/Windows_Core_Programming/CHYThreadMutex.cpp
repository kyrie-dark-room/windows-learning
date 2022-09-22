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
}
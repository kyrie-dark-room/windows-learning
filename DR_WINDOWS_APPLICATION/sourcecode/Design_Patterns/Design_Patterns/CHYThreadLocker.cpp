#include "CHYThreadLocker.h"

CHYThreadLocker::CHYThreadLocker()
{
	::InitializeCriticalSection(&m_Section);
}

CHYThreadLocker::~CHYThreadLocker()
{
	::DeleteCriticalSection(&m_Section);
}

void CHYThreadLocker::Lock()
{
	::EnterCriticalSection(&m_Section);
}

void CHYThreadLocker::UnLock()
{
	::LeaveCriticalSection(&m_Section);
}
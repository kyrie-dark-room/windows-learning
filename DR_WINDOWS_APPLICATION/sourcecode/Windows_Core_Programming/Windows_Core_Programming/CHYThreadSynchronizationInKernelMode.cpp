#include "CHYThreadSynchronizationInKernelMode.h"

namespace HY_KERNELOBJECT
{
	CHYThreadSynchronizationInKernelMode::CHYThreadSynchronizationInKernelMode()
	{

	}

	CHYThreadSynchronizationInKernelMode::~CHYThreadSynchronizationInKernelMode()
	{

	}

	DWORD CHYThreadSynchronizationInKernelMode::WaitForSingleObject(HANDLE hObject, DWORD dwMilliseconds)
	{
		return ::WaitForSingleObject(hObject, dwMilliseconds);
	}

	DWORD CHYThreadSynchronizationInKernelMode::WaitForMultipleObjects(DWORD dwCount, CONST HANDLE* phObjects, BOOL bWaitAll, DWORD dwMilliseconds)
	{
		return ::WaitForMultipleObjects(dwCount, phObjects, bWaitAll, dwMilliseconds);
	}

	HANDLE CHYThreadSynchronizationInKernelMode::CreateEvent(PSECURITY_ATTRIBUTES psa, BOOL bManualReset, BOOL bInitialState, PCTSTR pszName)
	{
		return ::CreateEvent(psa, bManualReset, bInitialState, pszName);
	}

	HANDLE CHYThreadSynchronizationInKernelMode::CreateEventEx(PSECURITY_ATTRIBUTES psa, PCTSTR pszName, DWORD dwFlags, DWORD dwDesiredAccess)
	{
		return ::CreateEventEx(psa, pszName, dwFlags, dwDesiredAccess);
	}

	HANDLE CHYThreadSynchronizationInKernelMode::OpenEvent(DWORD dwDesiredAccess, BOOL bInherit, PCTSTR pszName)
	{
		return ::OpenEvent(dwDesiredAccess, bInherit, pszName);
	}

	BOOL CHYThreadSynchronizationInKernelMode::CloseHandle(HANDLE hEventObject)
	{
		return ::CloseHandle(hEventObject);
	}

	BOOL CHYThreadSynchronizationInKernelMode::SetEvent(HANDLE hEvent)
	{
		return ::SetEvent(hEvent);
	}

	BOOL CHYThreadSynchronizationInKernelMode::ResetEvent(HANDLE hEvent)
	{
		return ::ResetEvent(hEvent);
	}

	BOOL CHYThreadSynchronizationInKernelMode::PulseEvent(HANDLE hEvent)
	{
		return ::PulseEvent(hEvent);
	}

	HANDLE CHYThreadSynchronizationInKernelMode::CreateWaitableTimer(PSECURITY_ATTRIBUTES psa, BOOL bManualReset, PCTSTR pszName)
	{
		return ::CreateWaitableTimer(psa, bManualReset, pszName);
	}

	HANDLE CHYThreadSynchronizationInKernelMode::OpenWaitableTimer(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName)
	{
		return ::OpenWaitableTimer(dwDesiredAccess, bInheritHandle, pszName);
	}

	BOOL CHYThreadSynchronizationInKernelMode::SetWaitableTimer(HANDLE hTimer, const LARGE_INTEGER* pDueTime, LONG lPeriod, PTIMERAPCROUTINE pfnCompletionRoutine, PVOID pvArgToCompletionRoutine, BOOL bResume)
	{
		return ::SetWaitableTimer(hTimer, pDueTime, lPeriod, pfnCompletionRoutine, pvArgToCompletionRoutine, bResume);
	}

	BOOL CHYThreadSynchronizationInKernelMode::CancelWaitableTimer(HANDLE hTimer)
	{
		return ::CancelWaitableTimer(hTimer);
	}

	HANDLE CHYThreadSynchronizationInKernelMode::CreateSemaphore(PSECURITY_ATTRIBUTES psa, LONG lInitialCount, LONG lMaximumCount, PCTSTR pszName)
	{
		return ::CreateSemaphore(psa, lInitialCount, lMaximumCount, pszName);
	}

	HANDLE CHYThreadSynchronizationInKernelMode::CreateSemaphoreEx(PSECURITY_ATTRIBUTES psa, LONG lInitialCount, LONG lMaximumCount, PCTSTR pszName, DWORD dwFlags, DWORD dwDesiredAccess)
	{
		if (0 != dwFlags)
		{
			dwFlags = 0;
		}

		return ::CreateSemaphoreEx(psa, lInitialCount, lMaximumCount, pszName, dwFlags, dwDesiredAccess);
	}

	HANDLE CHYThreadSynchronizationInKernelMode::OpenSemaphore(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName)
	{
		return ::OpenSemaphore(dwDesiredAccess, bInheritHandle, pszName);
	}

	BOOL CHYThreadSynchronizationInKernelMode::ReleaseSemaphore(HANDLE hSemaphore, LONG lReleaseCount, PLONG plPreviousCount)
	{
		return ::ReleaseSemaphore(hSemaphore, lReleaseCount, plPreviousCount);
	}

	HANDLE CHYThreadSynchronizationInKernelMode::CreateMutex(PSECURITY_ATTRIBUTES psa, BOOL bInitialOwner, PCTSTR pszName)
	{
		return ::CreateMutex(psa, bInitialOwner, pszName);
	}

	HANDLE CHYThreadSynchronizationInKernelMode::CreateMutexEx(PSECURITY_ATTRIBUTES psa, PCTSTR pszName, DWORD dwFlags, DWORD dwDesiredAccess)
	{
		return ::CreateMutexEx(psa, pszName, dwFlags, dwDesiredAccess);
	}

	HANDLE CHYThreadSynchronizationInKernelMode::OpenMutex(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName)
	{
		return ::OpenMutex(dwDesiredAccess, bInheritHandle, pszName);
	}

	BOOL CHYThreadSynchronizationInKernelMode::ReleaseMutex(HANDLE hMutex)
	{
		return ::ReleaseMutex(hMutex);
	}

	DWORD CHYThreadSynchronizationInKernelMode::WaitForInputIdle(HANDLE hProcess, DWORD dwMilliseconds)
	{
		return ::WaitForInputIdle(hProcess, dwMilliseconds);
	}

	DWORD CHYThreadSynchronizationInKernelMode::MsgWaitForMultipleObjects(DWORD dwCount, PHANDLE phObjects, BOOL bWaitAll, DWORD dwMilliseconds, DWORD dwWakeMask)
	{
		return ::MsgWaitForMultipleObjects(dwCount, phObjects, bWaitAll, dwMilliseconds, dwWakeMask);
	}

	DWORD CHYThreadSynchronizationInKernelMode::MsgWaitForMultipleObjectsEx(DWORD dwCount, PHANDLE phObjects, DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags)
	{
		return ::MsgWaitForMultipleObjectsEx(dwCount, phObjects, dwMilliseconds, dwWakeMask, dwFlags);
	}

	BOOL CHYThreadSynchronizationInKernelMode::WaitForDebugEvent(LPDEBUG_EVENT pde, DWORD dwMilliseconds)
	{
		return ::WaitForDebugEvent(pde, dwMilliseconds);
	}

	DWORD CHYThreadSynchronizationInKernelMode::SignalObjectAndWait(HANDLE hObjectToSignal, HANDLE hObjectToWaitOn, DWORD dwMilliseconds, BOOL bAlertable)
	{
		return ::SignalObjectAndWait(hObjectToSignal, hObjectToWaitOn, dwMilliseconds, bAlertable);
	}
}
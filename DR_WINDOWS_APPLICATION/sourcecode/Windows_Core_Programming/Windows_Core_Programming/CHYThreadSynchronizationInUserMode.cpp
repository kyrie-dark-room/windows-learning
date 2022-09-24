#include "CHYThreadSynchronizationInUserMode.h"

namespace HY_KERNELOBJECT
{
	CHYThreadSynchronizationInUserMode::CHYThreadSynchronizationInUserMode()
	{

	}

	CHYThreadSynchronizationInUserMode::~CHYThreadSynchronizationInUserMode()
	{

	}

	LONG CHYThreadSynchronizationInUserMode::InterlockedIncrement(LONG volatile* Addend)
	{
		return ::InterlockedIncrement(Addend);
	}

	LONG CHYThreadSynchronizationInUserMode::InterlockedDecrement(LONG volatile* plAddend)
	{
		return ::InterlockedDecrement(plAddend);
	}

	LONG CHYThreadSynchronizationInUserMode::InterlockedExchangeAdd(PLONG volatile plAddend, LONG lIncrement)
	{
		return ::InterlockedExchangeAdd(plAddend, lIncrement);
	}

	LONGLONG CHYThreadSynchronizationInUserMode::InterlockedExchangeAdd64(PLONGLONG volatile pllAddend, LONGLONG llIncrement)
	{
		return ::InterlockedExchangeAdd64(pllAddend, llIncrement);
	}

	void* CHYThreadSynchronizationInUserMode::_aligned_malloc(size_t size, size_t alignment)
	{
		return ::_aligned_malloc(size, alignment);
	}

	LONG CHYThreadSynchronizationInUserMode::InterlockedExchange(PLONG volatile plTarget, LONG lValue)
	{
		return ::InterlockedExchange(plTarget, lValue);
	}

	LONGLONG CHYThreadSynchronizationInUserMode::InterlockedExchange64(PLONGLONG volatile pllTarget, LONGLONG llValue)
	{
		return ::InterlockedExchange64(pllTarget, llValue);
	}

	PVOID CHYThreadSynchronizationInUserMode::InterlockedExchangePointer(PVOID* volatile ppvTarget, PVOID pvValue)
	{
		return ::InterlockedExchangePointer(ppvTarget, pvValue);
	}

	LONG CHYThreadSynchronizationInUserMode::InterlockedCompareExchange(LONG volatile* plDestination, LONG lExchage, LONG lCompared)
	{
		return ::InterlockedCompareExchange(plDestination, lExchage, lCompared);
	}

	PVOID CHYThreadSynchronizationInUserMode::InterlockedCompareExchangePointer(PVOID* ppvDestination, PVOID pvExchage, PVOID pvComparand)
	{
		return ::InterlockedCompareExchangePointer(ppvDestination, pvExchage, pvComparand);
	}

	LONGLONG CHYThreadSynchronizationInUserMode::InterlockedCompareExchange64(PLONGLONG pllDestination, LONGLONG llExchage, LONGLONG llCompared)
	{
		return ::InterlockedCompareExchange64(pllDestination, llExchage, llCompared);
	}

	void CHYThreadSynchronizationInUserMode::InitializeSListHead(PSLIST_HEADER ListHead)
	{
		return ::InitializeSListHead(ListHead);
	}

	PSLIST_ENTRY CHYThreadSynchronizationInUserMode::InterlockedPushEntrySList(PSLIST_HEADER ListHead, __drv_aliasesMem PSLIST_ENTRY ListEntry)
	{
		return ::InterlockedPushEntrySList(ListHead, ListEntry);
	}

	PSLIST_ENTRY CHYThreadSynchronizationInUserMode::InterlockedPopEntrySList(PSLIST_HEADER ListHead)
	{
		return ::InterlockedPopEntrySList(ListHead);
	}

	PSLIST_ENTRY CHYThreadSynchronizationInUserMode::InterlockedFlushSList(PSLIST_HEADER ListHead)
	{
		return ::InterlockedFlushSList(ListHead);
	}

	USHORT CHYThreadSynchronizationInUserMode::QueryDepthSList(PSLIST_HEADER ListHead)
	{
		return ::QueryDepthSList(ListHead);
	}

	typedef BOOL(WINAPI* LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
	WORD CHYThreadSynchronizationInUserMode::GetHighCacheLine()
	{
		WORD iHighCacheLine = 0;
		LPFN_GLPI glpi;
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
		DWORD returnLength = 0;

		HMODULE hHandle = GetModuleHandle(TEXT("kernel32"));
		if (NULL == hHandle)
		{
			return (0);
		}

		glpi = (LPFN_GLPI)GetProcAddress(hHandle, "GetLogicalProcessorInformation");
		if (NULL == glpi)
		{
			return (0);
		}

		BOOL done = FALSE;
		while (!done)
		{
			DWORD rc = glpi(buffer, &returnLength);
			if (FALSE == rc)
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					if (buffer)
						free(buffer);

					buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);
					if (NULL == buffer)
					{
						return (0);
					}
					else
					{
						iHighCacheLine = buffer->Cache.LineSize;
						free(buffer);
					}
				}
				else
				{
					return (0);
				}
			}
			else
			{
				done = TRUE;
			}
		}

		return iHighCacheLine;
	}

	BOOL CHYThreadSynchronizationInUserMode::SleepConditionVariableCS(PCONDITION_VARIABLE pConditionVariable, PCRITICAL_SECTION pCriticalSection, DWORD dwMilliseconds)
	{
		return ::SleepConditionVariableCS(pConditionVariable, pCriticalSection, dwMilliseconds);
	}

	BOOL CHYThreadSynchronizationInUserMode::SleepConditionVariableSRW(PCONDITION_VARIABLE pConditionVariable, PSRWLOCK pSRWLock, DWORD dwMilliseconds, ULONG Flags)
	{
		return ::SleepConditionVariableSRW(pConditionVariable, pSRWLock, dwMilliseconds, Flags);
	}

	VOID CHYThreadSynchronizationInUserMode::WakeConditionVariable(PCONDITION_VARIABLE ConditionVariable)
	{
		return ::WakeConditionVariable(ConditionVariable);
	}

	VOID CHYThreadSynchronizationInUserMode::WakeAllConditionVariable(PCONDITION_VARIABLE ConditionVariable)
	{
		return ::WakeAllConditionVariable(ConditionVariable);
	}
}
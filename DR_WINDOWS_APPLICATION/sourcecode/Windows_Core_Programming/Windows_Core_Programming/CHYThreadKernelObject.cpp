#include "CHYThreadKernelObject.h"
#include <tlhelp32.h>

namespace HY_KERNELOBJECT
{
	CHYThreadKernelObject::CHYThreadKernelObject()
	{

	}

	CHYThreadKernelObject::~CHYThreadKernelObject()
	{

	}

	HANDLE CHYThreadKernelObject::CreateThread(
		LPSECURITY_ATTRIBUTES   lpThreadAttributes,
		SIZE_T                  dwStackSize,
		LPTHREAD_START_ROUTINE  lpStartAddress,
		__drv_aliasesMem LPVOID lpParameter,
		DWORD                   dwCreationFlags,
		LPDWORD                 lpThreadId
	)
	{
		return ::CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
	}

	uintptr_t CHYThreadKernelObject::_beginthread( // NATIVE CODE
		void(__cdecl* start_address)(void*),
		unsigned stack_size,
		void* arglist
	)
	{
		return _beginthread(start_address, stack_size, arglist);
	}
	uintptr_t CHYThreadKernelObject::_beginthreadex( // NATIVE CODE
		void* security,
		unsigned stack_size,
		unsigned(__stdcall* start_address)(void*),
		void* arglist,
		unsigned initflag,
		unsigned* thrdaddr
	)
	{
		return _beginthreadex(security, stack_size, start_address, arglist, initflag, thrdaddr);
	}

	void CHYThreadKernelObject::_endthread(void)
	{
		return _endthread();
	}
	void CHYThreadKernelObject::_endthreadex(unsigned retval)
	{
		return _endthreadex(retval);
	}

	VOID CHYThreadKernelObject::ExitThread(DWORD dwExitCode)
	{
		return ::ExitThread(dwExitCode);
	}

	BOOL CHYThreadKernelObject::TerminateThread(HANDLE hThread, DWORD dwExitCode)
	{
		return ::TerminateThread(hThread, dwExitCode);
	}

	BOOL CHYThreadKernelObject::GetExitCodeThread(HANDLE hThread, PDWORD pdwExitCode)
	{
		return ::GetExitCodeThread(hThread, pdwExitCode);
	}

	HANDLE CHYThreadKernelObject::GetCurrentProcess()
	{
		return ::GetCurrentProcess();
	}

	HANDLE CHYThreadKernelObject::GetCurrentThread()
	{
		return ::GetCurrentThread();
	}

	BOOL CHYThreadKernelObject::GetProcessTimes(
		HANDLE     hProcess,
		LPFILETIME lpCreationTime,
		LPFILETIME lpExitTime,
		LPFILETIME lpKernelTime,
		LPFILETIME lpUserTime
	)
	{
		return ::GetProcessTimes(hProcess, lpCreationTime, lpExitTime, lpKernelTime, lpUserTime);
	}

	BOOL CHYThreadKernelObject::GetThreadTimes(
		HANDLE     hThread,
		LPFILETIME lpCreationTime,
		LPFILETIME lpExitTime,
		LPFILETIME lpKernelTime,
		LPFILETIME lpUserTime
	)
	{
		return ::GetThreadTimes(hThread, lpCreationTime, lpExitTime, lpKernelTime, lpUserTime);
	}

	BOOL CHYThreadKernelObject::DuplicateHandle(
		HANDLE hSourceProcess,
		HANDLE hSource,
		HANDLE hTargetProcess,
		PHANDLE phTarget,
		DWORD dwDesiredAccess,
		BOOL bInheritHandle,
		DWORD dwOptions
	)
	{
		return ::DuplicateHandle(hSourceProcess, hSource, hTargetProcess, phTarget, dwDesiredAccess, bInheritHandle, dwOptions);
	}

	BOOL CHYThreadKernelObject::GetSystemTimeAdjustment(
		PDWORD lpTimeAdjustment,
		PDWORD lpTimeIncrement,   // ´óÔ¼Ã¿¸ô20ms
		PBOOL  lpTimeAdjustmentDisabled
	)
	{
		return GetSystemTimeAdjustment(lpTimeAdjustment, lpTimeIncrement, lpTimeAdjustmentDisabled);
	}

	DWORD CHYThreadKernelObject::SuspendThread(HANDLE hThread)
	{
		return ::SuspendThread(hThread);
	}

	DWORD CHYThreadKernelObject::ResumeThread(HANDLE hThread)
	{
		return ::ResumeThread(hThread);
	}

	VOID CHYThreadKernelObject::SuspendProcess(DWORD dwProcessID, BOOL fSuspend)
	{
		// Get the list of threads in the system
		HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);
		if (hSnapshot != INVALID_HANDLE_VALUE)
		{
			// Walk the list of threads.
			THREADENTRY32 te = { sizeof(te) };
			BOOL fOk = Thread32First(hSnapshot, &te);
			for (; fOk; fOk = Thread32Next(hSnapshot, &te))
			{
				// Is this thread in the desired process?
				if (te.th32OwnerProcessID == dwProcessID)
				{
					//Attempt to convert the thread ID into handle/
					HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
					if (hThread != NULL)
					{
						// Suspend or resume the thread
						if (fSuspend)
						{
							::SuspendThread(hThread);
						}
						else
						{
							::ResumeThread(hThread);
						}
						CloseHandle(hThread);
					}
				}
			}

			CloseHandle(hSnapshot);
		}
	}

	VOID CHYThreadKernelObject::Sleep(DWORD dwMilliseconds)
	{
		return ::Sleep(dwMilliseconds);
	}

	BOOL CHYThreadKernelObject::SwitchToThread()
	{
		return ::SwitchToThread();
	}

	ULONGLONG CHYThreadKernelObject::GetTickCount64()
	{
		return ::GetTickCount64();
	}

	__int64 CHYThreadKernelObject::FileTimeToQuadWord(PFILETIME pft)
	{
		return (Int64ShllMod32(pft->dwHighDateTime, 32) | pft->dwLowDateTime);
	}

	BOOL CHYThreadKernelObject::QueryThreadCycleTime(HANDLE ThreadHandle, PULONG64 CycleTime)
	{
		return ::QueryThreadCycleTime(ThreadHandle, CycleTime);
	}

	BOOL CHYThreadKernelObject::QueryProcessCycleTime(HANDLE ProcessHandle, PULONG64 CycleTime)
	{
		return ::QueryProcessCycleTime(ProcessHandle, CycleTime);
	}

	BOOL CHYThreadKernelObject::QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency)
	{
		return ::QueryPerformanceFrequency(lpFrequency);
	}

	BOOL CHYThreadKernelObject::QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount)
	{
		return ::QueryPerformanceFrequency(lpPerformanceCount);
	}

	DWORD CHYThreadKernelObject::GetCPUFrequencyInMHz()
	{
		// change the priority to ensure the thread will have more chances
		// to be scheduled when Sleep() ends
		int currentPriority = GetThreadPriority(GetCurrentThread());
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

		// keep track of the elapsed time with the other timer
		__int64 elapsedTime = 0;

		// Create a stopwatch timer 
		CStopwatch stopwatch;
		__int64 perfCountStart = stopwatch.NowInMicro();

		// get the current number of cycles
		unsigned __int64 cyclesOnStart = ReadTimeStampCounter();

		// wait for ~1 second
		Sleep(1000);

		// get the number of cycles after ~1 second
		unsigned __int64 numberofCycles = ReadTimeStampCounter() - cyclesOnStart;

		// Get how much time ha selapsed with greater precision
		elapsedTime = stopwatch.NowInMicro() - perfCountStart;

		// Restore the thread priority
		SetThreadPriority(GetCurrentThread(), currentPriority);

		// Compute the frequency in MHz
		DWORD dwCPUFrequency = (DWORD)(numberofCycles / elapsedTime);
		return (dwCPUFrequency);
	}

	BOOL CHYThreadKernelObject::GetThreadContext(HANDLE hThread, PCONTEXT pContext)
	{
		return ::GetThreadContext(hThread, pContext);
	}

	BOOL CHYThreadKernelObject::SetThreadContext(HANDLE hThread, CONST CONTEXT* pContext)
	{
		return ::SetThreadContext(hThread, pContext);
	}

	BOOL CHYThreadKernelObject::SetThreadPriority(HANDLE hThread, int nPriority)
	{
		return ::SetThreadPriority(hThread, nPriority);
	}

	int CHYThreadKernelObject::GetThreadPriority(HANDLE hThread)
	{
		return ::GetThreadPriority(hThread);
	}

	BOOL CHYThreadKernelObject::SetProcessPriorityBoost(HANDLE hProcess, BOOL bDisablePriorityBoost)
	{
		return ::SetProcessPriorityBoost(hProcess, bDisablePriorityBoost);
	}

	BOOL CHYThreadKernelObject::SetThreadPriorityBoost(HANDLE hThread, BOOL bDisablePriorityBoost)
	{
		return ::SetThreadPriorityBoost(hThread, bDisablePriorityBoost);
	}

	BOOL CHYThreadKernelObject::GetProcessPriorityBoost(HANDLE hProcess, PBOOL pbDisablePriorityBoost)
	{
		return ::GetProcessPriorityBoost(hProcess, pbDisablePriorityBoost);
	}

	BOOL CHYThreadKernelObject::GetThreadPriorityBoost(HANDLE hThread, PBOOL pbDisablePriorityBoost)
	{
		return ::GetThreadPriorityBoost(hThread, pbDisablePriorityBoost);
	}

	BOOL CHYThreadKernelObject::SetFileInformationByHandle(
		HANDLE                    hFile,
		FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
		LPVOID                    lpFileInformation,
		DWORD                     dwBufferSize
	)
	{
		return ::SetFileInformationByHandle(hFile, FileInformationClass, lpFileInformation, dwBufferSize);
	}

	void CHYThreadKernelObject::GetSystemInfo(LPSYSTEM_INFO lpSystemInfo)
	{
		return ::GetSystemInfo(lpSystemInfo);
	}

	BOOL CHYThreadKernelObject::SetProcessAffinityMask(HANDLE hProcess, DWORD_PTR dwProcessAffinityMask)
	{
		return SetProcessAffinityMask(hProcess, dwProcessAffinityMask);
	}

	BOOL CHYThreadKernelObject::GetProcessAffinityMask(HANDLE hProcess, PDWORD_PTR pdwProcessAffinityMask, PDWORD_PTR pdwSystemAffinityMask)
	{
		return ::GetProcessAffinityMask(hProcess, pdwProcessAffinityMask, pdwSystemAffinityMask);
	}

	DWORD_PTR CHYThreadKernelObject::SetThreadAffinityMask(HANDLE hThread, DWORD_PTR dwThreadAffinityMask)
	{
		return ::SetThreadAffinityMask(hThread, dwThreadAffinityMask);
	}

	DWORD SetThreadIdealProcessor(HANDLE hThread, DWORD dwIdealProcessor)
	{
		return ::SetThreadIdealProcessor(hThread, dwIdealProcessor);
	}
}
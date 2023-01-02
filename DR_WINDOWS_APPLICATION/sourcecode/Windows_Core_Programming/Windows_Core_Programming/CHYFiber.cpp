#include "CHYFiber.h"

namespace HY_KERNELOBJECT
{
	CHYFiber::CHYFiber()
	{

	}

	CHYFiber::~CHYFiber()
	{

	}

	PVOID CHYFiber::ConvertThreadToFiber(PVOID pvParam)
	{
		return ::ConvertThreadToFiber(pvParam);
	}

	PVOID CHYFiber::ConvertThreadToFiberEx(PVOID pvParam, DWORD dwFlags)
	{
		return ::ConvertThreadToFiberEx(pvParam, dwFlags);
	}

	PVOID CHYFiber::CreateFiber(DWORD dwStackSize, PFIBER_START_ROUTINE pfnStartAddress, PVOID pvParam)
	{
		return ::CreateFiber(dwStackSize, pfnStartAddress, pvParam);
	}

	PVOID CHYFiber::CreateFiberEx(SIZE_T dwStackCommitSize, SIZE_T dwStackReserveSize, DWORD dwFlags, PFIBER_START_ROUTINE pfnStartAddress, PVOID pvParam)
	{
		return ::CreateFiberEx(dwStackCommitSize, dwStackReserveSize, dwFlags, pfnStartAddress, pvParam);
	}

	VOID CHYFiber::SwitchToFiber(PVOID pvFiberExecutionContext)
	{
		return ::SwitchToFiber(pvFiberExecutionContext);
	}

	VOID CHYFiber::DeleteFiber(PVOID pvFiberExecutionContext)
	{
		return ::DeleteFiber(pvFiberExecutionContext);
	}

	BOOL CHYFiber::ConvertFiberToThread()
	{
		return ::ConvertFiberToThread();
	}

	DWORD CHYFiber::FlsAlloc(PFLS_CALLBACK_FUNCTION lpCallback)
	{
		return ::FlsAlloc(lpCallback);
	}

	BOOL CHYFiber::FlsFree(DWORD dwFlsIndex)
	{
		return ::FlsFree(dwFlsIndex);
	}

	BOOL CHYFiber::FlsSetValue(DWORD dwFlsIndex, PVOID lpFlsData)
	{
		return ::FlsSetValue(dwFlsIndex, lpFlsData);
	}

	PVOID CHYFiber::FlsGetValue(DWORD dwFlsIndex)
	{
		return ::FlsGetValue(dwFlsIndex);
	}

	BOOL CHYFiber::IsThreadAFiber()
	{
		return ::IsThreadAFiber();
	}

	PVOID CHYFiber::GetCurrentFiber()
	{
		return ::GetCurrentFiber();
	}

	PVOID CHYFiber::GetFiberData()
	{
		return ::GetFiberData();
	}
}
#include "CHYThreadKernelObject.h"

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

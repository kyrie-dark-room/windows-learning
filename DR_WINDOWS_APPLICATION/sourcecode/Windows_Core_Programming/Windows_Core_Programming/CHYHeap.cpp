#include "CHYHeap.h"

namespace HY_MEMORYOBJECT
{
	CHYHeap::CHYHeap()
	{

	}

	CHYHeap::~CHYHeap()
	{

	}

	HANDLE CHYHeap::GetProcessHeap()
	{
		return ::GetProcessHeap();
	}

	HANDLE CHYHeap::HeapCreate(DWORD fdwOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
	{
		return ::HeapCreate(fdwOptions, dwInitialSize, dwMaximumSize);
	}

	BOOL CHYHeap::HeapSetInformation(
		HANDLE HeapHandle,
		HEAP_INFORMATION_CLASS HeapInformationClass,
		PVOID HeapInformation,
		SIZE_T HeapInformationLength)
	{
		return ::HeapSetInformation(HeapHandle, HeapInformationClass, HeapInformation, HeapInformationLength);
	}

	PVOID CHYHeap::HeapAlloc(HANDLE hHeap, DWORD fdwFlags, SIZE_T dwBytes)
	{
		return ::HeapAlloc(hHeap, fdwFlags, dwBytes);
	}

	PVOID CHYHeap::HeapReAlloc(HANDLE hHeap, DWORD fdwFlags, PVOID pvMem, SIZE_T dwBytes)
	{
		return ::HeapReAlloc(hHeap, fdwFlags, pvMem, dwBytes);
	}

	SIZE_T CHYHeap::HeapSize(HANDLE hHeap, DWORD fdwFlags, LPCVOID pvMem)
	{
		return ::HeapSize(hHeap,fdwFlags, pvMem);
	}

	BOOL CHYHeap::HeapFree(HANDLE hHeap, DWORD fdwFlags, PVOID pvMem)
	{
		return ::HeapFree(hHeap, fdwFlags, pvMem);
	}

	BOOL CHYHeap::HeapDestroy(HANDLE hHeap)
	{
		return ::HeapDestroy(hHeap);
	}

	BOOL CHYHeap::Heap32First(LPHEAPENTRY32 lphe, DWORD th32ProcessID, ULONG_PTR th32HeapID)
	{
		return ::Heap32First(lphe, th32ProcessID, th32HeapID);
	}

	BOOL CHYHeap::Heap32Next(LPHEAPENTRY32 lphe)
	{
		return ::Heap32Next(lphe);
	}

	BOOL CHYHeap::Heap32ListFirst(HANDLE hSnapshot, LPHEAPLIST32 lphl)
	{
		return ::Heap32ListFirst(hSnapshot, lphl);
	}

	BOOL CHYHeap::Heap32ListNext(HANDLE hSnapshot, LPHEAPLIST32 lphl)
	{
		return ::Heap32ListNext(hSnapshot, lphl);
	}

	DWORD CHYHeap::GetProcessHeaps(DWORD dwNumHeaps, PHANDLE phHeaps)
	{
		return ::GetProcessHeaps(dwNumHeaps, phHeaps);
	}

	BOOL CHYHeap::HeapValidate(HANDLE hHeap, DWORD fdwFlags, LPCVOID pvMem)
	{
		return ::HeapValidate(hHeap, fdwFlags, pvMem);
	}

	UINT CHYHeap::HeapCompact(HANDLE hHeap, DWORD fdwFlags)
	{
		return ::HeapCompact(hHeap, fdwFlags);
	}

	BOOL CHYHeap::HeapLock(HANDLE hHeap)
	{
		return ::HeapLock(hHeap);
	}

	BOOL CHYHeap::HeapUnlock(HANDLE hHeap)
	{
		return ::HeapUnlock(hHeap);
	}

	BOOL CHYHeap::HeapWalk(HANDLE hHeap, PPROCESS_HEAP_ENTRY pHeapEntry)
	{
		return ::HeapWalk(hHeap, pHeapEntry);
	}
}

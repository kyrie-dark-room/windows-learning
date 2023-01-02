#include "CHYVirtualMemory.h"

namespace HY_MEMORYOBJECT
{
	CHYVirtualMemory::CHYVirtualMemory()
	{
	}

	CHYVirtualMemory::~CHYVirtualMemory()
	{
	}

	PVOID CHYVirtualMemory::VirtualAlloc(PVOID pvAddress, SIZE_T dwSize, DWORD fdwAllocationType, DWORD fdwProtect)
	{
		return ::VirtualAlloc(pvAddress, dwSize, fdwAllocationType, fdwProtect);
	}

	PVOID CHYVirtualMemory::VirtualAllocExNuma(HANDLE hProcess, PVOID pvAddress, SIZE_T dwSize, DWORD fdwAllocationType, DWORD fdwProtect, DWORD dwPreferredNumaNode)
	{
		return ::VirtualAllocExNuma(hProcess, pvAddress, dwSize, fdwAllocationType, fdwProtect, dwPreferredNumaNode);
	}

	SIZE_T CHYVirtualMemory::GetLargePageMinimum()
	{
		return ::GetLargePageMinimum();
	}

	BOOL CHYVirtualMemory::VirtualFree(LPVOID pvAddress, SIZE_T dwSize, DWORD fdwFreeType)
	{
		return ::VirtualFree(pvAddress, dwSize, fdwFreeType);
	}

	BOOL CHYVirtualMemory::VirtualProtect(PVOID pvAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD pflOldProtect)
	{
		return ::VirtualProtect(pvAddress, dwSize, flNewProtect, pflOldProtect);
	}
}
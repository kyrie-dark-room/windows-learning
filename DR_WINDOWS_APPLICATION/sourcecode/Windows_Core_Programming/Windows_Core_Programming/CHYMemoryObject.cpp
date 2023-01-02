#include "CHYMemoryObject.h"
#include <shlwapi.h>

namespace HY_MEMORYOBJECT
{
	CHYMemoryObject::CHYMemoryObject()
	{

	}

	CHYMemoryObject::~CHYMemoryObject()
	{

	}

	VOID CHYMemoryObject::GetSystemInfo(LPSYSTEM_INFO psi)
	{
		return ::GetSystemInfo(psi);
	}

	BOOL CHYMemoryObject::IsWow64Process(HANDLE hProcess, PBOOL pbWow64Process)
	{
		return ::IsWow64Process(hProcess, pbWow64Process);
	}

	void CHYMemoryObject::GetNativeSystemInfo(LPSYSTEM_INFO lpSystemInfo)
	{
		return ::GetNativeSystemInfo(lpSystemInfo);
	}

	BOOL CHYMemoryObject::IsOS(DWORD dwOS)
	{
		return ::IsOS(dwOS);
	}

	VOID CHYMemoryObject::GlobalMemoryStatus(LPMEMORYSTATUS lpBuffer)
	{
		return ::GlobalMemoryStatus(lpBuffer);
	}

	BOOL CHYMemoryObject::GlobalMemoryStatusEx(LPMEMORYSTATUSEX pmst)
	{
		return ::GlobalMemoryStatusEx(pmst);
	}

	BOOL CHYMemoryObject::GetNumaAvailableMemoryNode(UCHAR uNode, PULONGLONG pulAvailableBytes)
	{
		return ::GetNumaAvailableMemoryNode(uNode, pulAvailableBytes);
	}

	BOOL CHYMemoryObject::GetNumaProcessorNode(UCHAR Processor, PUCHAR NodeNumber)
	{
		return ::GetNumaProcessorNode(Processor, NodeNumber);
	}

	BOOL CHYMemoryObject::GetNumaHighestNodeNumber(PULONG pulHighestNodeNumber)
	{
		return ::GetNumaHighestNodeNumber(pulHighestNodeNumber);
	}

	BOOL CHYMemoryObject::GetNumaNodeProcessorMask(UCHAR uNode, PULONGLONG pulProcessorMask)
	{
		return ::GetNumaNodeProcessorMask(uNode, pulProcessorMask);
	}

	BOOL CHYMemoryObject::GetProcessMemoryInfo(HANDLE hProcess, PPROCESS_MEMORY_COUNTERS ppmc, DWORD cbSize)
	{
		return ::GetProcessMemoryInfo(hProcess, ppmc, cbSize);
	}

	DWORD CHYMemoryObject::VirtualQuery(LPCVOID pvAddress, PMEMORY_BASIC_INFORMATION pmbi, DWORD dwLength)
	{
		return ::VirtualQuery(pvAddress, pmbi, dwLength);
	}

	DWORD CHYMemoryObject::VirtualQueryEx(HANDLE hProcess, LPCVOID pvAddress, PMEMORY_BASIC_INFORMATION pmbi, DWORD dwLength)
	{
		return ::VirtualQueryEx(hProcess, pvAddress, pmbi, dwLength);
	}
}
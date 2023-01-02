#include "CHYMemoryMappedFile.h"

namespace HY_MEMORYOBJECT
{
	CHYMemoryMappedFile::CHYMemoryMappedFile()
	{

	}

	CHYMemoryMappedFile::~CHYMemoryMappedFile()
	{

	}

	HANDLE CHYMemoryMappedFile::CreateFile(
		LPCWSTR pszFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		PSECURITY_ATTRIBUTES psa,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile)
	{
		return ::CreateFile(pszFileName, dwDesiredAccess, dwShareMode, psa, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	HANDLE CHYMemoryMappedFile::CreateFileMapping(HANDLE hFile, PSECURITY_ATTRIBUTES psa, DWORD fdwProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, PCTSTR pszName)
	{
		return ::CreateFileMapping(hFile, psa, fdwProtect, dwMaximumSizeHigh, dwMaximumSizeLow, pszName);
	}

	HANDLE CHYMemoryMappedFile::CreateFileMappingNuma(HANDLE hFile, PSECURITY_ATTRIBUTES psa, DWORD fdwProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, PCTSTR pszName, 
		DWORD dwPreferredNumaNode)
	{
		return ::CreateFileMappingNuma(hFile, psa, fdwProtect, dwMaximumSizeHigh, dwMaximumSizeLow, pszName, dwPreferredNumaNode);
	}

	PVOID CHYMemoryMappedFile::MapViewOfFile(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap)
	{
		return ::MapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
	}

	PVOID CHYMemoryMappedFile::MapViewOfFileExNuma(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap,
		LPVOID lpBaseAddress, DWORD dwPreferredNumaNode)
	{
		return ::MapViewOfFileExNuma(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap, lpBaseAddress, dwPreferredNumaNode);
	}

	PVOID CHYMemoryMappedFile::MapViewOfFileEx(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap, 
		PVOID pvBaseAddress)
	{
		return ::MapViewOfFileEx(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap, pvBaseAddress);
	}

	BOOL CHYMemoryMappedFile::UnmapViewOfFile(PVOID pvBaseAddress)
	{
		return ::UnmapViewOfFile(pvBaseAddress);
	}

	BOOL CHYMemoryMappedFile::FlushViewOfFile(PVOID pvAddress, SIZE_T dwNumberOfBytesToFlush)
	{
		return ::FlushViewOfFile(pvAddress, dwNumberOfBytesToFlush);
	}

	BOOL CHYMemoryMappedFile::CloseHandle(HANDLE hObject)
	{
		return ::CloseHandle(hObject);
	}
}
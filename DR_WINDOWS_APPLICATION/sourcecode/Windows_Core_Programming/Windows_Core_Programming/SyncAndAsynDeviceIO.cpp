#include "SyncAndAsynDeviceIO.h"

namespace HY_KERNELOBJECT
{
	SyncAndAsynDeviceIO::SyncAndAsynDeviceIO()
	{

	}

	SyncAndAsynDeviceIO::~SyncAndAsynDeviceIO()
	{

	}

	BOOL SyncAndAsynDeviceIO::SetCommConfig(HANDLE hCommDev, LPCOMMCONFIG pCC, DWORD dwSize)
	{
		return ::SetCommConfig(hCommDev, pCC, dwSize);
	}

	BOOL SyncAndAsynDeviceIO::SetMailslotInfo(HANDLE hMailslot, DWORD dwReadTimeout)
	{
		return ::SetMailslotInfo(hMailslot, dwReadTimeout);
	}

	BOOL SyncAndAsynDeviceIO::CloseHandle(HANDLE hObject)
	{
		return ::CloseHandle(hObject);
	}

	int SyncAndAsynDeviceIO::closesocket(SOCKET s)
	{
		return ::closesocket(s);
	}

	DWORD SyncAndAsynDeviceIO::GetFileType(HANDLE hDevice)
	{
		return ::GetFileType(hDevice);
	}

	HANDLE SyncAndAsynDeviceIO::CreateFile(
		PCTSTR pszName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		PSECURITY_ATTRIBUTES psa,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hFileTemplate)
	{
		return ::CreateFile(pszName, dwDesiredAccess, dwShareMode, psa, dwCreationDisposition, dwFlagsAndAttributes, hFileTemplate);
	}

	BOOL SyncAndAsynDeviceIO::GetFileSizeEx(HANDLE hFile, PLARGE_INTEGER pliFileSize)
	{
		return ::GetFileSizeEx(hFile, pliFileSize);
	}

	DWORD SyncAndAsynDeviceIO::GetCompressedFileSize(PCTSTR pszFileName, PDWORD pdwFileSizeHigh)
	{
		return ::GetCompressedFileSize(pszFileName, pdwFileSizeHigh);
	}

	BOOL SyncAndAsynDeviceIO::SetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER pliNewFilePointer, DWORD dwMoveMethod)
	{
		return ::SetFilePointerEx(hFile, liDistanceToMove, pliNewFilePointer, dwMoveMethod);
	}

	LARGE_INTEGER SyncAndAsynDeviceIO::GetFilePointer(HANDLE hFile)
	{
		LARGE_INTEGER liCurrentPosition = {0};
		if (!SetFilePointerEx(hFile, liCurrentPosition, &liCurrentPosition, FILE_CURRENT))
		{
			liCurrentPosition.QuadPart = 0;
		}

		return liCurrentPosition;
	}

	BOOL SyncAndAsynDeviceIO::SetEndOfFile(HANDLE hFile)
	{
		return ::SetEndOfFile(hFile);
	}

	BOOL SyncAndAsynDeviceIO::ReadFile(HANDLE hFile, PVOID pvBuffer, DWORD nNumBytesToRead, PDWORD pdwNumBytes, OVERLAPPED* pOverlapped)
	{
		return ::ReadFile(hFile, pvBuffer, nNumBytesToRead, pdwNumBytes, pOverlapped);
	}

	BOOL SyncAndAsynDeviceIO::WriteFile(HANDLE hFile, CONST VOID* pvBuffer, DWORD nNumBytesToWrite, PDWORD pdwNumBytes, OVERLAPPED* pOverlapped)
	{
		return ::WriteFile(hFile, pvBuffer, nNumBytesToWrite, pdwNumBytes, pOverlapped);
	}

	BOOL SyncAndAsynDeviceIO::FlushFileBuffers(HANDLE hFile)
	{
		return ::FlushFileBuffers(hFile);
	}

	BOOL SyncAndAsynDeviceIO::CancelSynchronousIo(HANDLE hThread)
	{
		return ::CancelSynchronousIo(hThread);
	}

	BOOL SyncAndAsynDeviceIO::CancelIo(HANDLE hFile)
	{
		return ::CancelIo(hFile);
	}

	BOOL SyncAndAsynDeviceIO::CancelIoEx(HANDLE hFile, LPOVERLAPPED pOverlapped)
	{
		return ::CancelIoEx(hFile, pOverlapped);
	}

	BOOL SyncAndAsynDeviceIO::SetFileCompletionNotificationModes(HANDLE hFile, UCHAR uFlags)
	{
		return ::SetFileCompletionNotificationModes(hFile, uFlags);
	}

	BOOL SyncAndAsynDeviceIO::GetOverlappedResult(HANDLE hFile, OVERLAPPED* pOverlapped, PDWORD pdwNumBytes, BOOL bWait)
	{
		return ::GetOverlappedResult(hFile, pOverlapped, pdwNumBytes, bWait);
	}

	BOOL SyncAndAsynDeviceIO::ReadFileEx(HANDLE hFile, PVOID pvBuffer, DWORD nNumBytesToRead, OVERLAPPED* pOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE pfnCompletionRoutine)
	{
		return ::ReadFileEx(hFile, pvBuffer, nNumBytesToRead, pOverlapped, pfnCompletionRoutine);
	}

	BOOL SyncAndAsynDeviceIO::WriteFileEx(HANDLE hFile, PVOID pvBuffer, DWORD nNumBytesToWrite, OVERLAPPED* pOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE pfnCompletionRoutine)
	{
		return ::WriteFileEx(hFile, pvBuffer, nNumBytesToWrite, pOverlapped, pfnCompletionRoutine);
	}

	DWORD SyncAndAsynDeviceIO::QueueUserAPC(PAPCFUNC pfnAPC, HANDLE hThread, ULONG_PTR dwData)
	{
		return ::QueueUserAPC(pfnAPC, hThread, dwData);
	}
}

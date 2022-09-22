#include "CHYProcessKernelObject.h"
#include <shellapi.h>
#include <string>
#include <strsafe.h>
#include <versionhelpers.h>
#include  <Shlobj.h>

const IMAGE_DOS_HEADER __ImageBase;


namespace HY_KERNELOBJECT
{
	void DumpModule()
	{
		return;
	}

	CHYProcessKernelObject::CHYProcessKernelObject()
	{

	}

	CHYProcessKernelObject::~CHYProcessKernelObject()
	{

	}

	HMODULE CHYProcessKernelObject::GetModuleHandle(PCTSTR pszModule)
	{
		return ::GetModuleHandle(pszModule);
	}

	HMODULE CHYProcessKernelObject::GetModuleHandleInDllOne()
	{
		return (HINSTANCE)&__ImageBase;
	}

	HMODULE CHYProcessKernelObject::GetModuleHandleInDllTwo()
	{
		HMODULE hModule = NULL;
		if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (PCTSTR)DumpModule, &hModule))
			return hModule;
		else
		{
			std::string sErr = std::to_string(GetLastError());
			return NULL;
		}
	}

	PTSTR CHYProcessKernelObject::GetCommandLine()
	{
		return ::GetCommandLine();
	}

	std::vector<std::wstring> CHYProcessKernelObject::CommandLineToArgv()
	{
		std::vector<std::wstring> vArgv;
		int nNumArgs;
		PWSTR* ppArgv = ::CommandLineToArgvW(::GetCommandLine(), &nNumArgs);
		for (int i = 0; i < nNumArgs; i++)
		{
			vArgv.push_back(ppArgv[i]);
		}

		::HeapFree(::GetProcessHeap(), 0, ppArgv);
		return vArgv;
	}

	std::map<std::wstring, std::wstring> CHYProcessKernelObject::DumpEnvStrings()
	{
		std::map<std::wstring, std::wstring> mEnvStrings;
		PTSTR pEnvBlock = ::GetEnvironmentStrings();
		if (NULL == pEnvBlock)
			return mEnvStrings;
		TCHAR szName[MAX_PATH];
		TCHAR szValue[MAX_PATH];
		PTSTR pszCurrent = pEnvBlock;
		HRESULT hr = S_OK;
		PCTSTR pszPos = NULL;
		int current = 0;

		while (pszCurrent != NULL)
		{
			if (*pszCurrent != TEXT('='))
			{
				pszPos = wcschr(pszCurrent, TEXT('='));
				if (pszPos == NULL)
				{
					break;
				}

				pszPos++;
				size_t cbNameLengh = (size_t)pszPos - (size_t)pszCurrent - sizeof(TCHAR);
				hr = ::StringCbCopyNW(szName, MAX_PATH, pszCurrent, cbNameLengh);
				if (FAILED(hr))
				{
					break;
				}

				hr = ::StringCchCopyNW(szValue, MAX_PATH, pszPos, wcslen(pszPos) + 1);
				if (SUCCEEDED(hr))
				{
					mEnvStrings.insert(std::make_pair<std::wstring, std::wstring>(szName, szValue));
				}
			}

			current++;
			while (*pszCurrent != TEXT('\0'))
			{
				pszCurrent++;
			}
			pszCurrent++;

			if (*pszCurrent == TEXT('\0'))
			{
				break;
			}
		}

		::FreeEnvironmentStrings(pEnvBlock);

		return mEnvStrings;
	}

	std::map<std::wstring, std::wstring> CHYProcessKernelObject::DumpEnvVariables(PTSTR pEnvBlock[])
	{
		std::map<std::wstring, std::wstring> mEnvStrings;
		int current = 0;
		PTSTR* pElement = (PTSTR*)pEnvBlock;
		PTSTR pCurrent = NULL;
		PCTSTR pszPos = NULL;
		TCHAR szName[MAX_PATH];
		TCHAR szValue[MAX_PATH];
		HRESULT hr = S_OK;
		while (pElement != NULL)
		{
			pCurrent = (PTSTR)(*pElement) ;
			if (pCurrent == NULL)
			{
				pElement = NULL;
			}
			else
			{
				pszPos = wcschr(pCurrent, TEXT('='));
				if (pszPos == NULL)
				{
					break;
				}

				pszPos++;
				size_t cbNameLengh = (size_t)pszPos - (size_t)pCurrent - sizeof(TCHAR);
				hr = ::StringCbCopyNW(szName, MAX_PATH, pCurrent, cbNameLengh);
				if (FAILED(hr))
				{
					break;
				}

				hr = ::StringCchCopyNW(szValue, MAX_PATH, pszPos, wcslen(pszPos) + 1);
				if (SUCCEEDED(hr))
				{
					mEnvStrings.insert(std::make_pair<std::wstring, std::wstring>(szName, szValue));
				}
				current++;
				pElement++;
			}
		}
		return mEnvStrings;
	}

	LRESULT CHYProcessKernelObject::SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		return ::SendMessage(hWnd, Msg, wParam, lParam);
	}

	DWORD CHYProcessKernelObject::GetEnvironmentVariable(PCTSTR pszName, PTSTR pszValue, DWORD cchValue)
	{
		return ::GetEnvironmentVariable(pszName, pszValue, cchValue);
	}

	DWORD CHYProcessKernelObject::ExpandEnvironmentStrings(PCTSTR pszSrc, PTSTR pszDst, DWORD chSize)
	{
		return ::ExpandEnvironmentStrings(pszSrc, pszDst, chSize);
	}

	UINT CHYProcessKernelObject::SetErrorMode(UINT fuErrorMode)
	{
		return ::SetErrorMode(fuErrorMode);
	}

	DWORD CHYProcessKernelObject::GetCurrentDirectory(DWORD cchCurDir, PTSTR pszCurDir)
	{
		::GetCurrentDirectory(cchCurDir, pszCurDir);
	}

	BOOL CHYProcessKernelObject::SetCurrentDirectory(PCTSTR pszCurDir)
	{
		return ::SetCurrentDirectory(pszCurDir);
	}

	DWORD CHYProcessKernelObject::GetFullPathName(PCTSTR pszFile, DWORD cchPath, PTSTR pszPath, PTSTR* ppszFilePart)
	{
		return ::GetFullPathName(pszFile, cchPath, pszPath, ppszFilePart);
	}

	BOOL CHYProcessKernelObject::IsWindowsXp()
	{
		/*
		OSVERSIONINFO osvi;
		BOOL bIsWindowsXP;

		ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		::GetVersionEx(&osvi);
		bIsWindowsXP = (osvi.dwMajorVersion < 6);
		return bIsWindowsXP;
		*/
		return (!IsWindowsVistaOrGreater());
	}

	BOOL CHYProcessKernelObject::IsWindowsVista()
	{
		OSVERSIONINFOEX osver = { 0 };
		osver.dwOSVersionInfoSize = sizeof(osver);
		osver.dwMajorVersion = 6;
		osver.dwMinorVersion = 0;
		osver.dwPlatformId = VER_PLATFORM_WIN32_NT;

		DWORDLONG dwlConfitionMask = 0;
		VER_SET_CONDITION(dwlConfitionMask, VER_MAJORVERSION, VER_EQUAL);
		VER_SET_CONDITION(dwlConfitionMask, VER_MINORVERSION, VER_EQUAL);
		VER_SET_CONDITION(dwlConfitionMask, VER_PLATFORMID, VER_EQUAL);

		if (VerifyVersionInfo(&osver, VER_MAJORVERSION | VER_MINORVERSION | VER_PLATFORMID, dwlConfitionMask))
		{
			return TRUE;
		}
		else
		{
			if (ERROR_OLD_WIN_VERSION == GetLastError())
				return FALSE;
			else
				return FALSE;
		}
	}

	BOOL CHYProcessKernelObject::CreateProcess(
		LPCWSTR	lpApplicationName,
		LPWSTR	lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL                  bInheritHandles,
		DWORD                 dwCreationFlags,
		LPVOID                lpEnvironment,
		LPCWSTR                lpCurrentDirectory,
		LPSTARTUPINFOW        lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation)
	{
		return ::CreateProcess(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
	}

	DWORD CHYProcessKernelObject::GetCurrentProcessId()
	{
		return ::GetCurrentProcessId();
	}

	DWORD CHYProcessKernelObject::GetCurrentThreadId()
	{
		return ::GetCurrentThreadId();
	}

	DWORD CHYProcessKernelObject::GetProcessId(HANDLE Process)
	{
		return ::GetProcessId(Process);
	}

	DWORD CHYProcessKernelObject::GetThreadId(HANDLE Thread)
	{
		return ::GetThreadId(Thread);
	}

	DWORD CHYProcessKernelObject::GetProcessIdOfThread(HANDLE Thread)
	{
		return ::GetProcessIdOfThread(Thread);
	}

	VOID CHYProcessKernelObject::ExitProcess(UINT fuExitCode)
	{
		return ::ExitProcess(fuExitCode);
	}

	BOOL CHYProcessKernelObject::TerminateProcess(HANDLE hProcess, UINT fuExitCode)
	{
		return ::TerminateProcess(hProcess, fuExitCode);
	}

	BOOL CHYProcessKernelObject::GetExitCodeProcess(HANDLE hProcess, PDWORD pdwExitCode)
	{
		return ::GetExitCodeProcess(hProcess, pdwExitCode);
	}

	BOOL CHYProcessKernelObject::GetProcessElevation(TOKEN_ELEVATION_TYPE* pElevationType, BOOL* pIsAdmin)
	{
		HANDLE hToken = NULL;
		DWORD dwSize;

		// Get current process token
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
			return(FALSE);

		BOOL bResult = FALSE;

		// Retrieve elevation type information
		if (GetTokenInformation(hToken, TokenElevationType, pElevationType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize))
		{
			// Create the SID corresponding to the Administrators group
			BYTE adminSID[SECURITY_MAX_SID_SIZE];
			dwSize = sizeof(adminSID);
			CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);

			if (*pElevationType == TokenElevationTypeLimited)
			{
				// Get handle to linked token(will have one if we are lua)
				HANDLE hUnfilteredToken = NULL;
				GetTokenInformation(hToken, TokenLinkedToken, (VOID*)&hUnfilteredToken, sizeof(HANDLE), &dwSize);   // 获取关联的非限制性令牌

				// Check if this original token contains admin SID
				if (CheckTokenMembership(hUnfilteredToken, &adminSID, pIsAdmin))
				{
					bResult = TRUE;
				}

				// Don't forget to close the unfiltered token
				CloseHandle(hUnfilteredToken);
			}
			else
			{
				*pIsAdmin = ::IsUserAnAdmin();
				bResult = TRUE;
			}
		}

		// Don't forget to close the process token
		CloseHandle(hToken);
		return(bResult);
	}

	BOOL CHYProcessKernelObject::GetProcessIoCounters(HANDLE hProcess, PIO_COUNTERS pIoCounters)
	{
		return ::GetProcessIoCounters(hProcess, pIoCounters);
	}

	BOOL CHYProcessKernelObject::SetPriorityClass(HANDLE hProcess, DWORD fdwPriority)
	{
		return ::SetPriorityClass(hProcess, fdwPriority);
	}

	DWORD CHYProcessKernelObject::GetPriorityClass(HANDLE hProcess)
	{
		return ::GetPriorityClass(hProcess);
	}
}
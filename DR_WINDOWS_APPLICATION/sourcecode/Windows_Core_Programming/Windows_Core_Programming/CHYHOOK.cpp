#include "CHYHOOK.h"
#include <dbghelp.h>

namespace HY_HOOK_Manager
{
	CHYHOOK::CHYHOOK()
	{

	}

	CHYHOOK::~CHYHOOK()
	{

	}

	LONG_PTR CHYHOOK::SetWindowLongPtr(
		HWND     hWnd,
		int      nIndex,
		LONG_PTR dwNewLong
	)
	{
		return ::SetWindowLongPtr(hWnd, nIndex, dwNewLong);
	}

	BOOL CHYHOOK::SetWindowSubclass(
		HWND         hWnd,
		SUBCLASSPROC pfnSubclass,
		UINT_PTR     uIdSubclass,
		DWORD_PTR    dwRefData
	)
	{
		return ::SetWindowSubclass(hWnd, pfnSubclass, uIdSubclass, dwRefData);
	}

	BOOL CHYHOOK::GetWindowSubclass(
		HWND         hWnd,
		SUBCLASSPROC pfnSubclass,
		UINT_PTR     uIdSubclass,
		DWORD_PTR* pdwRefData
	)
	{
		return ::GetWindowSubclass(hWnd, pfnSubclass, uIdSubclass, pdwRefData);
	}

	BOOL CHYHOOK::RemoveWindowSubclass(
		HWND         hWnd,
		SUBCLASSPROC pfnSubclass,
		UINT_PTR     uIdSubclass
	)
	{
		return ::RemoveWindowSubclass(hWnd, pfnSubclass, uIdSubclass);
	}

	LRESULT CHYHOOK::DefSubclassProc(
		HWND   hWnd,
		UINT   uMsg,
		WPARAM wParam,
		LPARAM lParam
	)
	{
		return ::DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}

	HHOOK CHYHOOK::SetWindowsHookEx(
		int       idHook,
		HOOKPROC  lpfn,
		HINSTANCE hmod,
		DWORD     dwThreadId
	)
	{
		return ::SetWindowsHookEx(idHook, lpfn, hmod, dwThreadId);
	}

	BOOL CHYHOOK::UnhookWindowsHookEx(HHOOK hhk)
	{
		return ::UnhookWindowsHookEx(hhk);
	}

	LPVOID CHYHOOK::VirtualAllocEx(
		HANDLE hProcess,
		LPVOID lpAddress,
		SIZE_T dwSize,
		DWORD  flAllocationType,
		DWORD  flProtect
	)
	{
		return ::VirtualAllocEx(hProcess, lpAddress, dwSize, flAllocationType, flProtect);
	}

	BOOL CHYHOOK::ReadProcessMemory(
		HANDLE  hProcess,
		LPCVOID lpBaseAddress,
		LPVOID  lpBuffer,
		SIZE_T  nSize,
		SIZE_T* lpNumberOfBytesRead
	)
	{
		return ::ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
	}

	BOOL CHYHOOK::WriteProcessMemory(
		HANDLE  hProcess,
		LPVOID  lpBaseAddress,
		LPCVOID lpBuffer,
		SIZE_T  nSize,
		SIZE_T* lpNumberOfBytesWritten
	)
	{
		return ::WriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
	}

	HANDLE CHYHOOK::CreateRemoteThread(
		HANDLE                 hProcess,
		LPSECURITY_ATTRIBUTES  lpThreadAttributes,
		SIZE_T                 dwStackSize,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID                 lpParameter,
		DWORD                  dwCreationFlags,
		LPDWORD                lpThreadId
	)
	{
		return ::CreateRemoteThread(hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
	}

	BOOL CHYHOOK::VirtualFreeEx(
		HANDLE hProcess,
		LPVOID lpAddress,
		SIZE_T dwSize,
		DWORD  dwFreeType
	)
	{
		return ::VirtualFreeEx(hProcess, lpAddress, dwSize, MEM_RELEASE);
	}

	void CHYHOOK::ReplaceIATEntryInOneMod(PCSTR pszCalleeModName, PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller)
	{
		// Get the address of the module's import section
		ULONG ulSize;

		// An exception was triggered by Explorer ( when browsing the content of a folder) into imagehlp.dll. It looks like one module was unloaded...
		// Maybe some threading problem: the list of modules from Toolhelp might not be accurate if FreeLibrary is called during the enumeration.
		PIMAGE_IMPORT_DESCRIPTOR pImportDesc = NULL;
		__try {
			pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);
		}
		__except (1/*InvalidReadExceptionFilter(GetExceptionInformation())*/) {
			// Nothing to do in here, thread continues to run normally with NULL for pImportDesc
		}

		if (pImportDesc == NULL)
		{
			return; // This module has no import section or is no longer loaded
		}

		// Find the import descriptor containing references to callee's functions
		for (; pImportDesc->Name; pImportDesc++) {
			PSTR pszModName = (PSTR)((PBYTE)hmodCaller + pImportDesc->Name);
			if (lstrcmpiA(pszModName, pszCalleeModName) == 0) {
				// Get caller's import address table (IAT) for the callee's functions
				PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((PBYTE)hmodCaller + pImportDesc->FirstThunk);

				// Replace current function address with new function address
				for (; pThunk->u1.Function; pThunk++) {
					// Get the address of the function address
					PROC* ppfn = (PROC*)&pThunk->u1.Function;

					// Is this the function we're looking for?
					BOOL bFound = (*ppfn == pfnCurrent);
					if (bFound) {
						if (!WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew, sizeof(pfnNew), NULL) && (ERROR_NOACCESS == GetLastError())) {
							DWORD dwOldProtect;
							if (VirtualProtect(ppfn, sizeof(pfnNew), PAGE_WRITECOPY, &dwOldProtect)) {
								WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew, sizeof(pfnNew), NULL);
								VirtualProtect(ppfn, sizeof(pfnNew), dwOldProtect, &dwOldProtect);
							}
						}
						return;
					}
				}
			}
		}
	}
}
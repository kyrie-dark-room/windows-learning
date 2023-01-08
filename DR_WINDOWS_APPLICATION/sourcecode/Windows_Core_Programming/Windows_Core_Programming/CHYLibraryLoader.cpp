#include "CHYLibraryLoader.h"
#include <psapi.h>
#include <imagehlp.h>

namespace HY_DLL_Manager
{
	CHYLibraryLoader::CHYLibraryLoader()
	{

	}

	CHYLibraryLoader::~CHYLibraryLoader()
	{

	}

	HMODULE CHYLibraryLoader::LoadLibrary(LPCWSTR lpLibFileName)
	{
		return ::LoadLibrary(lpLibFileName);
	}

	HMODULE CHYLibraryLoader::LoadLibraryEx(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
	{
		return ::LoadLibraryEx(lpLibFileName, hFile, dwFlags);
	}

	BOOL CHYLibraryLoader::SetDllDirectory(LPCWSTR lpPathName)
	{
		return ::SetDllDirectory(lpPathName);
	}

	DLL_DIRECTORY_COOKIE CHYLibraryLoader::AddDllDirectory(PCWSTR NewDirectory)
	{
		return ::AddDllDirectory(NewDirectory);
	}

	BOOL CHYLibraryLoader::RemoveDllDirectory(DLL_DIRECTORY_COOKIE Cookie)
	{
		return ::RemoveDllDirectory(Cookie);
	}

	BOOL CHYLibraryLoader::FreeLibrary(HMODULE hInstDll)
	{
		return ::FreeLibrary(hInstDll);
	}

	void CHYLibraryLoader::FreeLibraryAndExitThread(HMODULE hLibModule, DWORD dwExitCode)
	{
		return ::FreeLibraryAndExitThread(hLibModule, dwExitCode);
	}

	HMODULE CHYLibraryLoader::GetModuleHandle(PCTSTR pszModuleName)
	{
		return ::GetModuleHandle(pszModuleName);
	}

	DWORD CHYLibraryLoader::GetModuleFileName(HMODULE hModule, LPWSTR lpFilename, DWORD nSize)
	{
		return ::GetModuleFileName(hModule, lpFilename, nSize);
	}

	DWORD CHYLibraryLoader::HYGetModuleFileNameEx(HANDLE hProcess, HMODULE hModule, LPWSTR lpFilename, DWORD nSize)
	{
		return ::GetModuleFileNameEx(hProcess, hModule, lpFilename, nSize);
	}

	FARPROC CHYLibraryLoader::GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
	{
		return ::GetProcAddress(hModule, lpProcName);
	}

	BOOL CHYLibraryLoader::DisableThreadLibraryCalls(HMODULE hLibModule)
	{
		return ::DisableThreadLibraryCalls(hLibModule);
	}

	BOOL CHYLibraryLoader::ReBaseImage64(
		PCSTR   CurrentImageName,
		PCSTR   SymbolPath,
		BOOL    fReBase,
		BOOL    fRebaseSysfileOk,
		BOOL    fGoingDown,
		ULONG   CheckImageSize,
		ULONG* OldImageSize,
		ULONG64* OldImageBase,
		ULONG* NewImageSize,
		ULONG64* NewImageBase,
		ULONG   TimeStamp)
	{
		return ::ReBaseImage64(CurrentImageName, SymbolPath, fReBase, fRebaseSysfileOk, fGoingDown, CheckImageSize, OldImageSize, OldImageBase, NewImageSize, NewImageBase, TimeStamp);
	}

	BOOL CHYLibraryLoader::BindImageEx(
		DWORD                    Flags,
		PCSTR                    ImageName,
		PCSTR                    DllPath,
		PCSTR                    SymbolPath,
		PIMAGEHLP_STATUS_ROUTINE StatusRoutine)
	{
		return ::BindImageEx(Flags, ImageName, DllPath, SymbolPath, StatusRoutine);
	}
}
#include "CHYErrorHandler.h"

namespace HY_Windows_ErrorHandle
{
	CHYErrorHandler::CHYErrorHandler()
	{

	}

	CHYErrorHandler::~CHYErrorHandler()
	{

	}

	DWORD CHYErrorHandler::GetLastError()
	{
		return ::GetLastError();
	}

	std::wstring CHYErrorHandler::GetLastErrorMsg()
	{
		std::wstring sLastErrorMsg;

		// Get the error code
		DWORD dwError = GetLastError();

		// Buffer that gets the error message string
		HLOCAL hlocal = NULL;

		// Use the default system locale since we look for windows messages
		// Note: this MAKELANGID combination has a value of 0
		DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

		// Get the error code's textual description
		BOOL bOK = ::FormatMessage(
						FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
						NULL, dwError, systemLocale, (PTSTR)&hlocal, 0, NULL);
		if (!bOK)
		{
			// Is it a network-related error?
			HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES);
			if (hDll != NULL)
			{
				bOK = ::FormatMessage(
					FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
					hDll, dwError, systemLocale, (PTSTR)&hlocal, 0, NULL);
				FreeLibrary(hDll);
			}

		}

		if (bOK && (hlocal != NULL))
		{
			sLastErrorMsg = static_cast<std::wstring>((PCTSTR)LocalLock(hlocal));
		}
		else
		{
			sLastErrorMsg = TEXT("No text found for the last error number.");
		}

		return sLastErrorMsg;
	}
}
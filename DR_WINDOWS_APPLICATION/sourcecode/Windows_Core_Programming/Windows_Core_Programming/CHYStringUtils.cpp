#include "CHYStringUtils.h"
#include <shlwapi.h>

namespace HY_Windows_Utils_StringUtils
{
	std::wstring CHYStringUtils::HY_StrFormatKBSize(
		LONGLONG       qdw,
		PSTR           pszBuf,
		UINT           cchBuf)
	{
		TCHAR szBuf[MAX_PATH];
		::ZeroMemory(szBuf, sizeof(TCHAR) * _countof(szBuf));
		::StrFormatKBSize(qdw, szBuf, MAX_PATH);
		return std::wstring(szBuf);
	}

	std::wstring CHYStringUtils::HY_StrFormatByteSize(
		LONGLONG       qdw,
		PSTR           pszBuf,
		UINT           cchBuf)
	{
		TCHAR szBuf[MAX_PATH];
		::ZeroMemory(szBuf, sizeof(TCHAR) * _countof(szBuf));
		::StrFormatByteSize(qdw, szBuf, MAX_PATH);
		return std::wstring(szBuf);
	}

	int CHYStringUtils::HY_CompareString(
		LCID locale,
		DWORD dwCmdFlags,
		PCTSTR pString1,
		int cch1,
		PCTSTR pString2,
		int cch2)
	{
		// locale = GetThreadLocal();
		return ::CompareString(
			locale,
			dwCmdFlags,
			pString1,
			cch1,
			pString2,
			cch2);
	}

	std::wstring CHYStringUtils::StringToUnicode(std::string sSrc)
	{
		PWSTR pWideCharStr = NULL;
		int nLenOfWideCharStr;
		std::wstring wsUnicodeStr;
		nLenOfWideCharStr = ::MultiByteToWideChar(CP_ACP, 0, sSrc.c_str(), -1, NULL, 0);
		pWideCharStr = (PWSTR)HeapAlloc(GetProcessHeap(), 0, nLenOfWideCharStr * sizeof(wchar_t));
		if (!pWideCharStr)
		{
			return wsUnicodeStr;
		}
		
		ZeroMemory(pWideCharStr, nLenOfWideCharStr * sizeof(wchar_t));
		nLenOfWideCharStr = ::MultiByteToWideChar(CP_ACP, 0, sSrc.c_str(), -1, pWideCharStr, nLenOfWideCharStr);
		wsUnicodeStr = pWideCharStr;
		HeapFree(GetProcessHeap(), 0, pWideCharStr);
		return wsUnicodeStr;
	}

	std::string CHYStringUtils::UnicodeToString(std::wstring sSrc)
	{
		PSTR pMultiCharStr = NULL;
		int nLenOfMultiCharStr;
		std::string sMultiStr;
		nLenOfMultiCharStr = ::WideCharToMultiByte(CP_ACP, 0, sSrc.c_str(), -1, NULL, 0, NULL, NULL);
		pMultiCharStr = (PSTR)HeapAlloc(GetProcessHeap(), 0, nLenOfMultiCharStr * sizeof(char));
		if (!pMultiCharStr)
		{
			return sMultiStr;
		}
		ZeroMemory(pMultiCharStr, nLenOfMultiCharStr * sizeof(char));
		nLenOfMultiCharStr = ::WideCharToMultiByte(CP_ACP, 0, sSrc.c_str(), -1, pMultiCharStr, nLenOfMultiCharStr, NULL, NULL);
		sMultiStr = pMultiCharStr;
		HeapFree(GetProcessHeap(), 0, pMultiCharStr);
		return sMultiStr;
	}

	BOOL CHYStringUtils::HY_IsTextUnicode(CONST PVOID pvBuffer, int cb, PINT pResult)
	{
		return ::IsTextUnicode(pvBuffer, cb, pResult);
	}
}
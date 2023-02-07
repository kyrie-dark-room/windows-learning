#include "CHYSEG.h"

namespace HY_SEG
{
	CHYSEG::CHYSEG(void)
	{

	}

	CHYSEG::~CHYSEG(void)
	{

	}

	VOID CHYSEG::RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, CONST ULONG_PTR* pArguments)
	{
		return ::RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, pArguments);
	}

	LPTOP_LEVEL_EXCEPTION_FILTER CHYSEG::SetUnhandledExceptionFilter(
		LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter
	)
	{
		return ::SetUnhandledExceptionFilter(lpTopLevelExceptionFilter);
	}

	BOOL CHYSEG::IsDebuggerPresent()
	{
		return ::IsDebuggerPresent();
	}

	HRESULT CHYSEG::WerSetFlags(DWORD dwFlags)
	{
		return ::WerSetFlags(dwFlags);
	}

	HRESULT CHYSEG::WerGetFlags(HANDLE hProcess, PDWORD pdwFlags)
	{
		return ::WerGetFlags(hProcess, pdwFlags);
	}

	HRESULT CHYSEG::WerAddExcludedApplication(PCWSTR pwzExeName, BOOL bAllUsers)
	{
		return ::WerAddExcludedApplication(pwzExeName, bAllUsers);
	}

	HRESULT CHYSEG::WerRemoveExcludedApplication(PCWSTR pwzExeName, BOOL bAllUsers)
	{
		return ::WerRemoveExcludedApplication(pwzExeName, bAllUsers);
	}

	HRESULT CHYSEG::WerRegisterMemoryBlock(PVOID pvAddress, DWORD dwSize)
	{
		return ::WerRegisterMemoryBlock(pvAddress, dwSize);
	}

	HRESULT CHYSEG::WerRegisterFile(PCWSTR pszFileName, WER_REGISTER_FILE_TYPE regFileType, DWORD dwFlags)
	{
		return ::WerRegisterFile(pszFileName, regFileType, dwFlags);
	}

	HRESULT CHYSEG::WerUnregisterMemoryBlock(PVOID pvAddress)
	{
		return ::WerUnregisterMemoryBlock(pvAddress);
	}

	HRESULT CHYSEG::WerUnregisterFile(PCWSTR pszFileName)
	{
		return ::WerUnregisterFile(pszFileName);
	}

	HRESULT CHYSEG::WerReportCreate(PCWSTR pwzEventType, WER_REPORT_TYPE repType, PWER_REPORT_INFORMATION pReportInformation, HREPORT* phReportHandle)
	{
		return ::WerReportCreate(pwzEventType, repType, pReportInformation, phReportHandle);
	}

	HRESULT CHYSEG::WerReportSetParameter(HREPORT hReportHandle, DWORD dwparamID, PCWSTR pwzName, PCWSTR pwzValue)
	{
		return ::WerReportSetParameter(hReportHandle, dwparamID, pwzName, pwzValue);
	}

	HRESULT CHYSEG::WerReportAddDump(
		HREPORT hReportHandle,
		HANDLE hProcess,
		HANDLE hThread,
		WER_DUMP_TYPE dumpType,
		PWER_EXCEPTION_INFORMATION pExceptionParam,
		PWER_DUMP_CUSTOM_OPTIONS pDumpCustomOptions,
		DWORD dwFlags
	)
	{
		return ::WerReportAddDump(hReportHandle, hProcess, hThread, dumpType, pExceptionParam, pDumpCustomOptions, dwFlags);
	}

	HRESULT CHYSEG::WerReportAddFile(
		HREPORT hReportHandle,
		PCWSTR pwzPath,
		WER_FILE_TYPE repFileType,
		DWORD dwFileFlags
	)
	{
		return ::WerReportAddFile(hReportHandle, pwzPath, repFileType, dwFileFlags);
	}

	HRESULT CHYSEG::WerReportSetUIOption(
		HREPORT hReportHandle,
		WER_REPORT_UI repUITypeID,
		PCWSTR pwzValue
	)
	{
		return ::WerReportSetUIOption(hReportHandle, repUITypeID, pwzValue);
	}

	HRESULT CHYSEG::WerReportSubmit(
		HREPORT hReportHandle,
		WER_CONSENT consent,
		DWORD dwFlags,
		PWER_SUBMIT_RESULT pSubmitResult
	)
	{
		return ::WerReportSubmit(hReportHandle, consent, dwFlags, pSubmitResult);
	}

	HRESULT CHYSEG::WerReportCloseHandle(HREPORT hReportHandle)
	{
		return ::WerReportCloseHandle(hReportHandle);
	}

	HRESULT CHYSEG::RegisterApplicationRestart(PCWSTR pwzCommandline, DWORD  dwFlags)
	{
		return ::RegisterApplicationRestart(pwzCommandline, dwFlags);
	}

	HRESULT CHYSEG::UnregisterApplicationRestart(void)
	{
		return ::UnregisterApplicationRestart();
	}

	HRESULT CHYSEG::RegisterApplicationRecoveryCallback(
		APPLICATION_RECOVERY_CALLBACK pRecoveyCallback,
		PVOID pvParameter,
		DWORD dwPingInterval,
		DWORD dwFlags
	)
	{
		return ::RegisterApplicationRecoveryCallback(pRecoveyCallback, pvParameter, dwPingInterval, dwFlags);
	}
}
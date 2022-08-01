#include "CHYJobKernelObject.h"

CHYJobKernelObject::CHYJobKernelObject()
{

}

CHYJobKernelObject::~CHYJobKernelObject()
{

}

BOOL CHYJobKernelObject::IsProcessInJob(HANDLE hProcess, HANDLE hJob, PBOOL pbInJob)
{
	return ::IsProcessInJob(hProcess, hJob, pbInJob);
}

HANDLE CHYJobKernelObject::CreateJobObject(PSECURITY_ATTRIBUTES psa, PCTSTR pszName)
{
	return ::CreateJobObject(psa, pszName);
}

HANDLE CHYJobKernelObject::OpenJobObject(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName)
{
	return ::OpenJobObject(dwDesiredAccess, bInheritHandle, pszName);
}

BOOL CHYJobKernelObject::SetInformationJobObject(HANDLE hJob, JOBOBJECTINFOCLASS JobObjectInformationClass, PVOID pJobObjectInformation, DWORD cbJobObjectInformationSize)
{
	return ::SetInformationJobObject(hJob, JobObjectInformationClass, pJobObjectInformation, cbJobObjectInformationSize);
}

BOOL CHYJobKernelObject::QueryInformationJobObject(
	HANDLE             hJob,
	JOBOBJECTINFOCLASS JobObjectInformationClass,
	LPVOID             lpJobObjectInformation,
	DWORD              cbJobObjectInformationLength,
	LPDWORD            lpReturnLength
)
{
	return ::QueryInformationJobObject(hJob, JobObjectInformationClass, lpJobObjectInformation, cbJobObjectInformationLength, lpReturnLength);
}

BOOL CHYJobKernelObject::AssignProcessToJobObject(HANDLE hJob, HANDLE hProcess)
{
	return ::AssignProcessToJobObject(hJob, hProcess);
}

BOOL CHYJobKernelObject::TerminateJobObject(HANDLE hJob, UINT uExitCode)
{
	return ::TerminateJobObject(hJob, uExitCode);
}
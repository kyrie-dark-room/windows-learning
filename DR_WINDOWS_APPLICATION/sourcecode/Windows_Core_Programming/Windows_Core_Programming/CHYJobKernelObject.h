#pragma once
/*
* ֪ʶ��
* 1��Windows�ṩ��һ����ҵ��Job���ں˶������������ǽ����������һ�𲢴���һ����ɳ�䡱�����ƽ����ܹ���ʲô����ý���ҵ���������һ���������������ǣ�����ֻ����һ�����̵���ҵͬ��
*   �ǳ����ã���Ϊ�������ԶԽ���ʩ��ƽʱ����ʩ�ӵ����ơ�
* 2��������һ����ҵ֮�󣬽���һ��������ҵ�еĽ����ܹ�ִ����Щ����������һ��ɳ�䣨��ʩ�����ƣ�����������ҵӦ�����¼������͵����ƣ�
*   �������޶����չ�����޶���ڷ�ֹ��ҵ�еĽ��̶�ռϵͳ��Դ��
*	��������UI���ƣ����ڷ�ֹ��ҵ�ڵĽ��̸����û����档
*	����ȫ�޶���ڷ�ֹ��ҵ�ڵĽ��̷��ʰ�ȫ��Դ���ļ���ע�������ȣ���
* 3��
* 
*/

#include <Windows.h>
class CHYJobKernelObject
{
public:
	CHYJobKernelObject();
	~CHYJobKernelObject();

	// ��֤��ǰ�����Ƿ���һ�����е���ҵ����֮������
	BOOL IsProcessInJob(HANDLE hProcess, HANDLE hJob, PBOOL pbInJob);

	// ����һ���µ���ҵ�ں˶���
	HANDLE CreateJobObject(PSECURITY_ATTRIBUTES psa, PCTSTR pszName);

	// ��һ���Ѿ����ڵ���ҵ�ں˶���
	HANDLE OpenJobObject(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName);

	// ����ҵʩ������
	BOOL SetInformationJobObject(HANDLE hJob, JOBOBJECTINFOCLASS JobObjectInformationClass, PVOID pJobObjectInformation, DWORD cbJobObjectInformationSize);

	// ��ҵ�еĽ��̿��Ի����������ҵ�������Ϣ
	BOOL QueryInformationJobObject(
		HANDLE             hJob,
		JOBOBJECTINFOCLASS JobObjectInformationClass,
		LPVOID             lpJobObjectInformation,
		DWORD              cbJobObjectInformationLength,
		LPDWORD            lpReturnLength
	);

	// ��������ʽ�ط������½�����ҵ��
	BOOL AssignProcessToJobObject(HANDLE hJob, HANDLE hProcess);

	// Ҫ��ɱ������ҵ�ڲ������н��̣�ֻ��������´���
	BOOL TerminateJobObject(HANDLE hJob, UINT uExitCode);
};


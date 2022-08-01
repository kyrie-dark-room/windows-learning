#pragma once
/*
* 知识：
* 1）Windows提供了一个作业（Job）内核对象，它允许我们将进程组合在一起并创建一个“沙箱”来限制进程能够做什么。最好将作业对象想象成一个进程容器。但是，创建只包含一个进程的作业同样
*   非常有用，因为这样可以对进程施加平时不能施加的限制。
* 2）创建好一个作业之后，接着一般会根据作业中的进程能够执行哪些操作来建立一个沙箱（即施加限制）。可以向作业应用以下几种类型的限制：
*   ·基本限额和扩展基本限额，用于防止作业中的进程独占系统资源。
*	·基本的UI限制，用于防止作业内的进程更改用户界面。
*	·安全限额，用于防止作业内的进程访问安全资源（文件、注册表子项等）。
* 3）
* 
*/

#include <Windows.h>
class CHYJobKernelObject
{
public:
	CHYJobKernelObject();
	~CHYJobKernelObject();

	// 验证当前进程是否在一个现有的作业控制之下运行
	BOOL IsProcessInJob(HANDLE hProcess, HANDLE hJob, PBOOL pbInJob);

	// 创建一个新的作业内核对象
	HANDLE CreateJobObject(PSECURITY_ATTRIBUTES psa, PCTSTR pszName);

	// 打开一个已经存在的作业内核对象
	HANDLE OpenJobObject(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName);

	// 向作业施加限制
	BOOL SetInformationJobObject(HANDLE hJob, JOBOBJECTINFOCLASS JobObjectInformationClass, PVOID pJobObjectInformation, DWORD cbJobObjectInformationSize);

	// 作业中的进程可以获得其所属作业的相关信息
	BOOL QueryInformationJobObject(
		HANDLE             hJob,
		JOBOBJECTINFOCLASS JobObjectInformationClass,
		LPVOID             lpJobObjectInformation,
		DWORD              cbJobObjectInformationLength,
		LPDWORD            lpReturnLength
	);

	// 将进程显式地放入我新建的作业中
	BOOL AssignProcessToJobObject(HANDLE hJob, HANDLE hProcess);

	// 要“杀死”作业内部的所有进程，只需调用以下代码
	BOOL TerminateJobObject(HANDLE hJob, UINT uExitCode);
};


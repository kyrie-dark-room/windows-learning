#pragma once
/*
* 知识：
* 1）线程也有两个组成部分
*	·一个是线程的内核对象，操作系统用它管理线程。系统还用内核对象来存放线程统计信息的地方。
*	·一个线程栈，用于维护线程执行时所需的所有函数参数和局部变量。
* 2）如果想在进程中创建辅助线程，它必须有自己的入口点函数，形式如下：
*	DWORD WINAPI ThreadFunc(PVOID pvParam)
*	{
*		DWORD dwResult = 0;
*		...
*		return(dwResult);
*	}
* 3）关于线程函数，要注意以下几点。
*	·在默认情况下，主线程的入口点函数必须命名为main，wmain，WinMain或wWinMain(除非我们用/ENTRY:链接器选项来指定另一个函数作为入口点函数)。与此不同的是，线程函数可以任意命名。
*	  事实上，如果应用程序中有多个线程函数，必须为它们指定不同的名称，否则编译器/链接器会认为你创建了一个函数的多个实现。
*	·因为主线程的入口点函数有字符串参数，所以它提供了ANSI/Unicode版本供我们选择：main/wmain和WinMain/wWinMain。相反，线程函数只有一个参数，而且其意义由我们（飞操作系统）来定义。
*	  因此，我们不必担心ANSI/Unicode问题。
*	·线程函数必须返回一个值，它会称为该线程的退出代码。这类似于C/C++运行库的策略：令主线程的退出代码成为进程的退出代码。
*	·线程函数（实际包括所有函数）应该尽可能使用函数参数和局部变量。使用静态变量和全局变量时，多个线程可以同时访问这些变量，这样可能会破坏变量中保存的内容。然而，由于函数的参数
*	  和局部变量是在线程栈上创建的。因此，不太可能被其他线程破坏。
* 4）对于cbStackSize参数，CreateProcess使用了保存在可执行文件内部的一个值。可以使用链接器的/STACK开关来控制这个值，如下所示：
*		/STACK:[reserve] [,commit]
*    reserve参数用于设置系统将为线程栈预留多少地址空间，默认是1MB（在Itanium芯片组上，默认大小为4MB）。commit参数指定最初应为栈预留的地址空间区域调拨多少物理存储空间，默认是1个页面。
*	 随着线程中的代码开始执行，需要的存储空间可能不止1个页面。如果线程溢出它的栈，会产生异常。系统将捕获这种异常，并为已预订的空间区域调拨另一个页面（或者在commit参数中指定的任何大小）。
*    这样一来，线程栈就可以根据需要动态地增大。
* 5）线程可以通过以下4种方法来终止运行。
*	 ·线程函数返回（这是强烈推荐的）。
*    ·线程通过调用ExitThread函数“杀死”自己（要避免使用这种方法）。
*    ·同一个进程或另一个进程中的线程调用TerminateThread函数（要避免使用这种方法）。
*    ·包含线程的进程终止运行（这种方法避免使用）。
* 6）让线程函数返回，可以确保以下正确的应用程序清理工作都得以执行。
*	 ·线程函数中创建的所有C++对象都通过其析构函数被正确销毁。
*    ·操作系统正确释放线程栈使用的内存。
*    ·操作系统把线程的退出代码（在线程的内核对象中维护）设为线程函数的返回值。
*    ·系统递减少线程的内核对象的使用计数。
* 7）线程终止运行时，会发生下面这些事情。
*	 ·线程拥有的所有用户对象句柄会被释放。在Windows中，大多数对象都是由包含了“创建这些对象的线程”的进程拥有的。但一个线程有两个用户对象：窗口（Window）和挂钩(hook)。一个线程
*      终止运行时，系统会自动销毁由线程创建或安装的任何窗口，并卸载由线程创建或安装的任何挂钩。其他对象只有在拥有线程的进程终止时才被销毁。
*    ·线程的退出代码从STILL_ACTIVE变成传给ExitThread或TerminateThread的代码。
*    ·线程内核对象的状态变为触发状态。
*    ·如果线程是进程中的最后一个活动线程，系统认为进程也终止了。
*    ·线程内核对象的使用计数递减1。
* 8）系统创建和初始化一个线程必不可少的步骤：
*      对CreateThread函数的一个调用导致系统创建一个线程内核对象。该对象最初的使用计数为2。（除非线程终止，而且从CreateThread返回的句柄关闭，否则线程内核对象不会被销毁。）该线程内核
*    对象的其他属性也被初始化：暂停计数被设为1，退出代码被设为STILL_ACTIVE（0x103），而且对象被设为未触发状态。
*      一旦创建了内核对象，系统就分配内存，供线程的堆栈使用。此内存是从进程的地址空间内分配的，因为线程没有自己的地址空间。然后，系统将两个值写入新线程堆栈的最上端。（线程堆栈始终
     是从高位内存地址向低位内存地址构建的。）写入线程堆栈的第一个值是传给CreateThread函数的pvParam参数的值。紧接在它下方的是传给CreateThread函数的pfnStartAddr值。
	   每个线程都有其自己的一组CPU寄存器，称为线程的上下文（context）。上下文反映了当线程上一次执行时，线程的CPU寄存器的状态。线程的CPU寄存器全部保存在一个CONTEXT结构（在WinNT.h头
	 文件中定义）。CONTEXT结构本身保存在线程内核对象中。
	   指令指针寄存器和栈指针寄存器是线程上下文中最重要的两个寄存器。记住，线程始终在进程的上下文中运行。所以，这两个地址标识的内存都位于线程所在进程的地址空间中。当线程的内核对象
	 被初始化的时候，CONTEXT结构的堆栈指针寄存器被设为pfnStartAddr在线程堆栈中的地址。而指令指针寄存器被设为RtlUserThreadStart函数（该函数未见于正式文档）的地址，此函数是NTDLL.dll
	 模块导出的。
	   RtlUserThreadStart函数执行的操作基本如下：
	   VOID RtlUserThreadStart(PTHREAD_START_ROUTINE pfnStartAddr, PVOID pvParam){
			__try {
			    ExitThread((pfnStartAddr)(pvParam));
			}
			__except(UnhandledExceptionFilter(GetExceptionInformation())){
				ExitProcess(GetExceptionCode);
			}
			// NOTE: We never get here.
	   }
	   线程完全初始化好之后，系统将检查CREATE_SUSPENDED标志是否已被传给CreateThread函数。如果此标记没有传递，系统将线程的挂起计数递减至0；随后，线程就可以调度给一个处理器去执行。
	 然后，系统在实际的CPU寄存器中加载上一次在线程上下文中保存的值。现在，线程可以在其进程的地址空间中执行代码并处理数据了。
* 9）Microsoft Visual Studio附带的C/C++库
*    ——————————————————————————————————————————————————————————————————————
*        库 名 称   |                                           描  述
*    ——————————————————————————————————————————————————————————————————————
*      LibCMt.lib   |  库的静态链接发行版本
*      LibCMtD.lib  |  库的静态链接调试版本
*      MSVCRt.lib   |  导入库，用于动态链接MSVCR80.dll库的发行版本（这是新建项目时的默认库）
*      MSVCRtD.lib  |  导入库，用于动态链接MSVCR80.dll库的调试版本
*      MSVCMRt.lib  |  导入库，用于托管/本机代码混合
*      MSVCURt.lib  |  导入库，编译成百分之百的纯MSIL代码
*    ——————————————————————————————————————————————————————————————————————
* 10）一旦一个数据块被初始化并与线程关联，线程调用的任何需要“每线程实例数据”的C/C++运行库函数都可以轻易获取主调线程的数据块的地址（通过TlsGetValue），并操纵线程的数据。
*/

#include <Windows.h>

class CHYThreadKernelObject
{
public:
	CHYThreadKernelObject();
	~CHYThreadKernelObject();

	// 创建线程
	HANDLE CreateThread(
		LPSECURITY_ATTRIBUTES   lpThreadAttributes,
		SIZE_T                  dwStackSize,
		LPTHREAD_START_ROUTINE  lpStartAddress,
		__drv_aliasesMem LPVOID lpParameter,
		DWORD                   dwCreationFlags,
		LPDWORD                 lpThreadId
	);

	// 运行库创建线程
	uintptr_t _beginthread( // NATIVE CODE
		void(__cdecl* start_address)(void*),
		unsigned stack_size,
		void* arglist
	);
	uintptr_t _beginthreadex( // NATIVE CODE
		void* security,
		unsigned stack_size,
		unsigned(__stdcall* start_address)(void*),
		void* arglist,
		unsigned initflag,
		unsigned* thrdaddr
	);

	// 结束线程
	void _endthread(void);
	void _endthreadex(unsigned retval);

	// 为了强迫线程终止运行，可以让它调用ExitThread
	VOID ExitThread(DWORD dwExitCode);

	// 其它线程“杀死”另一个线程，异步动作，另外DLL不会收到线程退出的通知，其结果是不能执行正常的清理工作。
	BOOL TerminateThread(HANDLE hThread, DWORD dwExitCode);

	// 判断线程是否已终止运行，如果已终止运行，可判断其退出码
	BOOL GetExitCodeThread(HANDLE hThread, PDWORD pdwExitCode);
};


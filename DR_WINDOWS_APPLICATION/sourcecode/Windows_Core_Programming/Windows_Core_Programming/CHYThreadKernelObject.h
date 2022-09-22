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
*    因此，强烈建议使用_beginthreadex来创建线程，而不要用CreateThread。
* 11）线程的伪句柄是一个指向当前线程的句柄；换言之，指向的是发出函数调用的那个线程。
* 12）上下文切换：每个线程都有一个上下文（CONTEXT）,后者保存在线程的内核对象中。这个上下文放映了线程上一次执行时CPU寄存器的状态。大约每隔20ms（GetSystemTimeAdjuestment函数第二个参数的
	 返回值），Windows都会查看所有当前存在的线程内核对象。在这些对象中，只有一些被认为是可调度的。Windows在可调度的线程内核对象中选择一个，并将上次保存在线程上下文中的值载入CPU寄存器。
	 这一波操作被称为上下文切换（context switch）。
* 13）Windows实际上会记录每个线程运行的次数。使用Microsoft Spy++这样的工具可以看到这一点。
* 14）在线程内核对象中有一个值表示线程的挂起计数。调用CreateProcess或者CreateThread时，系统将创建线程内核对象，并把挂起计数初始化为1。这样，就不会给这个线程调度CPU了。这正是我们所希望
*    的，因为线程初始化需要时间，我们当然不想在线程准备好之前就开始执行它。
* 15）超线程（hyper-threading）是Xeon，Pentium4和更新的CPU支持的一种技术。超线程处理器芯片有多个“逻辑”CPU，每个都可以运行一个线程。每个线程都有自己的体系结构状态（一组寄存器），但是
*    所有线程共享主要的执行资源，比如CPU高速缓存。当一个线程中止时，CPU自动执行另一个线程，无须操作系统干预。只有在缓存未命中、 分支预测错误和需要等待前一个指令的结果等情况下，CPU才会
*    暂停。
* 16）在Windows Vista中，系统为线程分配CPU时间的方式发生了变化。操作系统不再依赖约10~15ms的间隔始终计时器，而是改用处理器的64位时间戳计时器（Time Stamp Counter, TSC），它计算的是机器
*    启动以来的时钟周期数。
* 17）在Windows定义的所有数据结构中，CONTEXT结构是唯一一个特定于CPU的。
* 18）在调度程序给另一个可调度线程分配CPU之前，CPU可以运行一个线程大约20ms。这是所有线程优先级都相同的情况。
* 19）每个线程都被赋予0（最低）~31（最高）的优先级数。当系统确定给哪个线程分配CPU时，它会首先查看优先级为31的线程，并以循环（round-robin）的方式进行调度。
* 20）较高优先级的线程总是会抢占较低优先级的线程，无论较低优先级的线程是否正在执行。
* 21）系统启动时，将创建一个名为页面清零线程（zero page thread）的特殊线程。这个线程的优先级定为0，而且是整个系统中唯一一个优先级为0的线程。页面清零线程负责在没有其他进程需要执行的时候，
*     将系统内存中的所有闲置页面清零。
* 22）Windows支持6个进程优先级类（priority class）：idle, below normal, normal, above normal, high和real-time。当然，normal是最常用的优先级类，为99%的应用程序所使用。
*																	进程优先级类
*	  ————————————————————————————————————————————————————————————————————————————————
*			优先级					|															描述
*     ————————————————————————————————————————————————————————————————————————————————
*		real-time（实时）			|	此进程中的线程必须立即响应事件，执行实时任务。此进程中的线程还会抢占操作系统的组件的CPU时间。使用该优先级类需要极为小心
*	  ————————————————————————————————————————————————————————————————————————————————
*       high（高）					|	此进程中的线程必须立即响应事件，执行实时任务。任务管理器运行在这一级，因此用户可以通过它结束失控的进程
*	  ————————————————————————————————————————————————————————————————————————————————
*		above normal（高于标准）	|	此进程中的线程运行在normal和high优先级类之间
*	  ————————————————————————————————————————————————————————————————————————————————
*		normal(标准)				|	此进程中的线程无需特殊的调度
*	  ————————————————————————————————————————————————————————————————————————————————
*		below normal(低于标准)		|	此进程中的线程运行在normal和idle优先级类之间
*	  ————————————————————————————————————————————————————————————————————————————————
*		idle(低)					|	此进程中的线程在系统空闲时运行。屏幕保护程序、后台实用程序和统计数据收集软件通常使用该进程
*	  ————————————————————————————————————————————————————————————————————————————————
* 23）Windows支持7个相对线程优先级：idle，lowest，below normal, normal, above normal, highest和time-critical。这些优先级是相对进程优先级的。
*	  ————————————————————————————————————————————————————————————————————————————————
*			相对线程优先级		|												描述
*	  ————————————————————————————————————————————————————————————————————————————————
*		time-critical			|	对于real-time优先级类，线程运行在31上；所有其他优先级运行在15
*	  ————————————————————————————————————————————————————————————————————————————————
*		highest					|	线程运行在高于normal之上两个级别
*	  ————————————————————————————————————————————————————————————————————————————————
*		above normal			|	线程运行在高于normal之上一个级别
*     ————————————————————————————————————————————————————————————————————————————————
*		normal					|	线程运行在进程normal级别上
*	  ————————————————————————————————————————————————————————————————————————————————
*		below normal			|   线程运行在低于normal之下一个级别
*	  ————————————————————————————————————————————————————————————————————————————————
*		lowest					|	线程运行在低于normal之下两个级别
*	  ————————————————————————————————————————————————————————————————————————————————
*		idle					|   对于real-time优先级，线程运行在16；所有其他优先级运行在1
*	  ————————————————————————————————————————————————————————————————————————————————
*	   应用程序的开发人员无需处理优先级，而是由系统将进程的优先级类和线程的相对优先级映射到一个优先级值。这个映射正是Microsoft不想做什么承诺的地方。事实上，这个映射在
*    操作系统的不同版本中已经发生了变化。
*      Windows Vista上这种映射的具体情况。
*													进程优先级和相对线程优先级与优先级值的映射
*     ________________________________________________________________________________________________________________________________________________________________
*						|															进程优先级类
*		相对线程优先级	|——————————————————————————————————————————————————————————————————————
*						|	idle	|	below normal	|	normal	|	above normal	|	high	|	real-time
*	  __________________|_____________________________________________________________________________________________________________________________________________
*		time-critical	|	15		|	15				|	15		|	15				|	15		|	31
*	  ----------------------------------------------------------------------------------------------------------------------------------------------------------------
*		highest			|	6		|	8				|	10		|	12				|	15		|	26
*	  ----------------------------------------------------------------------------------------------------------------------------------------------------------------
*		above normal	|	5		|	7				|	9		|	11				|	14		|	25
*	  ----------------------------------------------------------------------------------------------------------------------------------------------------------------
*		normal			|	4		|	6				|	8		|	10				|	13		|	24
*	  ----------------------------------------------------------------------------------------------------------------------------------------------------------------
*		below normal	|	3		|	5				|	7		|	9				|	12		|	23
*	  ----------------------------------------------------------------------------------------------------------------------------------------------------------------
*		lowest			|	2		|	4				|	6		|	8				|	11		|	22
*	  ----------------------------------------------------------------------------------------------------------------------------------------------------------------
*		idle			|	1		|	1				|	1		|	1				|	1		|	16
*	  ----------------------------------------------------------------------------------------------------------------------------------------------------------------
*		0优先级保留给页面清零线程了，系统不允许其他任何线程的优先级为0。而且应用程序也无法获得以下优先级：17,18,19,20,21,27,28,29，或者30。当然，如果编写的是运行在内核
*	  模式的设备驱动程序，那么我们可以获得这些优先级；用户模式的应用程序是不能获得这些优先级的。
* 24）动态提升线程优先级
*	  偶尔，系统也会提升一个线程的优先级--通常是为了响应某种I/O事件，比如窗口消息或者磁盘读取。
*	  注意，线程的当前优先级不会低于线程的基本优先级。而且使线程可调度的设备驱动程序能够决定提升的幅度。同样，Microsoft也没有在文档中记录任何一个设备驱动程序能够将线程
*   的优先级提升多少。
*	  系统只提升优先级值在1~15的线程。事实上，这因为如此，这个范围被称为动态优先级范围（dynamic priority range）。而且，系统不会把线程的优先级提升到实时范围（高于15）。
* 25）当系统检测到有线程已经处于饥饿状态3到4秒时，它会动态将饥饿线程的优先级提升为15，并允许该线程运行两个时间片。当两个时间片结束时，线程的优先级立即恢复到基本优先级。
* 26）默认情况下，Windows Vista在给线程分配处理器时，使用软关联（soft affinity）。意思是如果其他因素都一样，系统将使线程在上一次处理的处理器上运行。让线程始终在同一个
*	处理器上运行有助于重用仍在处理器高速缓存中的数据。
* 27）为了支持NUMA(Non-Uniform Memory Access)体系结构，Windows Vista允许我们设置进程和线程的关联性（affinity）。也就是说，我们可以控制CPU让哪些CPU运行特定的线程。这称为
*	硬关联（hard Affinity）。
* 28）当一个x86系统启动时，系统将执行代码，检查主机上的哪个CPU存在著名的Pentium浮点bug。系统必须对每个CPU做这项检查。检查的方法是，将一个进程的关联性设置为该CPU，执行可能
*	会出错的除法操作，然后比较结果是否与已知正确的结果相符。随后再采用同样的步骤检查下一个CPU，以此类推。
* 29）我们还可以在可执行文件的头部设置处理器关联性。奇怪的是，似乎没有哪个链接器开关允许我们这样做，但我们可以使用与下面类似的代码，它利用了ImageHlp.h中声明的一些函数：
*	// load the EXE into memory.
*	PLOADED_IMAGE pLoadedImage = ImageLoad(szExeName, NULL);
* 
*	// Get the current load configuration information for the EXE.
*	IMAGE_LOAD_CONFIG_DIRECTORY ilcd;
*	GetImageConfigInformation(pLoadedImage, &ilcd);
* 
*	// change the processor affinity mask.
*	ilcd.ProcessAffinityMask = 0x00000003;	// I desire CPUs 0 and 1
* 
*	// Save the new load configuration information
*	SetImageConfigInformation(pLoadedImage, &ilcd);
* 
*	// Unload the EXE from memory
*	ImageUnload(pLoadedImage);
* 30）当Windows Vista在x86计算机上启动时，我们可以限制系统将使用的CPU数量。在启动过程中，系统将检查启动配置数据（boot configuration data, BCD），BCD是一个取代老的boot.ini
*	文本文件的数据存库（data store），它是计算机的硬件和固件之上提供了一个抽象层。
*/

#include <Windows.h>

namespace HY_KERNELOBJECT
{
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

		// 获取主调线程的进程内核对象伪句柄（pseudohandle）。它们不会在主调进程的句柄表中新建句柄。而且，调用这两个函数，不会影响进程内核对象或线程内核对象的使用计数
		HANDLE GetCurrentProcess();

		// 获取主调线程的线程内核对象的一个伪句柄
		HANDLE GetCurrentThread();

		// 一个线程查询所在进程的使用时间
		BOOL GetProcessTimes(
			HANDLE     hProcess,
			LPFILETIME lpCreationTime,
			LPFILETIME lpExitTime,
			LPFILETIME lpKernelTime,
			LPFILETIME lpUserTime
		);

		// 一个线程可以查询自己的线程时间
		BOOL GetThreadTimes(
			HANDLE     hThread,
			LPFILETIME lpCreationTime,
			LPFILETIME lpExitTime,
			LPFILETIME lpKernelTime,
			LPFILETIME lpUserTime
		);

		// 将一个伪句柄转换为真正的句柄
		BOOL DuplicateHandle(
			HANDLE hSourceProcess,
			HANDLE hSource,
			HANDLE hTargetProcess,
			PHANDLE phTarget,
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			DWORD dwOptions
		);

		// 确定系统是否对其时间时钟应用定期时间调整，并获取任何此类调整的值和周期，获取线程调度的时间间隔
		BOOL GetSystemTimeAdjustment(
			PDWORD lpTimeAdjustment,
			PDWORD lpTimeIncrement,
			PBOOL  lpTimeAdjustmentDisabled
		);

		// 挂起一个线程
		DWORD SuspendThread(HANDLE hThread);

		// 恢复挂起的线程，恢复次数需要对应挂起次数
		DWORD ResumeThread(HANDLE hThread);

		// 没有十全十美的SuspendProcess函数，但是我们可以创建一个适用于许多情况的版本
		VOID SuspendProcess(DWORD dwProcessID, BOOL fSuspend);

		// 睡眠
		VOID Sleep(DWORD dwMilliseconds);

		// 切换到另一个线程
		BOOL SwitchToThread();

		// 检索自系统启动以来经过的毫秒数
		ULONGLONG GetTickCount64();

		// 获取文件时间的整数形式
		__int64 FileTimeToQuadWord(PFILETIME pft);

		// 检索指定线程所用的时钟周期数
		BOOL QueryThreadCycleTime(HANDLE ThreadHandle, PULONG64 CycleTime);

		// 检索指定进程所用的时钟周期数
		BOOL QueryProcessCycleTime(HANDLE ProcessHandle, PULONG64 CycleTime);

		// 检索性能计数器的频率
		BOOL QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency);

		// 检索性能计数器的当前值，该值是可用于时间间隔测量的高分辨率（<1us）时间戳
		BOOL QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount);

		/*
		* 功能：获取CPU处理器频率
		* 注意：处理器频率是随时间而不断变化的，具体值取决于终端用户选择的设置，以及计算机是否接上了电源。最后要说的是，再多处理器计算机上，可以给线程分配频率有差异的不同处理器。
		*/
		DWORD GetCPUFrequencyInMHz();

		// 获取当前CPU寄存器状态的集合
		BOOL GetThreadContext(HANDLE hThread, PCONTEXT pContext);

		// 设置当前CPU寄存器状态
		BOOL SetThreadContext(HANDLE hThread, CONST CONTEXT* pContext);

		// 设置线程相对优先级（注意：CreateThread总是创建相对线程优先级为normal的新线程。）
		BOOL SetThreadPriority(HANDLE hThread, int nPriority);

		// 获取线程相对优先级
		int GetThreadPriority(HANDLE hThread);

		// 允许或者禁止系统提升一个进程中所有线程的优先级
		BOOL SetProcessPriorityBoost(HANDLE hProcess, BOOL bDisablePriorityBoost);

		// 允许或者禁止提升某个线程的优先级
		BOOL SetThreadPriorityBoost(HANDLE hThread, BOOL bDisablePriorityBoost);

		// 判断是否启用进程优先级提升
		BOOL GetProcessPriorityBoost(HANDLE hProcess, PBOOL pbDisablePriorityBoost);

		// 判断是否启用线程优先级提升
		BOOL GetThreadPriorityBoost(HANDLE hThread, PBOOL pbDisablePriorityBoost);

		// normal优先级线程还可以执行对某个文件执行后台优先级I/O
		BOOL SetFileInformationByHandle(
			HANDLE                    hFile,
			FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
			LPVOID                    lpFileInformation,
			DWORD                     dwBufferSize
		);

		// 查询当前计算机系统的信息
		void GetSystemInfo(LPSYSTEM_INFO lpSystemInfo);

		// 限制某些线程只在可用CPU的一个子集上运行，子进程将继承进程关联性
		BOOL SetProcessAffinityMask(HANDLE hProcess, DWORD_PTR dwProcessAffinityMask);

		// 返回进程的关联性掩码
		BOOL GetProcessAffinityMask(HANDLE hProcess, PDWORD_PTR pdwProcessAffinityMask, PDWORD_PTR pdwSystemAffinityMask);

		// 分别设置各线程的关联性掩码，dwThreadAffinityMask必须是进程关联性掩码的真子集
		DWORD_PTR SetThreadAffinityMask(HANDLE hThread, DWORD_PTR dwThreadAffinityMask);

		// 给线程设置一个理想的CPU，可以调用SetThreadIdealProcessor
		DWORD SetThreadIdealProcessor(HANDLE hThread, DWORD dwIdealProcessor);
	};

	class CStopwatch {
	public:
		CStopwatch()
		{
			QueryPerformanceFrequency(&m_liPerfFreq);
			Start();
		}

		void Start()
		{
			QueryPerformanceCounter(&m_liPerfStart);
		}

		__int64 Now() const
		{
			LARGE_INTEGER liPerfNow;
			QueryPerformanceCounter(&liPerfNow);
			return (((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) / m_liPerfFreq.QuadPart);
		}

		__int64 NowInMicro() const
		{
			LARGE_INTEGER liPerfNow;
			QueryPerformanceCounter(&liPerfNow);
			return (((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000000) / m_liPerfFreq.QuadPart);
		}

	private:
		LARGE_INTEGER m_liPerfFreq;		// Counts per second
		LARGE_INTEGER m_liPerfStart;	// Starting count
	};
}
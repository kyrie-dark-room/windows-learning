#pragma once
#include <Windows.h>
/*
* 知识：
* 1）背景：
*    为了帮助各公司更快地、正确的将它们的代码移植到Windows，Microsoft在操作系统中增加了纤程。记住，我们当然应该尽量避免使用纤程，尽量对应用程序进行合理的设计，使之能够使用Windows提供的线    程。
* 2）纤程是在用户模式下实现的，内核对纤程一无所知，内核会根据我们定义的算法来对纤程进行调度。由于纤程调用算法是我们定义的，因此就内核而言，它对纤程的调度不是抢占式的。另一点是，一个线程
*   可以包含一个或多个纤程。就内核而言，它会对线程进行抢占式调度来让线程执行代码。但是，线程一次只能执行一个纤程的代码--哪个纤程由我们决定。
* 3）如果我们的纤程（线程）返回，或调用ExitThread，那么纤程和线程都将结束。
* 4）如果需要为每个纤程保存一些信息，那么我们可以使用纤程局部存储区（Fiber Local Storage, FLS）函数。这些函数对纤程所做的操作与TLS函数对线程所做的操作相同。
*/

namespace HY_KERNELOBJECT
{
	class CHYFiber
	{
	public:
		CHYFiber();
		virtual ~CHYFiber();

		// 将一个已有的线程转换为一个纤程
		PVOID ConvertThreadToFiber(PVOID pvParam);

		// 在默认情况下，在x86系统中，CPU的浮点数信息不属于CPU寄存器的一部分，不会每个纤程都维护一份，因此如果纤程需要执行浮点操作，那将会导致数据被破坏。为了覆盖系统默认行为，我们应该
		// 调用扩展后的函数
		PVOID ConvertThreadToFiberEx(PVOID pvParam, DWORD dwFlags);

		// 为了创建一个另一个纤程，线程（即当前正在运行的那个纤程）应该调用
		PVOID CreateFiber(DWORD dwStackSize, PFIBER_START_ROUTINE pfnStartAddress, PVOID pvParam);

		// 在默认情况下，函数会创建一个一开始只调拨了两个物理存储页（4KB*2）的栈，但栈的大小最大可以增长到1MB。如果指定的是非零值，那么函数会用这个指定的大小来预定栈区域并为之调拨物理存储器。如果要使用大量的纤程，那么我们可能希望纤程栈消耗更少的内存。
		PVOID CreateFiberEx(SIZE_T dwStackCommitSize, SIZE_T dwStackReserveSize, DWORD dwFlags, PFIBER_START_ROUTINE pfnStartAddress, PVOID pvParam);

		// 在一个线程中，同一时刻只能执行一个线程。为了让新的纤程执行，我们应该调用
		VOID SwitchToFiber(PVOID pvFiberExecutionContext);

		// 销毁纤程
		VOID DeleteFiber(PVOID pvFiberExecutionContext);

		// 当所有纤程都被删除的时候，我们可以通过调用ConvertFiberToThread，来解除线程的纤程状态（该线程原来是因为调用了ConvertThreadToFiber(Ex)而变成纤程的），同时释放将线程转换为纤程
		// 所占用的最后一块内存
		BOOL ConvertFiberToThread(void);

		// 分配一个FLS槽，使得当前进程中正在运行的所有纤程都可以使用该FLS槽
		DWORD FlsAlloc(PFLS_CALLBACK_FUNCTION lpCallback);
		// 当纤程被销毁，或当FLS槽由于FlsFree调用而被删除的时候，该回调函数会被调用
		BOOL FlsFree(DWORD dwFlsIndex);

		// 保存与每个纤程相关的数据
		BOOL FlsSetValue(DWORD dwFlsIndex, PVOID lpFlsData);
		// 获取数据
		PVOID FlsGetValue(DWORD dwFlsIndex);

		// 如果需要知道我们是否正在一个纤程执行上下文中运行，那么我们只需要调用
		BOOL IsThreadAFiber(void);

		// 得到当前正在运行的纤程的执行上下文的地址
		PVOID GetCurrentFiber(void);

		// 另一个便利的函数
		PVOID GetFiberData(void);
	};
}


#pragma once

/*
* 知识：
* 1）在以下两种情况下，线程之间需要相互通信。
*	   ● 需要让多个线程同时访问一个共享资源，同时不能破坏资源的完整性。
*	   ● 一个线程需要通知其他线程某项任务已经完成。
* 2）这些Interlocked函数如何工作，答案取决于代码运行的CPU平台。如果是x86系列CPU，那么Interlocked函数会在总线上维持一个硬件信号，这个信号会阻止其他CPU访问同一个内存地址。
* 3）我们必须保证传递给这些函数的变量地址是经过对齐的，否则这些函数可能会失败。
* 4）Interlocked函数执行得极快。调用一次通常只占用几个CPU周期（通常小于50），而且也不需要在用户模式和内核模式之间进行切换（这个切换通常需要占用1000个CPU周期以上）。
* 5）高速缓存行：
*	  如果想为装配有多处理器的机器构建高性能应用程序，那么应该注意高速缓存行。当CPU从内存中读取一个字节的时候，它并不只是从内存中取回一个字节，而是取回一个高速缓存行。
*	高速缓存行可能包含32字节（老式CPU），64字节，甚至是128字节（取决于CPU）,它们始终都对齐到32字节边界，64字节边界，或128字节边界。高速缓存行存在的目的是为了提高性能。
*	一般来说，应用程序会对一组相邻的字节进行操作。如果所有字节都在高速缓存中，那么CPU就不必访问内存总线，后者耗费的时间比前者耗费的时间要多得多。
*	  多处理器下会涉及到数据同步问题。当然，CPU芯片的设计者非常清楚这个问题，并做了专门的设计来对它进行处理。明确的说，当一个CPU修改了高速缓存行中的一个字节时，机器中
*   的其他CPU会收到通知，并使自己的高速缓存行作废。
*	  这一切都意味着我们应该根据高速缓存行的大小来将应用程序的数据组织在一起，并将数据与缓存行的边界对齐。这样做的目的是为了确保不同的CPU能够各自访问不同的内存地址，
*   而且这些地址不在同一个高速缓存中。此外，我们应该把只读数据（或不经常读的数据）与可读写数据分别存放。我们还应该把差不多会在同一时间访问的数据组织在一起。
*     要想确定CPU高速缓存行的大小，最简单的方法是调用Win32的GetLogicalProcessorInformation函数。一旦有了CPU的高速缓存行的大小。我们可以使用C/C++编译器的_declspec(align(#))
*   指示符来对字段对齐加以控制。
* 6）volatile关键字告诉编译器这个变量可能会被应用程序之外的其他东西修改，比如操作系统、硬件或者一个并发执行的线程。确切地说，volatile限定符告诉编译器不要对这个变量进行任何
*   形式的优化，而是始终从变量在内存中的位置读取变量的值。如果给一个结构加volatile限定符等于给结构中所有的成员都加volatile限定符，这样可以确保任何一个成员始终都是从内存中读取的。
* 7）关键段（critical section）是一小段代码，它在执行之前需要独占对一些共享资源的访问权。这种方式可以让多行代码以“原子方式”来对资源进行操控。这里的原子方式，指的是代码知道除了
*     当前线程之外，没有其他任何线程会同时访问该资源。当然，系统仍然可以暂停当前线程去调度其他线程。但是，在当前线程离开关键段之前，系统是不会去调度任何想要访问统一资源的其他线程的。
*     当不能用Interlocked函数解决同步问题的时候，我们应该试一试关键段。关键段的最大好处在于它们非常容易使用，而且它们在内部也使用了Interlocked函数，因此执行速度非常快。关键段的
*   最大缺点在于它们无法用来在多个进程之间对线程进行同步。
*/

#include <Windows.h>

namespace HY_KERNELOBJECT
{
	class CHYThreadSynchronizationInUserMode
	{
	public:
		CHYThreadSynchronizationInUserMode();
		~CHYThreadSynchronizationInUserMode();

		// 原子方式给一个值加1
		LONG InterlockedIncrement(LONG volatile* plAddend);
		// 减1函数
		LONG InterlockedDecrement(LONG volatile* plAddend);

		// 原子方式递增操作，如果做减法，只需要在第二个参数中传入一个负数。
		LONG InterlockedExchangeAdd(PLONG volatile plAddend, LONG lIncrement);
		LONGLONG InterlockedExchangeAdd64(PLONGLONG volatile pllAddend, LONGLONG llIncrement);

		// C运行库提供了一个_aligned_malloc函数，我们可以用这个函数来分配一块对齐过的内存。下面是函数的原型：
		void* _aligned_malloc(size_t size, size_t alignment);

		// 原子交换函数
		LONG InterlockedExchange(PLONG volatile plTarget, LONG lValue);
		LONGLONG InterlockedExchange64(PLONGLONG volatile pllTarget, LONGLONG llValue);
		PVOID InterlockedExchangePointer(PVOID* volatile ppvTarget, PVOID pvValue);

		// 以原子方式执行一个测试和设置操作
		LONG InterlockedCompareExchange(LONG volatile* Destination, LONG lExchage, LONG lCompared);
		PVOID InterlockedCompareExchangePointer(PVOID* ppvDestination, PVOID pvExchage, PVOID pvComparand);
		// 处理已对齐的64位值
		LONGLONG InterlockedCompareExchange64(PLONGLONG pllDestination, LONGLONG llExchage, LONGLONG llCompared);

		// Interlocked单向链表函数
		void InitializeSListHead(PSLIST_HEADER ListHead);
		PSLIST_ENTRY InterlockedPushEntrySList(PSLIST_HEADER ListHead, __drv_aliasesMem PSLIST_ENTRY ListEntry);
		PSLIST_ENTRY InterlockedPopEntrySList(PSLIST_HEADER ListHead);
		PSLIST_ENTRY InterlockedFlushSList(PSLIST_HEADER ListHead);
		USHORT QueryDepthSList(PSLIST_HEADER ListHead);

		// 获取CPU高速缓存行大小
		WORD GetHighCacheLine(void);
	};
}


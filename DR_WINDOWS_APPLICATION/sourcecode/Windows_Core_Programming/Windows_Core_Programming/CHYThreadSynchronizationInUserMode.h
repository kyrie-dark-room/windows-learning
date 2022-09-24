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
* 8）当线程试图进入一个关键段，但这个关键段正被另一个线程占用的时候，函数会立即把调用线程切换到等待状态。这意味着线程必须从用户模式切换到内核模式（大约1000个CPU周期），这个切换
*   的开销非常大。在配有多处理器的机器上，当前占用资源的线程可能在另一个处理器之上，而且可能很快就会结束对资源的访问。事实上，在需要等待的线程完全切换到内核模式之前，占用资源的
*   线程可能就已经释放了资源。如果发生这种情况，那会浪费大量的CPU时间。
*	 为了提高关键段的性能，Microsoft把旋转锁合并到了关键段中。因此，当调用EnterCriticalSection的时候，它会用一个旋转锁不断地循环，尝试在一段时间内获得对资源的访问权。只有当尝试
*   失败的时候，线程才会切换到内核模式并进入等待状态。
* 9）InitializeCriticalSection函数会失败，不过可能性很小。这个函数可能会失败的原因是它会分配一块内存，这样系统就可以提供一些内部调试信息，如果内存分配失败，那么函数会抛出STATUS_NO_MEMORY
*	异常。
* 10）在使用关键段的时候还可能产生另一个问题。如果有两个或两个以上线程在同一时刻争夺同一个关键段，那么关键段会在内部使用一个事件内核对象。由于争夺现象很少发生，因此只有当第一次要用到
*   事件内核对象的时候，系统才会创建它。因为大多数关键段从来都不会发生争夺现象，所以这样做可以节省大量的系统资源。顺便提一下，只有在调用DeleteCriticalSection的时候，系统才会释放
*   这个事件内核对象。因此，当用完关键段之后，绝对不应该忘记调用DeleteCriticalSection。
* 11）Slim读/写锁
*	  SRWLock允许我们区分那些想要读取资源的值的线程（读取者线程）和想要更新资源的值的线程（写入者线程）。让所有的读取者线程在同一时刻访问共享资源应该是可行的，这是因为仅仅读取
*	资源的值并不存在破坏数据的风险。只有当写入者线程想要对资源进行更新的时候才需要进行同步。这种情况下，写入者线程应该独占对资源的访问权：任何其他线程，无论是读取者线程还是写入者
*   线程，都不允许访问资源。这就是SRWLock提供的全部功能。
*	  SRWLOCK不需要删除或销毁，系统会自动执行清理工作。
*	  SRWLOCK不存在TryEnter也不能递归获取。
* 12）多种线程同步方式性能测试对比：双处理器
*														同步机制的性能比较
*	  ————————————————————————————————————————————————————————————————————
*		线程\   |	Volatile	|	Volatile	|	Interlocked		|	关键段	|	SRWLock		|	SRWLock		|	互斥量
		微秒	|	   读取		|	  写入		|	   递增			|			|   共享模式	|	独占模式	|
*	  ————————————————————————————————————————————————————————————————————
*		1		|		8		|		8		|		35			|	  66	|		66		|		67		|	1060
*     ————————————————————————————————————————————————————————————————————
*		2		|		8		|		76		|		153			|	  268	|		134		|		148		|	11082
*     ————————————————————————————————————————————————————————————————————
*		4		|		9		|		145		|		361			|	  768	|		244		|		307		|	23785
*     ————————————————————————————————————————————————————————————————————
*     SRWLock的性能与关键段的性能差不多旗鼓相当。事实上，从上面的结果我们可以看到，在许多测试中SRWLock的性能要胜过关键段。有鉴于此，我建议用SRWLock来代替关键段。
*   SRWLock不仅更快，而且允许多个线程同时读取，对于哪些只需要读取共享资源的线程来说，这提高了吞吐量和可伸缩性。
* 13）总结一下，如果希望在应用程序中得到最佳性能，那么首先应该尝试不要共享数据，然后一次使用volatile读取，volatile写入，Interlocked API，SRWLock以及关键段。当且
*   仅当所有这些都不能满足要求的时候，再使用内核对象。
* 14）某些情况下，如果读取者线程没有数据可以读取，那么它应该将锁释放并等待，直到写入者线程产生了新的数据为止。如果用来接受写入者线程产生的数据的数据结构已满，那么写入者线程
*   同样应该释放SRWLock并进入睡眠状态，直到读取者线程把数据结构清空为止。
*	  有时我们想让线程以原子方式把锁释放并将自己阻塞，直到某一个条件成立为止。要实现这样的线程同步是比较复杂的。Windows通过SleepConditionVariableCS或SleepConditionVariable
*   函数，提供了一种条件变量，来帮助我们简化这种情形下所需的工作。
*	  当另一个线程检测到相应的条件以及满足的时候，比如存在一个元素可让读取者线程读取，或者有足够的空间让写入者线程插入新的元素，它会调用WakeConditionVariable或WakeAllConditionVariable，
*   这样阻塞在Sleep*函数中的线程就会被唤醒。
* 15）当我们在使用锁的时候，比如关键段或读取者/写入者锁，应该养成一些良好的习惯并避免一些不太好的做法。下面几个窍门和技巧会对锁的使用所有帮助。这些技巧也同样适用于内核同步对象。
*		a.以原子方式操作一组对象时使用一个锁；
*		b.同时访问多个逻辑资源时候要格外注意；
*		c.不要长时间占用锁。
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

		// 释放锁，并在一个条件变量上等待
		BOOL SleepConditionVariableCS(PCONDITION_VARIABLE pConditionVariable, PCRITICAL_SECTION pCriticalSection, DWORD dwMilliseconds);
		BOOL SleepConditionVariableSRW(PCONDITION_VARIABLE pConditionVariable, PSRWLOCK pSRWLock, DWORD dwMilliseconds, ULONG Flags);

		// 唤醒等待条件变量的线程
		VOID WakeConditionVariable(PCONDITION_VARIABLE ConditionVariable);
		VOID WakeAllConditionVariable(PCONDITION_VARIABLE ConditionVariable);
	};
}


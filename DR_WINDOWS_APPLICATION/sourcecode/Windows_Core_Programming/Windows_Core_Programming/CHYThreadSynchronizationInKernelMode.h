#pragma once

#include <Windows.h>
/*
* 知识：
* 1）实际上，内核对象唯一的缺点就是它们的性能。当我们调用本章介绍的任何一个新函数时，调用线程必须从用户模式切换到内核模式。这种切换时非常耗时的：在x86平台上，一个空的系统
*   调用大概会占用200个CPU周期--当然，这还不包括执行被调用函数在内核模式下的实现代码。但是，造成内核对象比用户模式下的同步机制慢几个数量级的原因，是伴随调度新线程而来的
*   刷新高速缓存以及错过高速缓存（即未命中）。这里我们谈论的是成百上千个CPU周期。
* 2）下面的内核对象既可以处于触发状态，也可以处于未触发状态：
*		·进程
*		·线程
*		·作业
*		·文件以及控制台的标准输入流/输出流/错误流
*		·事件
*		·可等待的计时器（waitable timer）
*		·信号量
*		·互斥量
* 3）对一些内核事件来说，成功地调用WaitForSingleObject或WaitForMultipleObjects事实上会改变对象的状态。如果对象的状态发生了变化，我则称之为等待成功所引起的副作用
*	（successful wait side effects）。
* 4）WaitForMultipleObjects之所以这么有用的原因，是因为它能够以原子方式执行所有操作。当线程调用WaitForMultipleObjects的时候，函数会测试所有对象的触发状态，并引发
*   相应的副作用，所有这些都是作为一个操作来完成的。
* 5）如果多个线程等待同一个内核对象，那么当对象被触发的时候，系统如何决定应该唤醒哪个线程？Microsoft的官方回答是“算法是公平的”。Microsoft并不想对系统内部使用的算法
*   做任何承诺。它只是说算法是公平的，也就是说如果有多个线程在等待，那么每次当对象被触发的时候，每个线程都有机会被唤醒。
* 6）可等待的计时器是这样一种内核对象，它们会在某个指定的时间触发，或每隔一段时间触发一次。
* 7）如果我们希望时间一到就让计时器把一个APC添加到队列中去，就必须实现一个计时器APC函数，并把函数的地址传入。计时器被触发的时候，当且仅当SetWaitableTimer的调用线程
*   正处于可提醒状态（alertable state）时，这个函数会被同一个线程调用。换句话说，线程必须是由于调用SleepEx、WaitForSingleObjectEx、WaitForMultipleObjectsEx、
*	MsgWaitForMultipleObjectsEx或SignalObjectAndWait而进入的等待状态。
*    只有当所有的APC函数都处理完毕后，才会返回可警告函数（alertable function）。因此，我们必须确保自己的TimerAPCRoutine函数会在计时器再次被触发之前结束，不然APC函数
*   加入队列的速度就快过了处理它们的速度。
*    线程不应该在等待一个计时器句柄的同时以可提醒的方式等待同一个计时器。
* 8）与用户计时器（通过SetTimer函数来设置）比较。两者最大的区别在于用户计时器需要在应用程序中使用大量的用户界面基础设施，从而消耗更多的资源。此外，可等待计时器是内核对象，
*   这意味着它们不仅可以在多个线程间共享，而且可以具备安全性。
* 9）信号量内核对象用来对资源进行计数。与其他所有内核对象相同，它们也包含一个使用计数，但它们还包含另外两个32位值：一个最大资源计数和一个当前资源计数。最大资源计数表示信号量
*   可以控制的最大资源数量，当前资源计数表示信号量当前可用资源的数量。
* 10）信号量的规则如下：
*		·如果当前资源计数大于0，那么信号量处于触发状态
*		·如果当前资源计数等于0，那么信号量处于未触发状态
*		·系统绝对不会让当前资源计数变为负数
*		·当前资源计数绝对不会大于最大资源计数
* 11）信号量最大的优势在于它们会以原子方式来执行这些测试和设置操作，也就是说，当我们向信号量请求一个资源的时候，操作系统会检查资源是否可用，并将可用资源的数量递减，整个过程
*	不会被别的线程打断。只有当资源计数递减完成之后，系统才会允许另一个线程请求对资源的访问。
* 12）我们没有办法在不改变当前信号量资源计数的情况下来得到它的当前可用资源的值。
* 13）互斥量（mutex）内核对象用来确保一个线程独占对一个资源的访问。实际上，这也是互斥量名字的由来。互斥量对象包含一个使用计数、线程ID以及一个递归计数。线程ID用来标识当前占用
*	这个互斥量的是系统中的哪个线程，递归计数表示这个线程占用该互斥量的次数。互斥量有许多用途，它们是使用最为频繁的内核对象之一。
* 14）下面是互斥量的规则：
*		·如果线程ID为0（无效线程ID），那么该互斥量不为任何线程所占用，它处于触发状态
*		·如果线程ID为非零值，那么有一个线程已经占用了该互斥量，它处于未触发状态
*		·与所有其他内核对象不同，操作系统对互斥量进行了特殊处理，允许它们违反一些常规的规则
* 15）在用来触发普通内核对象和撤销触发普通内核对象的规则中，有一条不适用于互斥量。假设线程试图等待一个未触发的互斥量对象。在这种情况下，线程通常会进入等待状态。但是，系统会
*	检查想要获得互斥量的线程的线程ID与互斥量对象内部记录的线程ID是否相同。如果线程ID一致，那么系统会让线程保持可调度状态--即使该互斥量尚未触发。对系统中的任何其他内核对象来说，
*   我们都找不到这种“异常”的举动。每次线程成功地等待了一个互斥量，互斥量对象的递归计数会递增。使递归计数大于1的唯一途径是利用这个例外，让线程多次等待同一个互斥量。
* 16）如果占用互斥量的线程在释放互斥量之前终止，那对互斥量和正在等待该互斥量的线程来说，会发生什么情况呢？
*	答案是：系统认为互斥量被遗弃（abandoned）-- 由于占用互斥量的线程已经终止，因此再也无法释放它。当互斥量被遗弃的时候，系统会自动将互斥量对象的线程ID设为0，将它的递归计数
    设为0。
	  然后其他等待互斥量的线程可以被调度，唯一不同的是，等待函数返回的不是通常的WAIT_OBJECT_0，而是返回一个特殊的值WAIT_ABANDONED。这个特殊的返回值（只适用于互斥量）表示线程
	正在等待的互斥量为其他线程所占用，但该线程在完成对共享资源的使用之前终止了。这种情况有些尴尬。刚获得互斥量的线程并不知道资源目前处于什么状态--它可能已经被完全破坏了。在
	这种情况下，我们的应用程序必须自己决定该怎么做。
*/
namespace HY_KERNELOBJECT
{
	class CHYThreadSynchronizationInKernelMode
	{
	public:
		CHYThreadSynchronizationInKernelMode();
		~CHYThreadSynchronizationInKernelMode();

		// 等待函数
		DWORD WaitForSingleObject(HANDLE hObject, DWORD dwMilliseconds /* INFINITE */);
		DWORD WaitForMultipleObjects(DWORD dwCount, CONST HANDLE* phObjects, BOOL bWaitAll, DWORD dwMilliseconds);

		/******** 事件 ********/
		// 创建一个事件内核对象
		HANDLE CreateEvent(PSECURITY_ATTRIBUTES psa, BOOL bManualReset, BOOL bInitialState, PCTSTR pszName);
		HANDLE CreateEventEx(PSECURITY_ATTRIBUTES psa, PCTSTR pszName, DWORD dwFlags, DWORD dwDesiredAccess);
		// 打开已经存在的事件
		HANDLE OpenEvent(DWORD dwDesiredAccess, BOOL bInherit, PCTSTR pszName);
		// 关闭事件
		BOOL CloseHandle(HANDLE hEventObject);
		// 把事件变成触发状态
		BOOL SetEvent(HANDLE hEvent);
		// 把事件变成未触发状态
		BOOL ResetEvent(HANDLE hEvent);
		// 脉冲事件，会先触发事件然后立刻将其恢复到未触发状态
		BOOL PulseEvent(HANDLE hEvent);

		/******** 可等待的计时器内核对象 ********/
		// 创建一个可等待计时器
		HANDLE CreateWaitableTimer(PSECURITY_ATTRIBUTES psa, BOOL bManualReset, PCTSTR pszName);
		// 打开一个已经存在的可等待计时器的句柄，该句柄与当前进程相关联
		HANDLE OpenWaitableTimer(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName);
		// 在创建的时候，可等待的计时器对象总是处于未触发状态。当我们想要触发计时器的时候，必须调用SetWaitableTimer函数来告诉它
		BOOL SetWaitableTimer(HANDLE hTimer, const LARGE_INTEGER *pDueTime, LONG lPeriod, PTIMERAPCROUTINE pfnCompletionRoutine, PVOID pvArgToCompletionRoutine, BOOL bResume);
		// 取消计时器
		BOOL CancelWaitableTimer(HANDLE hTimer);

		/******** 信号量内核对象 ********/
		// 创建一个信号量内核对象
		HANDLE CreateSemaphore(PSECURITY_ATTRIBUTES psa, LONG lInitialCount, LONG lMaximumCount, PCTSTR pszName);
		HANDLE CreateSemaphoreEx(PSECURITY_ATTRIBUTES psa, LONG lInitialCount, LONG lMaximumCount, PCTSTR pszName, DWORD dwFlags, DWORD dwDesiredAccess);
		// 打开一个已经存在的信号量的句柄
		HANDLE OpenSemaphore(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName);
		// 递增信号量的当前资源计数
		BOOL ReleaseSemaphore(HANDLE hSemaphore, LONG lReleaseCount, PLONG plPreviousCount);

		/******** 互斥量内核对象 ********/
		// 创建一个互斥量内核对象
		HANDLE CreateMutex(PSECURITY_ATTRIBUTES psa, BOOL bInitialOwner, PCTSTR pszName);
		HANDLE CreateMutexEx(PSECURITY_ATTRIBUTES psa, PCTSTR pszName, DWORD dwFlags, DWORD dwDesiredAccess);
		// 打开一个已经存在的互斥量句柄
		HANDLE OpenMutex(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName);
		// 释放互斥量
		BOOL ReleaseMutex(HANDLE hMutex);

	private:

	};
}


#pragma once
#include <Windows.h>

/*
* 知识：
* 1）为了简化开发人员的工作，Windows提供了一个（与I/O完成端口相配套的）线程池机制来简化线程的创建、销毁以及日常管理。
*    这些新的线程池函数允许我们做以下这些事情：
*		·以异步的方式来调用一个函数
*		·每隔一段时间调用一个函数
*		·当内核对象触发的时候调用一个函数
*		·当异步I/O请求完成的时候调用一个函数
* 2）为了用线程池来以异步的方式执行一个函数，我们需要定义一个具有以下原型的函数：
*		VOID NTAPI SimpleCallback(
*			PTR_CALLBACK_INSTANCE pInstance,
*			PVOID pvContext);
*    然后为了让线程池中的一个线程执行该函数，我们需要向线程池提交一个请求。为了达到这个目的，我们需要调用下面的函数：
*		BOOL TrySubmitThreadpoolCallback(
*			PTP_SIMPLE_CALLBACK pfnCallback,
*			PVOID pvContext,
*			PTP_CALLBACK_ENVIRON pcbe);
*    注意，我们从来不需要自己调用CreateThread。系统会自动为我们的进程创建一个默认的线程池，并让线程池中的一个线程来调用我们的回调函数。
* 3）每一次调用TrySubmitThreadpoolCallback的时候，系统会在内部以我们的名义分配一个工作项。如果打算提交大量的工作项，那么处于对性能和内存使用的考虑，创建工作项一次，然后分多次提交它会更好。
*	 我们调用下面的函数来创建一个工作项：
*		PTP_WORK CreateThreadpoolWork(
*			PTP_WORD_CALLBACK pfnWorkHandler,
*			PVOID pvContext,
*			PTP_CALLBACK_ENVIRON pcbe);
*    当我们想要向线程池提交一个请求的时候，可以调用SubmitThreadpoolWork函数：VOID SubmitThreadpoolWork(PTP_WORK pWork);
*	 如果我们有另一个线程，该线程想要取消已经提交的工作项，或者该线程由于要等待工作项处理完毕而需要将自己挂起，那么可以调用下面的函数：
*		VOID WaitForThreadpoolWorkCallbacks(
*			PTP_WORK pWork,
*			BOOL bCancelPendingCallbacks);
*    不再需要一个工作项的时候，我们应该调用CloseThreadpoolWork并在它唯一的参数中传入指向该工作项的指针：VOID CloseThreadpoolWork(PTP_WORK pwk);
* 4）为了将一个工作项安排在某个时间执行，我们必须定义一个回调函数，它的原型如下：
*		VOID CALLBACK TimeoutCallback(
			PTP_CALLBACK_INSTANCE pInstance, PVOID pvContext, PTP_TIMER pTimer);
	 然后调用下面的函数来通知线程池应该在何时调用我们的函数：
		PTP_TIMER CreateThreadpoolTimer(
			PTP_TIMER_CALLBACK pfnTimerCallback, PVOID pvContext, PTP_CALLBACK_ENVIRON pcbe);
	 当我们想要向线程池注册计时器的时候，应该调用SetThreadpoolTimer函数：
		VOID SetThreadpoolTimer(
			PTP_TIMER pTimer, PFILETIME pftDueTime, DWORD msperiod, DWORD msWindowLength);
	 同时，我们可以通过调用IsThreadpoolTimerSet来确定某个计时器是否已经被设置（也就是说，它的pftDueTime值不为NULL）:
	    BOOL IsThreadpoolTimerSet(PTP_TIMER pti);
	 最后，我们可以通过调用WaitForThreadpoolTimerCallbacks来让线程等待一个计时器完成，还可以通过调用CloseThreadpoolTimer函数来释放计时器的内存。
* 5）如果想要注册一个工作项，让它在一个内核对象被触发的时候执行，首先，编写一个符合下面的原型的函数：
*		VOID CALLBACK WaitCallback(
*			PTP_CALLBACK_INSTANCE pInstance, PVOID Context, PTP_WAIT Wait, TP_WAIT_RESULT WaitResult);
*	 然后通过调用CreateThreadpoolWait来创建一个线程池等待对象：
*		PTP_WAIT CreateThreadpoolWait(
*			PTP_WAIT_CALLBACK pfnWaitCallback, PVOID pvContext, PTP_CALLBACK_ENVIRON pcbe);
*    当创建完成后，我们调用下面的函数来将一个内核对象绑定到这个线程池：
*		VOID SetThreadpoolWait(
			PTP_WAIT pWaitItem, HANDLE hObject, PFILETIME pftTimeout);
	 最后，我们可以通过调用WaitForThreadpoolWaitCallbacks函数来等待一个等待项完成，我们还可以通过调用CloseThreadpoolWait函数来释放一个等待项的内存。
* 6）在异步I/O请求完成时调用一个函数
*    首先，我们必须编写一个符合以下原型的函数：
*        VOID CALLBACK OverlappedCompletionRoutine(
*			PTP_CALLBACK_INSTANCE pInstance,
*			PVOID	pvContext,
*			PVOID	pOverlapped,
*			ULONG	IoResult,
*			ULONG_PTR NumberofBytesTransferred,
*			PTP_IO	pIo
*		 );
*    然后我们通过调用CreateThreadpoolIo来创建一个线程池I/O对象，并将我们想要与线程池内部的I/O完成端口相关联的文件/设备句柄（通过用FILE_FLAG_OVERLAPPED标志调用CreateFile函数所打开的），
*    在第一个参数中传入：
*		 PTP_IO CreateThreadpoolIo(
*			HANDLE hDevice,
*			PTP_WIN32_IO_CALLBACK pfnIoCallback,
*			PVOID	pvContext,
*			PTP_CALLBACK_ENVIRON pcbe);
*    当线程池I/O对象创建完毕后，我们通过调用下面的函数来嵌入在I/O项中的文件/设备与线程池内部的I/O完成端口关联起来：
*		 VOID StartThreadpoolIo(PTP_IO pio);
*    如果想在发出I/O请求之后让线程池停止调用我们的回调函数，那么可以调用下面的函数：
*		 VOID CancelThreadpoolIo(PTP_IO pio);
*    当对文件/设备的使用完成后，我们应该调用CloseHandle来将其关闭，并调用下面的函数来解除它与线程池的关联：
*		 VOID CloseThreadpoolIo(PTP_IO pio);
*    我们还可以调用下面的函数来让另一个线程等待一个待处理的I/O请求完成：
*        VOID WaitForThreadpoolIoCallbacks(
			PTP_IO pio, BOOL bCancelPendingCallbacks);
* 7）回调函数的终止操作
*	   线程池提供了一种便利的方法，用来描述在我们的回调函数返回之后，应该执行的一些操作。回调函数用传给它的不透明的pInstance参数来调用这些函数：
*	   VOID LeaveCriticalSectionWhenCallbackReturns(PTP_CALLBACK_INSTANCE pci, PCRITICAL_SECTION pcs);
*	   VOID ReleaseMutexWhenCallbackReturns(PTP_CALLBACK_INSTANCE pci, HANDLE mut);
*	   VOID ReleaseSemaphoreWhenCallbackReturns(PTP_CALLBACK_INSTANCE pci, HANDLE sem, DWORD crel);
*      VOID SetEventWhenCallbackReturns(PTP_CALLBACK_INSTANCE pci, HANDLE evt);
*	   VOID FreeLibraryWhenCallbackReturns(PTP_CALLBACK_INSTANCE pci, HMODULE mod);
*      除了这些终止函数之外，还有两个函数可用于回调函数的实例：
*	   BOOL CallbackMayRunLong(PTP_CALLBACK_INSTANCE pci);
*      VOID DisassociateCurrentThreadFromCallback(PTP_CALLBACK_INSTANCE pci);
* 8）我们可以在自己的应用程序中调用下面的函数来创建一个新的线程池：
*		PTP_POOL CreateThreadpool(PVOID reserved);
*    现在我们就可以调用下面的函数来设置线程池中线程的最大数量和最小数量了：
*		BOOL SetThreadpoolThreadMinimum(PTP_POOL pThreadPool, DWORD cthrdMin);
*		BOOL SetThreadpoolThreadMaximum(PTP_POOL pThreadPool, DWORD cthrdMost);
*    默认线程池的最小数量为1，最大数量为500。
*    当应用程序不再需要它为自己定制的线程池时，应该调用CloseThreadpool将其销毁：
*		VOID CloseThreadpool(PTP_POOL pThreadPool);
*    一旦我们创建了自己的线程池，并指定了线程的最小数量和最大数量，我们就可以初始化一个回调环境（callback environment），它包含了一些可应用于工作项的额外的设置或配置。
*    线程池回调环境的数据结构在WinNT.h中定义如下：
*		typedef struct _TP_CALLBACK_ENVIRON{
*			TP_VERSION	Version;
*			PTP_POOL	Pool;
*			PTP_CLEANUP_GROUP	CleanupGroup;
*			PTP_CLEANUP_GROUP_CANCEL_CALLBACK CleanupGroupCancelCallback;
*			PVOID		RaceDll;
*			struct _ACTIVATION_CONTEXT	*ActivationContext;
*			PTP_SIMPLE_CALLBACK	FinalizationCallback;
*			union{
*				DWORD	Flags;
*				struct{
*					DWORD	LongFunction : 1;
*					DWORD	Private	: 31;
*				}s;
*			}u;
*		}TP_CALLBACK_ENVIRON, *PTP_CALLBACK_ENVIRON;
*     为了对这个结构进行初始化，我们应该首先调用下面的函数：
*		VOID InitializeThreadpoolEnvironment(PTP_CALLBACK_ENVIRON pcbe);
*     同往常一样，当我们不再需要使用线程池回调环境的时候，应该调用DestroyThreadpoolEnvironment来对它进行清理：
*		VOID DestroyThreadpoolEnvironment(PTP_CALLBACK_ENVIRON pcbe);
*     为了将一个工作项添加到线程池的队列中，回调环境必须标明该工作项应该由哪个线程池来处理。我们可以调用SetThreadpoolCallbackPool，并传给它一个PTP_POOL值，来指定一个特定的线程池：
*		VOID SetThreadpoolCallbackPool(PTP_CALLBACK_ENVIRON pcbe, PTP_POOL pThreadPool);
*	  我们可以调用SetThreadpoolCallbackRunsLong函数来告诉回调环境，工作项通常需要较长的时间处理。这使得线程池会更快地创建线程，其目的是为了尝试在对工作项进行处理的时候，以一种更
*     公平的方式来替代最有效的方式。
*		VOID SetThreadpoolCallbackRunsLong(PTP_CALLBACK_ENVIRON pcbe);
*     我们可以调用SetThreadpoolCallbackLibrary来确保只要线程池中还有待处理的工作项，就将一个特定的DLL一直保持在进程的地址空间中。
*		VOID SetThreadpoolCallbackLibrary(PTP_CALLBACK_ENVIRON pcbe, PVOID mod);
* 9）得体的销毁线程池：清理组
*		为了帮助我们对线程池进行得体的清理，线程池提供了清理组（cleanup group）。清理组不适用于默认的线程池，这是因为它不会被销毁。默认的线程池的生命期与进程相同，在进程终止的时候，
*	 Windows会将其销毁并负责所有的清理工作。
*       为了得体地销毁私有线程池，我们首先需要通过调用CreateThreadpoolCleanupGroup来创建一个清理组：
*		  PTP_CLEANUP_GROUP CreateThreadpoolCleanupGroup();
*       我们然后需要调用下面的函数，将这个清理组与一个已经绑定到线程池的TP_CALLBACK_ENVIRON结构关联起来：
*		  VOID SetThreadpoolCallbackCleanupGroup(PTP_CALLBACK_ENVIRON pcbe, PTP_CLEANUP_GROUP ptpcg, PTP_CLEANUP_GROUP_CANCEL_CALLBACK pfng);
*       参数pfng表示一个回调函数，如果清理组被取消，那么这个回调函数会被调用，回调函数必须符合下面的原型：
*		  VOID CALLBACK CleanupGroupCancelCallback(PVOID pvObjectContext, PVOID pvCleanupContext);
*		现在，当我们的应用程序想要销毁线程池的时候，可以调用下面这个函数：
*		  VOID CloseThreadpoolCleanupGroupMembers(PTP_CLEANUP_GROUP ptpcg, BOOL bCancelPengdingCallbacks, PVOID pvCleanupContext);
*       当所有的工作项被取消或被处理之后，我们然后调用CloseThreadpoolCleanupGroup来释放清理组所占用的资源：
*		  VOID WINAPI CloseThreadpoolCleanupGroup(PTP_CLEANUP_GROUP ptpcg);
*       最后，我们可以调用DestroyThreadpoolEnvironment和CloseThreadpool，这样就得体地关闭了线程池。
*/
class CHYThreadPool
{
};


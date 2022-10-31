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
*/
class CHYThreadPool
{
};


#pragma once
#include <Windows.h>

/*
* ֪ʶ��
* 1��Ϊ�˼򻯿�����Ա�Ĺ�����Windows�ṩ��һ������I/O��ɶ˿������׵ģ��̳߳ػ��������̵߳Ĵ����������Լ��ճ�����
*    ��Щ�µ��̳߳غ�������������������Щ���飺
*		�����첽�ķ�ʽ������һ������
*		��ÿ��һ��ʱ�����һ������
*		�����ں˶��󴥷���ʱ�����һ������
*		�����첽I/O������ɵ�ʱ�����һ������
* 2��Ϊ�����̳߳������첽�ķ�ʽִ��һ��������������Ҫ����һ����������ԭ�͵ĺ�����
*		VOID NTAPI SimpleCallback(
*			PTR_CALLBACK_INSTANCE pInstance,
*			PVOID pvContext);
*    Ȼ��Ϊ�����̳߳��е�һ���߳�ִ�иú�����������Ҫ���̳߳��ύһ������Ϊ�˴ﵽ���Ŀ�ģ�������Ҫ��������ĺ�����
*		BOOL TrySubmitThreadpoolCallback(
*			PTP_SIMPLE_CALLBACK pfnCallback,
*			PVOID pvContext,
*			PTP_CALLBACK_ENVIRON pcbe);
*    ע�⣬���Ǵ�������Ҫ�Լ�����CreateThread��ϵͳ���Զ�Ϊ���ǵĽ��̴���һ��Ĭ�ϵ��̳߳أ������̳߳��е�һ���߳����������ǵĻص�������
* 3��ÿһ�ε���TrySubmitThreadpoolCallback��ʱ��ϵͳ�����ڲ������ǵ��������һ���������������ύ�����Ĺ������ô���ڶ����ܺ��ڴ�ʹ�õĿ��ǣ�����������һ�Σ�Ȼ��ֶ���ύ������á�
*	 ���ǵ�������ĺ���������һ�������
*		PTP_WORK CreateThreadpoolWork(
*			PTP_WORD_CALLBACK pfnWorkHandler,
*			PVOID pvContext,
*			PTP_CALLBACK_ENVIRON pcbe);
*    ��������Ҫ���̳߳��ύһ�������ʱ�򣬿��Ե���SubmitThreadpoolWork������VOID SubmitThreadpoolWork(PTP_WORK pWork);
*	 �����������һ���̣߳����߳���Ҫȡ���Ѿ��ύ�Ĺ�������߸��߳�����Ҫ�ȴ����������϶���Ҫ���Լ�������ô���Ե�������ĺ�����
*		VOID WaitForThreadpoolWorkCallbacks(
*			PTP_WORK pWork,
*			BOOL bCancelPendingCallbacks);
*    ������Ҫһ���������ʱ������Ӧ�õ���CloseThreadpoolWork������Ψһ�Ĳ����д���ָ��ù������ָ�룺VOID CloseThreadpoolWork(PTP_WORK pwk);
* 4��Ϊ�˽�һ�����������ĳ��ʱ��ִ�У����Ǳ��붨��һ���ص�����������ԭ�����£�
*		VOID CALLBACK TimeoutCallback(
			PTP_CALLBACK_INSTANCE pInstance, PVOID pvContext, PTP_TIMER pTimer);
	 Ȼ���������ĺ�����֪ͨ�̳߳�Ӧ���ں�ʱ�������ǵĺ�����
		PTP_TIMER CreateThreadpoolTimer(
			PTP_TIMER_CALLBACK pfnTimerCallback, PVOID pvContext, PTP_CALLBACK_ENVIRON pcbe);
	 ��������Ҫ���̳߳�ע���ʱ����ʱ��Ӧ�õ���SetThreadpoolTimer������
		VOID SetThreadpoolTimer(
			PTP_TIMER pTimer, PFILETIME pftDueTime, DWORD msperiod, DWORD msWindowLength);
	 ͬʱ�����ǿ���ͨ������IsThreadpoolTimerSet��ȷ��ĳ����ʱ���Ƿ��Ѿ������ã�Ҳ����˵������pftDueTimeֵ��ΪNULL��:
	    BOOL IsThreadpoolTimerSet(PTP_TIMER pti);
	 ������ǿ���ͨ������WaitForThreadpoolTimerCallbacks�����̵߳ȴ�һ����ʱ����ɣ�������ͨ������CloseThreadpoolTimer�������ͷż�ʱ�����ڴ档
* 5�������Ҫע��һ�������������һ���ں˶��󱻴�����ʱ��ִ�У����ȣ���дһ�����������ԭ�͵ĺ�����
*		VOID CALLBACK WaitCallback(
*			PTP_CALLBACK_INSTANCE pInstance, PVOID Context, PTP_WAIT Wait, TP_WAIT_RESULT WaitResult);
*	 Ȼ��ͨ������CreateThreadpoolWait������һ���̳߳صȴ�����
*		PTP_WAIT CreateThreadpoolWait(
*			PTP_WAIT_CALLBACK pfnWaitCallback, PVOID pvContext, PTP_CALLBACK_ENVIRON pcbe);
*    ��������ɺ����ǵ�������ĺ�������һ���ں˶���󶨵�����̳߳أ�
*		VOID SetThreadpoolWait(
			PTP_WAIT pWaitItem, HANDLE hObject, PFILETIME pftTimeout);
	 ������ǿ���ͨ������WaitForThreadpoolWaitCallbacks�������ȴ�һ���ȴ�����ɣ����ǻ�����ͨ������CloseThreadpoolWait�������ͷ�һ���ȴ�����ڴ档
* 6�����첽I/O�������ʱ����һ������
*    ���ȣ����Ǳ����дһ����������ԭ�͵ĺ�����
*        VOID CALLBACK OverlappedCompletionRoutine(
*			PTP_CALLBACK_INSTANCE pInstance,
*			PVOID	pvContext,
*			PVOID	pOverlapped,
*			ULONG	IoResult,
*			ULONG_PTR NumberofBytesTransferred,
*			PTP_IO	pIo
*		 );
*    Ȼ������ͨ������CreateThreadpoolIo������һ���̳߳�I/O���󣬲���������Ҫ���̳߳��ڲ���I/O��ɶ˿���������ļ�/�豸�����ͨ����FILE_FLAG_OVERLAPPED��־����CreateFile�������򿪵ģ���
*    �ڵ�һ�������д��룺
*		 PTP_IO CreateThreadpoolIo(
*			HANDLE hDevice,
*			PTP_WIN32_IO_CALLBACK pfnIoCallback,
*			PVOID	pvContext,
*			PTP_CALLBACK_ENVIRON pcbe);
*    ���̳߳�I/O���󴴽���Ϻ�����ͨ����������ĺ�����Ƕ����I/O���е��ļ�/�豸���̳߳��ڲ���I/O��ɶ˿ڹ���������
*		 VOID StartThreadpoolIo(PTP_IO pio);
*    ������ڷ���I/O����֮�����̳߳�ֹͣ�������ǵĻص���������ô���Ե�������ĺ�����
*		 VOID CancelThreadpoolIo(PTP_IO pio);
*    �����ļ�/�豸��ʹ����ɺ�����Ӧ�õ���CloseHandle������رգ�����������ĺ�������������̳߳صĹ�����
*		 VOID CloseThreadpoolIo(PTP_IO pio);
*    ���ǻ����Ե�������ĺ���������һ���̵߳ȴ�һ���������I/O������ɣ�
*        VOID WaitForThreadpoolIoCallbacks(
			PTP_IO pio, BOOL bCancelPendingCallbacks);
* 7���ص���������ֹ����
*	   �̳߳��ṩ��һ�ֱ����ķ������������������ǵĻص���������֮��Ӧ��ִ�е�һЩ�������ص������ô������Ĳ�͸����pInstance������������Щ������
*	   VOID LeaveCriticalSectionWhenCallbackReturns(PTP_CALLBACK_INSTANCE pci, PCRITICAL_SECTION pcs);
*	   VOID ReleaseMutexWhenCallbackReturns(PTP_CALLBACK_INSTANCE pci, HANDLE mut);
*	   VOID ReleaseSemaphoreWhenCallbackReturns(PTP_CALLBACK_INSTANCE pci, HANDLE sem, DWORD crel);
*      VOID SetEventWhenCallbackReturns(PTP_CALLBACK_INSTANCE pci, HANDLE evt);
*	   VOID FreeLibraryWhenCallbackReturns(PTP_CALLBACK_INSTANCE pci, HMODULE mod);
*      ������Щ��ֹ����֮�⣬�����������������ڻص�������ʵ����
*	   BOOL CallbackMayRunLong(PTP_CALLBACK_INSTANCE pci);
*      VOID DisassociateCurrentThreadFromCallback(PTP_CALLBACK_INSTANCE pci);
* 8�����ǿ������Լ���Ӧ�ó����е�������ĺ���������һ���µ��̳߳أ�
*		PTP_POOL CreateThreadpool(PVOID reserved);
*    �������ǾͿ��Ե�������ĺ����������̳߳����̵߳������������С�����ˣ�
*		BOOL SetThreadpoolThreadMinimum(PTP_POOL pThreadPool, DWORD cthrdMin);
*		BOOL SetThreadpoolThreadMaximum(PTP_POOL pThreadPool, DWORD cthrdMost);
*    Ĭ���̳߳ص���С����Ϊ1���������Ϊ500��
*    ��Ӧ�ó�������Ҫ��Ϊ�Լ����Ƶ��̳߳�ʱ��Ӧ�õ���CloseThreadpool�������٣�
*		VOID CloseThreadpool(PTP_POOL pThreadPool);
*    һ�����Ǵ������Լ����̳߳أ���ָ�����̵߳���С������������������ǾͿ��Գ�ʼ��һ���ص�������callback environment������������һЩ��Ӧ���ڹ�����Ķ�������û����á�
*    �̳߳ػص����������ݽṹ��WinNT.h�ж������£�
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
*     Ϊ�˶�����ṹ���г�ʼ��������Ӧ�����ȵ�������ĺ�����
*		VOID InitializeThreadpoolEnvironment(PTP_CALLBACK_ENVIRON pcbe);
*     ͬ����һ���������ǲ�����Ҫʹ���̳߳ػص�������ʱ��Ӧ�õ���DestroyThreadpoolEnvironment��������������
*		VOID DestroyThreadpoolEnvironment(PTP_CALLBACK_ENVIRON pcbe);
*     Ϊ�˽�һ����������ӵ��̳߳صĶ����У��ص�������������ù�����Ӧ�����ĸ��̳߳����������ǿ��Ե���SetThreadpoolCallbackPool����������һ��PTP_POOLֵ����ָ��һ���ض����̳߳أ�
*		VOID SetThreadpoolCallbackPool(PTP_CALLBACK_ENVIRON pcbe, PTP_POOL pThreadPool);
*	  ���ǿ��Ե���SetThreadpoolCallbackRunsLong���������߻ص�������������ͨ����Ҫ�ϳ���ʱ�䴦����ʹ���̳߳ػ����ش����̣߳���Ŀ����Ϊ�˳����ڶԹ�������д����ʱ����һ�ָ�
*     ��ƽ�ķ�ʽ���������Ч�ķ�ʽ��
*		VOID SetThreadpoolCallbackRunsLong(PTP_CALLBACK_ENVIRON pcbe);
*     ���ǿ��Ե���SetThreadpoolCallbackLibrary��ȷ��ֻҪ�̳߳��л��д�����Ĺ�����ͽ�һ���ض���DLLһֱ�����ڽ��̵ĵ�ַ�ռ��С�
*		VOID SetThreadpoolCallbackLibrary(PTP_CALLBACK_ENVIRON pcbe, PVOID mod);
* 9������������̳߳أ�������
*		Ϊ�˰������Ƕ��̳߳ؽ��е���������̳߳��ṩ�������飨cleanup group���������鲻������Ĭ�ϵ��̳߳أ�������Ϊ�����ᱻ���١�Ĭ�ϵ��̳߳ص��������������ͬ���ڽ�����ֹ��ʱ��
*	 Windows�Ὣ�����ٲ��������е���������
*       Ϊ�˵��������˽���̳߳أ�����������Ҫͨ������CreateThreadpoolCleanupGroup������һ�������飺
*		  PTP_CLEANUP_GROUP CreateThreadpoolCleanupGroup();
*       ����Ȼ����Ҫ��������ĺ������������������һ���Ѿ��󶨵��̳߳ص�TP_CALLBACK_ENVIRON�ṹ����������
*		  VOID SetThreadpoolCallbackCleanupGroup(PTP_CALLBACK_ENVIRON pcbe, PTP_CLEANUP_GROUP ptpcg, PTP_CLEANUP_GROUP_CANCEL_CALLBACK pfng);
*       ����pfng��ʾһ���ص���������������鱻ȡ������ô����ص������ᱻ���ã��ص�����������������ԭ�ͣ�
*		  VOID CALLBACK CleanupGroupCancelCallback(PVOID pvObjectContext, PVOID pvCleanupContext);
*		���ڣ������ǵ�Ӧ�ó�����Ҫ�����̳߳ص�ʱ�򣬿��Ե����������������
*		  VOID CloseThreadpoolCleanupGroupMembers(PTP_CLEANUP_GROUP ptpcg, BOOL bCancelPengdingCallbacks, PVOID pvCleanupContext);
*       �����еĹ����ȡ���򱻴���֮������Ȼ�����CloseThreadpoolCleanupGroup���ͷ���������ռ�õ���Դ��
*		  VOID WINAPI CloseThreadpoolCleanupGroup(PTP_CLEANUP_GROUP ptpcg);
*       ������ǿ��Ե���DestroyThreadpoolEnvironment��CloseThreadpool�������͵���عر����̳߳ء�
*/
class CHYThreadPool
{
};


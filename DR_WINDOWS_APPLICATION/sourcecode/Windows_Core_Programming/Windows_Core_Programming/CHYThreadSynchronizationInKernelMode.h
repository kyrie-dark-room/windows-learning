#pragma once

#include <Windows.h>
/*
* ֪ʶ��
* 1��ʵ���ϣ��ں˶���Ψһ��ȱ��������ǵ����ܡ������ǵ��ñ��½��ܵ��κ�һ���º���ʱ�������̱߳�����û�ģʽ�л����ں�ģʽ�������л�ʱ�ǳ���ʱ�ģ���x86ƽ̨�ϣ�һ���յ�ϵͳ
*   ���ô�Ż�ռ��200��CPU����--��Ȼ���⻹������ִ�б����ú������ں�ģʽ�µ�ʵ�ִ��롣���ǣ�����ں˶�����û�ģʽ�µ�ͬ��������������������ԭ���ǰ���������̶߳�����
*   ˢ�¸��ٻ����Լ�������ٻ��棨��δ���У�����������̸�۵��ǳɰ���ǧ��CPU���ڡ�
* 2��������ں˶���ȿ��Դ��ڴ���״̬��Ҳ���Դ���δ����״̬��
*		������
*		���߳�
*		����ҵ
*		���ļ��Լ�����̨�ı�׼������/�����/������
*		���¼�
*		���ɵȴ��ļ�ʱ����waitable timer��
*		���ź���
*		��������
* 3����һЩ�ں��¼���˵���ɹ��ص���WaitForSingleObject��WaitForMultipleObjects��ʵ�ϻ�ı�����״̬����������״̬�����˱仯�������֮Ϊ�ȴ��ɹ�������ĸ�����
*	��successful wait side effects����
* 4��WaitForMultipleObjects֮������ô���õ�ԭ������Ϊ���ܹ���ԭ�ӷ�ʽִ�����в��������̵߳���WaitForMultipleObjects��ʱ�򣬺�����������ж���Ĵ���״̬��������
*   ��Ӧ�ĸ����ã�������Щ������Ϊһ����������ɵġ�
* 5���������̵߳ȴ�ͬһ���ں˶�����ô�����󱻴�����ʱ��ϵͳ��ξ���Ӧ�û����ĸ��̣߳�Microsoft�Ĺٷ��ش��ǡ��㷨�ǹ�ƽ�ġ���Microsoft�������ϵͳ�ڲ�ʹ�õ��㷨
*   ���κγ�ŵ����ֻ��˵�㷨�ǹ�ƽ�ģ�Ҳ����˵����ж���߳��ڵȴ�����ôÿ�ε����󱻴�����ʱ��ÿ���̶߳��л��ᱻ���ѡ�
* 6���ɵȴ��ļ�ʱ��������һ���ں˶������ǻ���ĳ��ָ����ʱ�䴥������ÿ��һ��ʱ�䴥��һ�Ρ�
* 7���������ϣ��ʱ��һ�����ü�ʱ����һ��APC��ӵ�������ȥ���ͱ���ʵ��һ����ʱ��APC���������Ѻ����ĵ�ַ���롣��ʱ����������ʱ�򣬵��ҽ���SetWaitableTimer�ĵ����߳�
*   �����ڿ�����״̬��alertable state��ʱ����������ᱻͬһ���̵߳��á����仰˵���̱߳��������ڵ���SleepEx��WaitForSingleObjectEx��WaitForMultipleObjectsEx��
*	MsgWaitForMultipleObjectsEx��SignalObjectAndWait������ĵȴ�״̬��
*    ֻ�е����е�APC������������Ϻ󣬲Ż᷵�ؿɾ��溯����alertable function������ˣ����Ǳ���ȷ���Լ���TimerAPCRoutine�������ڼ�ʱ���ٴα�����֮ǰ��������ȻAPC����
*   ������е��ٶȾͿ���˴������ǵ��ٶȡ�
*    �̲߳�Ӧ���ڵȴ�һ����ʱ�������ͬʱ�Կ����ѵķ�ʽ�ȴ�ͬһ����ʱ����
* 8�����û���ʱ����ͨ��SetTimer���������ã��Ƚϡ������������������û���ʱ����Ҫ��Ӧ�ó�����ʹ�ô������û����������ʩ���Ӷ����ĸ������Դ�����⣬�ɵȴ���ʱ�����ں˶���
*   ����ζ�����ǲ��������ڶ���̼߳乲�����ҿ��Ծ߱���ȫ�ԡ�
* 9���ź����ں˶�����������Դ���м����������������ں˶�����ͬ������Ҳ����һ��ʹ�ü����������ǻ�������������32λֵ��һ�������Դ������һ����ǰ��Դ�����������Դ������ʾ�ź���
*   ���Կ��Ƶ������Դ��������ǰ��Դ������ʾ�ź�����ǰ������Դ��������
* 10���ź����Ĺ������£�
*		�������ǰ��Դ��������0����ô�ź������ڴ���״̬
*		�������ǰ��Դ��������0����ô�ź�������δ����״̬
*		��ϵͳ���Բ����õ�ǰ��Դ������Ϊ����
*		����ǰ��Դ�������Բ�����������Դ����
* 11���ź������������������ǻ���ԭ�ӷ�ʽ��ִ����Щ���Ժ����ò�����Ҳ����˵�����������ź�������һ����Դ��ʱ�򣬲���ϵͳ������Դ�Ƿ���ã�����������Դ�������ݼ�����������
*	���ᱻ����̴߳�ϡ�ֻ�е���Դ�����ݼ����֮��ϵͳ�Ż�������һ���߳��������Դ�ķ��ʡ�
* 12������û�а취�ڲ��ı䵱ǰ�ź�����Դ��������������õ����ĵ�ǰ������Դ��ֵ��
* 13����������mutex���ں˶�������ȷ��һ���̶߳�ռ��һ����Դ�ķ��ʡ�ʵ���ϣ���Ҳ�ǻ��������ֵ��������������������һ��ʹ�ü������߳�ID�Լ�һ���ݹ�������߳�ID������ʶ��ǰռ��
*	�������������ϵͳ�е��ĸ��̣߳��ݹ������ʾ����߳�ռ�øû������Ĵ������������������;��������ʹ����ΪƵ�����ں˶���֮һ��
* 14�������ǻ������Ĺ���
*		������߳�IDΪ0����Ч�߳�ID������ô�û�������Ϊ�κ��߳���ռ�ã������ڴ���״̬
*		������߳�IDΪ����ֵ����ô��һ���߳��Ѿ�ռ���˸û�������������δ����״̬
*		�������������ں˶���ͬ������ϵͳ�Ի��������������⴦����������Υ��һЩ����Ĺ���
* 15��������������ͨ�ں˶���ͳ���������ͨ�ں˶���Ĺ����У���һ���������ڻ������������߳���ͼ�ȴ�һ��δ�����Ļ�������������������£��߳�ͨ�������ȴ�״̬�����ǣ�ϵͳ��
*	�����Ҫ��û��������̵߳��߳�ID�뻥���������ڲ���¼���߳�ID�Ƿ���ͬ������߳�IDһ�£���ôϵͳ�����̱߳��ֿɵ���״̬--��ʹ�û�������δ��������ϵͳ�е��κ������ں˶�����˵��
*   ���Ƕ��Ҳ������֡��쳣���ľٶ���ÿ���̳߳ɹ��صȴ���һ��������������������ĵݹ�����������ʹ�ݹ��������1��Ψһ;��������������⣬���̶߳�εȴ�ͬһ����������
* 16�����ռ�û��������߳����ͷŻ�����֮ǰ��ֹ���ǶԻ����������ڵȴ��û��������߳���˵���ᷢ��ʲô����أ�
*	���ǣ�ϵͳ��Ϊ��������������abandoned��-- ����ռ�û��������߳��Ѿ���ֹ�������Ҳ�޷��ͷ���������������������ʱ��ϵͳ���Զ���������������߳�ID��Ϊ0�������ĵݹ����
    ��Ϊ0��
	  Ȼ�������ȴ����������߳̿��Ա����ȣ�Ψһ��ͬ���ǣ��ȴ��������صĲ���ͨ����WAIT_OBJECT_0�����Ƿ���һ�������ֵWAIT_ABANDONED���������ķ���ֵ��ֻ�����ڻ���������ʾ�߳�
	���ڵȴ��Ļ�����Ϊ�����߳���ռ�ã������߳�����ɶԹ�����Դ��ʹ��֮ǰ��ֹ�ˡ����������Щ���Ρ��ջ�û��������̲߳���֪����ԴĿǰ����ʲô״̬--�������Ѿ�����ȫ�ƻ��ˡ���
	��������£����ǵ�Ӧ�ó�������Լ���������ô����
*/
namespace HY_KERNELOBJECT
{
	class CHYThreadSynchronizationInKernelMode
	{
	public:
		CHYThreadSynchronizationInKernelMode();
		~CHYThreadSynchronizationInKernelMode();

		// �ȴ�����
		DWORD WaitForSingleObject(HANDLE hObject, DWORD dwMilliseconds /* INFINITE */);
		DWORD WaitForMultipleObjects(DWORD dwCount, CONST HANDLE* phObjects, BOOL bWaitAll, DWORD dwMilliseconds);

		/******** �¼� ********/
		// ����һ���¼��ں˶���
		HANDLE CreateEvent(PSECURITY_ATTRIBUTES psa, BOOL bManualReset, BOOL bInitialState, PCTSTR pszName);
		HANDLE CreateEventEx(PSECURITY_ATTRIBUTES psa, PCTSTR pszName, DWORD dwFlags, DWORD dwDesiredAccess);
		// ���Ѿ����ڵ��¼�
		HANDLE OpenEvent(DWORD dwDesiredAccess, BOOL bInherit, PCTSTR pszName);
		// �ر��¼�
		BOOL CloseHandle(HANDLE hEventObject);
		// ���¼���ɴ���״̬
		BOOL SetEvent(HANDLE hEvent);
		// ���¼����δ����״̬
		BOOL ResetEvent(HANDLE hEvent);
		// �����¼������ȴ����¼�Ȼ�����̽���ָ���δ����״̬
		BOOL PulseEvent(HANDLE hEvent);

		/******** �ɵȴ��ļ�ʱ���ں˶��� ********/
		// ����һ���ɵȴ���ʱ��
		HANDLE CreateWaitableTimer(PSECURITY_ATTRIBUTES psa, BOOL bManualReset, PCTSTR pszName);
		// ��һ���Ѿ����ڵĿɵȴ���ʱ���ľ�����þ���뵱ǰ���������
		HANDLE OpenWaitableTimer(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName);
		// �ڴ�����ʱ�򣬿ɵȴ��ļ�ʱ���������Ǵ���δ����״̬����������Ҫ������ʱ����ʱ�򣬱������SetWaitableTimer������������
		BOOL SetWaitableTimer(HANDLE hTimer, const LARGE_INTEGER *pDueTime, LONG lPeriod, PTIMERAPCROUTINE pfnCompletionRoutine, PVOID pvArgToCompletionRoutine, BOOL bResume);
		// ȡ����ʱ��
		BOOL CancelWaitableTimer(HANDLE hTimer);

		/******** �ź����ں˶��� ********/
		// ����һ���ź����ں˶���
		HANDLE CreateSemaphore(PSECURITY_ATTRIBUTES psa, LONG lInitialCount, LONG lMaximumCount, PCTSTR pszName);
		HANDLE CreateSemaphoreEx(PSECURITY_ATTRIBUTES psa, LONG lInitialCount, LONG lMaximumCount, PCTSTR pszName, DWORD dwFlags, DWORD dwDesiredAccess);
		// ��һ���Ѿ����ڵ��ź����ľ��
		HANDLE OpenSemaphore(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName);
		// �����ź����ĵ�ǰ��Դ����
		BOOL ReleaseSemaphore(HANDLE hSemaphore, LONG lReleaseCount, PLONG plPreviousCount);

		/******** �������ں˶��� ********/
		// ����һ���������ں˶���
		HANDLE CreateMutex(PSECURITY_ATTRIBUTES psa, BOOL bInitialOwner, PCTSTR pszName);
		HANDLE CreateMutexEx(PSECURITY_ATTRIBUTES psa, PCTSTR pszName, DWORD dwFlags, DWORD dwDesiredAccess);
		// ��һ���Ѿ����ڵĻ��������
		HANDLE OpenMutex(DWORD dwDesiredAccess, BOOL bInheritHandle, PCTSTR pszName);
		// �ͷŻ�����
		BOOL ReleaseMutex(HANDLE hMutex);

	private:

	};
}


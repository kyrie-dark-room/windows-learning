#pragma once
#include <Windows.h>
/*
* ֪ʶ��
* 1��������
*    Ϊ�˰�������˾����ء���ȷ�Ľ����ǵĴ�����ֲ��Windows��Microsoft�ڲ���ϵͳ���������˳̡���ס�����ǵ�ȻӦ�þ�������ʹ���˳̣�������Ӧ�ó�����к������ƣ�ʹ֮�ܹ�ʹ��Windows�ṩ����    �̡�
* 2���˳������û�ģʽ��ʵ�ֵģ��ں˶��˳�һ����֪���ں˻�������Ƕ�����㷨�����˳̽��е��ȡ������˳̵����㷨�����Ƕ���ģ���˾��ں˶��ԣ������˳̵ĵ��Ȳ�����ռʽ�ġ���һ���ǣ�һ���߳�
*   ���԰���һ�������˳̡����ں˶��ԣ�������߳̽�����ռʽ���������߳�ִ�д��롣���ǣ��߳�һ��ֻ��ִ��һ���˳̵Ĵ���--�ĸ��˳������Ǿ�����
* 3��������ǵ��˳̣��̣߳����أ������ExitThread����ô�˳̺��̶߳���������
* 4�������ҪΪÿ���˳̱���һЩ��Ϣ����ô���ǿ���ʹ���˳ֲ̾��洢����Fiber Local Storage, FLS����������Щ�������˳������Ĳ�����TLS�������߳������Ĳ�����ͬ��
*/

namespace HY_KERNELOBJECT
{
	class CHYFiber
	{
	public:
		CHYFiber();
		virtual ~CHYFiber();

		// ��һ�����е��߳�ת��Ϊһ���˳�
		PVOID ConvertThreadToFiber(PVOID pvParam);

		// ��Ĭ������£���x86ϵͳ�У�CPU�ĸ�������Ϣ������CPU�Ĵ�����һ���֣�����ÿ���˳̶�ά��һ�ݣ��������˳���Ҫִ�и���������ǽ��ᵼ�����ݱ��ƻ���Ϊ�˸���ϵͳĬ����Ϊ������Ӧ��
		// ������չ��ĺ���
		PVOID ConvertThreadToFiberEx(PVOID pvParam, DWORD dwFlags);

		// Ϊ�˴���һ����һ���˳̣��̣߳�����ǰ�������е��Ǹ��˳̣�Ӧ�õ���
		PVOID CreateFiber(DWORD dwStackSize, PFIBER_START_ROUTINE pfnStartAddress, PVOID pvParam);

		// ��Ĭ������£������ᴴ��һ��һ��ʼֻ��������������洢ҳ��4KB*2����ջ����ջ�Ĵ�С������������1MB�����ָ�����Ƿ���ֵ����ô�����������ָ���Ĵ�С��Ԥ��ջ����Ϊ֮��������洢�������Ҫʹ�ô������˳̣���ô���ǿ���ϣ���˳�ջ���ĸ��ٵ��ڴ档
		PVOID CreateFiberEx(SIZE_T dwStackCommitSize, SIZE_T dwStackReserveSize, DWORD dwFlags, PFIBER_START_ROUTINE pfnStartAddress, PVOID pvParam);

		// ��һ���߳��У�ͬһʱ��ֻ��ִ��һ���̡߳�Ϊ�����µ��˳�ִ�У�����Ӧ�õ���
		VOID SwitchToFiber(PVOID pvFiberExecutionContext);

		// �����˳�
		VOID DeleteFiber(PVOID pvFiberExecutionContext);

		// �������˳̶���ɾ����ʱ�����ǿ���ͨ������ConvertFiberToThread��������̵߳��˳�״̬�����߳�ԭ������Ϊ������ConvertThreadToFiber(Ex)������˳̵ģ���ͬʱ�ͷŽ��߳�ת��Ϊ�˳�
		// ��ռ�õ����һ���ڴ�
		BOOL ConvertFiberToThread(void);

		// ����һ��FLS�ۣ�ʹ�õ�ǰ�������������е������˳̶�����ʹ�ø�FLS��
		DWORD FlsAlloc(PFLS_CALLBACK_FUNCTION lpCallback);
		// ���˳̱����٣���FLS������FlsFree���ö���ɾ����ʱ�򣬸ûص������ᱻ����
		BOOL FlsFree(DWORD dwFlsIndex);

		// ������ÿ���˳���ص�����
		BOOL FlsSetValue(DWORD dwFlsIndex, PVOID lpFlsData);
		// ��ȡ����
		PVOID FlsGetValue(DWORD dwFlsIndex);

		// �����Ҫ֪�������Ƿ�����һ���˳�ִ�������������У���ô����ֻ��Ҫ����
		BOOL IsThreadAFiber(void);

		// �õ���ǰ�������е��˳̵�ִ�������ĵĵ�ַ
		PVOID GetCurrentFiber(void);

		// ��һ�������ĺ���
		PVOID GetFiberData(void);
	};
}


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
*/
class CHYThreadPool
{
};


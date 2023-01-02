#pragma once
#include "CHYMemoryObject.h"
#include <tlhelp32.h>

/*
* ֪ʶ�㣺
* 1���ѷǳ��ʺϷ��������С�����ݡ��ѵ��ŵ�������������ר�Ľ����ͷ�ϵ����⣬���������������Ⱥ�ҳ��߽��������顣�ѵ�ȱ���Ƿ�����ͷ��ڴ����ٶȱ�������ʽ��������Ҳ�޷�
*	�ٶ�����洢���ĵ����ͳ�����������ֱ�ӿ��ơ�
* 2����ϵͳ�ڲ����Ѿ���һ��Ԥ���ĵ�ַ�ռ����򡣸տ�ʼ�������ڵĴ󲿷�ҳ�涼û�е�������洢�����������ǲ��ϵشӶ��з����ڴ棬�ѹ���������ѵ���Խ��Խ�������洢������Щ����
*	�洢��ʼ���Ǵ�ҳ�����ļ��з���ġ��ͷŶ��е��ڴ��ʱ���ѹ������᳷���ѵ���������洢����
* 3�����̳�ʼ����ʱ��ϵͳ���ڽ��̵ĵ�ַ�ռ��д���һ���ѡ�����ѱ���Ϊ���̵�Ĭ�϶ѣ�default heap������Ĭ������£�����ѵĵ�ַ�ռ�����Ĵ�С��1MB�����ǣ�ϵͳ����������̵�
*	Ĭ�϶ѣ�ʹ������1MB������Ҳ�����ڴ���Ӧ�ó����ʱ��ʹ��/HEAP�������������ı�Ĭ�ϵ������С�����ڶ�̬���ӿ⣨DLL��û����֮�����Ķѣ�����ڴ���DLL��ʱ��Ӧ��ʹ��/HEAP���ء�
* 4���������Windows�����õ��˽��̵�Ĭ�϶ѣ�����Ӧ�ó����������߳�ͬʱ���ø���Windows��������˶�Ĭ�϶ѵķ��ʱ������ν��С����仰˵��ϵͳ��֤������ʲôʱ��һ��ֻ��һ���߳�
*	��Ĭ�϶��з�����ͷ��ڴ�顣�������η��ʻ�����ܲ�����΢��Ӱ�졣���Ӧ�ó���ֻ��һ���̣߳���������ϣ���������ٶȷ��ʶѣ���ô����Ӧ�ô����Լ��ĶѶ���Ҫʹ�ý��̵�Ĭ�϶ѡ�
* 5��һ������ͬʱ�����ж���ѣ��������������������ڿ��Դ�����������Щ�ѡ����ǣ�Ĭ�϶����ڽ��̿�ʼ����֮ǰ��ϵͳ�Զ������ģ��ڽ�����ֹ����Զ����١������޷����ٽ��̵�Ĭ�϶ѡ�
*	ÿ���Ѷ���һ��������ʶ�Լ��ĶѾ�������з�����ͷ��ڴ��ĶѺ��������ڲ������õ�����Ѿ����
* 6����������ԭ�����ǿ���ϣ����Ӧ�ó����д�������Ķѣ�
*		����������б���
*		������Ч���ڴ�������ʼ�մӶ��з���ͬ����С�Ķ������ǾͿ��Զ����ǽ��и�����Ч�Ĺ���
*		���ֲ�����
*		�������߳�ͬ���Ŀ���
*		�������ͷ�
* 7�����κγ�����ͼ�Ӷ��з���һ���ڴ��ʱ��HeapAlloc��������ִ�����²�����
*		�������ѷ����ڴ������������ڴ������
*		���ҵ�һ���㹻��������ڴ�顣
*		������һ���µ��ڴ棬Ҳ���ǽ����ҵ��������ڴ����Ϊ�ѷ��䡣
*		�����·�����ڴ����ӵ��ѷ����ڴ�������С�
* 8���ڷ������ڴ棨1MB����ࣩ��ʱ��Ӧ�ñ���ʹ�öѺ���������ʹ��VirtualAlloc������
* 9����Windows XP��Windows Server 2003֮��İ汾�У����ǿ���ǿ�Ʋ���ϵͳ�ڷ����ڴ��ʱ��ʹ��һ�ֽе���Ƭ�ѣ�lowfragmentation heap�����㷨���ڶദ�����Ļ����ϣ�����Ƭ�ѵ�����
*	�õ��˼������ߡ�����Ĵ��������л�������Ƭ�ѣ�
*		ULONG HeapInformationValue = 2;
*		if (HeapSetInformation(hHeap, HeapCompatibilityInformation, &HeapInformationValue, sizeof(HeapInformationValue))
*		{
*			// hHeap is turned into a low fragmentation heap
*		}
*		else
*		{
			// hHeap can't be turned into a low fragmentation heap.
			// Maybe because it has been created with the HEAP_NO_SERIALIZE flag
*		}
*	 ���⻹Ҫע�⣬�ѹ������Լ�Ҳ������еķ���������м�ز�����һЩ�ڲ��Ż���
*/

namespace HY_MEMORYOBJECT
{
	class CHYHeap : public CHYMemoryObject
	{
	public:
		CHYHeap(void);
		~CHYHeap(void);

		// �õ����̵�Ĭ�϶Ѿ��
		HANDLE GetProcessHeap();

		// ���Լ��Ľ����д�������Ķ�
		HANDLE HeapCreate(DWORD fdwOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);

		// ���ǿ����öѹ��������κ�Heap*������һ���������ƻ������׳�һ���쳣��
		BOOL HeapSetInformation(
			HANDLE HeapHandle,
			HEAP_INFORMATION_CLASS HeapInformationClass,
			PVOID HeapInformation,
			SIZE_T HeapInformationLength);

		// �Ӷ��з���һ���ڴ�ֻ�����ǵ���һ��
		PVOID HeapAlloc(HANDLE hHeap, DWORD fdwFlags, SIZE_T dwBytes);

		// ���Ǿ�����Ҫ�����ڴ��Ĵ�С��
		PVOID HeapReAlloc(HANDLE hHeap, DWORD fdwFlags, PVOID pvMem, SIZE_T dwBytes);

		// ����ڴ��Ĵ�С
		SIZE_T HeapSize(HANDLE hHeap, DWORD fdwFlags, LPCVOID pvMem);

		// �ͷ��ڴ��
		BOOL HeapFree(HANDLE hHeap, DWORD fdwFlags, PVOID pvMem);

		// ���ٶ�
		BOOL HeapDestroy(HANDLE hHeap);

		// ö�ٽ��̶ѣ��Լ����з�����ڴ��
		BOOL Heap32First(LPHEAPENTRY32 lphe, DWORD th32ProcessID, ULONG_PTR th32HeapID);
		BOOL Heap32Next(LPHEAPENTRY32 lphe);
		BOOL Heap32ListFirst(HANDLE hSnapshot, LPHEAPLIST32 lphl);
		BOOL Heap32ListNext(HANDLE hSnapshot, LPHEAPLIST32 lphl);

		// ��ȡ�����е����ж�
		DWORD GetProcessHeaps(DWORD dwNumHeaps, PHANDLE phHeaps);

		// ��֤�ѵ�������
		BOOL HeapValidate(HANDLE hHeap, DWORD fdwFlags, LPCVOID pvMem);

		// Ϊ���ö������õ��ڴ�������½����һ�𣬲��������������������ڴ��Ĵ洢��
		UINT HeapCompact(HANDLE hHeap, DWORD fdwFlags);

		// ����/������
		BOOL HeapLock(HANDLE hHeap);
		BOOL HeapUnlock(HANDLE hHeap);

		// �����ѵ����ݣ��ú���ֻ���ڵ���
		BOOL HeapWalk(HANDLE hHeap, PPROCESS_HEAP_ENTRY pHeapEntry);

	private:
		CHYHeap(const CHYHeap&) = delete;
		CHYHeap& operator=(const CHYHeap&) = delete;
		CHYHeap(CHYHeap&&) = delete;
		CHYHeap& operator=(CHYHeap&&) = delete;
	};
}


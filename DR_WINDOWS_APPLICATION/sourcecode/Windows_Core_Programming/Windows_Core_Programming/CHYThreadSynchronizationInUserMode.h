#pragma once

/*
* ֪ʶ��
* 1����������������£��߳�֮����Ҫ�໥ͨ�š�
*	   �� ��Ҫ�ö���߳�ͬʱ����һ��������Դ��ͬʱ�����ƻ���Դ�������ԡ�
*	   �� һ���߳���Ҫ֪ͨ�����߳�ĳ�������Ѿ���ɡ�
* 2����ЩInterlocked������ι�������ȡ���ڴ������е�CPUƽ̨�������x86ϵ��CPU����ôInterlocked��������������ά��һ��Ӳ���źţ�����źŻ���ֹ����CPU����ͬһ���ڴ��ַ��
* 3�����Ǳ��뱣֤���ݸ���Щ�����ı�����ַ�Ǿ�������ģ�������Щ�������ܻ�ʧ�ܡ�
* 4��Interlocked����ִ�еü��졣����һ��ͨ��ֻռ�ü���CPU���ڣ�ͨ��С��50��������Ҳ����Ҫ���û�ģʽ���ں�ģʽ֮������л�������л�ͨ����Ҫռ��1000��CPU�������ϣ���
* 5�����ٻ����У�
*	  �����Ϊװ���жദ�����Ļ�������������Ӧ�ó�����ôӦ��ע����ٻ����С���CPU���ڴ��ж�ȡһ���ֽڵ�ʱ��������ֻ�Ǵ��ڴ���ȡ��һ���ֽڣ�����ȡ��һ�����ٻ����С�
*	���ٻ����п��ܰ���32�ֽڣ���ʽCPU����64�ֽڣ�������128�ֽڣ�ȡ����CPU��,����ʼ�ն����뵽32�ֽڱ߽磬64�ֽڱ߽磬��128�ֽڱ߽硣���ٻ����д��ڵ�Ŀ����Ϊ��������ܡ�
*	һ����˵��Ӧ�ó�����һ�����ڵ��ֽڽ��в�������������ֽڶ��ڸ��ٻ����У���ôCPU�Ͳ��ط����ڴ����ߣ����ߺķѵ�ʱ���ǰ�ߺķѵ�ʱ��Ҫ��öࡣ
*	  �ദ�����»��漰������ͬ�����⡣��Ȼ��CPUоƬ������߷ǳ����������⣬������ר�ŵ�������������д�����ȷ��˵����һ��CPU�޸��˸��ٻ������е�һ���ֽ�ʱ��������
*   ������CPU���յ�֪ͨ����ʹ�Լ��ĸ��ٻ��������ϡ�
*	  ��һ�ж���ζ������Ӧ�ø��ݸ��ٻ����еĴ�С����Ӧ�ó����������֯��һ�𣬲��������뻺���еı߽���롣��������Ŀ����Ϊ��ȷ����ͬ��CPU�ܹ����Է��ʲ�ͬ���ڴ��ַ��
*   ������Щ��ַ����ͬһ�����ٻ����С����⣬����Ӧ�ð�ֻ�����ݣ��򲻾����������ݣ���ɶ�д���ݷֱ��š����ǻ�Ӧ�ðѲ�����ͬһʱ����ʵ�������֯��һ��
*     Ҫ��ȷ��CPU���ٻ����еĴ�С����򵥵ķ����ǵ���Win32��GetLogicalProcessorInformation������һ������CPU�ĸ��ٻ����еĴ�С�����ǿ���ʹ��C/C++��������_declspec(align(#))
*   ָʾ�������ֶζ�����Կ��ơ�
* 6��volatile�ؼ��ָ��߱���������������ܻᱻӦ�ó���֮������������޸ģ��������ϵͳ��Ӳ������һ������ִ�е��̡߳�ȷ�е�˵��volatile�޶������߱�������Ҫ��������������κ�
*   ��ʽ���Ż�������ʼ�մӱ������ڴ��е�λ�ö�ȡ������ֵ�������һ���ṹ��volatile�޶������ڸ��ṹ�����еĳ�Ա����volatile�޶�������������ȷ���κ�һ����Աʼ�ն��Ǵ��ڴ��ж�ȡ�ġ�
* 7���ؼ��Σ�critical section����һС�δ��룬����ִ��֮ǰ��Ҫ��ռ��һЩ������Դ�ķ���Ȩ�����ַ�ʽ�����ö��д����ԡ�ԭ�ӷ�ʽ��������Դ���вٿء������ԭ�ӷ�ʽ��ָ���Ǵ���֪������
*     ��ǰ�߳�֮�⣬û�������κ��̻߳�ͬʱ���ʸ���Դ����Ȼ��ϵͳ��Ȼ������ͣ��ǰ�߳�ȥ���������̡߳����ǣ��ڵ�ǰ�߳��뿪�ؼ���֮ǰ��ϵͳ�ǲ���ȥ�����κ���Ҫ����ͳһ��Դ�������̵߳ġ�
*     ��������Interlocked�������ͬ�������ʱ������Ӧ����һ�Թؼ��Ρ��ؼ��ε����ô��������Ƿǳ�����ʹ�ã������������ڲ�Ҳʹ����Interlocked���������ִ���ٶȷǳ��졣�ؼ��ε�
*   ���ȱ�����������޷������ڶ������֮����߳̽���ͬ����
*/

#include <Windows.h>

namespace HY_KERNELOBJECT
{
	class CHYThreadSynchronizationInUserMode
	{
	public:
		CHYThreadSynchronizationInUserMode();
		~CHYThreadSynchronizationInUserMode();

		// ԭ�ӷ�ʽ��һ��ֵ��1
		LONG InterlockedIncrement(LONG volatile* plAddend);
		// ��1����
		LONG InterlockedDecrement(LONG volatile* plAddend);

		// ԭ�ӷ�ʽ���������������������ֻ��Ҫ�ڵڶ��������д���һ��������
		LONG InterlockedExchangeAdd(PLONG volatile plAddend, LONG lIncrement);
		LONGLONG InterlockedExchangeAdd64(PLONGLONG volatile pllAddend, LONGLONG llIncrement);

		// C���п��ṩ��һ��_aligned_malloc���������ǿ������������������һ���������ڴ档�����Ǻ�����ԭ�ͣ�
		void* _aligned_malloc(size_t size, size_t alignment);

		// ԭ�ӽ�������
		LONG InterlockedExchange(PLONG volatile plTarget, LONG lValue);
		LONGLONG InterlockedExchange64(PLONGLONG volatile pllTarget, LONGLONG llValue);
		PVOID InterlockedExchangePointer(PVOID* volatile ppvTarget, PVOID pvValue);

		// ��ԭ�ӷ�ʽִ��һ�����Ժ����ò���
		LONG InterlockedCompareExchange(LONG volatile* Destination, LONG lExchage, LONG lCompared);
		PVOID InterlockedCompareExchangePointer(PVOID* ppvDestination, PVOID pvExchage, PVOID pvComparand);
		// �����Ѷ����64λֵ
		LONGLONG InterlockedCompareExchange64(PLONGLONG pllDestination, LONGLONG llExchage, LONGLONG llCompared);

		// Interlocked����������
		void InitializeSListHead(PSLIST_HEADER ListHead);
		PSLIST_ENTRY InterlockedPushEntrySList(PSLIST_HEADER ListHead, __drv_aliasesMem PSLIST_ENTRY ListEntry);
		PSLIST_ENTRY InterlockedPopEntrySList(PSLIST_HEADER ListHead);
		PSLIST_ENTRY InterlockedFlushSList(PSLIST_HEADER ListHead);
		USHORT QueryDepthSList(PSLIST_HEADER ListHead);

		// ��ȡCPU���ٻ����д�С
		WORD GetHighCacheLine(void);
	};
}


#pragma once
#include "CHYMemoryObject.h"

namespace HY_MEMORYOBJECT
{
	class CHYVirtualMemory : public CHYMemoryObject
	{
	public:
		CHYVirtualMemory();
		~CHYVirtualMemory();

		// ���ǵ���VirtualAlloc������Ԥ�������еĵ�ַ�ռ�����
		PVOID VirtualAlloc(PVOID pvAddress, SIZE_T dwSize, DWORD fdwAllocationType, DWORD fdwProtect);
		// ���Ӧ�ó����ڷ�ͳһ�ڴ���ʣ�NUMA�����������У���ô���ǿ��Ե�������ĺ�����ǿ��ϵͳ��ĳ���������ڴ��з���һ���������ڴ棬�Դ��������ܡ�
		PVOID VirtualAllocExNuma(HANDLE hProcess, PVOID pvAddress, SIZE_T dwSize, DWORD fdwAllocationType, DWORD fdwProtect, DWORD dwPreferredNumaNode);

		/* 
		1����Ԥ������������ǻ���Ҫ�������������洢�����������ܷ������е��ڴ��ַ��ϵͳ���ҳ�����ļ�������������洢���������ڵ�������洢��ʱ����ʼ��ַʼ�ն���
		   ҳ���С����������������СҲ��ҳ���С����������
		
		PVOID VirtualAlloc(PVOID pvAddress, SIZE_T dwSize, DWORD fdwAllocationType, DWORD fdwProtect);
		*/

		/* 
		2����ʱ������ҪͬʱԤ�����򲢸������������洢����ֻ�����VirtualAllocһ�ξ��ܴﵽ��һĿ�ġ�
		PVOID VirtualAlloc(PVOID pvAddress, SIZE_T dwSize, DWORD fdwAllocationType(MEM_RESERVE | MEM_COMMIT), DWORD fdwProtect);
		*/

		/* 
		3��Windows���ṩ�˴�ҳ��֧�֣������ڴ������ڴ��ʱ���������ܡ���������£�ϵͳ�ڷ����ڴ�ʱ������ʹ��SYSTEM_INFO�ṹ�е�dwPageSize�ֶ�����Ϊ�������ȣ�����ʹ��
		  ����ĺ������صĴ�ҳ��������ȣ�
		*/
		SIZE_T GetLargePageMinimum();
		    /* ���⣬����������������������
		    �� Ҫ������ڴ��Ĵ�С����dwSize������ֵ����������GetLargePageMinimum�����ķ���ֵ����������
			�� �ڵ���VirtualAllocʱ�������MEM_RESERVE | MEM_COMMIT��fdwAllocationType������λ�����������仰˵�����Ǳ���ͬʱԤ���͵����ڴ棬���ǲ�����Ԥ��һ������Ȼ���ٸ����е�һ����
			   ��������洢����
			�� ����VirtualAlloc�����ڴ�ʱ���봫PAGE_READWRITE�������Ը�fdwProtect������
			*/
		/* 
		4�������ڴ���ϡȱ��Դ����MEM_LARGE_PAGE��־����VirtualAlloc��Ҫ���÷������ڴ�������ҳ�棨Lock Pages In Memory�����û�Ȩ�ޣ����������û�ʧ�ܡ�Ĭ������²����κ�
		  �û����û��鶼�����Ȩ�޵ġ��������һ������ʽ��Ӧ�ó��������ô�ҳ�棬��ô�ڵ�¼�������������Ӧ�ó���֮ǰ���������ù���Ա�������Ȩ�ޡ��û�Ȩ�������û���¼ʱ����ġ�
		  �������Ա�ո��������ڴ�������ҳ��Ȩ�ޣ���ô���Ǳ������˳���¼��Ȼ�����µ�¼����ʹ֮��Ч����ע�⣬���������е�ʱ�������˶�Ӧ����Ȩ֮�⣬Ӧ�ó������ͨ����������Ȩ��
		  �ķ�ʽ�����С�
		*/

		/*
		5�����������ַ�����������ȷ���Ƿ���Ҫ�������е�ĳһ���ֵ�������洢����
		  �����ǳ��Ե�������洢����
		  ����ʹ��VirtualQuery���������ж��Ƿ��Ѿ�����������洢����
		  ����¼��Щҳ���Ѿ���������Щҳ����δ������
		  ��ʹ�ýṹ���쳣����structured exception handling��������ΪSEH��-- ��ѷ�����
		*/

		// ������������洢�����ͷ�����
		BOOL VirtualFree(LPVOID pvAddress, SIZE_T dwSize, DWORD fdwFreeType);

		// ��Ȼ��ʵ���к�����Ҫ�ı��ѵ���������洢ҳ�ı������ԣ�����������Ȼ�ǿ��еġ���������ҳ������������洢ҳ������ģ��Ҳ��ܸı����ɿ��������������洢ҳ��
		BOOL VirtualProtect(PVOID pvAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD pflOldProtect);

		/*
		* 6����������洢��������
		*     ��������洢������˼�ǣ����Ǹ���ϵͳһ���򼸸�����洢ҳ�е�����û�б��޸Ĺ������ϵͳ���ڲ���һҳ�����ڴ沢���ҵ���һ���޸Ĺ���ҳ�棬��ôϵͳ����Ѹ��ڴ�Ҳд�뵽
		*   ҳ�����ļ��С���������Ƚ�������Ӱ�����ܡ����ڴ����Ӧ�ó�����˵�����Ƕ�ϣ��ϵͳ���޸ĺ��ҳ�汣�浽ҳ�����ļ��С�
		*     ���ǣ���ЩӦ�ó���ֻ��Ҫ��һС��ʱ����ʹ�ô洢����֮��Ҳ����Ҫ����洢���е����ݡ�Ϊ���������ܣ�Ӧ�ó�����Ը���ϵͳ��Ҫ��ҳ�����ļ��б���ָ���Ĵ洢ҳ��
		*     Ϊ�����ô洢����Ӧ�ó���Ӧ�õ���VirtualAlloc���������ڵ����������д�MEM_RESET��־��
		*	  ����ϵͳ����Ϊ�������ǵ�����֮�У�������Ǳ�����Ϊһ��������ҳ�棬���е����ݶ�����������
		*     �����ô洢��ʱ����Ҫ��������һЩ���
		        ���ȣ�������VirtualAllocʱ������ַͨ���ᱻ����ȡ����ҳ���С��������������С��ᱻ����ȡ����ҳ���С���������������ô洢��ʱ��
		*   ���ַ�ʽ�Ի���ַ�ʹ�С����ȡ���Ƿǳ�Σ�յġ��м��ڴˣ�����������MEM_RESET����ôVirtualAlloc����෴�ķ������ȡ������������һ��������ϵͳ��ȷ����ֻ����������ҳ��
		*   �����������ݵ��ڴ�ҳ��
		*       �ڶ���Ҫ��ס����MEM_RESETֻ�ܵ���ʹ�ã����ܽ�����������־��λ��������
		*/

		/*
		* 7����ַ������չ��Address Windowing Extension, ������ΪAWE��
		*     ����ʱ������ƣ�Ӧ�ó�����ҪԽ��Խ����ڴ档������Ӧ�ó���������ˣ�����Խ��Խ��Ŀͻ������󵽷��������������������𽥽��͡�Ϊ��������ܣ�������Ӧ�ó�����Ҫ���ڴ���
		*   ���ָ���������Լ��ٴ��̺��ڴ���ҳ�������������͵�Ӧ�ó��򣨱������ݿ⡢�����ࡢ��ѧ�ࣩҲ��Ҫ�ܹ��ٿش��Ĵ洢��������ЩӦ�ó�����˵��32λ��ַ�ռ仹�����á�
		*     Ϊ�˰�����ЩӦ�ó���Windows�ṩ��һ�����ԣ�����ַ������չ���ڴ���AWEʱ��Microsoft����������Ŀ�꣺
		*		������Ӧ�ó�����һ������ķ�ʽ�����ڴ棬����ϵͳ��֤���Ὣ�����ַ�ʽ������ڴ滻���������ϡ�
		*		������Ӧ�ó�����ʱȽ��̵�ַ�ռ仹Ҫ����ڴ档
		*     �����ϣ�AWE�ṩ��һ�ַ�ʽ��������Ӧ�ó������һ������ڴ档��һ��ʼ����ʱ���ڽ��̵ĵ�ַ�ռ����ǿ�������Щ�ڴ��ġ�Ӧ�ó���Ȼ��ͨ������VirtualAlloc��Ԥ����ַ�ռ�
		*   ��������ǵ�ַ���ڡ�Ӧ�ó���Ȼ�����һ��������ÿ����һ�ΰ�һ���ڴ�ָ�����õ�ַ���ڡ����ڴ��ָ������ַ�����Ƿǳ���ģ�ͨ���Ǻ��뼶�ģ���
		*     ʾ������
		*     // First, reserve a 1MB region for the address window
		*	  ULONG_PTR ulRAMBytes = 1024 * 1024;
		*	  PVOID pvWindow = VirtualAlloc(NULL, ulRAMBytes, MEM_RESERVE | MEM_PHYSICAL, PAGE_READWRITE);
		*     // Get the number of tytes in a page for this CPU platform
		*     SYSTEM_INFO sinf;
		*     GetSystemInfo(&sinf);
		* 
		*     // Calculate the required number of RAM pages for the desired number of bytes
		*     ULONG_PTR ulRAMPages = (ulRAMBytes + sinf.dwpageSize-1)/sinf.dwPageSize;
		* 
		*     // Allocate array for RAM page's page frame numbers
		*     ULONG_PTR* aRAMPages = (ULONG_PTR*)new ULONG_PTR[ulRAMPages];
		* 
		*     // Allocate the pages of RAM(requires Lock Pages in Memory user right)
		*     AllocateUserPhysicalPages(GetCurrentProcess(), &ulRAMPages, aRAMPages);
		* 
		*     // Assign the RAM pages to our window
		*     MapUserPhysicalPages(pvWindow, ulRAMPages, aRAMPages);
		* 
		*     // Access the RAM pages via the pvWindow virtual address
		*     ...
		*     // Free the block of RAM pages
		*     FreeUserPhysicalPages(GetCurrentProcess(), &ulRAMPages, aRAMPages);
		* 
		*     // Destroy the address window
		*     VirtualFree(pvWindow, 0, MEM_RELEASE);
		*     delete[] aRAMPages;
		*     
		*/
	};
}


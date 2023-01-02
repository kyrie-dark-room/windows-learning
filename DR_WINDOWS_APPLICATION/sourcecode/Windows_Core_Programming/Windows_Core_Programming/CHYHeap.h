#pragma once
#include "CHYMemoryObject.h"
#include <tlhelp32.h>

/*
* 知识点：
* 1）堆非常适合分配大量的小型数据。堆的优点是它能让我们专心解决手头上的问题，而不必理会分配粒度和页面边界这类事情。堆的缺点是分配和释放内存块的速度比其他方式慢，而且也无法
*	再对物理存储器的调拨和撤销调拨进行直接控制。
* 2）在系统内部，堆就是一块预订的地址空间区域。刚开始，区域内的大部分页面都没有调拨物理存储器。随着我们不断地从堆中分配内存，堆管理器会给堆调拨越来越多的物理存储器。这些物理
*	存储器始终是从页交换文件中分配的。释放堆中的内存块时，堆管理器会撤销已调拨的物理存储器。
* 3）进程初始化的时候，系统会在进程的地址空间中创建一个堆。这个堆被称为进程的默认堆（default heap）。在默认情况下，这个堆的地址空间区域的大小是1MB。但是，系统可以增大进程的
*	默认堆，使它大于1MB。我们也可以在创建应用程序的时候使用/HEAP链接器开关来改变默认的区域大小。由于动态链接库（DLL）没有与之关联的堆，因此在创建DLL的时候不应该使用/HEAP开关。
* 4）由于许多Windows函数用到了进程的默认堆，而且应用程序会有许多线程同时调用各种Windows函数，因此对默认堆的访问必须依次进行。换句话说，系统保证不管在什么时候，一次只让一个线程
*	从默认堆中分配或释放内存块。这种依次访问会对性能产生轻微的影响。如果应用程序只有一个线程，而我们又希望以最快的速度访问堆，那么我们应该创建自己的堆而不要使用进程的默认堆。
* 5）一个进程同时可以有多个堆，进程在整个生命周期内可以创建和销毁这些堆。但是，默认堆是在进程开始运行之前由系统自动创建的，在进程终止后会自动销毁。我们无法销毁进程的默认堆。
*	每个堆都有一个用来标识自己的堆句柄，所有分配和释放内存块的堆函数都会在参数中用到这个堆句柄。
* 6）由于以下原因，我们可能希望在应用程序中创建额外的堆：
*		·对组件进行保护
*		·更有效的内存管理（如果始终从堆中分配同样大小的对象，我们就可以对它们进行更加有效的管理）
*		·局部访问
*		·避免线程同步的开销
*		·快速释放
* 7）当任何程序试图从堆中分配一块内存的时候，HeapAlloc函数必须执行以下操作：
*		·遍历已分配内存的链表和闲置内存的链表。
*		·找到一块足够大的闲置内存块。
*		·分配一块新的内存，也就是将刚找到的闲置内存块标记为已分配。
*		·将新分配的内存块添加到已分配内存的链表中。
* 8）在分配大块内存（1MB或更多）的时候应该避免使用堆函数，建议使用VirtualAlloc函数。
* 9）在Windows XP和Windows Server 2003之后的版本中，我们可以强制操作系统在分配内存的时候使用一种叫低碎片堆（lowfragmentation heap）的算法。在多处理器的机器上，低碎片堆的性能
*	得到了极大的提高。下面的代码用来切换到地碎片堆：
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
*	 另外还要注意，堆管理器自己也会对所有的分配请求进行监控并进行一些内部优化。
*/

namespace HY_MEMORYOBJECT
{
	class CHYHeap : public CHYMemoryObject
	{
	public:
		CHYHeap(void);
		~CHYHeap(void);

		// 得到进程的默认堆句柄
		HANDLE GetProcessHeap();

		// 在自己的进程中创建额外的堆
		HANDLE HeapCreate(DWORD fdwOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);

		// 我们可以让堆管理器在任何Heap*函数中一旦发生堆破坏，就抛出一个异常。
		BOOL HeapSetInformation(
			HANDLE HeapHandle,
			HEAP_INFORMATION_CLASS HeapInformationClass,
			PVOID HeapInformation,
			SIZE_T HeapInformationLength);

		// 从堆中分配一块内存只不过是调用一下
		PVOID HeapAlloc(HANDLE hHeap, DWORD fdwFlags, SIZE_T dwBytes);

		// 我们经常需要调整内存块的大小。
		PVOID HeapReAlloc(HANDLE hHeap, DWORD fdwFlags, PVOID pvMem, SIZE_T dwBytes);

		// 获得内存块的大小
		SIZE_T HeapSize(HANDLE hHeap, DWORD fdwFlags, LPCVOID pvMem);

		// 释放内存块
		BOOL HeapFree(HANDLE hHeap, DWORD fdwFlags, PVOID pvMem);

		// 销毁堆
		BOOL HeapDestroy(HANDLE hHeap);

		// 枚举进程堆，以及堆中分配的内存块
		BOOL Heap32First(LPHEAPENTRY32 lphe, DWORD th32ProcessID, ULONG_PTR th32HeapID);
		BOOL Heap32Next(LPHEAPENTRY32 lphe);
		BOOL Heap32ListFirst(HANDLE hSnapshot, LPHEAPLIST32 lphl);
		BOOL Heap32ListNext(HANDLE hSnapshot, LPHEAPLIST32 lphl);

		// 获取进程中的所有堆
		DWORD GetProcessHeaps(DWORD dwNumHeaps, PHANDLE phHeaps);

		// 验证堆的完整性
		BOOL HeapValidate(HANDLE hHeap, DWORD fdwFlags, LPCVOID pvMem);

		// 为了让堆中闲置的内存块能重新结合在一起，并撤销调拨给堆中闲置内存块的存储器
		UINT HeapCompact(HANDLE hHeap, DWORD fdwFlags);

		// 锁定/解锁堆
		BOOL HeapLock(HANDLE hHeap);
		BOOL HeapUnlock(HANDLE hHeap);

		// 遍历堆的内容，该函数只用于调试
		BOOL HeapWalk(HANDLE hHeap, PPROCESS_HEAP_ENTRY pHeapEntry);

	private:
		CHYHeap(const CHYHeap&) = delete;
		CHYHeap& operator=(const CHYHeap&) = delete;
		CHYHeap(CHYHeap&&) = delete;
		CHYHeap& operator=(CHYHeap&&) = delete;
	};
}


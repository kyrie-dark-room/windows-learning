#pragma once
#include "CHYMemoryObject.h"

namespace HY_MEMORYOBJECT
{
	class CHYVirtualMemory : public CHYMemoryObject
	{
	public:
		CHYVirtualMemory();
		~CHYVirtualMemory();

		// 我们调用VirtualAlloc函数来预定进程中的地址空间区域
		PVOID VirtualAlloc(PVOID pvAddress, SIZE_T dwSize, DWORD fdwAllocationType, DWORD fdwProtect);
		// 如果应用程序在非统一内存访问（NUMA）机器上运行，那么我们可以调用下面的函数来强制系统在某个的物理内存中分配一部分虚拟内存，以此提升性能。
		PVOID VirtualAllocExNuma(HANDLE hProcess, PVOID pvAddress, SIZE_T dwSize, DWORD fdwAllocationType, DWORD fdwProtect, DWORD dwPreferredNumaNode);

		/* 
		1）在预定了区域后，我们还需要给区域调拨物理存储器，这样才能访问其中的内存地址。系统会从页交换文件中来调拨物理存储器给区域。在调拨物理存储器时，起始地址始终都是
		   页面大小的整数倍，整个大小也是页面大小的整数倍。
		
		PVOID VirtualAlloc(PVOID pvAddress, SIZE_T dwSize, DWORD fdwAllocationType, DWORD fdwProtect);
		*/

		/* 
		2）有时我们想要同时预定区域并给区域调拨物理存储器。只需调用VirtualAlloc一次就能达到这一目的。
		PVOID VirtualAlloc(PVOID pvAddress, SIZE_T dwSize, DWORD fdwAllocationType(MEM_RESERVE | MEM_COMMIT), DWORD fdwProtect);
		*/

		/* 
		3）Windows还提供了大页面支持，可以在处理大块内存的时候提升性能。这种情况下，系统在分配内存时，不再使用SYSTEM_INFO结构中的dwPageSize字段来作为分配粒度，而是使用
		  下面的函数返回的大页面分配粒度：
		*/
		SIZE_T GetLargePageMinimum();
		    /* 此外，还必须满足以下三个条件
		    · 要分配的内存块的大小（即dwSize参数的值），必须是GetLargePageMinimum函数的返回值的整数倍。
			· 在调用VirtualAlloc时，必须把MEM_RESERVE | MEM_COMMIT与fdwAllocationType参数按位或起来。换句话说，我们必须同时预定和调拨内存，我们不能先预定一块区域然后再给其中的一部分
			   调拨物理存储器。
			· 在用VirtualAlloc分配内存时必须传PAGE_READWRITE保护属性给fdwProtect参数。
			*/
		/* 
		4）由于内存是稀缺资源，用MEM_LARGE_PAGE标志调用VirtualAlloc需要调用方具有内存中锁定页面（Lock Pages In Memory）的用户权限，否则函数调用会失败。默认情况下不是任何
		  用户或用户组都有这个权限的。如果想在一个交互式的应用程序中利用大页面，那么在登录到计算机并运行应用程序之前，必须先让管理员分配这个权限。用户权限是在用户登录时给予的。
		  如果管理员刚给予我们内存中锁定页面权限，那么我们必须先退出登录，然后重新登录才能使之有效。请注意，除了在运行的时候启用了对应的特权之外，应用程序必须通过提升运行权限
		  的方式来运行。
		*/

		/*
		5）有以下四种方法可以用来确定是否需要给区域中的某一部分调拨物理存储器。
		  ·总是尝试调拨物理存储器。
		  ·（使用VirtualQuery函数来）判断是否已经调拨了物理存储器。
		  ·记录哪些页面已经调拨而哪些页面尚未调拨。
		  ·使用结构化异常处理（structured exception handling，后面简称为SEH）-- 最佳方案。
		*/

		// 撤销调拨物理存储器及释放区域
		BOOL VirtualFree(LPVOID pvAddress, SIZE_T dwSize, DWORD fdwFreeType);

		// 虽然在实际中很少需要改变已调拨的物理存储页的保护属性，但这样做仍然是可行的。保护属性页是与整个物理存储页相关联的，且不能改变若干跨区域的连续物理存储页。
		BOOL VirtualProtect(PVOID pvAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD pflOldProtect);

		/*
		* 6）重置物理存储器的内容
		*     重置物理存储器的意思是，我们告诉系统一个或几个物理存储页中的数据没有被修改过。如果系统正在查找一页闲置内存并且找到了一个修改过的页面，那么系统必须把该内存也写入到
		*   页交换文件中。这个操作比较慢，会影响性能。对于大多数应用程序来说。我们都希望系统把修改后的页面保存到页交换文件中。
		*     但是，有些应用程序只需要在一小段时间内使用存储器，之后也不需要保存存储器中的内容。为了提升性能，应用程序可以告诉系统不要在页交换文件中保存指定的存储页。
		*     为了重置存储器，应用程序应该调用VirtualAlloc函数，并在第三个参数中传MEM_RESET标志。
		*	  由于系统的行为不在我们的掌握之中，因此我们必须认为一旦重置了页面，其中的内容都将是垃圾。
		*     在重置存储器时还需要紧记其他一些事项。
		        首先，当调用VirtualAlloc时，基地址通常会被向下取整到页面大小的整数倍，而大小则会被向上取整到页面大小的整数倍。在重置存储器时，
		*   这种方式对基地址和大小进行取整是非常危险的。有鉴于此，如果传入的是MEM_RESET，那么VirtualAlloc会从相反的方向进行取整操作。这样一来，操作系统就确保了只会重置整个页面
		*   都是垃圾数据的内存页。
		*       第二点要记住的是MEM_RESET只能单独使用，不能将它和其他标志按位或起来。
		*/

		/*
		* 7）地址窗口扩展（Address Windowing Extension, 后面简称为AWE）
		*     随着时间的推移，应用程序需要越来越多的内存。服务器应用程序尤其如此：随着越来越多的客户发请求到服务器，服务器的性能逐渐降低。为了提高性能，服务器应用程序需要在内存中
		*   保持更多的数据以减少磁盘和内存间的页交换。其他类型的应用程序（比如数据库、工程类、科学类）也需要能够操控大块的存储器。对这些应用程序来说，32位地址空间还不够用。
		*     为了帮助这些应用程序，Windows提供了一项特性，即地址窗口扩展。在创建AWE时，Microsoft有以下两个目标：
		*		·允许应用程序以一种特殊的方式分配内存，操作系统保证不会将以这种方式分配的内存换出到磁盘上。
		*		·允许应用程序访问比进程地址空间还要多的内存。
		*     基本上，AWE提供了一种方式，可以让应用程序分配一块或多块内存。当一开始分配时，在进程的地址空间中是看不见这些内存块的。应用程序然后（通过调用VirtualAlloc）预定地址空间
		*   区域，这就是地址窗口。应用程序然后调用一个函数，每调用一次把一块内存指定到该地址窗口。把内存块指定到地址窗口是非常快的（通常是毫秒级的）。
		*     示例程序：
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


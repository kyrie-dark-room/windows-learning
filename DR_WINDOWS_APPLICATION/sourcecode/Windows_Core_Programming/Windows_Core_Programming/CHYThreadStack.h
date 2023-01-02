#pragma once
#include "CHYMemoryObject.h"

/*
* 知识点：
* 1）有时候系统会在用户进程的地址空间中预定区域。系统在分配进程环境快和线程环境快时会发生这种情况。系统在用户进程的地址空间中预定区域的另一种可能是分配线程栈。
* 2）当系统创建线程时，会为线程栈预定一块地址空间区域（每个线程都有自己的栈），并给区域调拨一些物理存储器。默认情况下，系统会预定1MB的地址空间并调拨两个页面的
*   存储器。但是，在构建应用程序时开发人员可以通过两种方法来改变该默认值，一种方法是使用Microsoft C++编译器的/F选项，另一种方法是使用Microsoft C++链接器的/STACK选项：
*	/Freserve
*	/STACK:reserve[,commit]
*	在构建应用程序时，链接器会把想要的栈的大小写入到.exe或.dll文件的PE文件头中。当系统创建线程栈的时候，会根据PE文件头中的大小来预订地址空间区域。
* 3）线程栈的地址空间区域和所有调拨给该区域的物理存储器都具有PAGE_READWRITE保护属性。
* 4）在预定地址空间区域后，系统会给区域顶部（即地址最高）的两个页面调拨物理存储器。在让线程开始执行之前，系统会把线程栈的指针指向区域顶部的那个页面的末尾（该地址非常
*	接近0x08100000）。这个页面就是线程开始使用栈的地方。区域顶部往下的第二个页面被称为保护页面（guard page），随着线程调用越来越多的函数，调用树也越来越深，线程也需要
*   越来越多的栈空间。
*    当线程试图访问防护页面中的内存时，系统会得到通知。这时系统会先给防护页面下面的那个页面调拨存储器，接着去除当前防护页面的PAGE_GUARD保护属性标志，然后给刚调拨的存储页
*   指定PAGE_GUARD保护属性标志。该项技术使得系统能够在线程需要的时候才增大栈存储器的大小。
* 5）系统永远不会给区域底部的那个页面调拨存取器，具体的原因马上就会看到：
*	  当系统给区域底部页面的上一个页面（即倒数第二个页面）调拨物理存储器时，系统不会给刚调拨的物理存储器指定防护属性。但它会执行一个额外的操作--抛出EXCEPTION_STACK_OVERFLOW异常，
*	该异常对应的值为0xC00000FD。通过使用结构化异常处理（structured exception handling，后面简称为SEH），系统会发生这一情况时通知我们的程序，从而使程序能够得体地从这一异常情况下恢复。
* 6）如果线程在引发栈溢出异常后继续使用栈，那么它会用尽倒数第二个页面中的内存，并试图访问底部页面中的内存。当线程试图访问这个（尚未调拨物理存储器）的页面时，系统会抛出访问违规异常。
*   如果访问违规异常是在线程试图访问栈时所引发的，那么线程的麻烦就大了。此时系统会收回控制权并将控制权交给Windows错误报告服务（Windows Error Reporting service），后者会结束整个进程，
*   并弹出警告对话框。
*     为了避免这种情况，应用程序可以调用SetThreadStackGuarante函数，借此来抛出前文所提到的EXCEPTION_STACK_OVERFLOW异常。该函数可以确保在Windows错误报告服务接管并终止进程之前，
*   地址空间中还有指定数量的内存可供使用，这使得应用程序能够处理栈溢出异常并恢复运行。
* 7）当线程访问最后一个防护页面时，系统会抛出EXCEPTION_STACK_OVERFLOW异常。如果线程捕获了该异常并继续执行，那么系统将不会在同一个线程中再次抛出EXCEPTION_STACK_OVERFLOW异常，这是
*   因为后面再也没有防护页面了。如果希望在同一线程中继续收到EXCEPTION_STACK_OVERFLOW异常，那么应用程序必须重置防护页面。这很容易办到，只需调用C运行库的_resetstkoflw函数。
* 8）C++的运行库中有一个栈检查函数。在编译源代码时，编译器会在必要时生成代码来调用该函数。这个函数的目的是为了确保已经给线程栈调拨了物理存储器。当编译器再处理程序中的每个函数时，
*   它会算出函数需要多大的栈空间。如果需要的栈空间大于目标系统的页面大小，编译器就会自动插入代码来调用栈检查函数。
* 9）前面提到编译器会根据目标平台的页面大小，自动插入代码来调用StackCheck函数。为了能让开发人员对所使用的页面大小的阈值进行控制，Microsoft Visual C++提供了/Gs编译器开关。
*/

namespace HY_MEMORYOBJECT
{
	class CHYThreadStack : public CHYMemoryObject
	{
	public:
		CHYThreadStack(void);
		~CHYThreadStack(void);

		BOOL SetThreadStackGuarantee(PULONG StackSizeInBytes);

		int ResetStkOFlw(void);
	private:
		CHYThreadStack(const CHYThreadStack&) = delete;
		CHYThreadStack& operator=(const CHYThreadStack&) = delete;
		CHYThreadStack(CHYThreadStack&&) = delete;
		CHYThreadStack& operator=(CHYThreadStack&&) = delete;
	};
}


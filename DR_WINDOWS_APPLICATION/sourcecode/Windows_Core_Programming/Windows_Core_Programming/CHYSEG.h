#pragma once

/*
* 知识点：
* 1）结构化异常处理（structured exception handling，SEH），带来的好处是我们在写代码时，可以先集中精力完成软件的正常工作流程。如果在运行的时候出现了什么问题，系统会捕获这个问题，并且
*	通知我们。
* 2）促使Microsoft将SEH加入Windows系统的因素之一是它可以简化操作系统本身的开发工作。操作系统的开发人员使用SEH来让系统更加健壮，而我们也可以使用SEH来让应用程序更加健壮。
* 3）为了让SEH运作起来，编译器的工作量要大于操作系统。在进入和离开异常处理代码块时，编译器必须生成一些特殊的代码，以及产生一些关于支持SEH的数据结构表，还必须提供回调函数给操作系统
*	调用，以便系统遍历异常代码块。编译器还负责准备进程的栈框架（stack frame）和其他一些内部信息，这些信息都是操作系统需要使用或者引用的。让编译器支持SEH不是一个简单的任务，就算不同
*	的编译器厂商以不同的方式来实现它，也不是什么奇怪的事情。幸运的是，我们可以直接利用编译器对SEH的支持，而不需要理会编译器如何支持它的细节。
* 4）不要混淆结构化异常处理与C++异常处理。C++异常处理在形式上表现为使用关键字catch和throw，这和结构化异常处理的形式不同。Microsoft Visual C++支持异常处理，它在内部实现上其实就是利用
*	了编译器和Windows操作系统的结构化异常处理功能。
* 5）SEH实际上包含两方面的功能：终止处理（termination handling）和异常处理（exception handling）。
* 6）终止处理程序确保不管一个代码块（被保护代码（the guarded body））是如何退出的，另一个代码块（终止处理程序）总能被调用和执行。终止处理的语法（当使用Microsoft Visual C++编译器时）如下
*	所示：
*		__try{
*			// Guarded body
*			...
*		}
*		__finally{
*			// Termination handler
*			...
*		}
*	 __try和__finally关键字标记了终止处理程序的两个部分（被保护代码和终止处理程序）。在前面这段代码中，操作系统和编译器的协同工作保证了不管被保护代码部分是如何退出的--无论我们在被保护
*	代码中使用了return，还是goto，又或是longjump语句（除非调用ExitProcess，ExitThread，TerminateProcess，TerminateThread来终止进程或线程）--终止处理程序都会被调用，即__finally代码块都
	能执行。
* 7）读者可能想知道，编译器如何保证finally块可以在try代码块退出前被执行。原来当编译器检查程序代码时，会发现在try代码块里有一个return语句。于是，编译器就会生成一些代码先将返回值保存在
*	一个由它创建的临时变量里，然后再执行finally代码块，这个过程被称之为局部展开（local unwind）。更确切地说，当系统因为try代码块中的代码提前退出而执行finally代码块时，就会发生局部展开。
*	一旦finally代码块执行完毕，编译器所创建的临时变量的值就会返回给函数的调用者。
* 8）在异常发生时，finally块也不能保证绝对能得到执行。
*		·在windows XP系统里，如果一个“栈耗尽异常”（stack exhaustion exception）发生在try代码块里，finally块就很有可能得不到运行机会，因为运行在出错进程里的WER代码都可能没有足够的
*		  栈空间去报告错误。所以在这种情况下，进程往往是不加任何提示地被终止。
*		·如果异常导致SEH链的中断，终止处理程序也不会得到执行。
*		·最后，如果异常发生在异常过滤程序里，终止处理程序也不会被执行。
*	 一条经验法则是尽量限制在catch或者finally块中代码所做的工作，否则进程很有可能会在finally块执行前突然终止。这也就是为什么在Windows Vista系统上，错误报告过程（WER）运行在另一个独立
*	的进程里。
* 9）为了帮助我们尽可能避免写出让try块提前退出的代码，Microsoft为它的C/C++编译器加入了一个关键字：__leave。关键字__leave会导致代码执行控制流跳转到try块的结尾--也可以认为是跳转到了闭
*	花括号处。因为在这种情况下，代码执行将正常地从try块进入finally块，所以不会产生任何额外开销。
* 10）到目前为止，我们已经明确区分并讨论了以下两种会引起finally块执行的情形，同时强调第三种。
*		·从try块到finally的正常代码控制流。
*		·局部展开：从try块中的提前退出（由goto，longjump，continue，break，return等语句引起）将程序控制流强制转入finally块。
*		·第三种情形--全局展开--Windows Vista以前的系统上，try块中的代码抛出了访问内存违规或其他异常。对于Windows Vista来说，全局展开在默认情况下是不会被触发的，所以finally不会被执行。
*	 要确定是三种情况中的哪一种，可以（只能）在finally块里调用BOOL AbormalTermination()这个内在（由编译器识别并处理的特殊函数。编译器会为这个函数生成内联代码，而不是生成代码来调用这个
	函数）函数，它将返回一个布尔值来表明一个与当前finally块相关的try块是否已经提前退出。

*****************************************************************************************************************************************************************************************
* 11）CPU负责捕获类似非法内存访问和以0作为除数这样的问题，一旦侦测到这些错误行为，它会抛出相应的异常。由CPU抛出的异常都是硬件异常。操作系统和应用程序也可以抛出异常，这些异常通常被称为
*	软件异常。当一个硬件或者软件异常被抛出时，操作系统会给我们的应用程序一个查看异常类型的机会，并允许应用程序自己处理这个异常。下面的代码演示了异常处理程序的语法结构：
*			__try {
*				// Guarded body
*				...
*			}
*			__except (exception filter){
*				// exception handler
*				...
*			}
*	  请注意__except关键字，任何时候创建一个try块，后面必须跟一个finally代码块或except代码块。但是try块后不能同时有finally块和except块，也不能同时有多个finally或except块。不过却可以将
*	try-finally块嵌套于try-except块中，反过来也可以。
* 12）与终止处理程序不同，异常过滤程序（exception filter）与异常处理程序（exception handler）主要由操作系统来负责执行--在异常过滤程序表达式计算和异常处理程序执行方面，编译器所做的工作
*	十分有限。当异常抛出时，系统定位到except块的开始处，并对异常过滤程序的表达式求值，这个表达式的值必定为以下三个标识符之一，这些标识符在Microsoft Windows的Excpt.h文件中定义。
*																	异常过滤程序的返回值
*				———————————————————————————————————————————————————————————————————
*													标    识									|						值
*				———————————————————————————————————————————————————————————————————
*					EXCEPTION_EXECUTE_HANDLER													|						1
*				———————————————————————————————————————————————————————————————————
*					EXCEPTION_CONTINUE_SEARCH													|						0
*				———————————————————————————————————————————————————————————————————
*					EXCEPTION_CONTINUE_EXECUTION												|						-1
*				———————————————————————————————————————————————————————————————————
* 13）EXCEPTION_EXECUTE_HANDLER：这个值基本上等于告诉系统，“我知道这个异常，并预计这个异常在某种情况下会发生，同时已经写了一些代码来处理它，让这些代码现在就执行吧。”于是，系统
*	执行全局展开（即global unwind），让程序执行跳转到except代码块处（即异常处理程序代码）。当except代码块运行结束后，系统就会认为异常已经得到了处理，于是允许应用程序继续执行。这个
*	机制允许Windows应用程序捕获错误，处理它们，并继续执行，用户甚至不知道曾经发生过错误。当except块中的代码执行完毕后，代码执行从except块后的第一句代码继续执行。
* 14）全局展开
*	     void funcOStimpy1(){
*			// 1.Do any processing here.
*			...
*			__try {
*				// 2.Call another function
*				FuncORen1();
*				
*				// Code here nener executes.
*			}
*			__except (6.Evaluate filter.EXCEPTION_EXECUTE_HANDLER){
				// 8.After the unwind, the exception handler executes.
				MessageBox(...);
*			}
*			// 9.Exception handled--continue execution.
			...
*		 }

		 void FuncORen1() {
			DWORD dwTemp = 0;

			// 3.Do any processing here.
			...
			__try{
				// 4.Request permissiong to access protected data.
				WaitForSingleObject(g_hSem, INFINITE);

				// 5.Modify the data.
				// An exception is generated here.
				g_dwProtectedData = 5 / dwTemp;
			}
			__finally{
				// 7.Global unwind occurs because filter evaluated
				// to EXCEPTION_EXECUTE_HANDLER.

				// Allow others to use protected data.
				ReleaseSemaphore(g_hSem, 1, NULL);
			}

			// Continue processing--never executes.
			...
		 }
*		  注释前面的数字代表了执行的顺序，这就是结构化异常的工作机制。我们代码的执行与系统的控制有很大关联，使得SEH很难理解。代码执行控制流不再是从头到尾，系统根据自己的意图决定
*		代码每一部分的执行顺序。系统的执行顺序虽然复杂，但仍然是可推断的。
*		  为了更好地理解执行的顺序，让我们从另一个稍微不同的角度来看看发生了什么。当异常过滤程序返回EXCEPTION_EXECUTE_HANDLER时，等于在告诉操作系统，当前线程指令指针应该指向except
*		块中的指令。然而，实际上当前指令指针指向的是try块中的指令。我们知道当一个线程离开try-finally结构的try块部分，系统需要确保finally块中的代码得到执行。当异常发生时，系统用来
*		确保这条规则成立的机制就是全局展开。
*		  我们可以通过将return语句置于finally块中以阻止系统完成全局展开。
*		  注意：从Windows Vista开始，如果一个异常发生在try/finally块中，并在其上层又没有try/except块（同时过滤程序返回EXCEPTION_EXECUTE_HANDLER），进程就会立即终止。即全局展开并不会
*		发生，finally块也不会执行。但是在早期版本的Windows中，全局展开会在进程终止前发生，从而finally块也有机会得到执行。
* 15）EXCEPTION_CONTINUE_EXECUTION：系统在看到过滤程序返回值为EXCEPTION_CONTINUE_EXECUTION后，将进程控制流跳转到导致异常的那条指令，并尝试重新执行这条指令。
*		示例：
*			TCHAR g_szBuffer[100];
*			void FunclinRoosevelt1(){
*				int x = 0;
*				TCHAR *pchBuffer = NULL;
*				__try {
*					*pchBuffer = TEXT('J');
*					x = 5 / x;
*				}
*				__except (OilFilter1(&pchBuffer)){
*					MessageBox(NULL, TEXT("An exception occurred"), NULL, MB_OK);
*				}
*				MessageBox(NULL, TEXT("Function completed"), NULL, MB_OK);
*			}
* 
*			LONG OilFilter1(TCHAR **ppchBuffer) {
*				if (*ppchBuffer == NULL) {
*					*ppchBuffer = g_szBuffer;
*					return(EXCEPTION_CONTINUE_EXECUTION);
*				}
*				return(EXCEPTION_EXECUTE_HANDLER);
*			}
*	    如上所示：实际上，可以让异常过滤程序调用一个函数，来决定返回三个标识符中的哪一个。我们可以在异常过滤程序里面添加很多代码。当然，过滤程序必须返回三个异常标识符中的其中一个。
*	  但是它也可以执行其他我们想要它执行的任务。不管怎样，请记住在一个异常被抛出后，进程很可能已经变得不稳定。所以，最好还是让异常过滤程序中的代码相对简单。比如，在堆被破坏的情况下，
*	  在过滤程序里运行很多的代码可能导致进程被挂起或者不加任何提示地终止。
*		谨慎使用EXCEPTION_CONTINUE_EXECUTION。尝试纠正导致异常发生的错误有时候可以成功，但并不是每次都这么幸运 -- 它成功与否取决于程序的目标CPU，和我们的编译器如何为C/C++语句产生具体
*	  机器指令和编译选项。有一种情况，返回EXCEPTION_CONTINUE_EXECUTION将保证继续执行始终都能成功：当我们为已预订的内存区域稀疏地调拨存储器时。
* 16）EXCEPTION_CONTINUE_SEARCH：这个标识符让系统在调用栈中向上查找前一个带except块的try代码块，并调用这个try块对应的异常过滤程序。
* 17）决定在异常过滤程序中返回什么标识符前，我们必须具体情况具体分析。异常过滤程序应根据具体的情况返回合适的值。
*		示例：
*			__try {
*				x = 0;
*				y = 4 / x; 
*			}
*			__except ((GetExceptionCode() == EXCEPTION_INT_DIVIDE_BY_ZERO) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH){
*			}
*	  GetExceptionCode是内在函数，它的返回值表明刚刚发生的异常的类型。
*	  下面列出了一些预定义的异常标识符（异常代码遵循在WinError.h文件中定义的有关错误代码的规则）和它们具体的含义，它们来自于Platform SDK的文档。我们可以WinBase.h文件中找到它们。归类如下：
*		与内存相关的异常如下：
*			· EXCEPTION_ACCESS_VIOLATION 线程试图读写一个虚拟内存地址，但在这个地址它并不具备相应的权限。这是最常见的异常。
*			· EXCEPTION_DATATYPE_MISALIGNMENT
*			· EXCEPTION_ARRAY_BOUNDS_EXCEEDED
*			· EXCEPTION_IN_PAGE_ERROR
*			· EXCEPTION_GUARD_PAGE
*			· EXCEPTION_STACK_OVERFLOW
* 			· EXCEPTION_ILLEGAL_INSTRUCTION
* 			· EXCEPTION_PRIV_INSTRUCTION
*		与异常本身相关的异常如下：
* 			· EXCEPTION_INVALID_DISPOSITION
* 			· EXCEPTION_NONCONTINUABLE_EXCEPTION
*		与调试相关的异常如下：
*  			· EXCEPTION_BREAKPOINT
*   		· EXCEPTION_SINGLE_STEP
*   		· EXCEPTION_INVALID_HANDLE
*		与整形相关的异常如下：
*   		· EXCEPTION_INT_DIVIDE_BY_ZERO
*   		· EXCEPTION_INT_OVERFLOW
*		与浮点数相关的异常如下：
*   		· EXCEPTION_FLT_DENORMAL_OPERAND
*   		· EXCEPTION_FLT_DIVIDE_BY_ZERO
*   		· EXCEPTION_FLT_INEXACT_RESULT
*   		· EXCEPTION_FLT_INVALID_OPERATION
*   		· EXCEPTION_FLT_OVERFLOW
*   		· EXCEPTION_FLT_STACK_CHECK
*   		· EXCEPTION_FLT_UNDERFLOW
*		内在函数GetExceptionCode只能在异常过滤程序里（即__except之后的括号里）或者异常处理程序的代码里调用。但是我们不能在异常过滤程序的函数里调用GetExceptionCode。
* 18）当一个异常发生时，操作系统将向发生异常的线程的栈中压入三个结构：EXCEPTION_RECORD，CONTEXT和EXCEPTION_POINTERS。
*			· EXCEPTION_RECORD：结构包含关于抛出异常的信息，这些信息的内容与具体的CPU没有关系。
*			· CONTEXT：结构则包含关于异常但与CPU也有关的信息。
*			· EXCEPTION_POINTERS：结构仅包含两个数据成员，它们分别为指向被压入栈中的EXCEPTION_RECORD结构的指针和指向CONTEXT结构的指针。
*	  如果想要得到这些信息，并在应用程序中使用它们，可以如下调用函数GetExceptionInformation: 
*			PEXCEPTION_POINTERS GetExceptionInformation();
*	  重要提示：这个函数只能在异常过滤程序中调用，这是因为这三个结构只有在系统计算异常过滤程序时才是有效的。一旦程序控制流转移到异常处理程序或者别的地方，这些栈上的数据结构
*	就被销毁了。
* 19）其实，我们也可以在应用程序代码里强制抛出一个异常。这是函数将运行失败通知其调用者的另一种方式。
*	  抛出一个软件异常再容易不过了，只需要调用RaiseException函数。
*			
*/

#include <Windows.h>

namespace HY_SEG
{
	class CHYSEG
	{
	public:
		CHYSEG(void);
		~CHYSEG(void);

		// 获取刚刚发生的异常的类型
		//DWORD GetExceptionCode();

		// 抛出一个软件异常
		VOID RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, CONST ULONG_PTR *pArguments);

	private:
		CHYSEG(const CHYSEG&) = delete;
		CHYSEG& operator=(const CHYSEG&) = delete;
		CHYSEG(CHYSEG&&) = delete;
		CHYSEG& operator=(CHYSEG&&) = delete;
	};
}


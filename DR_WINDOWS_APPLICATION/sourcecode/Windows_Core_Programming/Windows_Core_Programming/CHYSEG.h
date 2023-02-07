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
***********************************************************************************************************************************************************************************
* 20）异常过滤程序返回EXCEPTION_CONTINUE_SEARCH时所发生的情况：系统继续在调用树中的上层寻找异常过滤程序。可是如果每个异常过滤程序都返回EXCEPTION_CONTINUE_SEARCH，又会发生什么呢？
*	在这种情况下，我们会遇到所谓的未处理异常。
*	  Microsoft Windows提供了函数SetUnhandledExceptionFilter，它给了我们处理异常的最后机会，否则Windows就会正式认为这个异常没有得到处理。
*	  我们通常应在进程初始化阶段调用这个函数。一旦这个调用了这个函数，进程中任意线程抛出未处理异常都会导致我们指定（通过SetUnhandledExceptionFilter的参数）的最上层异常过滤函数执行。
*	这个异常过滤函数原型必须与下面类似：
*			LONG WINAPI TopLevelUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
*     在异常过滤程序里，只要返回三个EXCEPTION_*标识符之一，就可以做任何我们想要做的处理。注意，进程可能因为栈溢出而未释放同步信号，或者没有释放在堆上分配的内存，而已经处于被破坏的
*	状态。所以，我们应该尽量减少异常过滤函数的处理工作，而且不要再申请动态内存，因为堆可能已经被破坏。
* 21）当我们设置新的未处理异常过滤程序时，SetUnhandledExceptionFilter返回上次安装的异常过滤程序的地址。注意，如果程序使用的是C/C++运行库，在进程入口点函数开始执行前，C/C++运行函数
*	就会安装一个默认的全局异常过滤程序，即__CxxUnhandledExceptionFilter。这个函数所做的工作很简单，首先检查异常是不是属于C++异常。如果是，则在结束时执行abort函数，后者将调用Kernel32.dll
*   中的UnhandledExceptionFilter函数。在早期版本的C/C++运行库中，进程到此就终止了。_set_abort_behavior函数可以用来配置由abort函数执行的错误报告。如果__CxxUnhandledExceptionFilter认为
*	当前异常不是一个C++异常，就会返回EXCEPTION_CONTINUE_SEARCH，来让Windows来处理这个未处理异常。
*	  我们可以以NULL作为参数调用SetUnhandledExceptionFilter来将全局未处理异常过滤程序设为UnhandledExceptionFilter。
* 22）我们知道每个线程的执行都是从NTDLL.dll中的函数BaseThreadStart开始的：
*			VOID BaseThreadStart(PTHREAD_START_ROUTINE pfnStartAddr, PVOID pvParam){
*				__try {
*					ExitThread((pfnStartAddr)(pvParam));
*				}
*				__except(UnhandledExceptionFilter(GetExceptionInformation())) {
*					ExitProcess(GetExceptionCode());
*				}
*				// NOTE: We never get here
*			}
*	  这个函数包含一个结构化异常处理框（structured exception handling frame, SEH代码框）：它首先进入try代码块，然后在try块中调用线程/程序的入口点函数。所以，如果我们的线程抛出一个异常，
*	并且所有安装的异常过滤程序都返回EXCEPTION_CONTINUE_SEARCH，系统提供的一个特殊函数将被自动调用，即UnhandledExceptionFilter。
*			LONG UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
*	  和普通异常过滤程序一样，这个函数返回三个EXCEPTION_*异常标识符中的一个。
*	  说明：在发生嵌套异常（即在异常过滤程序里又抛出一个异常）的情况下，UnhandledExceptionFilter将返回EXCEPTION_NESTED_CALL。但在Windows Vista之前的平台，UnhandledExceptionFilter并不
*	返回，进程不加任何提示地结束。
* 23）UnhandledExceptionFilter在返回这些异常标识符之前，还会执行大量的代码。UnhandledExceptionFilter函数在处理异常时会按顺序执行5个步骤。我们将依次讨论每个步骤。在执行完这些步骤后，
*	UnhandledExceptionFilter将执行控制交给Windows错误报告（Windows Error Reporting，后面简称为WER）。
*		·步骤1：允许对资源进行写入操作并继续执行
*		·步骤2：将未处理异常报告给调试器
*		·步骤3：通知我们设置的全局异常过滤函数
*		·步骤4：将未处理异常报告给调试器（再次）
*		·步骤5：终止进程
*	  在这些步骤里，UnhandledExceptionFilter工作于后台，试图解决引发异常的问题，通知调试器（如果有），或者在必要情况下简单地终止程序运行。如果不能处理异常，就会返回
	EXCEPTION_CONTINUE_SEARCH，于是系统内核得到程序控制，将程序运行错误通知用户。
* 24）从Windows Vista开始，UnhandledExceptionFilter函数不再发送错误报告到Microsoft的服务器。作为替代，返回EXCEPTION_CONTINUE_SEARCH。于是，系统内核知道一个异常在用户态线程没有得到处理。
*	然后，关于异常的通知被发送给WerSvc专用服务。这个通知是由一个叫做“高级本地过程调用（Advanced Local Procedure Call，后面称为ALPC）”的机制发出的，ALPC机制并没有对外公开。ALPC会先
*	阻塞线程，直到WerSvc完成它的处理。后者通过调用CreateProcess来启动WerFault.exe，并等待新进程的结束。报告的创建和发送由WerFault.exe完成。在WerFault程序中，会弹出一个对话框来允许用户
*	选择关闭应用程序或者附着一个调试器。如果用户选择关闭程序，WerFault.exe会调用TerminateProcess来不加提示地完全结束应用程序。由此可见，大量处理会在发生错误的进程之外来保证错误报告行为
*	的稳定。
*	  我们可以通过注册表配置在这个过程中呈现给用户的界面，当HKEY_CURRENT_USER\Software\Microsoft\Windows\Windows Error Reporting子项之下的DontShowUI值设置为1时，就不会有任何对话框弹出，
*	报告会在后台生成并发送给Microsoft的服务器。如果想在问题发生时让用户选择发送或者不发送错误报告给Microsoft，我们可以改变DefaultConsent的值，值类型为DWORD，位于Consent子项下。但是，
*	更值得推荐的做法是在Control Panel（控制面板）中的Problem Reports And Solution（问题报告与解决方案）来打开WER控制台，并单击设置来访问这些选项。
*	  现在我们来讨论一下当未处理异常发生时，WER提供的最后一个功能：即时调试，它让程序员的梦想称为现实。如果用户选择调试出错的进程，WerFault.exe程序就会创建一个事件，将手动重置设置为True，
*	同时置为未触发状态，并且设置bInheritHandles参数为True。从而允许WerFault.exe的子进程（比如调试器），可以继承这个事件句柄，然后WER找到并启动默认调试器，让它附着到出错的进程上。
* 25）即时调试
*	  可以动态附着调试器到进程是Windows最好的功能之一。
*	  工作原理：
*		注册表有这个相关子项目：
			HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug
		这个子项有一个名为Debugger的值。在Visual Studio的安装过程被设置为如下:
			"C:\Windows\system32\vsjitdebugger.exe" -p %ld -e %ld
		vsjitdebugger.exe本身并不是一个调试器。他只是运行一个对话框允许我们来选择调试器的应用程序。
		WerFault.exe会给调试器传入两个参数。第一个是要调试的进程ID，第二个是继承过来的事件句柄，它指向由WerSvc服务创建的处于未触发的手动重置事件。注意，问题进程也在等待WerSvc返回的ALPC
	  通知。第三方厂商实现的调试器要能够识别-p 和-e两个开关，代表进程ID和事件句柄。
	    WerFault.exe先将进程ID和事件句柄合并到一个字符串里，然后通过CreateProcess启动调试器，并将bInheritHandles参数设置为TRUE，允许调试器的进程可以继承事件对象的句柄。此时调试器进程启
	  动并查看命令行参数。如果-p开关存在，调试器拿到进程ID并通过DebugActiveProcess将自己附着到对应进程上：
		BOOL DebugActiveProcess(DWORD dwProcessID);
	    操作系统对调试器完成附着以后，会将被调试程序的状态报告给调试器。比如，操作系统会告诉调试器被调试程序有多少线程以及被调试程序载入多少DLL。调试器需要花时间处理这些数据来准备调试
	  。在这些准备工作进行时，发生问题的进程只能等待。
		检测到未处理异常的那段代码（步骤4）也在等待从WerSvc返回的ALPC通知（步骤5）而ALPC本身因为以WerFault.exe进程句柄为参数调用WaitForSingleObjectEx被阻塞，直到WER完成它的工作。注意
	  这里是WaitForSingleObjectEx，线程可以在可通知状态下等待。队列里的异步过程调用（asynchronous procedure call， APC）都能得到处理。
	    调试器初始化以后，开始检查-e开关。如果存在 调试器得到事件句柄并调用SetEvent。
		设置事件（步骤11）将通知WerFault.exe调试器已经附着到了问题进程，可以接受并处理异常了。于是WerFault.exe进程将正常退出，而WerSvc服务也发现其子进程退出了，于是让ALPC返回（步骤12）
	  。这就导致了被调试程序的线程被唤醒。它会通知内核一个调试器已经附着到当前线程，并准备好接受未处理异常的通知（第13步）这就和25.1的步骤3效果相同。紧接着，调试器将收到关于未处理异常
	  的通知，它将载入正确的源码文件，并定位到抛出异常的那条指令。这很酷，不是吗！
	    技巧提示：
		  HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug注册表子项还包含一个名为Auto的数值，类型为REG_SZ。这个值表示是否让WER询问用户调试还是关闭问题进程。如果
	  Auto值为1，WER不询问用户，直接启动调试器。这应该是我们想要的默认行为，而不是让WER弹出两个消息框--因为当未处理异常发生时，我们总是迫切希望调试问题进程。
		  有时，我们并不希望调试一些容器进程，比如svchost.exe的某个服务崩溃。在这种情况下，可以在AeDebug子项下再添加一个子项AutoExclusionList，并在它下面创建一个DWORD数值，将其名称设为
	  我们不想调试的程序，值设为1.
		  如果需要进一步细分哪个程序需要自动即时调试，我们可以设置Auto为1，但是在HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\Windows Error Reporting之下添加一个子项DebugApplications。
	  在这个子项下，创建一个DWORD数值，将其名称设为我们想要在未处理异常发生时自动调试的程序，值设为1。
* 26）向量化异常和继续处理程序(https://blog.csdn.net/sesiria/article/details/78895499)
*	  前面介绍的SEH机制是一种基于代码框（frame-based）机制，即每次当线程进入一个try块（或代码框）时，系统会在链表里加入一个节点。如果发生异常，系统依次访问链表中的每个代码框--从最晚进入
    的try块开始一直到最早进入的try块--并寻找每个try块的catch处理程序。一旦找到一个catch处理程序，系统再次访问链表，执行finally块。当展开结束以后（或者try块由于没有发生异常而正常退出），
	系统从链表中移出所有代码框。
	  Windows也提供了向量化异常处理（vectored exception handling， VEH）机制，它同SEH协同工作。程序可以注册一个函数，而不需要依赖于语言有关的关键字，每当异常发生或者一个未处理异常脱离
	标准SEH的控制时，这个函数就会被调用。
	  AddVectoredExceptionHandler函数负责异常处理程序的注册，注册的异常处理程序会被添加到函数列表里，当进程中任意线程发生异常时，这些函数都会被调用。
		PVOID AddVectoredExceptionHandler(
			ULONG bFirstInTheList,
			PVECTORED_EXCEPTION_HANDLER pfnHandler
		);
	  Handler是指向向量化异常处理程序的指针，该函数必须有如下签名：
		LONG WINAPI ExceptionHandler(struct _EXCEPTION_POINTERS* pExceptionInfo);
	  如果给bFirstInTheList参数传递的值为0， 那么通过pfnHandler传递的异常处理函数就会添加到列表的尾端。如果传递给bFirstInTheList传递的值非0，函数就会被至于内部列表的头部。当异常发生时，
	系统在执行SEH过滤程序之前，将按列表顺序逐个调用这些函数。 一旦某个函数能够纠正发生的问题，应该马上返回EXCEPTION_CONTINUE_EXECUTION，以让抛出异常的指令再次执行。只要某个向量处理程序
	返回EXCEPTION_CONTINUE_EXECUTION, SEH过滤程序便不会有处理异常的机会。 如果异常处理函数不能纠正问题，应该返回EXCEPTION_CONTINUE_SEARCH, 让列表中的其他处理函数有机会去处理异常。如果
	所有的处理函数都返回EXCEPTION_CONTINUE_SEARCH, SEH过滤程序就会执行，需要注意的是，VEH过滤函数不能返回EXCEPTION_EXECUTION_HANDLER。
	  可以通过以下函数来删除之前安装的VEH异常处理函数：
	    ULONG RemoveVectoredExceptionHandler(PVOID pHandler);
	  Handle参数即之前安装的函数句柄。这个句柄可以从AddVectoredExceptionHandler的返回值得到。

	  除了能在SEH之前处理异常，VEH还能让我们在未处理异常发生时能得到通知，要得到这个通知的详细信息，我们需要通过调用下面这个函数注册一个继续处理程序（continue handler）：
		PVOID AddVectoredContinueHandler(ULONG bFirstInTheList, PVECTORED_EXCEPTION_HANDLER pfnHandler);
	  同样，如果给bFirstInTheList参数传递0，那么通过pfnHandler传入的函数会添加到继续处理程序列表的尾部。如果bFirstInTheList是一个非0值，函数会被置于继续处理程序列表的头部。当未处理异常
	发生时，系统会按照列表次序逐个执行继续处理程序函数。需要特别说明的是，这些处理程序的执行是在SetUnhandledExceptionFilter所安装的全局异常处理程序返回EXCEPTION_CONTINUE_SEARCH之后才开始
	的。一个继续处理程序可以返回EXCEPTION_CONTINUE_EXECUTION来停止在它后面的继续处理程序的执行，并让系统再次执行失败命令。或者返回EXCEPTION_CONTINUE_SEARCH让系统执行在它后面的处理程序。
	  我们可以通过下面这个函数来从列表里删除之前安装的继续处理程序：
		ULONG RemoveVectoredContinueHandler(PVOID pHandler);
	  Handle参数表示之前安装的处理程序函数的句柄。 这个句柄可以从AddVectoredExceptionHandler的返回值得到。
	  由此可以想象，继续处理程序一般用来实现诊断与跟踪。
* 27）C++异常与结构化异常的比较
*	  SEH是操作系统所提供的便利，它在任何语言中都可以使用。而C++异常处理只有在编写C++代码时才可以使用。如果读者在开发C++应用程序，那就应该使用C++异常，而不是结构化异常。理由是C++异常
*	是语言的一部分，编译器知道什么是一个C++对象。这也就意味着编译器会自动生成代码来调用C++对象的析构函数，保证对象的释放。
*	  我们应该了解Microsoft的Visual C++编译器使用操作系统的结构化异常机制来实现C++异常处理机制。所以，在创建一个C++try块时，编译器会为我们生成一个SEH __try块。C++的catch语句对应SEH
*   异常过滤程序，catch块中的代码则对应SEH __except块中的代码。而编译器也会为C++throw语句生成对Windows RaiseException函数的调用。throw语句所使用的变量则成为RaiseException的附加参数。
*		示例：
*		C++异常处理										C++编译器生成的对应的就结构化异常处理代码
*		void ChunkyFunky(){								void ChunkyFunky(){
*			try {											try {
*				// Try body										// Try body
*				...												...
*				throw 5;										RaiseException(Code=0xE06D7363, Flag=EXCEPTION_NONCONTINUABLE, Args=5);
*			}												}
*			catch (int x) {									__except((ArgType == Integer) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTIOIN_CONTINUE_SEARCH) {
*				// Catch body									// Catch body
*				...												...
*			}												}
*			...												...
*		}												}
* 28）Microsoft Visual Studio的调试器为异常调试提供了出色的支持。当进程中的某个线程抛出异常，操作系统会马上通知调试器（如果调试器已经附着）。这个通知被称为“首次机会通知”
	（first-chance notification）。调试器将响应这个通知，促使线程寻找异常过滤程序。如果所有的异常过滤程序都返回EXCEPTION_CONTINUE_SEARCH，操作系统会给调试器一个“最后机会通知”
	（last-chance notification）。这两个通知的存在使得软件开发人员能更好地控制异常调试的过程。
	  针对每个解决方案，我们可以通过调试器的Exceptions对话框决定调试器如何响应首次机会异常通知。
*****************************************************************************************************************************************************************************************
* 29）Windows错误报告控制台
*	  当一个进程因为未处理异常而终止时，WER会创建关于未处理异常及其执行上下文（execution context）的错误报告（error reporting）。
*	  如果得到用户的许可，这个报告就会通过安全信道发送给Microsoft的服务器，在那里它会和数据库里的已知问题做比较。如果数据库里已经有这个问题的解决方案，服务器会将它发送给用户，这样
*	用户就可以采取相应步骤来继续完成自己的工作。
*	  硬件与软件厂商可以利用这个技术来访问与其注册产品相关的报告。这个过程也适用于内核模式设备驱动程序的崩溃或者挂起，因此可能的解决方案将是多种多样的。
*	  就算用户不愿发送报告给Microsoft的服务器，生成的报告也会保存在用户的机器上。通过WER控制台，用户可以在本地机器上浏览这些曾经发生的问题并查看相应的报告（控制面板\所有控制面板项\安全
*	和维护\问题报告）。
* 30）可编程的Windows错误报告
*	  应用程序可以选择在失败时不让WER发送报告。如果应用程序还处于部署和发布前的开发和测试阶段，那么这个选项对我们很有用处。
* 31）有时，我们可能想让应用程序通过调用一系列的WER函数定制错误报告。下面是我们可能需要定制错误报告的三种情况。
*		·我们在编写自己的未处理异常过滤程序。
*		·我们想要应用程序在未处理异常没有发生的情况下也生成报告。
*		·我们想往错误报告里添加更多信息。
*	  定制错误报告的一种简单方式是指出哪些数据块或者文件需要添加到为进程生成的每个问题报告里。
* 32）在应用程序中定制错误报告。读者可以在错误（包含那些和异常处理没有什么关系的问题）出现时，使用这里描述的函数。应用程序在创建报告后不需要马上终止。我们应该尽量使用Windows错误报告，
*	而不是将一些含义模糊的信息写入到Windows事件日志。但是，WER框架通过一些注册表数值限制了报告的大小和数目，我们可以在HKEY_CURRENT_USER\Software\Microsoft\Windows\Windows Error Reporting
*	子项下找到这些值：
*		————————————————————————————————————————————————————————————————————————————
*				注册表设置			|														描述
*		————————————————————————————————————————————————————————————————————————————
*			MaxArchiveCount			|	存档中文件数目上限。为1~5000之间的一个值。默认值为1000
*		————————————————————————————————————————————————————————————————————————————
*			MaxQueueCount			|	在发送给Microsoft的服务器之间保存在本地机器队列里的报告数目上限。为1~500之间的一个值。默认值为50
*		————————————————————————————————————————————————————————————————————————————
*	  说明：
*		已发送报告的跟踪信息保存在当前用户的AppData\Local\Microsoft\Windows\WER\ReportArchive目录。但是，附加文件不在这个目录。未发送报告队列保存在当前用户的AppData\Local\Microsoft
*	  Windows\WER\ReportQueue目录。遗憾的是，WER控制台用来访问这些报告的API并没有公开，所以我们不能枚举应用程序的问题报告。
* 33）问题报告的创建、定制和提交给WER是按一下步骤调用不同的函数来实现的。
*		(1) 调用WerReportCreate来创建一个新问题报告。
*		(2) 调用WerReportSetParameter零次或者多次来设置报告参数。
*		(3) 调用WerReportAddDump来将小型转储放进报告。
*		(4) 调用WerReportAddFile零次或者多次来将任意文件（比如用户文档）放进报告。
*		(5) 调用WerReportSetUIOption来修改一些字符串，当调用WerReportSubmit时，这些字符串可能会在同意对话框中呈现给用户。
*		(6) 调用WerReportSubmit来提交一个报告。根据不同的标志，Windows可能会将报告放进队列，或者提示用户将报告发送给服务器，或者直接发送报告。
*		(7) 调用WerReportCloseHandle来关闭报告。
* 34）应用程序在运行期发生一个严重的问题时，WER可以在程序终止以后自动重启它。对Windows Vista系统来说，这是一个衡量程序质量的要求，随同Windows Vista一起发布的大部分应用程序（比如
*	Windows Explorer，Internet Exporer，RegEdit以及一些游戏）就支持自动重启。更有甚者，WER允许应用程序在被终止前恢复任何重要的数据。
* 35）但是应用程序重启特性其实是一套更为通用的API的一部分，这套API被称为重启管理（Restart Manager）。为了防止重复地重启出错的应用程序，WER在重启它之前会先检查它是否已经运行了至少60秒。
* 36）进程可以注册一个回调函数让WER在进程非正常终止时调用。这个回调函数可以保存任意数据和状态信息。
*		示例：
*			DWORD WINAPI ApplicationRecoveryCallback(PVOID pvParameter){
*				DWORD dwReturn = 0;
*				BOOL bCancelled = FALSE;
*				while (!bCancelled)
*				{
*					// Show progress
*					ApplicationRecoveryInProgress(&bCancelled);
*					
*					// Check for user cancellation
*					if (bCancelled) {
*						// The user clicked the Cancel button
*						// Notify that we have failed to finish the recovery process
*						ApplicationRecoveryFinished(FALSE);
*					}
*					else
*					{
*						// Save teh state required for recovery, chunk by chunk
*						if (MoreInformationToSave()) {
*							// Save a chunk of data in fewer milliseconds than the delay set with the dwPingInterval parameter you paased to RegisterApplicationRecoveryCallback
*						}
*						else 
*						{
*							// Notify that we have finished the recovery process 
*							ApplicationRecoveryFinished(TRUE);
*							// Set bCancelled to TRUE when finished to exit the loop
*							bCancelled = TRUE;
*						}
*					}
*				}
*				return(dwReturn);
*			}
*	  需要记住的是，当回调函数运行时，进程可能处在一个被破坏的状态，同时，对异常过滤程序的限制同样适用于这种类型的回调函数。
*/

#include <Windows.h>
#include <werapi.h>

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

		// 使应用程序能够取代进程的每个线程的顶级异常处理程序
		LPTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilter(
			LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter
		);

		// 检测当前进程是不是处于调试器的控制之下
		BOOL IsDebuggerPresent();

		// *************** 可编程的错误报告 ****************
		// 下面的函数可以让我们为进程改变一些设置
		HRESULT WerSetFlags(DWORD dwFlags);
		// 如果想知道某个进程的当前设置，可以调用下面这个函数
		HRESULT WerGetFlags(HANDLE hProcess, PDWORD pdwFlags);
		// 可以调用下面这个函数来禁用报告生成与发送
		HRESULT WerAddExcludedApplication(PCWSTR pwzExeName, BOOL bAllUsers);
		// 启用错误报告
		HRESULT WerRemoveExcludedApplication(PCWSTR pwzExeName, BOOL bAllUsers);
		
		// 要添加任意数据块(可以多次调用，来将多个数据库保存在小型转储文件中)
		HRESULT WerRegisterMemoryBlock(PVOID pvAddress, DWORD dwSize);
		// 要添加任意文件到问题报告中，我们只需要调用
		HRESULT WerRegisterFile(PCWSTR pszFileName, WER_REGISTER_FILE_TYPE regFileType, DWORD dwFlags);
		// 移除已经注册的数据库
		HRESULT WerUnregisterMemoryBlock(PVOID pvAddress);
		// 移除已经注册的文件
		HRESULT WerUnregisterFile(PCWSTR pszFileName);
		
		// 创建一个自定义的错误报告，并传入报告的详细信息
		HRESULT WerReportCreate(PCWSTR pwzEventType, WER_REPORT_TYPE repType, PWER_REPORT_INFORMATION pReportInformation, HREPORT* phReportHandle);
		// 设置唯一标识指定报表的事件的参数
		HRESULT WerReportSetParameter(HREPORT hReportHandle, DWORD dwparamID, PCWSTR pwzName, PCWSTR pwzValue);
		// 将小型转储文件放入报告
		HRESULT WerReportAddDump(
			HREPORT hReportHandle,
			HANDLE hProcess,
			HANDLE hThread,
			WER_DUMP_TYPE dumpType, 
			PWER_EXCEPTION_INFORMATION pExceptionParam, 
			PWER_DUMP_CUSTOM_OPTIONS pDumpCustomOptions, 
			DWORD dwFlags
		);
		// 将任意文件放入报告
		HRESULT WerReportAddFile(
			HREPORT hReportHandle,
			PCWSTR pwzPath,
			WER_FILE_TYPE repFileType,
			DWORD dwFileFlags
		);
		// 修改对话框文本
		HRESULT WerReportSetUIOption(
			HREPORT hReportHandle,
			WER_REPORT_UI repUITypeID,
			PCWSTR pwzValue
		);
		// 提交错误报告
		HRESULT WerReportSubmit(
			HREPORT hReportHandle,
			WER_CONSENT consent,
			DWORD dwFlags,
			PWER_SUBMIT_RESULT pSubmitResult
		);
		// 在报告提交以后，记得要关闭问题报告
		HRESULT WerReportCloseHandle(HREPORT hReportHandle);

		// 应用程序如果想允许自动重启，必须通过下面这个函数在WER中注册自己
		HRESULT RegisterApplicationRestart(PCWSTR pwzCommandline, DWORD  dwFlags);
		// 告诉WER不要重启当前的应用程序，可以通过下面这个函数
		HRESULT UnregisterApplicationRestart(void);

		// 进程可以注册一个回调函数让WER在进程非正常终止时调用。这个回调函数可以保存任意数据和状态信息
		HRESULT RegisterApplicationRecoveryCallback(
			APPLICATION_RECOVERY_CALLBACK pRecoveyCallback,
			PVOID pvParameter,
			DWORD dwPingInterval,
			DWORD dwFlags
		);

	private:
		CHYSEG(const CHYSEG&) = delete;
		CHYSEG& operator=(const CHYSEG&) = delete;
		CHYSEG(CHYSEG&&) = delete;
		CHYSEG& operator=(CHYSEG&&) = delete;
	};
}


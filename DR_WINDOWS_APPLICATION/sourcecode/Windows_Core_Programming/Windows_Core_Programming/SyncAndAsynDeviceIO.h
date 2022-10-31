#pragma once

#include <Windows.h>
/*
* 知识：
* 1）为了不让线程闲下来，我们需要让各个线程就它们正在执行的操作相互通信。Microsoft在这个领域花了数年的时间进行研究和测试，并开发出了一种非常好的机制来进行这类通信。
*	这种机制被称为I/O完成端口（I/O completion port），它可以帮助我们创建高性能而且伸缩性好的应用程序。通过使用I/O完成端口，我们可以让线程在读取设备和写入设备的时候不必
*	等待设备的影响，从而显著地提高吞吐量。
* 2）作为一名Windows开发人员，这么多年来，我发现I/O端口有越来越多的用处，也认为每个Windows开发人员都必须完全理解I/O完成端口的工作方式。虽然本章介绍的I/O完成端口与设备I/O
*	有关，但值得注意的是I/O完成端口也可以和设备I/O完全无关--简单地说，它是一种有无数种用途的绝佳的线程间通信机制。努力称为I/O完成端口的一名忠实拥护者。
* 3）Windows的优势之一是它所支持的设备数量。就我们的讨论而言，我们把设备定义为能够与之进行通信的任何东西。
*																		各种设备及其常见用途
*	=============================================================================================================================================================
*			设备		|															常见用途
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		文件			|	永久存储任何数据
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		目录			|	属性和文件压缩的设置
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		逻辑磁盘驱动器	|	格式化驱动器
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		物理磁盘驱动器	|	访问分区表
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		串口			|	通过电话线传输数据
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		并口			|	将数据传输至打印机
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		邮件槽			|	一对多数据传输，通常是通过网络传到另一个进行Windows的机器上
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		命名管道		|	一对一数据传输，通常是通过网络传到另一个进行Windows的机器上
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		匿名管道		|	单机上的一对一数据传输（绝对不会跨网络）
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		套接字			|	报文或数据流的传输，通常是通过网络传输到任何支持套接字的机器上（机器不一定要运行Windows操作系统）
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		控制台			|	文本窗口的屏幕缓存
*	=============================================================================================================================================================
* 4）为了执行任何类型的I/O，我们必须先打开想要操作的设备并得到一个句柄。得到设备句柄的方式取决于具体的设备类型。
*																		用来打开各种设备的函数
*	=============================================================================================================================================================
*			设备		|															用来打开设备的函数
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		文件			|	CreateFile(pszName为路径名或UNC路径名)
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		目录			|	CreateFile(pszName为路径名或UNC路径名)。如果在调用CreateFile的时候指定FILE_FLAG_BACKUP_SEMANTICS标志，那么Windows允许我们打开一个目录。
*						|	打开目录使我们能够改变目录的属性（比如正常、隐藏，等等）和它的时间戳。
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		逻辑磁盘驱动器	|	CreateFile(pszName为"\\.\x:")。如果指定的字符串是"\\.\x:"的形式，那么Windows允许我们打开一个逻辑磁盘驱动器，其中的x是驱动器的盘符。
*						|	打开驱动器使我们能够格式化驱动器或检测驱动器媒介的大小
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		物理磁盘驱动器	|	CreateFile(pszName为"\\.\PHYSICALDRIVEx")。如果指定的字符串是"\\.\PHYSICALDRIVEx"的形式，那么Windows允许我们打开一个物理磁盘驱动器，
*						|	其中的x是物理驱动器号。例如，为了读写用户的第一个物理驱动器的扇区，我们应该指定"\\.\PHYSICALDRIVE0"。打开物理驱动器使我们能够直接访问
*						|	磁盘的分区表。打开物理驱动器有潜在的危险，错误地写入设备可能会导致操作系统的文件系统无法访问磁盘的内容。
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		串口			|	CreateFile(pszName为"COMx")
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		并口			|	CreateFile(pszName为"LPTx")
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		邮件槽			|	服务器：CreateMailslot(pszName为"\\.\mailslot\mailslotname")。客户端：CreateFile(pszName为"\\servername\mailslot\mailslotname")
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		命名管道		|	服务端：CreateNamedPipe(pszName为"\\.\pipe\pipename")。客户端：CreateFile(pszName为"\\servername\pipe\pipename")
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		匿名管道		|	CreatePipe用来打开服务器和客户端
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		套接字			|	Socket，accept或AcceptEx
*	-------------------------------------------------------------------------------------------------------------------------------------------------------------
*		控制台			|	CreateConsoleScreenBuffer或GetStdHandle
*	=============================================================================================================================================================
* 5）Windows的设计允许我们处理非常大的文件。Microsoft最初的设计者选择了64位值来表示文件大小，而不是32位值。这意味着理论上一个文件最大可以达到16EB(exabytes)。
* 6）调用CreateFile会使系统创建一个文件内核对象来管理对文件的操作。在这个内核对象内部有一个文件指针，它是一个64位偏移量，表示应该在哪里执行下一次同步读取或写入操作。
*	这个文件指针一开始被设为0。
* 7）把异步I/O请求加入队列是设计高性能、可伸缩性好的应用程序的本质所在。
* 8）在执行异步设备I/O的时候，我们必须在pOverlapped参数中传入一个已初始化的OVERLAPPED结构。“overlapped”在这里的意思是执行I/O请求的时间与线程执行其他任务的时间是
*	重叠的（overlapped）。下面是OVERLAPPED结构的定义：
*		typedef struct _OVERLAPPED {
*			DWORD Internal;			// Error code
*			DWORD InternalHigh;		// Number of bytes transferred
*			DWORD Offset;			// Low 32-bit file offset
*			DWORD OffsetHigh;		// High 32-bit file offset
*			HANDLE hEvent;			// Event handle or data
*		} OVERLAPPED, *LPOVERLAPPED;
* 9）WinBase.h中定义的HasOverlappedIoCompleted宏允许我们检查一个异步I/O操作是否已经完成。
* 10）设备驱动程序不必以先入先出的方式来处理队列中的I/O请求。如果不按顺序来执行I/O请求能够提高性能，那么设备驱动程序一般都会这样做。
* 11）在I/O请求完成之前，不移动或销毁这些数据缓存是至关重要的。否则，内存就会遭到破坏。另外，我们必须为每个I/O请求分配并初始化一个不同的OVERLAPPED结构。
* 12）取消队列中的设备I/O请求
*	  有时候，我们可能想要在设备驱动程序对一个已经加入队列的设备I/O请求进行处理之前将其取消。Windows提供了多种方式来达到这一目的。
*		·我们可以调用CancelIo来取消由给定句柄所标识的线程添加到队列中的所有I/O请求（除非该句柄具有与之相关联的I/O完成端口）：
*		·我们可以关闭设备句柄，来取消已经添加到队列中的所有I/O请求，而不管它们是由哪个线程添加的。
*		·当线程终止的时候，系统会自动取消该线程发出的所有I/O请求，但如果请求被发往的设备句柄具有与之相关联的I/O完成端口，那么它们不在被取消的队列。
*		·如果需要将发往给定文件句柄的一个指定的I/O请求取消，那么我们可以调用CandelIoEx，使用CancleIoEx，我们能够将调用线程之外的其他线程发出的待处理的I/O请求取消。
*		这个函数会将hFile设备的代理处理的I/O请求中所有的pOverlapped参数相关联的请求都标记为已经取消。由于每个待处理的I/O请求都应该有自己的OVERLAPPED结构，因此每个
*		CancelIoEx调用都应该只取消一个待处理的请求。但是，如果pOverlapped为NULL，那么CancelIoEx会将hFile指定的设备的所有待处理I/O请求都取消掉。
* 13）Windows提供了4种不同的方法来接收I/O请求已经完成的通知。下表按照复杂度编排列出了这些方法，从最容易理解和实现的（触发设备内核对象）到最难理解和实现的（I/O完成端口）。
*		================================================================================================================================================================
*				技 术		|															摘 要
*		----------------------------------------------------------------------------------------------------------------------------------------------------------------
*		  触发设备内核对象	| 当向一个设备同时发出多个I/O请求的时候，这种方法没什么用。它允许一个线程发出I/O请求，另一个线程对结果进行处理
*		----------------------------------------------------------------------------------------------------------------------------------------------------------------
*		  触发事件内核对象	| 这种方法允许我们向一个设备同时发出多个I/O请求。它允许一个线程发出I/O请求，另一个线程对结果进行处理
*		----------------------------------------------------------------------------------------------------------------------------------------------------------------
*		  使用可提醒I/O		| 这种方法允许我们向一个设备同时发出多个I/O请求。发出I/O请求的线程必须对结果进行处理
*		----------------------------------------------------------------------------------------------------------------------------------------------------------------
*		  使用I/O完成端口	| 这种方法允许我们向一个设备同时发出多个I/O请求。它允许一个线程发出I/O请求，另一个线程对结果进行处理。这项技术具有高度的伸缩性和最佳的灵活性
*		================================================================================================================================================================
* 14）触发设备内核对象
*	  ReadFile和WriteFile函数在将I/O请求添加到队列之前，会先将设备内核对象设为未触发状态。当设备驱动程序完成了请求之后，驱动程序会将设备内核对象设为触发状态。
*		示例：
*		HANDLE hFile = Create(..., FILE_FLAG_OVERLAPPED, ...);
*		BYTE bBuffer[100];
*		OVERLAPPED o = {0};
*		o.Offset = 345;
*		BOOL bReadDone = ReadFile(hFile, bBuffer, 100, NULL, &o);
*		DWORD dwError = GetLastError();
* 
*		if (!bReadDone && (dwError == ERROR_IO_PENGDING))
*		{
*			WaitForSingleObject(hFile, INFINITE);
*			bReadDone = TRUE;
*		}
* 
*		if (bReadDone)
*		{
*			// o.Internal contains the I/O error
*			// o.InteranlHigh contains the number of bytes transferred
*			// bBuffer contains the read data
*		}
*		else
*		{
*			// An error occurred; see dwError
*		}
* 15）触发事件内核对象
*	  OVERLAPPED结构的最后一个成员hEvent用来标识一个事件内核对象。我们必须通过CreateEvent来创建这个事件对象。当一个异步I/O请求完成的时候，设备驱动程序会检查OVERLAPPED结构的hEvent
*	是否为NULL。如果hEvent不为NULL，那么驱动程序会调用SetEvent来触发事件。
*	  为了略微提高性能，我们可以告诉Windows在操作完成的时候不要触发文件对象。我们通过调用SetFileCompletionNotificationModes函数来达到这一目的。
* 16）可提醒I/O
	  最初Microsoft对开发人员吹嘘道，可提醒I/O是创建高性能且伸缩性好的应用程序的最佳途径。但当开发人员开始使用可提醒I/O之后，很快就意识到可提醒I/O并不能达到Microsoft预期的承诺。
*	  我曾经用过可提醒I/O，也会是第一个告诉你真相的人，可提醒I/O非常糟糕，应该避免使用。但是，为了使可提醒I/O能够正常工作，Microsoft在操作系统中添加了一些基础设施，我发现这些
*	基础设施非常有用，也很有价值。
*	  当系统创建一个线程的时候，会同时创建一个与线程相关联的队列。这个队列被称为异步过程调用（asynchronous procedure call，APC）队列。当发出一个I/O请求的时候，我们可以告诉设备
*	驱动程序在调用线程的APC队列中添加一项。为了将I/O完成通知添加到线程的APC队列中，我们应该调用ReadFileEx和WriteFileEx函数。
*	  顺便提一下，当一个可提醒I/O完成时，设备驱动程序不会试图去触发一个事件对象。事实上，设备根本就没有用到OVERLAPPED结构的hEvent成员。因此，如果需要，我们可以将hEvent据为己用。
*	  当线程处于可提醒状态的时候（马上就会讨论到），系统会检查它的APC队列，对队列中的每一项，系统会调用完成函数，并传入I/O错误码，已传输的字节数，以及OVERLAPPED结构的地址。注意，
*	错误码和已传输的字节数也可以通过OVERLAPPED结构的Internal和InternalHigh成员得到。
*	  APC队列是由系统内部维护的。系统会以任意的顺序来执行我们添加到队列中的I/O请求。
*	  当I/O请求完成的时候，系统会将它们添加到APC队列中--回调函数并不会立即被调用，这是因为线程可能还在忙于处理其他事情，不能被打断。为了对线程APC队列中的项进行处理，线程必须将
*	置于可提醒状态。这只不过意味着我们的线程在执行的过程中已经到达了一个点，在这个点上它能够处理被中断的情况。Windows提供了6个函数，可以将线程置为可提醒状态：
*		DWORD SleepEx(DWORD dwMilliseconds);
*		DWORD WaitForSingleObjectEx(HANDLE hObject, DWORD deMilliseconds, BOOL bAlertable);
*		DWORD WaitForMultipleObjectsEx(DWORD cObjects, CONST HANDLE* phObjects, BOOL bWaitAll, DWORD dwMilliseconds, BOOL bAlertable);
*		BOOL SignalObjectAndWait(HANDLE hObjectToSignal, HANDLE hObjectToWaitOn, DWORD dwMilliseconds, BOOL bAlertable);
*		BOOL GetQueuedCompletionStatusEx(HANDLE hCompPort, LPOVERLAPPED_ENTRY pCompPortEntries, DWORD dwMilliseconds, BOOL bAlerttable);
*		DWORD MsgWaitForMultipleObjectsEx(DWORD nCount, CONST HANDLE* pHandles, DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags);
*	  当我们调用刚才提到的6个函数之一并将线程置为可提醒状态时，系统会首先检查线程的APC队列。如果队列中至少有一项，那么系统不会让线程进入睡眠状态。系统会将APC队列的那一项取出，
*	让线程调用回调函数，并传入已完成I/O请求的错误码，已传输的字节数，以及OVERLAPPED结构的地址。当回调函数返回的时候，系统会检查APC队列中是否还有其他的项，如果还有，那么会继续
*	处理。但如果没有其他的项，那么我们对可提醒函数的调用会返回。需要牢记在心的是，当调用这些函数中的任何一个时，只要线程的APC队列中至少有一项，线程就不会进入睡眠状态。
* 17）可提醒I/O的优劣
*	  现在我们需要知道两个问题，正是这两个问题使得将可提醒I/O用于设备I/O成为一种非常糟糕的选择。
*		·回调函数	可提醒I/O要求我们必须创建一个回调函数，这使得代码的实现变得更加复杂。由于这些函数一般来说并没有足够的与某个问题有关的上下文信息，因此我们最终不得不将大量
*		  的信息放在全局变量。幸运的是，我们不需要对这些全局变量进行同步，这是因为调用六个可提醒函数之一的线程与执行回调函数的线程是同一个线程。由于同一个线程不可能同时在两个
*		  地方执行，因此全局变量是安全的。
*		·线程问题	实际上可提醒I/O的最大问题是：发出I/O请求的线程必须同时对完成通知进行处理。如果一个线程发出多个请求，那么即使其他线程完全处于空闲状态，该线程也必须对每个请求
*		  的完成通知做出响应。由于不存在负载均衡机制，因此应用程序的伸缩性不会太好。
*	  可提醒I/O基础设施有哪些优点：
*		Windows提供了一个函数，允许我们手动将一项添加到APC队列中：
*			DWORD QueueUserAPC(PAPCFUNC pfnAPC, HANDLE hThread, ULONG_PTR dwData);
*		我们可以使用QueueUserAPC来进行非常高效的线程间通信，甚至能够跨越进程的界限。
* 18）I/O完成端口
*	  回顾历史，我们能够采用以下两种模型之一来架构一个服务应用程序。
*		·串行模型（serial model）：一个线程等待一个客户（通常是通过网络）发出请求。当请求到达的时候，线程会被唤醒并对客户请求进行处理。
*		·并发模型（concurrent model）：一个线程等待一个客户请求，并创建一个新的线程来处理请求。当新线程正在处理客户请求的时候，原来的线程会进入下一次循环并等待另一个客户请求。
*	    当处理客户请求的线程完成整个处理过程的时候，该线程就会终止。
* 19）I/O完成端口背后的理论是并发运行的线程的数量必须有一个上限--也就是说，同时发出的500个客户请求不应该允许出现500个可运行的线程。这是并发模型的一个潜在缺点。
* 20）并发模型的另一个缺点是需要为每个客户请求创建一个新的线程。I/O完成端口的设计初衷就是与线程池配合使用。
* 21）I/O完成端口可能是最复杂的内核对象了。为了创建一个I/O完成端口，我们应该调用CreateIoCompletionPort：
*		HANDLE CreateIoCompletionPort(
			HANDLE hFile,
			HANDLE hExistingCompletionPort,
			ULONG_PTR CompletionKey,
			DWORD dwNumberOfConcurrentThreads);
	  这个函数执行两项不同的任务：它不仅会创建一个I/O完成端口，而且会将一个设备与一个I/O端口关联起来。依我之见，该函数已经太复杂了，Microsoft应该将它分成两个单独的函数。
	  i. 创建I/O完成端口:
	     HANDLE CreateIoCompletionPort(DWORD dwNumberOfConcurrentThreads)
		 {
			return (CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, dwNumberOfConcurrentThreads));
		 }
		 我们不需要传递一个SECURITY_ATTRIBUTES结构给CreateIoCompletionPort，在所有用来创建内核对象的Windows函数中，CreateIoCompletionPort大概是绝无仅有的。这是因为I/O完成端口
	    的设计初衷就是只在一个进程中使用。
	  ii.将设备与I/O完成端口关联起来。
	    BOOL AssociateDeviceWithCompletionPort(HANDLE hCompletionPort, HANDLE hDevice, DWORD dwCompletionKey)
		{
			HANDLE h = CreateIoCompletionPort(hDevice, hCompetionPort, dwCompletionKey, 0);
			return (h == hCompletionPort);
		}
		完成键（即Complete key，一个对我们有意义的值，操作系统并不关心我们在这里传入的到底是什么值。）
* 22）当我们创建一个I/O完成端口的时候，系统内核实际上会创建5个不同的数据结构，“.\\Structs_Of_IO_Completion_Port\\IO完成端口的内部运作.jpg”。
*	  第二个数据结构是一个I/O完成队列。当设备的一个异步I/O请求完成的时候，系统会检查设备是否与一个I/O完成端口相关联，如果设备与一个I/O完成端口相关联，那么系统会将该项已完成的I/O请求
*   追加到I/O完成端口的I/O完成队列的末尾。
*     说明：向设备发出I/O请求，但不把该项已完成的I/O请求添加到I/O完成端口的队列中也是有可能的。通常我们并不需要这样做，但这样做偶尔还是有用的--比如，我们通过一个套接字发送数据，但并
*   不关心数据实际上到底有没有送达。
*     为了发出一个在完成的时候不要被添加到队列中的I/O请求，我们必须在OVERLAPPED结构的hEvent成员中保存一个有效的事件句柄，并将它与1按位或起来，如下面的代码所示：
*		Overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE，NULL);
*		Overlapped.hEvent = (HANDLE)((DWORD_PTR) Overlapped.hEvent | 1);
*		ReadFile(..., &Overlapped);
*   另外，在关闭这个事件句柄的时候，不要忘了将最低位清掉：
*		CloseHandle((HANDLE) ((DWORD_PTR) Overlapped.hEvent & ~1));
* 23）I/O完成端口的周边架构
*	  当我们的服务应用程序初始化的时候，应该调用CreateNewCompletionPort之类的函数来创建I/O完成端口。应用程序接着应该创建一个线程池来处理客户请求。关于线程池中应该有多少线程？
	就目前而言，标准的经验法则是取主机的CPU数量并将其乘以2。
	  线程池中的所有线程应该执行同一个函数。一般来说，这个线程函数会先进性一些初始化工作，然后进入一个循环，当服务进程被告知要停止的时候，这个循环也应该就此终止。在循环内部，
	线程将自己切换到睡眠状态，来等待设备I/O请求完成并进入完成端口。调用GetQueuedCompletionStatus可以达到这一目的：
		BOOL GetQueuedCompletionStatus(
				HANDLE		hCompletionPort,
				PDWORD		pdwNumberOfBytesTransferred,
				PULONG_PTR	pCompletionKey,
				OVERLAPPED**	ppOverlapped,
				DWORD		dwMilliseconds);
	  确定GetQueuedCompletionStatus返回的原因有些困难，下面这段代码展示了正确的做法：
		DWORD dwNumBytes;
		ULONG_PTR CompletionKey;
		OVERLAPPED* pOverlapped;

		// hIOCP is initialized somewhere else in the program
		BOOL bOk = GetQueuedCompletionStatus(hIOCP, &dwNumBytes, &CompletionKey, &pOverlapped, 1000);
		DWORD dwError = GetLastError();
		if (bOk){
			// Process a successfully completed I/O request
		}
		else{
			if (pOverlapped != NULL)
			{
				// Process a failed completed I/O request
				// dwError contains the reason for failure
			}
			else
			{
				if (dwError == WAIT_TIMEOUT)
				{
					// Time-out while waiting for completed I/O entry
				}
				else
				{
					// Bad call to GetQueuedCompletionStatus
					// dwError contains the reason for the bad call
				}
			}
		}
	  在Windows Vista中，如果预计会不断地接收到大量的I/O请求，那么我们可以调用下面的函数来同时取得多个I/O请求的结果，而不必让许多线程等待完成端口，从而可以避免由此产生的上下文
	切换所带来的开销：
	  BOOL GetQueuedCompletionStatusEx(
			HANDLE hCompletionPort,
			LPOVERLAPPED_ENTRY pCompletionPortEntries,
			ULONG ulCount,
			PULONG pulNumEntriesRemoved,
			DWORD dwMilliseconds,
			BOOL bAlertable);
* 24）我们可以通过以下3种方式之一来结束线程/完成端口的指派：
*	  ·让线程退出。
*	  ·让线程调用GetQueuedCompletionStatus，并传入另一个不同的I/O完成端口的句柄。
*	  ·销毁线程当前被指派的I/O完成端口。
* 25）总结下：假设我们在一台有两个CPU的机器上运行。我们创建了一个同时最多只允许两个线程被唤醒的完成端口，还创建了4个线程来等待已完成的I/O请求。如果3个已完成的I/O请求被添加到端口的队列中，
*	只有两个线程被唤醒来对请求进行处理，这降低了可运行线程的数量，并节省了上下文切换的时间。现在，如果一个可运行线程调用了Sleep，WaitForSingleObject，WaitForMultipleObjects，
	SignalObjectAndWait，一个异步I/O调用或任何能够导致线程变成不可运行状态的函数，I/O完成端口会检测到这一情况并立即唤醒第3个线程。完成端口的目标是使CPU保持在满负荷状态下工作。
	  最后，第一个线程将再次变成可运行状态。当发生这种情况的时候，可运行线程的数量将超过系统中CPU的数量。但是，完成端口仍然知道这一点，在线程数量降到低于CPU数量之前，它是不会再唤醒任何
	线程的。I/O完成端口体系结构假定可运行线程的数量只会在很短一段时间内高于最大允许的线程数量，一旦线程进入下一次循环并调用GetQueuedCompletionStatus，可运行线程的数量就会迅速下降。这就
	解释了为什么线程池中的线程数量应该大于在完成端口中设置的并发线程数量。
*/
namespace HY_KERNELOBJECT
{
	class SyncAndAsynDeviceIO
	{
	public:
		SyncAndAsynDeviceIO();
		~SyncAndAsynDeviceIO();

		// 设置串口波特率
		BOOL SetCommConfig(HANDLE hCommDev, LPCOMMCONFIG pCC, DWORD dwSize);

		// 在等待邮件槽读取数据的时候，设置一个超时值
		BOOL SetMailslotInfo(HANDLE hMailslot, DWORD dwReadTimeout);

		// 关闭设备句柄
		BOOL CloseHandle(HANDLE hObject);
		// 如果是套接字
		int closesocket(SOCKET s);

		// 查看设备类型
		DWORD GetFileType(HANDLE hDevice);
		/* ===========================================================================================
		*				值				|								描 述
		*  -------------------------------------------------------------------------------------------
		*	FILE_TYPE_UNKNOWN			|	指定的文件为未知类型
		*  -------------------------------------------------------------------------------------------
		*	FILE_TYPE_DISK				|	指定的文件是一个磁盘文件
		*  -------------------------------------------------------------------------------------------
		*	FILE_TYPE_CHAR				|	指定的文件是一个字符文件，一般来说是一个并口设备或控制台
		*  -------------------------------------------------------------------------------------------
		*	FILE_TYPE_PIPE				|	指定的文件是一个命名管道或匿名管道
		*  ===========================================================================================
		*/

		// 打开设备(注意：失败返回的是INVALID_HANDLE_VALUE(-1)，而不是NULL)
		HANDLE CreateFile(
			PCTSTR pszName,
			DWORD dwDesiredAccess,
			DWORD dwShareMode,
			PSECURITY_ATTRIBUTES psa,
			DWORD dwCreationDisposition,
			DWORD dwFlagsAndAttributes,
			HANDLE hFileTemplate);

		// 获取文件的逻辑大小
		BOOL GetFileSizeEx(HANDLE hFile, PLARGE_INTEGER pliFileSize);
		// 获取文件的物理大小，即文件在磁盘上实际占用的字节数
		DWORD GetCompressedFileSize(PCTSTR pszFileName, PDWORD pdwFileSizeHigh);

		// 如果要随机访问文件，那么我们需要改变与文件内核对象相关联的文件指针
		BOOL SetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER pliNewFilePointer, DWORD dwMoveMethod);
		// 获取文件位置
		LARGE_INTEGER GetFilePointer(HANDLE hFile);
		// 设置文件尾
		BOOL SetEndOfFile(HANDLE hFile);

		/* 执行同步设备I/O */ 
		// 最方便和最常见的对设备数据进行读/写的函数是ReadFile和WriteFile
		BOOL ReadFile(HANDLE hFile, PVOID pvBuffer, DWORD nNumBytesToRead, PDWORD pdwNumBytes, OVERLAPPED* pOverlapped);
		BOOL WriteFile(HANDLE hFile, CONST VOID *pvBuffer, DWORD nNumBytesToWrite, PDWORD pdwNumBytes, OVERLAPPED* pOverlapped);
		// 强制系统将缓存数据写入到设备
		BOOL FlushFileBuffers(HANDLE hFile);
		// 取消同步I/O请求
		BOOL CancelSynchronousIo(HANDLE hThread);

		/* 执行异步设备I/O */
		// 取消由给定句柄所标识的线程添加到队列中的所有I/O请求
		BOOL CancelIo(HANDLE hFile);
		// 将发往给定文件句柄的一个指定I/O请求取消
		BOOL CancelIoEx(HANDLE hFile, LPOVERLAPPED pOverlapped);
		// 告诉Windows在操作完成的时候不要触发文件对象
		BOOL SetFileCompletionNotificationModes(HANDLE hFile, UCHAR uFlags);
		// Microsoft当初并不打算公开OVERLAPPED结构的Internal和InternalHigh成员，这意味着Microsoft需要提供另一种方式来让开发人员知道在I/O处理过程中传输了多少字节，并取得I/O的错误码
		BOOL GetOverlappedResult(HANDLE hFile, OVERLAPPED* pOverlapped, PDWORD pdwNumBytes, BOOL bWait);
		// 使用可提醒I/O
		BOOL ReadFileEx(HANDLE hFile, PVOID pvBuffer, DWORD nNumBytesToRead, OVERLAPPED* pOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE pfnCompletionRoutine);
		BOOL WriteFileEx(HANDLE hFile, PVOID pvBuffer, DWORD nNumBytesToWrite, OVERLAPPED* pOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE pfnCompletionRoutine);
		// 手动将一项添加到APC队列中
		DWORD QueueUserAPC(PAPCFUNC pfnAPC, HANDLE hThread, ULONG_PTR dwData);
	};
}



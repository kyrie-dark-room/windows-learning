#pragma once
#include <Windows.h>
#include <commctrl.h>

/*
* 知识点：
* 1）应用程序需要跨越进程边界来访问另一个进程的地址空间的情况如下：
*	·我们想要从另一个进程创建的窗口派生子类窗口。
*	·我们需要一些手段来辅助调试--例如，我们需要确定另一个进程正在使用哪些DLL。
*	·我们想要给另一个进程安装挂钩。
* 2）我们可以使用多种机制来将一个DLL注入到另一个进程的地址空间中，一旦DLL代码进入另一个地址空间，那么我们就可以在那个进程中随心所欲，肆意妄为了。这应该够吓人的，
	因此在真的打算这么做之前，请务必慎重考虑。
* 3）使用注册表来注入DLL
*	a.注册表项：HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows下的键为AppInit_DLLs的值可能会包含一个DLL的文件名或一组DLL的文件名（通过空格或逗号分隔）。由于空格是     用来分隔文件名的，因此我们必须避免在文件名中包含空格。第一个DLL的文件名可以包含路径，但其他DLL包含的路径则被忽略。
*	b.为了能让系统使用这个注册表项，我们还必须创建一个名为LoadAppInit_Dlls，类型为DWORD的注册表项，并将它的值设为1。
*	c.当User32.dll被映射到一个新的进程时，会收到DLL_PROCESS_ATTACH通知。当User32.dll对它进行处理的时候，会取得上述注册表键的值，并调用LoadLibrary来载入这个字符串中指定的每个DLL。
*	d.在用来注入DLL的所有方法中，这是最方便的一种。我们所要做的只不过是在注册表项中添加两个值。但这种方法也有一些缺点，具体如下所示：
*		· 我们的DLL只会被映射到那些使用了User32.dll的进程中。所有基于GUI的应用程序都使用了User32.dll，但大多数基于CUI的应用程序都不会使用它。因此，如果想要将DLL注入到编译器或链接器，
*		   那这种方法就不可行。
*		· 我们的DLL会被映射到每个基于GUI的应用程序中，但我们可能只想把DLL注入到一个或少数几个应用程序中。
*		· 我们的DLL会被映射到每个基于GUI的应用程序中，在应用程序终止之前，它将一直存在于进程的地址空间中。
* 4）使用Windows挂钩来注入DLL
*		安装挂钩：
*			HHOOK hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, hInstDll, 0);
*			现在让我们来看一看接下来会发生什么：
*			a. 进程B中的一个线程准备向一个窗口派送一条消息。
*			b. 系统检查该线程是否已经安装了WH_GETMESSAGE挂钩。
*			c. 系统检查GetMsgProc所在的DLL是否已经被映射到进程B的地址空间中。
*			d. 如果DLL尚未被映射，那么系统会强制将DLL映射到进程B的地址空间中，并将进程B中该DLL的锁计数器（lock count）递增。
*			e. 由于DLL的hInstDll是在进程B中映射的，因此系统会对它进行检查，看它与该DLL在进程A中的位置是否相同。不同则进行转换。
*			f. 系统在进程B中递增该DLL的锁计数器。
*			g. 系统在进程B的地址空间中调用GetMsgProc函数。
*			h. 当GetMsgProc返回的时候，系统递减该DLL在进程中的锁计数器。
*		卸载挂钩：
*			BOOL UnhookWindowsHookEx(HHOOK hHook);
*			当一个线程调用UnhookWindowsHookEx的时候，系统会遍历自己内部的一个已经注入过该DLL的进程列表，并将该DLL的锁计数器递减。当锁计数器减到0的时候，系统会自动从进程的地址空间
*			中撤销对该DLL的映射。
*       项目实例：Desktop Item Position Saver(DIPS)工具
* 5）使用远程线程来注入DLL
		注入DLL的第三种方法是使用远程线程（remote thread），它提供了最高的灵活性。
		大多数Windows函数只允许一个进程对它自己进行操作。但是，Windows也提供了一些函数来让一个进程对另一个进程进行操作。虽然这些函数中的大多数一开始都是为了调试器或其他工具设计的，
	  但是任何应用程序都可以调用这些函数。
		从根本上说，DLL注入技术要求目标进程中的一个线程调用LoadLibrary来载入我们想要的DLL。由于我们不能轻易地控制别人进程中的线程，因此这种方法要求我们在目标进程中创建一个新的线程。
	  由于这个线程是我们自己创建的，因此我们可以对它执行的代码加以控制。
	    注入步骤
		a.用VirtualAllocEx函数在远程进程的地址空间中分配一块内存。
		b.用WriteProcessMemory函数把DLL的路径名复制到a中分配的内存中。
		c.用GetProcAddress函数来得到LoadLibraryW或LoadLibraryA函数（在Kernel32.dll中）的实际地址。
		d.用CreateRemoteThread函数在远程线程中创建一个线程，让新线程调用正确LoadLibrary函数并在参数中传入a分配的内存地址。这时，DLL已经被注入到远程进程的地址空间中，DLL的DllMain函数
		  会收到DLL_PROCESS_ATTACH通知并且可以执行我们想要执行的代码。当DllMain返回的时候，远程线程会从LoadLibraryW/A调用返回到BaseThreadStart函数。BaseThreadStart然后调用ExitThread，
		  使远程线程终止。
		清理动作
		e.用VirtualFreeEx来释放第a步分配的内存。
		f.用GetProcAddress来得到FreeLibrary函数（在Kernel32.dll中）的实际地址。
		g.用CreateRemoteThread函数在远程进程中创建一个线程，让该线程调用FreeLibrary函数并在参数中传入远程DLL的HMODULE。
		示例：
		    a.PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
		    b.HANDLE hThread = CreateRemoteThread(hProcessRemote, NULL, 0, pfnThreadRtn, L"C:\\MyLib.dll", 0, NULL);
* 6）一些进程是用本地系统账号运行的，如WinLogon，SvcHost和Csrss，登录的用户是无法对这些进程进行修改的。如果我们已经获得调试安全特权（debug security privalege）的授权，并且也启用了
*	调试安全特权，那么我们也许能够打开这些进程的句柄。
* 7）使用木马DLL来注入DLL
*		a.把我们知道的进程必然会载入的一个DLL替换掉，在我们自己的DLL的内部，我们导出原来DLL导出的所有符号。
*		b.修复应用程序.exe模块的导入段。这种方法不差，但前提是我们必须相当熟悉.exe和DLL的文件格式。
* 8）把DLL作为调试器来注入
*	 调试器可以在被调试进程中执行许多特殊的操作。系统载入一个被调试程序（debuggee）的时候，会在被调试程序的地址空间准备完毕之后，但被调试程序的主线程尚未开始执行任何代码之前，自动
*	通知调试器。这时，调试器可以强制将一些代码注入到被调试程序的地址空间中（比如使用WriteProcessMemory），然后让被调试程序的主线程去执行这些代码。
*	 这种方法要求我们对被调试线程的CONTEXT结构进程操作，这也意味着我们必须编写与CPU相关的代码。为了让这种方法能在不同的CPU平台上正常工作，我们必须对源代码进行修改。此外，我们可能
*	还必须手工编写一些我们想让被调试程序执行的机器语言指令。
*	 在默认情况下，如果调试器终止，那么Windows会自动终止被调试程序。但是，调试器可以通过调用DebugSetProcessKillOnExit并传入FALSE来改变默认的行为。在不终止一个进程的前提下停止调试
*	该进程也是有可能的，这要归功于DebugActiveProcessStop函数。
* 9）使用CreateProcess来注入代码
*	 下面是让进程对它的子进程的主线程执行的代码进行控制的一种方法：
*		a.让进程生成一个被挂起的子进程。
*		b.从.exe模块的文件头中取得主线程的起始内存地址。
*		c.将位于该内存地址处的机器指令保存起来。
*		d.强制将一些手动编写的机器指令写入到该内存地址处。这些指令应该调用LoadLibrary来载入一个DLL。
*		e.让子进程的主线程恢复运行，从而让这些指令得到执行。
*		f.把保存起来的原始指令恢复到起始地址处。
*		g.让进程从起始地址继续执行，就好像什么都没有发生过一样。
*	 缺点：只有当我们的代码在父进程中的时候，我们才能用这种方法来注入DLL。当然，这种方法还与CPU相关，我们必须为不同的CPU平台做相应的修改。
* 10）拦截API
*	 方法一：覆盖代码
*		a.在内存中对要拦截的函数进行定位，从而得到它的内存地址。
*		b.把这个函数起始的几个字节保存到我们自己的内存中。
*		c.用CPU的一条JUMP指令来覆盖这个函数起始的几个字节，这条JUMP指令用来跳转到我们的替代函数的内存地址。
*		d.现在，当线程调用被拦截韩束的时候，跳转指令实际上会跳转到我们的替代函数。这时，我们就可以执行自己想要执行的任何代码。
*		e.为了撤销对函数的拦截，我们必须把b中保存的字节放回拦截函数起始的几个字节中。
*		f.我们调用被拦截函数（现在已经不再对它进行拦截了），让该函数执行它的正常处理。
*		g.当原来的函数返回时，我们再次执行b和c，这样我们的替代函数将来还会被调用到。
*		这种方法存在一些严重的不足，我强烈建议不要使用它：对CPU有依赖性，其次，在抢占式、多线程环境下根本不能工作。
*	 方法二：通过修改模块的导入段来拦截API
*		为了拦截一个特定的函数，我们所需要做的就是修改它在模块的导入段中的地址。就这么简单，完全不存在对CPU的依赖性。而且，由于我们并没有修改函数的代码，因此也就不担心线程同步的问题。
*/

namespace HY_HOOK_Manager
{
	class CHYHOOK
	{
	public:
		CHYHOOK(void);
		~CHYHOOK(void);

		// 更改指定窗口的属性，同时可以用来从一个窗口派生子类窗口
		LONG_PTR SetWindowLongPtr(
			HWND     hWnd,
			int      nIndex,
			LONG_PTR dwNewLong
		);

		// Installs or updates a window subclass callback
		BOOL SetWindowSubclass(
			HWND         hWnd,
			SUBCLASSPROC pfnSubclass,
			UINT_PTR     uIdSubclass,
			DWORD_PTR    dwRefData
		);
		// 检索指定窗口子类回调的引用数据
		BOOL GetWindowSubclass(
			HWND         hWnd,
			SUBCLASSPROC pfnSubclass,
			UINT_PTR     uIdSubclass,
			DWORD_PTR* pdwRefData
		);
		// 从窗口中删除子类回调
		BOOL RemoveWindowSubclass(
			HWND         hWnd,
			SUBCLASSPROC pfnSubclass,
			UINT_PTR     uIdSubclass
		);
		// Calls the next handler in a window's subclass chain. The last handler in the subclass chain calls the original window procedure for the window
		LRESULT DefSubclassProc(
			HWND   hWnd,
			UINT   uMsg,
			WPARAM wParam,
			LPARAM lParam
		);

		// 将应用程序定义的挂钩过程安装到挂钩链中。 你将安装挂钩过程来监视系统的某些类型的事件。 这些事件与特定线程或调用线程位于同一桌面中的所有线程相关联。
		HHOOK SetWindowsHookEx(
			int       idHook,
			HOOKPROC  lpfn,
			HINSTANCE hmod,
			DWORD     dwThreadId
		);
		// 删除 SetWindowsHookEx 函数在挂钩链中安装的挂钩过程。
		BOOL UnhookWindowsHookEx(HHOOK hhk);

		// 在指定进程的虚拟地址空间中保留、提交或更改内存区域的状态。 该函数初始化它分配给零的内存
		LPVOID VirtualAllocEx(
			HANDLE hProcess,
			LPVOID lpAddress,
			SIZE_T dwSize,
			DWORD  flAllocationType,
			DWORD  flProtect
		);

		// 一个进程对另一个进程的地址空间进行读
		BOOL ReadProcessMemory(
			HANDLE  hProcess,
			LPCVOID lpBaseAddress,
			LPVOID  lpBuffer,
			SIZE_T  nSize,
			SIZE_T* lpNumberOfBytesRead
		);

		// Writes data to an area of memory in a specified process. The entire area to be written to must be accessible or the operation fails
		BOOL WriteProcessMemory(
			HANDLE  hProcess,
			LPVOID  lpBaseAddress,
			LPCVOID lpBuffer,
			SIZE_T  nSize,
			SIZE_T* lpNumberOfBytesWritten
		);

		// 创建在另一个进程的虚拟地址空间中运行的线程
		HANDLE CreateRemoteThread(
			HANDLE                 hProcess,
			LPSECURITY_ATTRIBUTES  lpThreadAttributes,
			SIZE_T                 dwStackSize,
			LPTHREAD_START_ROUTINE lpStartAddress,
			LPVOID                 lpParameter,
			DWORD                  dwCreationFlags,
			LPDWORD                lpThreadId
		);

		// 释放、取消提交或释放并释放指定进程的虚拟地址空间中的内存区域
		BOOL VirtualFreeEx(
			HANDLE hProcess,
			LPVOID lpAddress,
			SIZE_T dwSize,
			DWORD  dwFreeType
		);

		// 在一个模块的导入段中查找对一个符号的引用，如果存在这样的引用，它便会修改该符号的地址。
		void ReplaceIATEntryInOneMod(PCSTR pszCalleeModName, PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller);
	private:
		CHYHOOK(const CHYHOOK&) = delete;
		CHYHOOK& operator=(const CHYHOOK&) = delete;
		CHYHOOK(CHYHOOK&&) = delete;
		CHYHOOK& operator=(CHYHOOK&&) = delete;
	};
}


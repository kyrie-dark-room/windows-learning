#pragma once
/*
* 知识：
* 1）一般将进程定义成一个正在运行的程序的一个实例，它由以下两部分构成：
*	● 一个内核对象，操作系统用它来管理进程。内核对象也是系统保存进程统计信息的地方。
*	● 一个地址空间，其中包含所有可执行文件（executable）或DLL模块的代码和数据。此外，它还包含动态内存分配，比如线程堆栈和堆的分配。
* 2）Windows支持两种类型的应用程序：GUI程序和CUI程序。前者是“图形用户界面”（Graphical User Interface）的简称，后者是“控制台用户界面”（Console User Interface）的简称。
* 3）用Microsoft Visual Studio来创建一个应用程序项目时，集成开发环境会设置各种链接器开关，使链接器将子系统的正确类型嵌入最终生成的可执行文件（executable）中。对于CUI程序
*   ，这个链接器开关是/SUBSYSTEM:CONSOLE，对于GUI程序，则是/SUBSYSTEM:WINDOWS。用户运行应用程序时，操作系统的加载程序（loader）会检查可执行文件映像的文件头，并获取这个子系统
*   值。如果此值表明是一个CUI程序，加载程序会自动确保有一个可用的文本控制台窗口（比如从命令提示符启动这个程序的时候）。另外，如有必要，会创建一个新窗口（比如从Windows资源管理器
*   启动这个CUI程序的时候）。如果此值表明是一个GUI程序，加载器就不会创建控制台窗口；相反，它只是加载这个程序。一旦应用程序开始运行，操作系统就不再关心应用程序的界面是什么类型的。
* 4）C/C++开发人员可以使用以下两种入口函数：
*   Int WINAPI _tWinMain(HINSTANCE hInstanceExe, HINSTANCE, PTSTR pszCmdLine, int nCmdShow)
*   int _tmain(int argc, TCHAR *argv[], TCHAR *envp[]);
*     操作系统实际并不调用我们所写的入口点函数。相反，它会调用由C/C++运行库实现并在链接时使用-entry:命令行选项来设置的一个C/C++运行时启动函数。该函数将初始化C/C++运行，使我们
*   能调用malloc和free之类的函数。它还确保了在我们的代码开始执行之前，我们声明的任何全局和静态C++对象都被正确地构造。
*																应用程序类型和相应的入口点函数
*   ————————————————————————————————————————————————————————————————————————————————
*					应用程序类型				|		入口点函数（入口）	|			嵌入可执行文件的启动函数
*   ————————————————————————————————————————————————————————————————————————————————
*		处理ANSI字符和字符串的GUI应用程序		|    _tWinMain(WinMain)		|	WinMainCRTStartup
*   ————————————————————————————————————————————————————————————————————————————————
*       处理Unicode字符和字符串的GUI应用程序	|    _tWinMain(wWinMain)	|   wWinMainCRTStartup
*   ————————————————————————————————————————————————————————————————————————————————
*       处理ANSI字符和字符串的CUI应用程序		|    _tmain(Main)			|   mainCRTStartup
*   ————————————————————————————————————————————————————————————————————————————————
*       处理Unicode字符和字符串的CUI应用程序	|    _tmain(Wmain)			|   wmainCRTStartup
*   ————————————————————————————————————————————————————————————————————————————————
*     在链接可执行文件时，链接器将选择正确的C/C++运行库启动函数。
* 5）所有C/C++运行库启动函数所做的事情基本都是一样的，区别在于它们要处理的是ANSI字符串，还是Unicode字符串；以及在初始化C运行库之后，它们调用的是哪一个入口点函数。
*   Visual C++自带C运行库的源代码。可以在ertexe.c文件中找到4个启动函数的源代码。这些启动函数的用途简单总结如下：
*    ·获取指向新进程的完整命令行的一个指针。
*    ·获取指向新进程的环境变量的一个指针。
*    ·初始化C/C++运行库的全局变量（https://blog.csdn.net/xmmdbk/article/details/119548615）。如果包含了StdLib.h，我们的代码就可以访问这些变量。
*    ·初始化C运行库内存分配函数（malloc和calloc）和其他底层I/O例程使用的堆（heap）。
*    ·调用所有全局和静态C++类对象的构造函数。
*    完成所有这些初始化工作之后，C/C++启动函数就会调用应用程序的入口点函数。
*    如果我们写了一个_tWinMain函数，而且定义了_UNICODE，其调用过程将如下所示：
*        GetStartupInfo(&StartupInfo);
*        int nMainRetVal = wWinMain((HINSTANCE)&__ImageBase, NULL, pszCommandLineUnicode, (StartupInfo.dwFlags & STARTF_USESHOWWINDOW) ? StartupInfo.wShowWindow : SW_SHOWDEFAULT);
*    如果我们没有定义_UNICODE，其调用过程将如下所示：
*        GetStartupInfo(&StartupInfo);
*        int nMainRetVal = wWinMain((HINSTANCE)&__ImageBase, NULL, pszCommandLineAnsi, (StartupInfo.dwFlags & STARTF_USESHOWWINDOW) ? StartupInfo.wShowWindow : SW_SHOWDEFAULT);
*    注意，_ImageBase是一个链接器定义的伪变量，表明可执行文件被映射到应用程序内存中的什么位置。
*    如果我们写了一个_tmain函数，而且定义了_UNICODE，那么其调用过程如下：
*		 int nMainRetVal = wmain(argc, argv, envp);
*    如果我们没有定义_UNICODE，调用过程如下：
*        int nMainRetVal = main(argc, argv, envp);
*	 注意，用Visual Studio向导生成应用程序时，CUI应用程序的入口中没有定义第三个参数（环境变量块），如下所示：
*		 int _tmain(int argc, TCHAR* argv[]);
*    如果需要访问进程的环境变量，只需要将上述调用替换成下面这一行：
*		 int _tmain(int argc, TCHAR* argv[], TCHAR* env[]);
*    这个env参数指向一个数组，数组中包含所有环境变量及其值，两者用等号（=）分隔。
*    入口点函数返回后，启动函数将调用C运行库函数exit，向其传递返回值（nMainRetVal）。
*    exit函数执行以下任务。
*        ·调用_onexit函数调用所注册的任何一个函数。
*		 ·调用所有全局和静态C++类对象的析构函数。
*		 ·在DEBUG生成中，如果设置了_CRTDBG_LEAK_CHECK_DF标志，就通过调用_CrtDumpMemoryLeaks函数来生成内存泄露报告。
*		 ·调用操作系统的ExitProcess函数，向其传入nMainRetVal。这会导致操作系统“杀死”我们的进程，并设置它的退出代码。
* 6）事实上，HMODULE和HINSTANCE完全是一回事。如果某个函数的文档指出需要一个HMODULE参数，我们可以传入一个HINSTANCE，反之亦然。之所以有两种数据类型，是由于在16位Windows中，
*    HMODULE和HINSTANCE表示不同类型的数据。
* 7）hInstanceExe参数实际值是一个内存基地址；系统将可执行文件的影响加载到进程地址空间中的这个位置。
*      可执行文件的映像具体加载到哪一个基地址，是由链接器决定的。不同的链接器使用不同的默认基地址。由于历史原因，Visual Studio链接器使用的默认基地址是0x00400000，这是在运行
*    Windows 98时，可执行文件的映像能加载到的最低的一个地址。使用Microsoft链接器的/BASE:address链接器开关，可以更改要将应用程序加载到哪个基地址。
* 8）如果一个参数没有用到，编译器会报告一个“参数没有被引用到”（parameter not referenced）警告。有两种方法处理：1.参数不指定参数名 2.C++ GUI项目中，利用UNREFERENCED_PARAMETER
*    宏来消除这种警告。
* 9）每个进程都有一个与它关联的环境快（environment block），这是在进程地址空间内分配的一块内存，其中包含字符串和下面类似：
*    =::=::\ ...
*    VarName1=VarValue1\0
*    VarName2=VarValue2\0 ...
*    VarNameX=VarValueX\0
*    \0
* 10）由于等号用于分隔名称和值，所以它并不是名称的一部分。另外，空格是有意义的。
* 11）用户登录Windows时，系统会创建外壳（shell）进程，并将一组环境字符串与其关联。系统通过检查注册表中的两个注册表项来获得初始的环境字符串。
*     第一个注册表项包含应用于系统的所有环境变量的列表：
*         HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment
*     第二个注册表项包含应用于当前登录用户的所有环境变量的列表：
*         HKEY_CURRENT_USER\Environment
* 12）修改环境变量：
*     可以通过系统高级设置控制面板来设置；
*     修改注册表方式，例如，加入更新了注册表项，并希望应用程序立即更新它们的环境快，可以进行如下调用：
*		SendMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)TEXT("Environment"));
* 13）在许多字符串的内部，都包含了“可替换字符串”。例如，我在注册表的某个地方发现了下面这个字符串：%USERPROFILE%\Documents。两个百分号（%）之间的部分内容就是一个“可替换字符串”。
* 14）子进程会继承父进程的“处理器关联性”（Processor affinity）。
* 15）与每个进程都关联了一组标志，这些标志的作用是让系统知道进程如何响应严重错误，包括磁盘介质错误、未处理的异常、文件查找错误以及数据对齐错误等。进程可以调用SetErrorMode函数来
*   告诉系统如何处理这些错误。
* 16）Windows的MAX_PATH：https://www.cnblogs.com/findumars/p/3817091.html
* 17）系统跟踪记录着进程的当前驱动器和目录，但它没有记录每个驱动器的当前目录。不过利用操作系统提供的支持，可以处理多个驱动器的当前目录。这个支持是通过进程的环境字符串来提供的。
*     例如，一个进程可以有如下所示的两个环境变量：
*     =C:=C:\Utility\Bin
*     =D:=D:\Program Files
*     如果调用一个函数，并且传入的路径名限定的是当前驱动器以外的驱动器，系统会在进程的环境块中查找与指定驱动器号（也称盘符）关联的变量。如果找到与指定驱动器号关联变量，系统就
*   将变量的值作为当前目录使用。如果变量没有找到，系统就假定指定驱动器的当前目录是它的根目录。
*     可以使用C运行库函数_chdir而不是Windows的SetCurrentDirectory函数来更改当前目录。_chdir函数在内部调用SetCurrentDirectory，但_chdir还会调用SetEnvironmentVariable来添加或者
*   修改环境变量，从而使不同驱动器的当前目录得以保留。
* 18）进程可以通过GetFullPathName来获得它的当前目录。
* 19）当我们用CreateProcess函数来创建一个进程，系统做的工作如下：
*	  一个线程调用CreateProcess时，系统将创建一个进程内核对象，其初始使用计数为1。进程内核对象不是进程本身，而是系统用来管理这个进程的一个小型数据结构--可以把进程内核对象
*   想象成由进程统计信息构成的一个小型数据结构。然后，系统为新进程创建一个虚拟地址空间，并将可执行文件（和所有必要的DLL）的代码及数据加载到进程的地址空间。然后，系统为新进程
*   的主线程创建一个线程内核对象（其使用计数为1）。和进程内核对象一样，线程内核对象也是一个小型数据结构，操作系统用它来管理这个线程。这个主线程一开始就会执行C/C++运行时的启动
*   例程，它是由链接器设为应用程序入口的，最终会调用应用程序WinMain，wWinMain，main或wmian函数。如果系统成功创建了新进程和主线程，CreateProcess将返回TRUE。
* 20）终止进程
*   进程可以通过以下4种方式终止：
*	·主线程的入口点函数返回（强烈推荐的方式）
*	·进程中的一个线程调用ExitProcess函数（要避免这种方式）
*	·另一个进程中的线程调用TerminateProcess函数（要避免这种方式）
*	·进程中的所有线程都“自然死亡”（这种情况几乎从来不会发生）
* 21）主线程的入口点函数返回
*		设计一个应用程序时，应该保证只有在主线程的入口点函数返回之后，这个应用程序的进程才终止。只有这样，才能保证主线程的所有资源都被正确清理。
*		让主线程的入口点函数返回，可以保证以下操作会被执行。
*		·该线程创建的任何C++对象都将由这些对象的析构函数正确销毁。
*		·操作系统将正确释放线程栈使用的内存。
*		·系统将进程的退出代码（在进程内核对象中维护）设为入口点函数的返回值。
*		·系统递减进程内核对象的引用计数。
* 22）当进程终止运行时
*    一个进程终止时，系统会依次执行以下操作：
*		（1）终止进程中遗留的任何线程。
*		（2）释放进程分配的所有用户对象和GDI对象，关闭所有内核对象（如果没有其他进程打开这些内核对象的句柄，那么它们也会被销毁。不过，如果其他进程打开了它们的句柄，那么它们就不会被销毁）。
*		（3）进程的退出代码从STILL_ACTIVE变为传给ExitProcess或TerminateProcess函数的代码。
*		（4）进程内核对象的状态变成已触发状态。这就是为什么系统中的其他线程可以挂起它们自己，直至另一个线程终止运行。
*		（5）进程内核对象的使用计数递减1。
* 23）运行独立的子进程
*	 为了断绝与子进程的所有联系，Windows资源管理器必须调用CloseHandle来关闭新进程及其主线程的句柄：
*       PROCESS_INFORMATION pi;
*		// Spawn the child process
*		BOOL fSuccess = CreateProcess(..., &pi);
*		if (fSuccess)
*		{
*			CloseHandle(pi.hThread);
*			CloseHandle(pi.hProcess);
*		}
* 24）用户账户控制（User Account Control，UAC）
*	  在Windows Vista中，如果用户使用管理员这样的一个被授予高特权的账户登录，那么除了与这个账户对应的安全令牌之外，还会创建一个经过筛选的令牌（filtered token），后者将被授予
*   标准用户（Standart User）的权限。以后，从包括Windows资源管理器在内的第一个进程开始，这个筛选后的令牌会与系统代表最终用户启动的所有新进程关联。
* 25）权限提升
*	  首先，我们可以要求操作系统提升权限，但只能在进程边界上提升。这是什么意思呢？默认情况下，一个进程启动时，它会与当前登录用户的筛选后的令牌关联起来。要为进程授予更多权限，
*   我们（开发人员）需要指示Windows做这样一件事情：在进程启动之前，先友好地征得最终用户（对于提升权限）的同意。作为最终用户，可以使用快捷菜单中的Run as administrator（以管理员身份运行）
*   命令。Windows只允许在进程边界上进行权限提升。一旦进程启动，再要求更多的权限就已经迟了。
* 26）手动提升进程的权限
*	  CreateProcess函数没有专门提供什么标记或参数来指定对这种权限提升的需求。相反，我们需要调用ShellExecuteEx函数：
*	  BOOL ShellExecuteEx(LPSHELLEXECUTEINFO pExecInfo);
*	  typedef struct _SHELLEXECUTEINFOA {
			DWORD     cbSize;
			ULONG     fMask;
			HWND      hwnd;
			LPCSTR    lpVerb;
			LPCSTR    lpFile;
			LPCSTR    lpParameters;
			LPCSTR    lpDirectory;
			int       nShow;
			HINSTANCE hInstApp;
			void      *lpIDList;
			LPCSTR    lpClass;
			HKEY      hkeyClass;
			DWORD     dwHotKey;
			union {
				HANDLE hIcon;
				HANDLE hMonitor;
			} DUMMYUNIONNAME;
			HANDLE    hProcess;
	   } SHELLEXECUTEINFOA, *LPSHELLEXECUTEINFOA;
	   SHELLEXECUTEINFO结构中唯一有趣的字段是lpVerb和lpFile。前者必须被设为"runas"，后者必须包含使用提升后的权限来启动的一个可执行文件的路径。
* 27）Windows NT有一个不断更新的数据库，称为“性能数据”（Performance Data）数据库。该数据库包含海量信息，这些信息可以通过注册表函数来访问，例如RegQueryValueEx（把注册表的）
*   根项设为KEY_PERFORMANCE_DATA）。由于以下原因，几乎很少有Windows程序员知道这个性能数据库：
*		·它没有自己专门的函数，使用的是线程的注册表函数
*		·它在Windows95和Windows98上不可用
*		·数据库中的信息分布非常复杂，许多开发人员都怕用它，这妨碍了它的流行。
*	  为了更方便地使用这个数据库，Microsoft创建了一套Performance Data Helper函数（包含在PDH.dll中）。
* 28）Windows Vista还实现了另一个安全机制，即“Windows完整性机制”（Windows Integrity Mechanism），以前称为“强制完整性控制”（Mandatory Integrity Control）。
*   除了众所周知的安全描述符(SID)和访问控制列表(access control list, ACL), 系统还通过在系统访问控制列表(SACL)中新增一个名为强制标签的访问控制项（access control entry, ACE）
*   来为受保护的资源分配一个所谓的完整性级别（integrity level）。凡是没有这个ACE的安全对象，操作系统将默认其拥有“中”（Medium）完整性级别。另外，每个进程都有一个基于其
*   安全令牌的完整性级别，它与系统授予的一个信任级别是对应的。
*	——————————————————————————————————————————————————————————————————
*     级别  |                                   应用程序示例
*	——————————————————————————————————————————————————————————————————
*     低    |    保护模式中的Internet Explorer是以“低”信任级别来运行的，目的在于拒绝从网上下载的代码修改其他应用程序和Windows环境
*	——————————————————————————————————————————————————————————————————
*     中    |    默认情况下，所有应用程序都以“中”信任级别来启动，并使用一个筛选过的令牌来运行
*   ——————————————————————————————————————————————————————————————————
*     高    |    如果应用程序以提升后的权限来启动，则以“高”信任级别来运行
*   ——————————————————————————————————————————————————————————————————
*     系统  |    只有以Local System 或 Local Service的身份运行的进程，才能获得这个信任级别
*   ——————————————————————————————————————————————————————————————————
* 
*/

#include <Windows.h>
#include <vector>
#include <string>
#include <map>

namespace HY_KERNELOBJECT
{
	class CHYProcessKernelObject
	{
	public:
		CHYProcessKernelObject();
		~CHYProcessKernelObject();

		// 获取可执行文件或DLL文件加载到进程地址空间的位置，如果pszModule为NULL，则返回主调进程的可执行文件的基地址。
		HMODULE GetModuleHandle(PCTSTR pszModule);

		// DLL中获取模块基址的方法一
		HMODULE GetModuleHandleInDllOne();

		// DLL中获取模块基址的方法二
		HMODULE GetModuleHandleInDllTwo();

		// 获取指向进程完整命令行的指针
		PTSTR GetCommandLine();

		// 分解命令行
		std::vector<std::wstring> CommandLineToArgv();

		// 获取完整的环境块
		std::map<std::wstring, std::wstring> DumpEnvStrings();

		// 获取完整的环境块，CUI程序专用
		std::map<std::wstring, std::wstring> DumpEnvVariables(PTSTR pEnvBlock[]);

		// 通知其他进程更新环境变量
		LRESULT SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		// 获取一个环境变量的值
		DWORD GetEnvironmentVariable(PCTSTR pszName, PTSTR pszValue, DWORD cchValue);

		// 扩展“可替换字符串”
		DWORD ExpandEnvironmentStrings(PCTSTR pszSrc, PTSTR pszDst, DWORD chSize);

		// 设置进程的错误处理模式
		// SEM_FAULCRITICALERRORS		系统不显示严重错误处理程序（critical-error-handler）消息框，并将错误返回主调进程
		// SEM_NOGPFAULTERRORBOX		系统不显示常规包含错误（general-protection-fault）消息框。此标志只应该由调试程序设置；该调试程序用一个异常处理程序来自行处理常规保护（general protection, GP）错误
		// SEM_NOOPENFILEERRORBOX		系统查找文件失败时，不显示消息框
		// SEM_NOALIGNMENTFAULTEXCEPT	系统自动修复内存对齐错误，并使应用程序看不到这些错误。此标志对x86/x64处理器无效
		UINT SetErrorMode(UINT fuErrorMode);

		// 系统在内部跟踪着一个进程的当前驱动器和目录
		// 一个线程获取其所在进程的当前驱动器和目录
		DWORD GetCurrentDirectory(DWORD cchCurDir, PTSTR pszCurDir);

		// 一个线程设置其所在进程的当前驱动器和目录
		BOOL SetCurrentDirectory(PCTSTR pszCurDir);

		// 进程获得当前目录
		DWORD GetFullPathName(PCTSTR pszFile, DWORD cchPath, PTSTR pszPath, PTSTR* ppszFilePart);

		// 获取系统版本，判断是否为Windows Vista以下版本系统
		BOOL IsWindowsXp();

		// 测试系统是不是Windows Vista
		BOOL IsWindowsVista();

		// 创建进程
		BOOL CreateProcess(
			LPCWSTR	lpApplicationName,
			LPWSTR	lpCommandLine,
			LPSECURITY_ATTRIBUTES lpProcessAttributes,
			LPSECURITY_ATTRIBUTES lpThreadAttributes,
			BOOL                  bInheritHandles,
			DWORD                 dwCreationFlags,
			LPVOID                lpEnvironment,
			LPCWSTR                lpCurrentDirectory,
			LPSTARTUPINFOW        lpStartupInfo,
			LPPROCESS_INFORMATION lpProcessInformation);

		// 获取当前进程的ID
		DWORD GetCurrentProcessId();

		// 获取当前正在运行的线程的ID
		DWORD GetCurrentThreadId();

		// 获得与指定句柄对应的一个进程的ID
		DWORD GetProcessId(HANDLE Process);

		// 获得与指定句柄对应的一个线程的ID
		DWORD GetThreadId(HANDLE Thread);

		// 获得线程句柄所在进程的ID
		DWORD GetProcessIdOfThread(HANDLE Thread);

		// 在该进程中的一个线程中终止进程
		VOID ExitProcess(UINT fuExitCode);

		// 终止另外的进程
		BOOL TerminateProcess(HANDLE hProcess, UINT fuExitCode);

		// 获取进程的退出代码
		BOOL GetExitCodeProcess(HANDLE hProcess, PDWORD pdwExitCode);

		// 返回提升类型和指出进程是否正在以管理员身份运行的布尔值
		BOOL GetProcessElevation(TOKEN_ELEVATION_TYPE* pElevationType, BOOL* pIsAdmin);
	};
}


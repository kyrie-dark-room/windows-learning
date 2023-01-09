#pragma once
#include <Windows.h>
#include <imagehlp.h>
/*
* 知识点：
* 1）FreeLibraryAndExitThread函数在Kernel32.dll中的实现如下：
*		VOID FreeLibraryAndExitThread(HMODULE hLibModule, DWORD dwExitCode){
*			FreeLibrary(hLibModule);
*			ExitThread(dwExitCode);
*		}
*	 注意：如果在DLL中线程分别FreeLibrary和ExitThread，那么会出现一个严重的问题。这个问题就是FreeLibrary会立即从进程的地址空间中撤销对DLL的映射。当FreeLibrary调用返回的时候，
*	调用ExitThread的代码已经不复存在了，线程试图执行的是不存在的代码。这将引发访问违规，并导致整个进程被终止！
*	 但是，如果线程调用FreeLibraryAndExitThread，那么这个函数会调用FreeLibrary，这使得DLL的映射会立即被撤销。要执行下一条指令仍在Kernel32.dll中，而不是在已经被撤销映射的DLL中。
*	这意味着线程可以继续执行并调用ExitThread。ExitThread会使线程终止并且不再返回。
* 2）混用LoadLibrary和LoadLibraryEx可能会导致将同一个DLL映射到同一个地址空间中的不同位置。当我们用LOAD_LIBRARY_AS_DATAFILE，LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE
	或LOAD_LIBRARY_AS_IMAGE_RESOURCE标志调用LoadLibraryEx的时候，操作系统会先检测该DLL是否已经被LoadLibrary或LoadLibraryEx（但没有使用这些标志）载入过。如果已经被载入过，那么
	函数会返回地址空间中DLL原先已经被映射到的地址。但是，如果DLL尚未被载入，那么Windows会将该DLL载入到地址空间中一个可用的地址，但不认为它是一个完全载入的DLL。这时如果用这个
	模块句柄来调用GetModuleFileName，那么得到的返回值将为0。这是一种非常好的方法，可以让我们知道与一个DLL相对应的模块句柄并不包含动态函数，因此无法通过GetProcAddress来得到函数
	地址并对函数进行调用。
* 3）为了让DLL更易于使用，Microsoft Visual C++提供了一项很棒的特性，即延迟载入DLL。一个延迟载入的DLL是隐式链接的，系统一开始不会将该DLL载入，只有当我们的代码试图去引用DLL中包含的
*	一个符号时，系统才会实际载入该DLL。延迟载入DLL在下列情况下非常有用。
*		·如果应用程序使用了多个DLL，那么它的初始化可能会比较慢，因为加载程序将所有必须的DLL映射到进程的地址空间中。缓解这个问题的一种方法时将DLL的载入过程延迟到进程的执行过程中。
*		·如果我们在代码中调用一个新的函数，然后又试图在一个不提供该函数的老版本的操作系统中运行该引用程序，那么加载程序会报告一个错误并且不允许应用程序运行。
*	 延迟载入的局限性：
*		·一个导出了字段（或数据，亦即全局变量）的DLL是无法延迟载入的。
*		·Kernel32.dll模块是无法延迟载入的，这是因为必须载入该模块才能调用LoadLibrary和GetProcAddress。
*		·不应该在DLLMain入口点函数中调用一个延迟载入的函数，因为这样可能会导致程序崩溃。
*	 链接可执行文件的时候，我们必须修改一些链接器开关：/DelayLoad:MyDll.dll。开关告诉链接器下列事项：
		·要将指定的函数__delayLoadHelper2嵌入到我们的可执行文件中。
		·将MyDll.dll从可执行模块的导入段中去除，这样当进程初始化的时候，操作系统的加载程序就不会隐式地载入该DLL。
		·在可执行模块中嵌入一个新的延迟载入段（即Delay Import section，称为.didata）来表示要从MyDll.dll中导入哪些函数。
		·通过让对延迟载入函数的调用跳转到__delayLoadHelper2函数，来完成对延迟载入函数的解析。
	 应用程序运行的时候，对延迟载入函数的调用实际上会调用__delayLoadHelper2函数。这个函数会引用那个特殊的延迟载入段，并会先后调用LoadLibrary和GetProcAddress。一旦得到了对应的
	延迟载入函数的地址，__delayLoadHelper2会修复对该函数的调用，这样今后的调用将直接调用该延迟载入函数。
	 * Microsoft实现的延迟载入DLL提供了更多的特性。
		·应用程序可以将一个延迟载入的DLL卸载。为了卸载一个延迟载入的DLL，我们必须做两件事情。首先，必须在构建可执行文件的时候指定一个额外的链接器开关(/Delay:unload)。其次，必须
		  修改源代码，在我们想要卸载DLL的地方调用__FUnloadDelayLoadedDLL2函数：
			BOOL __FUnloadDelayLoadedDLL2(PCSTR szDll);
		  链接器开关/Delay:unload告诉链接器在文件中嵌入另一个段。这个段包含必要的信息，来重置我们已经调用过的函数，这样当程序调用这些函数的时候，会再次调用__delayLoadHelper2函数。
		  调用__FUnloadDelayLoadedDLL2的时候，需要传入我们想要卸载的延迟载入的DLL的名字。该函数会引用文件的卸载段，将该DLL所有的函数地址重置。然后__FUnloadDelayLoadedDLL2会调用
		  FreeLibrary来卸载该DLL。
		·延迟载入DLL的另一个特性是，在默认的情况下，我们调用的函数会被绑定到进程地址空间中的一个内存地址上，这个地址是系统认为该函数应该在的位置。由于创建可绑定的延迟载入DLL段
		  会增加可执行文件的大小，因此链接器也支持一个/Delay:nobind开关。因为通常情况下绑定都是需要的，因此大多数应用程序不应该使用这个链接器开关。
		·延迟载入DLL的最后一项特性是为高级用户设计的，它也真正显示了Microsoft对细节的关注。当__delayLoadHelper2执行的时候，可以调用一些我们提供的挂钩函数(hook function)。这些
		  函数可以接收__delayLoadHelper2的进度通知和错误通知。因此，这些函数还可以改变载入DLL以及得到函数虚拟地址的方式。
		  为了得到通知或覆盖默认的行为，我们必须在源代码中做两件事情。
		      ·首先，必须编写一个挂钩函数，挂钩函数的原型为：
				typedef FARPROC (WINAPI *pfnDliHook) (unsigned dliNotify, pDelayLoadInfo pdli);
			  ·在DelayImp.lib静态链接库的内部，定义了两个全局变量：__pfnDliNotifyHook2和__pfnDliFailureHook2。这两个变量的类型都是pfnDliHook。
			    DelayImp.lib在内部将这两个函数初始化为NULL，这告诉__delayLoadHelper2不要调用任何挂钩函数。为了让系统调用我们的挂钩函数，我们必须将这两个变量之一设为挂钩函数的地址。

*/

namespace HY_DLL_Manager
{
	class CHYLibraryLoader
	{
	public:
		CHYLibraryLoader();
		~CHYLibraryLoader();

		// HMODULE表示文件映像被映射到的虚拟内存地址，类型等价于HINSTANCE，两者可以换用
		HMODULE LoadLibrary(LPCWSTR lpLibFileName);
		HMODULE LoadLibraryEx(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);

		// 将目录添加到用于查找应用程序的 DLL 的搜索路径，添加到默认搜索路径顺序：进程的当前目录、通过SetDLLDirectory所设置的文件夹、Windows的系统目录、......
		BOOL SetDllDirectory(LPCWSTR lpPathName);
		// 将目录添加到用于查找应用程序的 DLL 的搜索路径(注意：支持的系统版本)
		DLL_DIRECTORY_COOKIE AddDllDirectory(PCWSTR NewDirectory);
		// 使用 AddDllDirectory 删除已添加到进程 DLL 搜索路径的目录
		BOOL RemoveDllDirectory(DLL_DIRECTORY_COOKIE Cookie);

		// 释放加载的动态链接库 (DLL) 模块，并在必要时递减其引用计数。 当引用计数达到零时，将从调用进程的地址空间卸载模块，并且句柄不再有效
		BOOL FreeLibrary(HMODULE hInstDll);
		// 递减加载的动态链接库的引用计数 (DLL) ，然后调用 ExitThread 终止调用线程
		void FreeLibraryAndExitThread(HMODULE hLibModule, DWORD dwExitCode);

		// 线程可以通过调用下面函数来检测一个DLL是否已经被映射到了进程的地址空间中，如果参数为NULL，则返回应用程序的可执行文件的句柄。
		HMODULE GetModuleHandle(PCTSTR pszModuleName);

		// 检索包含指定模块的文件的完全限定路径，模块必须由当前进程加载，如果传NULL给hInstModule参数，那么会返回正在运行的应用程序的可执行文件的文件名
		DWORD GetModuleFileName(HMODULE hModule, LPWSTR lpFilename, DWORD nSize);
		// 若要查找其他进程加载的模块的文件
		DWORD HYGetModuleFileNameEx(HANDLE hProcess, HMODULE hModule, LPWSTR lpFilename, DWORD nSize);

		// 从指定的动态链接库 (DLL) 检索导出函数 (也称为过程) 或变量
		FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName);

		// 禁用指定动态链接库 (DLL) 的DLL_THREAD_ATTACH和DLL_THREAD_DETACH通知。 这可以减少某些应用程序的工作集的大小
		BOOL DisableThreadLibraryCalls(HMODULE hLibModule);

		// Changes the load address for the specified image, which reduces the required load time for a DLL
		BOOL ReBaseImage64(
			PCSTR   CurrentImageName,
			PCSTR   SymbolPath,
			BOOL    fReBase,
			BOOL    fRebaseSysfileOk,
			BOOL    fGoingDown,
			ULONG   CheckImageSize,
			ULONG* OldImageSize,
			ULONG64* OldImageBase,
			ULONG* NewImageSize,
			ULONG64* NewImageBase,
			ULONG   TimeStamp
		);

		// 计算导入的每个函数的虚拟地址
		BOOL BindImageEx(
			DWORD                    Flags,
			PCSTR                    ImageName,
			PCSTR                    DllPath,
			PCSTR                    SymbolPath,
			PIMAGEHLP_STATUS_ROUTINE StatusRoutine
		);

		// 要使用动态TLS，我们必须先调用TlsAlloc，这个函数让系统对进程中的位标志进行检索并找到一个FREE标志。然后系统会将该标志从FREE改为INUSE并让TlsAlloc返回该标志在位数组中的索引。
		DWORD TlsAlloc();
		// 当系统创建一个线程的时候，会分配TLS_MINIMUM_AVAILABLE个PVOID值，将它们都初始化为0，并与线程关联起来。每个线程都有自己的PVOID数组，数组中的每个PVOID可以保存任意值。
		// 为了把一个值放到线程的数组中，我们应该调用
		BOOL TlsSetValue(DWORD dwTlsIndex, PVOID pvTlsValue);
		// 为了从线程的数组中取回一个值，我们应该调用
		PVOID TlsGetValue(DWORD dwTlsIndex);
		// 当我们不再需要一个已经预定的TLS元素时，应该调用TlsFree
		BOOL TlsFree(DWORD dwTlsIndex);

	private:
		CHYLibraryLoader(const CHYLibraryLoader&) = delete;
		CHYLibraryLoader& operator=(const CHYLibraryLoader&) = delete;
		CHYLibraryLoader(CHYLibraryLoader&&) = delete;
		CHYLibraryLoader& operator=(CHYLibraryLoader&&) = delete;
	};
}


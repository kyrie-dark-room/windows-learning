// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

/*
* 知识点：
* 1）DLL的入口点函数（DllMain是区分大小写的）：一个DLL可以有一个入口点函数。系统会在不同的时候调用这个入口点函数。这些调用是通知性质的，通常被DLL用来执行一些与进程或线程
*   有关的初始化和清理工作。如果DLL不需要这些通知。那么我们可以不必在源代码中实现这些入口点函数。
* 2）参数hModule包含该DLL实例的句柄。这个值表示一个虚拟内存地址，DLL的文件映像就被映射到进程地址空间中的这个位置。我们通常将这个参数保存在一个全局变量中，这样在调用资源载入函数
*   的时候，我们就可以使用它。
* 3）参数ul_reason_for_call表示系统调用入口点函数的原因。
* 4）必须记住，DLL使用DllMain函数来对自己进行初始化。Platform SDK文档说DllMain函数只应该执行简单的初始化，比如设置线程局部存储区，创建内核对象，打开文件，
    等等（https://learn.microsoft.com/zh-cn/windows/win32/dlls/dllmain）。
* 5）DLL_PROCESS_ATTACH通知
*    当系统第一次将一个DLL映射到进程的地址空间中时，会调用DLLMain函数，并将fdwReason参数中传入DLL_PROCESS_ATTACH。
*    当然，系统中的某个线程负责执行DLLMain函数中的代码。创建新的进程时，系统会分配进程地址空间并将.exe的文件影像以及所需DLL的文件影像映射到进程的地址空间中。然后，系统将创建进程
*   的主线程并用这个线程来调用每个DLL的DllMain函数，同时传入DLL_PROCESS_ATTACH。当所有已映射的DLL都完成了对该通知的处理后，系统会先让进程的主线程开始执行可执行模块的C/C++运行时的
    启动代码（startup code），然后执行可执行模块的入口点函数（_tmain或_tWinMain）。如果任何一个DLL的DllMain函数返回FALSE，也就是说初始化没有成功，那么系统会把所有的文件影像从地址
    空间中清除，向用户显示一个消息框来告诉用户进程无法启动，然后终止整个进程。
     现在让我们来看一看显示载入DLL的时候会发生什么情况。进程调用LoadLibrary(Ex)的时候，系统会对指定的DLL进行定位，并将该DLL映射到进程的地址空间中。然后系统会用调用LoadLibrary(Ex)
    的线程来调用DLL的DllMain函数，并传入DLL_PROCESS_ATTACH值。当DLL的DLLMain函数完成了对通知的处理后，系统会让LoadLibrary(Ex)调用返回，这样线程就可以继续正常执行。如果DllMain函数
    返回FALSE，也就是说初始化不成功，那么系统会自动从进程的地址空间中撤销对DLL文件影像的映射，并让LoadLibrary(Ex)返回NULL。
* 6）DLL_PROCESS_DETACH通知
*    当系统将一个DLL从进程的地址空间中撤销映射时，会调用DLL的DllMain函数，并在fdwReason参数中传入DLL_PROCESS_DETACH。
*    如果撤销映射的原因使因为进程要终止，那么调用ExitProcess函数的线程负责执行DllMain函数的代码。在正常情况下，这个线程就是应用程序的主线程。当我们的入口点函数返回到C/C++运行时的
*   启动代码后，启动代码会现实地调用ExitProcess来终止进程。
*    如果撤销映射的原因是因为进程中的一个线程调用了FreeLibrary或FreeLibraryAndExitThread，那么发出调用的线程将执行DllMain函数中的代码。如果调用的是FreeLibrary，那么在DllMain处理完
*   DLL_PROCESS_DETACH通知之前，线程是不会从该调用中返回的。
* 7）DLL_THREAD_ATTACH通知
*    当进程创建一个线程的时候，系统会检查当前映射到该进程的地址空间中的所有DLL文件影像，并用DLL_THREAD_ATTACK来调用每个DLL的DllMain函数。这告诉DLL需要执行与线程相关的初始化。新
*   创建的线程负责执行所有DLL的DllMain函数中的代码。只有当所有DLL都完成了对该通知的处理之后，系统才会让新线程开始执行它的线程函数。
*    另外要注意的是，系统不会让进程的主线程用DLL_THREAD_ATTACK值来调用DllMain函数。
* 8）DLL_THREAD_DETACH通知
*    让线程终止的首选方式是让它的线程函数返回。这会使得系统调用ExitThread来终止线程。ExitThread告诉系统该线程想要终止，但系统不会立即终止该线程，而会让这个即将终止的线程用
*   DLL_THREAD_DETACH来调用所有已映射DLL的DllMain函数。这个通知告诉DLL执行与线程有关的清理。
* 9）系统会将对DLL的DllMain函数的调用序列化。
* 10）在链接DLL的时候，链接器会将DLL的入口点函数的地址嵌入到生成的DLL文件影像中。我们可以用链接器的/ENTRY开关来指定入口点函数的地址。在默认的情况下，如果用的是Microsoft链接器
*	并指定了/DLL开关，那么链接器会认为入口点函数的函数名是_DllMainCRTStartup。这个函数包含在C/C++运行库中，在链接DLL的时候会被静态链接到DLL的文件影像。（即便用的是C/C++运行库
	的DLL版本，对这个函数的链接仍然会是静态的。）
	 系统将DLL的文件影像映射到进程的地址空间中时，实际上调用的是_DllMainCRTStartup函数，而不是我们的DllMain函数。在将所有的通知都转发到_DllMainCRTStartup函数之前，为了支持/GS
	开关所提供的安全性特性，_DllMainCRTStartup函数会对DLL_PROCESS_ATTACK通知进行处理。_DllMainCRTStartup函数会初始化C/C++运行库，并确保在_DllMainCRTStartup收到DLL_PROCESS_ATTACK
	通知的时候，所有全局或静态C++对象都已经构造完毕。在C/C++运行时的初始化完成之后，_DllMainCRTStartup函数会调用我们的DllMain函数。
	 当DLL收到DLL_PROCESS_DETACH通知的时候，系统会再次调用_DllMainCRTStartup函数。这一次，该函数会调用我们的DllMain函数，当DllMain函数返回的时候，_DllMainCRTStartup会调用DLL
	中所有全局或静态C++对象的析构函数。当接收到DLL_THREAD_ATTACH或DLL_THREAD_DETACH通知的时候，_DllMainCRTStartup不会做任何的特殊处理。
* 11）如果没有自己的DllMain函数，那么我们可以使用C/C++运行库提供的DllMain函数，它的实现看起来大致如下（如果静态连接到C/C++运行库）：
*		BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
*		{
*			if (ul_reason_for_call == DLL_PROCESS_ATTACH)
*				DisableThreadLibraryCalls(hInstDll);
*			return(TRUE);
*		}
*/

HINSTANCE g_hDllModule = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    // 保存DLL实例的句柄
    g_hDllModule = hModule;

    // 系统调用入口点函数的原因分析
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


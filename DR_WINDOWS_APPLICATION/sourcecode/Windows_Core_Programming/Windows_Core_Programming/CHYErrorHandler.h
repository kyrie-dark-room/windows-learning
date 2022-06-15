#pragma once

/*
* 引用：
*	  调用Windows函数时，它会先验证我们传给它的参数，然后再开始执行任务。如果传入的参数无效，或者由于其他原因操作无法进行，则函数的返回值将指出函数因为某些原因失败了。
* 
* 常见的Windows函数返回值数据类型
* ————————————————————————————————————————————————————————————————————————————————
*     数据类型    |														指出函数调用失败的值
* ————————————————————————————————————————————————————————————————————————————————
*     VOID		  |	   这个函数不可能失败。只有极少数Windows函数的返回值类型为VOID
* ————————————————————————————————————————————————————————————————————————————————
*     BOOL        |    这个函数失败，返回值为0；否则，返回值是一个非0值。应避免测试返回值是否为TRUE；最稳妥的做法是检查它是否不为FALSE
* ————————————————————————————————————————————————————————————————————————————————
*     HANDLE      |    如果函数失败，则返回值通常为NULL；否则，HANDLE将标识一个可以操纵对象。请注意这种返回值，因为某些函数会返回INVALID_HANDLE_VALUE的一个句柄值，
*				  |	   它被定义为-1。函数的Platform SDK文档清楚说明了函数是返回NULL还是INVALID_HANDLE_VALUE来标识失败。
* ————————————————————————————————————————————————————————————————————————————————
*     PVOID       |    如果函数调用失败，则返回值为NULL；否则，PVOID将标识一个数据块的内存地址
* ————————————————————————————————————————————————————————————————————————————————
*     LONG/DWORD  |    这种类型比较棘手。返回计数的函数通常会返回一个LONG或DWORD。如果函数出于某种原因不能对我们想要计数的东西进行计数，它通常会返回0或-1
*					  （具体取决于函数）。如果要调用一个返回LONG/DWORD的函数，务必仔细阅读Platform SDK文档，确保我们会正确地检查可能出现的错误
* ————————————————————————————————————————————————————————————————————————————————
* 
* Windows错误码
*     Microsoft编辑了一个列表，其中列出了所有可能的错误代码，并为每个错误代码都分配了一个32位的编号。
* 
* Windows错误码实现机制
*     在内部，当Windows函数检测到错误时，它会使用一种名为“线程本地存储区”（thread-local-storage）的机制将相应的错误代码与“主调线程”（或者说发出调用的线程，即
*   calling thread）关联到一起。这种机制使不同的线程能独立运行，不会出现相互干扰对方的错误代码的情况。函数返回时，其返回值会指出已发生一个错误。要查看具体是什么
*   错误，请调用GetLastError函数。
*     DWORD GetLastError()
* Windows错误码定义
*     有了32位错误代码之后，接着需要把它转换为更有用的信息。WinError.h头文件包含了Microsoft定义的的错误代码列表。部分摘录如下
*     // MessageId: ERROR_SUCCESS
*	  //
*     // MessageText:
*     //
*     // The operation completed successfully.
*     //
*     #define ERROR_SUCCESS						0L
*     
*     #define NO_ERROR							0L    // dderror
*	  #define SEC_E_OK							((HRESULT)0x00000000L)
* 
*     // MessageId: ERROR_INVALID_FUNCTION
*	  //
*     // MessageText:
*     //
*     // Incorrect funtion.
*     //
*     #define ERROR_INVALID_FUNCTION			1L
* 
*	  可以看出，每个错误都有三种表示：一个消息ID（一个可在源代码中使用的宏，用于与GetLastError的返回值进行比较）、消息文本（描述错误的英文文本）和一个编号
*  （应该避免使用此编号，尽量使用消息ID）。
* 
* 其它知识：
*	  1）在Microsoft Visual Studio中，Microsoft的调试器支持一个很有用的功能--我们可以配置Watch（监视）窗口，让它始终显式线程的上一个错误代码和错误的文本描述。具体的
*   做法是：在Watch窗口中选择一行，然后输入$err,hr。
*	  2）Visual Studio还搭载了一个很小的实用程序，名为Error Lookup。利用它，可以将错误代码转换为相应的文本描述。
*	  3）Windows提供了一个函数，可以将错误代码转换为相应的文本描述。次函数名为FormatMessage，如下所示：
*			DWORD FormatMessage(
*				DWORD		dwFlags,
*				LPCVOID		pSource,
*				DWORD		dwMessageId,
*				DWORD		dwLanguageId,
*				PTSTR		pszBuffer,
*				DWORD		nSize,
*				va_list		*Arguments);
*	  4）为了指出错误，只需设置线程的上一个错误代码，然后令自己的函数返回FALSE，INVALID_HANDLE_VALUE、NULL或者其他合适的值。为了设置线程的上一个错误代码，只需要
*   调用以下函数，并传递我们认为合适的任何32位值：
*			VOID SetLastError(DWORD dwErrCode);
*     5）我们尽量使用WinError.h中出现的代码--只要代码能很好地反应我们想报告的错误。如果WinError.h中的任何一个代码都不能准确反应一个错误，就可以创建自己的代码。错误
*   代码是一个32位数，由下表描述的几个不同的字段组成
*        ——————————————————————————————————————————————————
*			位	|		31-30		|		29		|		28		|		27-16		|		15-0
*		 ——————————————————————————————————————————————————
*		  内容  | 严重性			| Microsoft/客户| 保留			| Facility代码		| 异常代码
*		 ——————————————————————————————————————————————————
*		  含义  | 0 = 成功			| 0 = Microsoft | 必须为0		| 前256个值由		| Microsoft/客户
*				| 1 = 信息（提示）  | 定义的代码    |				| Microsoft保留		| 定义的代码
*				| 2 = 警告			| 1 = 客户定义  |				|					|
*				| 3 = 错误			| 的代码		|				|					|
*		 ——————————————————————————————————————————————————
*        唯一需要注意的重要字段是第29位。Microsoft承诺，在它所生成的所有错误代码中，此位将始终为0。但是，如果要创建我们自己的错误代码，就必须在此位放入一个1。通过
*      这种方式，可以保证我们的错误码绝对不会与Microsoft现在和将来定义的错误代码冲突。注意，Facility字段非常大，足以容纳4096个可能的值。其中，前256个值是为Microsoft
*      保留的，其余的值可由我们自己的应用程序来定义。
*	  6）每个DLL(或.exe)都可以有自己的一套错误代码。我们也可以向自己的模块添加错误代码，具体做法是使用Message Compiler（MC.exe）来创建一个消息资源并将其添加到DLL(或.exe)模块中。
*     
*/

#include <Windows.h>
#include <string>

namespace HY_Windows_ErrorHandle
{
	class CHYErrorHandler
	{
	public:
		CHYErrorHandler();
		virtual ~CHYErrorHandler();
		CHYErrorHandler(const CHYErrorHandler&) {};
		CHYErrorHandler& operator=(const CHYErrorHandler&) {};
		CHYErrorHandler(CHYErrorHandler&& s) {};

		// 获取错误码
		DWORD GetLastError();
		// 获取错误信息
		std::wstring GetLastErrorMsg();

	protected:

	private:

	};

}


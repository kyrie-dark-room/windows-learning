#pragma once

/*
* 引用：
*		我们应该在应用程序始终使用Unicode字符串，而且始终应该通过新的安全字符串函数来处理这些字符串。这会增强应用程序的执行性能，并且为本地化工作奠定基础。另外，它还有利于
*	同COM和.Net Framework的互操作。
* 
* Unicode的诞生：
*		Unicode是1988年由Apple和Xerox共同建立的一项标准。1991年，成立了专门的协会来开发和推动Unicode。
* 
* Unicode的实现：
*		在Windows Visia中，每个Unicode字符都使用UTF-16编码，UTF的全称是Unicode Transformation Format（Unicode转换格式）。UTF-16将每个字符编码为2个字节（或者说16位）。
*		Windows之所以使用UTF-16，是因为全球各地使用的大部分语言中，每个字符很容易用一个16位值来表示。这样一来，应用程序很容易遍历字符串并计算出它的长度。但是，16位
*	不足以表示某些语言的所有字符。对于这些语言，UTF-16支持使用代理（surrogate），后者是用32位（或者说4个字节）来表示一个字符的一种方式。由于只有少数应用程序需要表示这些
*	语言中的字符，所以UTF-16在节省空间和简化编码这两个目标之间，提供了一个很好的折中。注意，.NET Framework使用使用UTF-16来编码所有字符和字符串。
* 
* 其他用于表示字符的UTF标准：
*	● UTF-8	UTF-8将一些字符编码为1个字节，一些字符编码为2个字节，一些字符编码为3个字节，一些字符编码为4个字节。值在0x0080以下的字符压缩为1个字节，这对美国使用的字符非常
*			适合。0x0080和0x07FF之间的字符转换为2个字节，这对欧洲和中东地区的语言非常适用。0x0800以上的字符都转换为3个字节，适合东亚地区的语言。最后，代理对（surrogate pair）
*			被写成4个字节。UTF-8是一种相当流行的编码格式。但在对值为0x0800及以上的大量字符进行编码的时候，不如UTF-16高效。
*	● UTF-32	UTF-32将每个字符都编码为4个字节。如果打算写一个简单的算法来遍历字符（任何语言中使用的字符），但又不想处理字节数不定的字符，这种编码方式就非常有用。例如，采用
*			UTF-32编码方式，就不需要关心代理(surrogate)的问题，因为每个字符都是4个字符。显然，从内存使用这个角度来看，UTF-32并不是一种高校的编码格式。因此，在将字符串保存到
*			文件或传到网络的时候，很少会用到这种格式。这种编码格式一般在应用程序内部使用。
* 
* Unicode字符集和字母表
*		目前，Unicode为阿拉伯语、汉语拼音、西里尔文（俄语）、希腊语、希伯来语、日语片假名、朝鲜语和拉丁语（英语）字符等——这些字符称为文字符号（script）——定义了码位
	（code point, 即一个符号在字符集中的位置）。每个版本的Unicode都在现有的文字符号的基础上引入了新的字符，甚至会引入新的文字符号，比如腓尼基文（一种古地中海文字）。字符集
	中还包含大量标点符号、数学符号、技术符号、箭头、装饰标志、读音符号以及其他符号。
		这65535个字符被划分为若干个区域，下表展示了部分区域以及分配到这些区域的字符。
		————————————————————————————————————————————————————————————————————————————
			6位代码			|		字符		|			16位代码			|			字母/书写符号
		————————————————————————————————————————————————————————————————————————————
			0000-007F		|	ASCII			|		0300-036F				|			常见的变音符号
		————————————————————————————————————————————————————————————————————————————
			0080-00FF		|	西欧语系字母	|		0400-04FF				|			西里尔字母
		————————————————————————————————————————————————————————————————————————————
			0100-017F		|	欧洲拉丁字母	|		0530-058F				|			亚美尼亚文
		————————————————————————————————————————————————————————————————————————————
			0180-01FF		|	扩充拉丁字母	|		0590-05FF				|			希伯来文
		————————————————————————————————————————————————————————————————————————————
			0250-02AF		|	标准音标		|		0600-06FF				|			阿拉伯文
		————————————————————————————————————————————————————————————————————————————
			02B0-02FF		|	进格修饰字母	|		0900-097F				|			梵文字母
		————————————————————————————————————————————————————————————————————————————
* WinNT.h定义了以下类型和宏：
*     #ifdef UNICODE
*         typedef WCHAR TCHAR, *PTCHAR, PTSTR;
*         typedef CONST WCHAR *PCTSTR;
*         #define __TEXT(quote) L##quote
*     #else
*         typedef CHAR TCHAR, *PTCHAR, PTSTR;
*         typedef CONST CHAR, *PCTSTR;
*         #define __TEXT(quote) quote
*     #endif
*     #define TEXT(quote) __TEXT(quote)
*     利用这些类型和宏（少数不太常用的没有在这里列出）来写代码，无论使用ANSI还是UNICODE字符，它都能通过编译。
* 强烈建议开发人员在开发应用程序的时候使用Unicode字符和字符串。具体理由如下：
*     1）Unicode有利于应用程序的本地化。
*     2）使用Unicode，只需发布一个二进制（.exe或DLL）文件，即可支持所有语言。
*     3）Unicode提升了应用程序的效率，因为代码执行速度更快，占用内存更少。Windows内部的一切工作都是使用Unicode字符和Unicode字符串来进行的。所以，假如我们坚持
*       传入ANSI字符或字符串，Windows就会被迫分配内存，并将ANSI字符或字符串转换为等价的Unicode形式。
*     4）使用Unicode，应用程序能轻松调用所有尚未弃用（nondeprecated）的Windows函数，因为一些Windows函数提供的版本只能处理Unicode字符和字符串。
*     5）使用Unicode，应用程序的代码和容易与COM集成（后者要求使用Unicode字符和Unicode字符串）。
*     6）使用Unicode，应用程序的代码很容易与.NET Framework集成（后者要求使用Unicode字符和Unicode字符串）。
*     7）使用Unicode，能保证应用程序的代码能够轻松操纵我们自己的资源（其中的字符串总是Unicode形式的）。
*/

#include <string>
#include <Windows.h>

namespace HY_Windows_Utils_StringUtils
{
	class CHYStringUtils
	{
		// 格式化为KB类型的字符串
		static std::wstring HY_StrFormatKBSize(
			LONGLONG       qdw,
			PSTR           pszBuf,
			UINT           cchBuf);

		// 格式化为Byte类型的字符串
		static std::wstring HY_StrFormatByteSize(
			LONGLONG       qdw,
			PSTR           pszBuf,
			UINT           cchBuf);

		// 进行字符串比较
		static int HY_CompareString(
			LCID locale,
			DWORD dwCmdFlags,
			PCTSTR pString1,
			int cch1,
			PCTSTR pString2,
			int cch2);

		// ANSI字符转Unicode字符
		static std::wstring StringToUnicode(std::string sSrc);

		// Unicode字符转ANSI字符
		static std::string UnicodeToString(std::wstring sSrc);

		// 判断是否为Unicode字符
		static BOOL HY_IsTextUnicode(CONST PVOID pvBuffer, int cb, PINT pResult);
	};
}


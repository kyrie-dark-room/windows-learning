#pragma once
/*
* 知识：
* 1）对EnterCriticalSection的调用最终会超时并引发异常。导致超时的时间长度由下面这个注册表子项中包含的CriticalSectionTimeout值决定。
*		HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager
*    这个值以秒为单位，它的默认值为2592000秒，也就是大约30天。
*/

#include <Windows.h>

namespace HY_KERNELOBJECT
{
	class CHYThreadMutex
	{
	public:
		CHYThreadMutex();
		virtual ~CHYThreadMutex();

		void Lock();
		BOOL TryLock();
		void Unlock();

	private:
		CRITICAL_SECTION m_csThreadLocker;
	};
}



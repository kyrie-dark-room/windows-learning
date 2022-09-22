#pragma once
/*
* ֪ʶ��
* 1����EnterCriticalSection�ĵ������ջᳬʱ�������쳣�����³�ʱ��ʱ�䳤�����������ע��������а�����CriticalSectionTimeoutֵ������
*		HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager
*    ���ֵ����Ϊ��λ������Ĭ��ֵΪ2592000�룬Ҳ���Ǵ�Լ30�졣
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



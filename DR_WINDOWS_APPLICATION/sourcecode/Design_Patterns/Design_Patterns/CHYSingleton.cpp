#include "CHYSingleton.h"
#include "CHYThreadLocker.h"

Singleton* Singleton::Instantialize()
{
	if (m_pInstance == NULL)
	{
		CHYThreadLockerEx ThreadLocker(m_Section);
		if (m_pInstance == NULL)
		{
			m_pInstance = new Singleton();
		}
	}

	return m_pInstance;
}
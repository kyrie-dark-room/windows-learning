#pragma once
#include "CHYKernelObject.h"

namespace HY_KERNELOBJECT
{
	class CHYCreatePrivateNamespace : public CHYKernelObject
	{
	public:
		CHYCreatePrivateNamespace();
		virtual ~CHYCreatePrivateNamespace();
		virtual BOOL CreatePrivateNamespace();

	private:
		HANDLE m_hBoundary;
		PCTSTR m_szBoundary;
		PCTSTR m_szNamespace;
		HANDLE m_hNamespace;
		BOOL m_bNamespaceOpened;
	};
}


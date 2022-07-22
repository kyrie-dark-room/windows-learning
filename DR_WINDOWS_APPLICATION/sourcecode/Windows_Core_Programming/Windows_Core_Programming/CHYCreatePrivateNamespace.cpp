#include "CHYCreatePrivateNamespace.h"
#include <sddl.h>

namespace HY_KERNELOBJECT
{
	CHYCreatePrivateNamespace::CHYCreatePrivateNamespace():
		m_hBoundary(NULL),
		m_szBoundary(TEXT("3-Boundary")),
		m_szNamespace(TEXT("3-Namespace")),
		m_hNamespace(NULL),
		m_bNamespaceOpened(FALSE)
	{
		BOOL bRet = CreatePrivateNamespace();
	}

	CHYCreatePrivateNamespace::~CHYCreatePrivateNamespace()
	{
		if (m_hNamespace != NULL)
		{
			if (m_bNamespaceOpened)
				ClosePrivateNamespace(m_hNamespace, 0);
			else
				ClosePrivateNamespace(m_hNamespace, PRIVATE_NAMESPACE_FLAG_DESTROY);
		}

		if (m_hBoundary != NULL)
			DeleteBoundaryDescriptor(m_hBoundary);
	}

	BOOL CHYCreatePrivateNamespace::CreatePrivateNamespace()
	{
		// Create the boundary descrptor
		m_hBoundary = ::CreateBoundaryDescriptor(m_szBoundary, 0);

		// Create a SID corresponding to the Local Administrator group
		BYTE localAdminSID[SECURITY_MAX_SID_SIZE];
		PSID pLocalAdminSID = &localAdminSID;
		DWORD cbSID = sizeof(localAdminSID);
		if (!::CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, pLocalAdminSID, &cbSID))
		{
			return FALSE;
		}

		// Associate the Local admin SID to the boundary descriptor
		// -- > only applications running under an administrator user
		// will be able to access the kernel objects in the same namespace
		if (!::AddSIDToBoundaryDescriptor(&m_hBoundary, pLocalAdminSID))
		{
			return FALSE;
		}

		// Create the namespace for Local Administrators only
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.bInheritHandle = FALSE;
		if (!::ConvertStringSecurityDescriptorToSecurityDescriptor(TEXT("D:(A;;GA;;;BA)"), SDDL_REVISION_1, &sa.lpSecurityDescriptor, NULL))
		{
			return FALSE;
		}

		m_hNamespace = ::CreatePrivateNamespace(&sa, m_hBoundary, m_szNamespace);

		// Don't forget to release memory for the security descriptor
		LocalFree(sa.lpSecurityDescriptor);

		// Check the private namespace creation result
		DWORD dwLastError = GetLastError();
		if (m_hNamespace == NULL)
		{
			// Nothing to do if access is denied
			// -->this code must run under a Local Administrator account
			if (dwLastError == ERROR_ACCESS_DENIED)
			{
				return FALSE; // È¨ÏÞ²»¹»
			}
			else
			{
				if (dwLastError == ERROR_ALREADY_EXISTS)
				{
					// If another instance has already created the namespace,
					// we need to open it instead.
					m_hNamespace = ::OpenPrivateNamespace(m_hBoundary, m_szNamespace);
					if (m_hNamespace == NULL)
					{
						return FALSE;
					}
					else
					{
						m_bNamespaceOpened = TRUE;
					}
				}
				else
				{
					return FALSE;
				}
			}
		}
		return TRUE;
	}
}
#include "CHYSEG.h"

namespace HY_SEG
{
	CHYSEG::CHYSEG(void)
	{

	}

	CHYSEG::~CHYSEG(void)
	{

	}

	VOID CHYSEG::RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, CONST ULONG_PTR* pArguments)
	{
		return ::RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, pArguments);
	}
}
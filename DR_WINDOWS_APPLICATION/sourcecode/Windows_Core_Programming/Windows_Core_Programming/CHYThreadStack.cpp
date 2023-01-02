#include "CHYThreadStack.h"
#include <malloc.h>

namespace HY_MEMORYOBJECT
{
	CHYThreadStack::CHYThreadStack()
	{

	}

	CHYThreadStack::~CHYThreadStack()
	{

	}

	BOOL CHYThreadStack::SetThreadStackGuarantee(PULONG StackSizeInBytes)
	{
		return ::SetThreadStackGuarantee(StackSizeInBytes);
	}

	int CHYThreadStack::ResetStkOFlw()
	{
		return _resetstkoflw();
	}
}
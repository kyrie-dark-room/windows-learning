#pragma once
#include "CHYMemoryObject.h"

/*
* ֪ʶ�㣺
* 1��
*/

namespace HY_MEMORYOBJECT
{
	class CHYHeap : public CHYMemoryObject
	{
	public:
		CHYHeap(void);
		~CHYHeap(void);

	private:
		CHYHeap(const CHYHeap&) = delete;
		CHYHeap& operator=(const CHYHeap&) = delete;
		CHYHeap(CHYHeap&&) = delete;
		CHYHeap& operator=(CHYHeap&&) = delete;
	};
}


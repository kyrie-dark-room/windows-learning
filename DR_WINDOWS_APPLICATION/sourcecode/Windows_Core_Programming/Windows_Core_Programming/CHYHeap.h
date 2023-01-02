#pragma once
#include "CHYMemoryObject.h"

/*
* ÖªÊ¶µã£º
* 1£©
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


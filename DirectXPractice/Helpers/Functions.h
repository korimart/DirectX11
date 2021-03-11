#pragma once
#include <queue>

namespace UniChili
{
	template<typename From>
	void popQueueToSize(std::queue<From>& queue, int maxSize) noexcept
	{
		while (queue.size() >= maxSize) queue.pop();
	}
}

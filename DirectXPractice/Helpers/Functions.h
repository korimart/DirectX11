#pragma once
#include <queue>

namespace UniChili
{
	template<typename T>
	void popQueueToSize(std::queue<T>& queue, int maxSize) noexcept
	{
		while (queue.size() >= maxSize) queue.pop();
	}
}

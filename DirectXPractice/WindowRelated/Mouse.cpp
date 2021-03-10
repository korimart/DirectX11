#include "Mouse.h"

using namespace UniChili;

std::optional<Mouse::Event> Mouse::popQueue() noexcept
{
	if (!eventQueue.empty())
	{
		Mouse::Event ev = eventQueue.front(); eventQueue.pop();
		return ev;
	}

	return std::optional<Event>();
}

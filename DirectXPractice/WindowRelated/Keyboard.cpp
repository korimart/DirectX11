#include "Keyboard.h"
#include "../Helpers/Functions.h"

using namespace UniChili;

bool UniChili::Keyboard::isDown(unsigned char keycode) const noexcept
{
	return keystates[keycode];
}

std::optional<Keyboard::Event> UniChili::Keyboard::popKeyQueue() noexcept
{
	if (!keyQueue.empty())
	{
		Keyboard::Event ev = keyQueue.front(); keyQueue.pop();
		return ev;
	}

	return std::optional<Event>();
}

bool UniChili::Keyboard::isKeyQueueEmpty() const noexcept
{
	return keyQueue.empty();
}

void UniChili::Keyboard::clearKeyQueue() noexcept
{
	keyQueue = {};
}

std::optional<char> UniChili::Keyboard::readChar() noexcept
{
	if (!charQueue.empty())
	{
		char ch = charQueue.front(); charQueue.pop();
		return ch;
	}

	return {};
}

bool UniChili::Keyboard::isCharQueueEmpty() const noexcept
{
	return charQueue.empty();
}

void UniChili::Keyboard::clearCharQueue() noexcept
{
	charQueue = {};
}

void UniChili::Keyboard::enableAutorepeat() noexcept
{
	autorepeat = true;
}

void UniChili::Keyboard::disableAutorepeat() noexcept
{
	autorepeat = false;
}

bool UniChili::Keyboard::isAutorepeatEnabled() const noexcept
{
	return autorepeat;
}

void UniChili::Keyboard::onKeyDown(unsigned char keycode) noexcept
{
	keystates[keycode] = true;
	keyQueue.push({ Event::Type::Pressed, keycode });
	popQueueToSize(keyQueue, queueSize);
}

void UniChili::Keyboard::onKeyUp(unsigned char keycode) noexcept
{
	keystates[keycode] = false;
	keyQueue.push({ Event::Type::Released, keycode });
	popQueueToSize(keyQueue, queueSize);
}

void UniChili::Keyboard::onChar(char character) noexcept
{
	charQueue.push(character);
	popQueueToSize(charQueue, queueSize);
}

void UniChili::Keyboard::clearState() noexcept
{
	keystates.reset();
}

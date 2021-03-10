#pragma once
#include <utility>
#include <optional>
#include <queue>
#include "../WindowRelated/ChiliWin.h"
#include "../Helpers/Functions.h"

namespace UniChili
{
	class Mouse
	{
		friend class Window;

	public:
		struct Event
		{
			enum class Type
			{
				LDown,
				LUp,
				RDown,
				RUp,
				WheelUp,
				WheelDown,
				Move,
				Enter,
				Leave,
				Other
			};
			
			Type type;
			bool leftPressed;
			bool rightPressed;
			int x;
			int y;

			Event(Type type, const Mouse& mouse) noexcept
				:
				type(type),
				leftPressed(mouse.isLeftPressed()),
				rightPressed(mouse.isRightPressed())
			{
				auto xy = mouse.getPos();
				x = xy.first;
				y = xy.second;
			}
		};

	public:
		Mouse() = default;
		Mouse(const Mouse&) = delete;
		Mouse& operator=(const Mouse&) = delete;

	public:
#pragma region interface
		std::pair<int, int> getPos() const noexcept
		{
			return { x, y };
		}

		bool isLeftPressed() const noexcept
		{
			return lDown;
		}

		bool isRightPressed() const noexcept
		{
			return rDown;
		}

		bool isInWindow() const noexcept
		{
			return inWindow;
		}
		
		/// <summary>
		/// Event Queue is like a stream; it is constantly updated by Window. 
		/// If you want to manually pop from queue then call this function.
		/// </summary>
		/// <returns></returns>
		std::optional<Mouse::Event> popQueue() noexcept;

		bool isQueueEmpty() const noexcept
		{
			return eventQueue.empty();
		}

		void clearQueue() noexcept
		{
			eventQueue = {};
		}
#pragma endregion

	private:
#pragma region callbacks
		void onMouseMove(int x, int y) noexcept
		{
			eventQueue.push({ Event::Type::Move, *this });
			update(x, y);
		}

		void onLButtonDown(int x, int y) noexcept
		{
			eventQueue.push({ Event::Type::LDown, *this });
			lDown = true;
			update(x, y);
		}

		void onLButtonUp(int x, int y) noexcept
		{
			eventQueue.push({ Event::Type::LUp, *this });
			lDown = false;
			update(x, y);
		}

		void onRButtonDown(int x, int y) noexcept
		{
			eventQueue.push({ Event::Type::RDown, *this });
			rDown = true;
			update(x, y);
		}

		void onRButtonUp(int x, int y) noexcept
		{
			eventQueue.push({ Event::Type::RUp, *this });
			rDown = false;
			update(x, y);
		}

		void onWheelUp(int x, int y) noexcept
		{
			eventQueue.push({ Event::Type::WheelUp, *this });
			update(x, y);
		}

		void onWheelDown(int x, int y) noexcept
		{
			eventQueue.push({ Event::Type::WheelDown, *this });
			update(x, y);
		}

		void onMouseEnter(int x, int y) noexcept
		{
			eventQueue.push({ Event::Type::Enter, *this });
			inWindow = true;
			update(x, y);
		}

		void onMouseLeave(int x, int y) noexcept
		{
			eventQueue.push({ Event::Type::Leave, *this });
			inWindow = false;
			update(x, y);
		}

		void onWheelDelta(int x, int y, int delta)
		{
			// Wheel delta (one mouse wheel) is usually 120 but this value
			// is device-dependent and can be less than or greater than 120.
			// The standard way of handling one mouse wheel is to do it 
			// when the delta reaches 120. Or you can implement something like
			// 0.5 mouse wheel for deltas less than 120.
			// Here we have a bucket of volume 120 for the standard way.
			wheelDeltaBucket += delta;

			while (wheelDeltaBucket > WHEEL_DELTA)
			{
				wheelDeltaBucket -= WHEEL_DELTA;
				onWheelUp(x, y);
			}

			while (wheelDeltaBucket < -WHEEL_DELTA)
			{
				wheelDeltaBucket += WHEEL_DELTA;
				onWheelDown(x, y);
			}
		}

		void update(int x, int y) noexcept
		{
			this->x = x;
			this->y = y;
			popQueueToSize(eventQueue, queueSize);
		}
#pragma endregion

		void trimRawInputBuffer() noexcept;

	private:
		static constexpr unsigned int queueSize = 16u;

		int x = 0;
		int y = 0;
		bool lDown = false;
		bool rDown = false;
		bool inWindow = false;
		int wheelDeltaBucket = 0;
		std::queue<Event> eventQueue;
	};
}
#pragma once
#include <optional>
#include <queue>
#include <bitset>

namespace UniChili
{
	class Keyboard
	{
		// Window class will call callback functions in Keyboard class
		friend class Window;

	public:
		/// <summary>
		/// Simple struct that contains key event information
		/// </summary>
		struct Event
		{
			enum class Type
			{
				Pressed,
				Released
			};

			Type type;
			unsigned char code;
		};

	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;

	public:
		/* key event stuff --------------------------*/
		bool isDown(unsigned char keycode) const noexcept;

		/// <summary>
		/// Key queue is constantly updated by Window. If you want to read from key queue, 
		/// you should pop from the queue faster than Window removes them to make room for new events.
		/// </summary>
		std::optional<Event> popKeyQueue() noexcept;
		bool isKeyQueueEmpty() const noexcept;
		void clearKeyQueue() noexcept;

		/* char event stuff -------------------------*/
		/// <summary>
		/// See popKeyQueue()
		/// </summary>
		/// <seealso cref = "popKeyQueue" / >
		std::optional<char> readChar() noexcept;
		bool isCharQueueEmpty() const noexcept;
		void clearCharQueue() noexcept;

		/* auto repeat control ----------------------*/
		void enableAutorepeat() noexcept;
		void disableAutorepeat() noexcept;
		bool isAutorepeatEnabled() const noexcept;

	private:
		// methods below are called by Window on WM_ messages
		void onKeyDown(unsigned char keycode) noexcept;
		void onKeyUp(unsigned char keycode) noexcept;
		void onChar(char character) noexcept;
		void clearState() noexcept;

	private:
		static constexpr unsigned int nKeys = 256u;
		static constexpr unsigned int queueSize = 16u;

		bool autorepeat = false;
		std::bitset<nKeys> keystates;
		std::queue<Event> keyQueue;
		std::queue<char> charQueue;
	};
}
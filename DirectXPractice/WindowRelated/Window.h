#pragma once
#include <memory>
#include "ChiliWin.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "../Exceptions/WindowsException.h"
#include "../DirectXRelated/Graphics.h"

namespace UniChili
{
	class Window
	{
	private:
		/// <summary>
		/// Singleton class that represents a single Window Class.
		/// Refactor this out to register and use more Window Classes.
		/// </summary>
		class KoriWindow
		{
		public:
			static const wchar_t* getName() noexcept;
			static HINSTANCE getInstance() noexcept;

		private:
			KoriWindow() noexcept;
			~KoriWindow() noexcept;
			KoriWindow(const KoriWindow&) = delete;
			KoriWindow& operator=(const KoriWindow&) = delete;

		private:
			static constexpr const wchar_t* wndClassName = L"KoriWindow";
			// singleton
			static KoriWindow wndClass;
			// some global variable that we'd like to keep
			static HINSTANCE hInst;
		};

	public:
		Window(int width, int height, const wchar_t* name);
		~Window() noexcept;
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

#pragma region interface
	public:
		/// <summary>
		/// Processes the window message. Optionally returns an exit code.
		/// </summary>
		/// <returns>Exit Code</returns>
		static std::optional<int> processWindowMessage();

		/// <summary>
		/// Sets the window text. Throws a window exception if it fails.
		/// </summary>
		/// <param name="text">The text.</param>
		void setWindowText(const std::wstring& text)
		{
			if (!SetWindowText(hWnd, text.c_str()))
				throw MakeExceptionFromLastError();
		}

		Keyboard& keyboard()
		{
			return _keyboard;
		}

		Mouse& mouse()
		{
			return _mouse;
		}

		std::optional<Graphics>& graphics()
		{
			return _graphics;
		}
#pragma endregion

	private:
		static LRESULT CALLBACK wndProcInit(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK wndProcRelay(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	
	private:
		LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	
	private:
		int width;
		int height;
		HWND hWnd;
		Keyboard _keyboard;
		Mouse _mouse;
		std::optional<Graphics> _graphics;
	};
}
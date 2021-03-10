#include "Window.h"
#include "../Imgui/imgui_impl_win32.h"

using namespace UniChili;

const wchar_t* Window::KoriWindow::getName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::KoriWindow::getInstance() noexcept
{
    return hInst;
}

Window::KoriWindow::KoriWindow() noexcept
{
    hInst = GetModuleHandle(nullptr);

    const auto pClassName = wndClassName;
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = wndProcInit;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = pClassName;
    wc.hIconSm = nullptr;

    RegisterClassEx(&wc);
}

Window::KoriWindow::~KoriWindow() noexcept
{
    UnregisterClass(wndClassName, hInst);
}

Window::Window(int width, int height, const wchar_t* name)
    : width(width), height(height)
{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;

	if (!AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))
		throw MakeExceptionFromLastError();

    hWnd = CreateWindowEx(
        0,
        KoriWindow::getName(),
        name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, // x, y, width, height
        nullptr, nullptr, KoriWindow::getInstance(), this // very important
    );

	if (hWnd == nullptr)
		throw MakeExceptionFromLastError();

	// init Imgui
	// note: because of this, you cannot instantiate more than one Window.
	// Refactor this if you want to support more than one Window.
	ImGui_ImplWin32_Init(hWnd);

    ShowWindow(hWnd, SW_SHOWDEFAULT);

	// now that we have hwnd,
	_graphics.emplace(hWnd);
}

Window::~Window() noexcept
{
    DestroyWindow(hWnd);
	ImGui_ImplWin32_Shutdown();
}

std::optional<int> UniChili::Window::processWindowMessage()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return (int)msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

LRESULT Window::wndProcInit(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndProcRelay));
        return pWindow->wndProc(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::wndProcRelay(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWindow->wndProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		// the message has been handled by imgui.
		// but this doesn't return true when keys are handled (see below).
		return true;

    switch (msg)
    {
    case WM_CLOSE:
        // without this GetMessage while loop does not break
        PostQuitMessage(69);
        break;

    // WM_KEYDOWN doesn't have the notion of capital letters because well keydown
    // But WM_CHAR only responds to inputs that have ASCII matchings like return (new line) and spacebar
    // and capital letters. WM_CHAR message is generated by TranslateMessage from WM_KEYDOWN
    case WM_CHAR:
		keyboard().onChar((char) wParam);
        break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN: // see below
		if (ImGui::GetIO().WantCaptureKeyboard)
			break;

		// lParam bit 30 is true if prev key was down (autorepeat)
		if (!(lParam & 0x40000000) || keyboard().isAutorepeatEnabled())
			keyboard().onKeyDown((unsigned char) wParam);
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP: // WM_KEYUP is not generated when system keys such as alt are pressed
		if (ImGui::GetIO().WantCaptureKeyboard)
			break;

		keyboard().onKeyUp((unsigned char) wParam);
		break;

	// We need this so when the window loses focus, there are no key presses 
	// hanging in the window
	case WM_KILLFOCUS:
		keyboard().clearState();
		break;

	case WM_MOUSEMOVE:
	{
		const auto pt = MAKEPOINTS(lParam);
		if (0 <= pt.x && pt.x < width && 0 <= pt.y && pt.y < height)
		{
			mouse().onMouseMove(pt.x, pt.y);
			
			if (!mouse().isInWindow())
			{
				mouse().onMouseEnter(pt.x, pt.y);
				// Capture means we will receive mouse events even though
				// mouse leaves the client area of the window.
				// (Mouse events inside window are received regardless
				// of capture)
				// Need to set capture because we release capture when
				// mouse is outside the window (conditionally though)
				SetCapture(hWnd);
			}
		}
		else
		{
			if (mouse().isLeftPressed() || mouse().isRightPressed())
				mouse().onMouseMove(pt.x, pt.y);
			else
			{
				mouse().onMouseLeave(pt.x, pt.y);
				ReleaseCapture();
			}
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		const auto pt = MAKEPOINTS(lParam);
		mouse().onLButtonDown(pt.x, pt.y);
		break;
	}

	case WM_LBUTTONUP:
	{
		const auto pt = MAKEPOINTS(lParam);
		mouse().onLButtonUp(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONDOWN:
	{
		const auto pt = MAKEPOINTS(lParam);
		mouse().onRButtonDown(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONUP:
	{
		const auto pt = MAKEPOINTS(lParam);
		mouse().onRButtonUp(pt.x, pt.y);
		break;
	}

	case WM_MOUSEWHEEL:
	{
		const auto pt = MAKEPOINTS(lParam);
		auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse().onWheelDelta(pt.x, pt.y, delta);
		break;
	}
	}

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::KoriWindow Window::KoriWindow::wndClass;
HINSTANCE Window::KoriWindow::hInst;

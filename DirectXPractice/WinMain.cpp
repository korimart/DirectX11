#include <sstream>
#include "App.h"

int CALLBACK wWinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPWSTR lpCmdLine,
    _In_     int nShowCmd)
{
	try
	{
		UniChili::App app(800, 600, L"Welcome Aboard");
		return app.run();
	}
	catch (const UniChili::ChiliException& e)
	{
		MessageBoxA(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception & e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "Unknown Error Occurred", "Unknown Error", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}
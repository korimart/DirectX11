#include <sstream>
#include "WindowsException.h"

using namespace UniChili;

WindowsException::WindowsException(int line, const char* file, HRESULT hr) noexcept
	: ChiliException(line, file), hr(hr)
{
}

const char* UniChili::WindowsException::what() const noexcept
{
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "[Error Code (HRESULT)] " << getErrorCode() << std::endl
		<< "[Description] " << getErrorString() << std::endl
		<< getOriginString() << std::endl;

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* UniChili::WindowsException::getType() const noexcept
{
	return "WindowsException";
}

std::string UniChili::WindowsException::translateErrorCode(HRESULT hr) noexcept
{
	char *pMsgBuf = nullptr;

	DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf),
		0,
		nullptr);

	if (nMsgLen == 0)
		return "Unidentified Error Code";

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT UniChili::WindowsException::getErrorCode() const noexcept
{
	return hr;
}

std::string UniChili::WindowsException::getErrorString() const noexcept
{
	return translateErrorCode(hr);
}

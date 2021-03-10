#include <sstream>
#include "ChiliException.h"

using namespace UniChili;

const char* ChiliException::what() const noexcept
{
	std::ostringstream oss;
	oss << getType() << std::endl;
	oss << getOriginString() << std::endl;

	if (!msg.empty())
		oss << "[Message] " << msg << std::endl;

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

int ChiliException::getLine() const noexcept
{
	return line;
}

const std::string& ChiliException::getFile() const noexcept
{
	return file;
}

std::string ChiliException::getOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl;
	oss << "[Line] " << line;
	return oss.str();
}

const char* ChiliException::getType() const noexcept
{
	return "ChiliException";
}

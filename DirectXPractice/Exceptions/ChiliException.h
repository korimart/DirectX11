#pragma once
#include <exception>
#include <string>

namespace UniChili
{
	class ChiliException : std::exception
	{
	public:
		ChiliException(int line, const char* file, const char* msg) noexcept
			: line(line), file(file), msg(msg)
		{}

		ChiliException(int line, const char* file) noexcept
			: ChiliException(line, file, "")
		{}

		ChiliException(const ChiliException&) noexcept = delete;
		ChiliException& operator=(const ChiliException&) noexcept = delete;
	public:
		const char* what() const noexcept override;

	public:
		/// <summary>
		/// Gets the line number.
		/// </summary>
		int getLine() const noexcept;

		/// <summary>
		/// Gets the file path string.
		/// </summary>
		const std::string& getFile() const noexcept;

		/// <summary>
		/// Gets the location of exception in the file system.
		/// </summary>
		std::string getOriginString() const noexcept;

		/// <summary>
		/// Gets the type of this exception. Kinda like a reflection but returns a string.
		/// </summary>
		virtual const char* getType() const noexcept;

	private:
		int line;
		std::string file;
		std::string msg;

	protected:
		// This is needed so when we return what() the returned pointer is alive.
		mutable std::string whatBuffer;
	};
}
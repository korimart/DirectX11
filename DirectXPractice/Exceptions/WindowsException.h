#pragma once
#include "../WindowRelated/ChiliWin.h"
#include "ChiliException.h"

namespace UniChili
{
	/// <summary>
	/// Converts HRESULT error to ChiliException
	/// </summary>
	/// <seealso cref="ChiliException" />
	class WindowsException : public ChiliException
	{
	public:
		WindowsException(int line, const char* file, HRESULT hr) noexcept;

	public:
		const char* what() const noexcept override;
		const char* getType() const noexcept override;

	public:
		/// <summary>
		/// Takes an error code (HRESULT) and returns a formatted description string of the error.
		/// </summary>
		static std::string translateErrorCode(HRESULT hr) noexcept;
		
		HRESULT getErrorCode() const noexcept;

		/// <summary>
		/// Gets the description string of the error code (HRESULT) stored in this exception.
		/// </summary>
		std::string getErrorString() const noexcept;

	private:
		HRESULT hr;
	};

#define	MakeExceptionFromHr(hr) WindowsException(__LINE__, __FILE__, hr)
#define MakeExceptionFromLastError() WindowsException(__LINE__, __FILE__, GetLastError())
}
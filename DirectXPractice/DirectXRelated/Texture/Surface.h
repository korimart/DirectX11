/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Surface.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/

#pragma once
#include "../../WindowRelated/ChiliWin.h"
#include "../../Exceptions/ChiliException.h"
#include <string>
#include <assert.h>
#include <memory>

namespace UniChili
{
	/// <summary>
	/// A simple container of Colors (a Color is a 32-bit member).
	/// Surface class has convenience functions to load images as
	/// Colors using GDI+ library.
	/// </summary>
	class Surface
	{
	public:
		/// <summary>
		/// A simple structure that represents a color in 32 bits.
		/// The members are declared constexpr to make this class
		/// instantiable (if that's a word) in compile time since
		/// this class is just a 32 bit word in disguise.
		/// </summary>
		class Color
		{
		public:
			unsigned int dword;

		public:
			constexpr Color() noexcept : dword()
			{}

			constexpr Color(const Color& col) noexcept
				:
				dword(col.dword)
			{}

			constexpr Color(unsigned int dw) noexcept
				:
				dword(dw)
			{}

			constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b) noexcept
				:
				dword((x << 24u) | (r << 16u) | (g << 8u) | b)
			{}

			constexpr Color(unsigned char r, unsigned char g, unsigned char b) noexcept
				:
				dword((r << 16u) | (g << 8u) | b)
			{}

			constexpr Color(Color col, unsigned char x) noexcept
				:
				Color((x << 24u) | col.dword)
			{}

			Color& operator =(Color color) noexcept
			{
				dword = color.dword;
				return *this;
			}

			constexpr unsigned char GetX() const noexcept
			{
				return dword >> 24u;
			}

			constexpr unsigned char GetA() const noexcept
			{
				return GetX();
			}

			constexpr unsigned char GetR() const noexcept
			{
				return (dword >> 16u) & 0xFFu; }

			constexpr unsigned char GetG() const noexcept
			{
				return (dword >> 8u) & 0xFFu;
			}

			constexpr unsigned char GetB() const noexcept
			{
				return dword & 0xFFu;
			}

			void SetX(unsigned char x) noexcept
			{
				dword = (dword & 0xFFFFFFu) | (x << 24u);
			}

			void SetA(unsigned char a) noexcept
			{
				SetX(a);
			}

			void SetR(unsigned char r) noexcept
			{
				dword = (dword & 0xFF00FFFFu) | (r << 16u);
			}

			void SetG(unsigned char g) noexcept
			{
				dword = (dword & 0xFFFF00FFu) | (g << 8u);
			}

			void SetB(unsigned char b) noexcept
			{
				dword = (dword & 0xFFFFFF00u) | b;
			}
		};

	public:
		/// <summary>
		/// Creates an empty surface with the given width and the height.
		/// </summary>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		Surface(unsigned int width, unsigned int height) noexcept;

		Surface(Surface&& source) noexcept;
		Surface(Surface&) = delete;
		Surface& operator=(Surface&& donor) noexcept;
		Surface& operator=(const Surface&) = delete;
		~Surface();
public:
	/// <summary>
	/// Creates a surface from a file.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <returns></returns>
	static Surface FromFile(const std::string& name);

	public:
#pragma region interface
		void Clear(Color fillValue) noexcept;

		/// <summary>
		/// Changes a pixel to Color c.
		/// </summary>
		/// <param name="x">The x.</param>
		/// <param name="y">The y.</param>
		/// <param name="c">The c.</param>
		/// <returns></returns>
		void PutPixel(unsigned int x, unsigned int y, Color c) noexcept;

		Color GetPixel(unsigned int x, unsigned int y) const noexcept;
		unsigned int GetWidth() const noexcept;
		unsigned int GetHeight() const noexcept;
		Color* GetBufferPtr() noexcept;
		const Color* GetBufferPtr() const noexcept;
		const Color* GetBufferPtrConst() const noexcept;
		void Save(const std::string& filename) const;
		void Copy(const Surface& src) noexcept;
#pragma endregion

	private:
		Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept;

	private:
		std::unique_ptr<Color[]> pBuffer;
		unsigned int width;
		unsigned int height;
	};
}

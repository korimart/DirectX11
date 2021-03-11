#pragma once
#include "IndexedTriangleList.h"

namespace UniChili
{
	/// <summary>
	/// Factory namespace to make a mesh.
	/// Warning: Plane does not mean an airplane.
	/// </summary>
	namespace Plane
	{
		template<class From>
		IndexedTriangleList<From> makeTesselated(int divX, int divY)
		{
			namespace dx = DirectX;

			constexpr float width = 1.f;
			constexpr float height = 1.f;
			const int vertCountX = divX + 1;
			const int vertCountY = divY + 1;
			const float divSizeX = width / divX;
			const float divSizeY = height / divY;
			auto bottomLeft = dx::XMVectorSet(-width / 2, -height / 2, 0.f, 0.f);

			// start from bottom left, move right faster than up.
			// so the first vertex is left bottom and the second vertex
			// is the one on the right and so on.
			std::vector<From> vertices((size_t)vertCountX * vertCountY);
			int vertIndex = 0;
			for (int y = 0; y < vertCountY; y++)
			{
				float yVertPos = y * divSizeY;
				for (int x = 0; x < vertCountX; x++)
				{
					auto vVector = dx::XMVectorAdd(bottomLeft, dx::XMVectorSet(
						x * divSizeX, yVertPos, 0.f, 0.f
					));

					dx::XMStoreFloat3(&vertices[vertIndex].pos, vVector);
					vertIndex++;
				}
			}

			std::vector<uint16_t> indices;
			// needs 6 indices per division.
			indices.reserve((size_t)divX * divY * 6);
			for (int y = 0; y < divY; y++)
			{
				for (int x = 0; x < divX; x++)
				{
					// indices
					int bottomLeft = vertCountX * y + x;
					int topLeft = bottomLeft + vertCountX;

					// clockwise
					indices.emplace_back(bottomLeft);
					indices.emplace_back(topLeft);
					indices.emplace_back(bottomLeft + 1);
					indices.emplace_back(bottomLeft + 1);
					indices.emplace_back(topLeft);
					indices.emplace_back(topLeft + 1);
				}
			}

			return { std::move(vertices), std::move(indices) };
		}

		template<class From>
		IndexedTriangleList<From> make()
		{
			return makeTesselated<From>(1, 1);
		}
	}
}
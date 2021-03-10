#pragma once
#include "IndexedTriangleList.h"

namespace UniChili
{
	namespace Sphere
	{
		// https://github.com/planetchili/hw3d/blob/eaec95100f08d8796aafbae76b3769756c68239d/hw3d/Sphere
		template<class T>
		IndexedTriangleList<T> makeTesselated(int latDiv, int longDiv)
		{
			namespace dx = DirectX;

			constexpr float PI = 3.1415926f;

			constexpr float radius = 1.0f;
			const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
			const float lattitudeAngle = PI / latDiv;
			const float longitudeAngle = 2.0f * PI / longDiv;

			std::vector<T> vertices;
			for (int iLat = 1; iLat < latDiv; iLat++)
			{
				const auto latBase = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationX(lattitudeAngle * iLat)
				);
				for (int iLong = 0; iLong < longDiv; iLong++)
				{
					vertices.emplace_back();
					auto v = dx::XMVector3Transform(
						latBase,
						dx::XMMatrixRotationZ(longitudeAngle * iLong)
					);
					dx::XMStoreFloat3(&vertices.back().pos, v);
				}
			}

			// add the cap vertices
			const auto iNorthPole = (unsigned short)vertices.size();
			vertices.emplace_back();
			dx::XMStoreFloat3(&vertices.back().pos, base);
			const auto iSouthPole = (unsigned short)vertices.size();
			vertices.emplace_back();
			dx::XMStoreFloat3(&vertices.back().pos, dx::XMVectorNegate(base));

			const auto calcIdx = [latDiv, longDiv](unsigned short iLat, unsigned short iLong)
			{ return iLat * longDiv + iLong; };
			std::vector<unsigned short> indices;
			for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++)
			{
				for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
				{
					indices.push_back(calcIdx(iLat, iLong));
					indices.push_back(calcIdx(iLat + 1, iLong));
					indices.push_back(calcIdx(iLat, iLong + 1));
					indices.push_back(calcIdx(iLat, iLong + 1));
					indices.push_back(calcIdx(iLat + 1, iLong));
					indices.push_back(calcIdx(iLat + 1, iLong + 1));
				}
				// wrap band
				indices.push_back(calcIdx(iLat, longDiv - 1));
				indices.push_back(calcIdx(iLat + 1, longDiv - 1));
				indices.push_back(calcIdx(iLat, 0));
				indices.push_back(calcIdx(iLat, 0));
				indices.push_back(calcIdx(iLat + 1, longDiv - 1));
				indices.push_back(calcIdx(iLat + 1, 0));
			}

			// cap fans
			for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
			{
				// north
				indices.push_back(iNorthPole);
				indices.push_back(calcIdx(0, iLong));
				indices.push_back(calcIdx(0, iLong + 1));
				// south
				indices.push_back(calcIdx(latDiv - 2, iLong + 1));
				indices.push_back(calcIdx(latDiv - 2, iLong));
				indices.push_back(iSouthPole);
			}
			// wrap triangles
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, longDiv - 1));
			indices.push_back(calcIdx(0, 0));
			// south
			indices.push_back(calcIdx(latDiv - 2, 0));
			indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
			indices.push_back(iSouthPole);

			return { std::move(vertices),std::move(indices) };
		}

		template<class T>
		IndexedTriangleList<T> make()
		{
			return makeTesselated<T>(12, 24);
		}
	}
}
#pragma once
#include "IndexedTriangleList.h"

namespace UniChili
{
	/// <summary>
	/// Factory namespace to make a mesh.
	/// </summary>
	namespace Cube
	{
		// should be here because it's a templated function (duh).
		template<class T> 
		IndexedTriangleList<T> make()
		{
			namespace dx = DirectX;

			// This makes a cube whose side is 1 unit long.
			constexpr float side = 1.f / 2.f;

			std::vector<T> vertices(8);
			vertices[0].pos = { -side, -side, -side };
			vertices[1].pos = { side, -side, -side };
			vertices[2].pos = { -side, side, -side };
			vertices[3].pos = { side, side, -side };
			vertices[4].pos = { -side, -side, side };
			vertices[5].pos = { side, -side, side };
			vertices[6].pos = { -side, side, side };
			vertices[7].pos = { side, side, side };

			std::vector<uint16_t> indices =
			{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			};
			
			return { std::move(vertices), std::move(indices) };
		}

		template<class T>
		IndexedTriangleList<T> makeWithNormals()
		{
			namespace dx = DirectX;

			// This makes a cube whose side is 1 unit long.
			constexpr float side = 1.f / 2.f;

			constexpr decltype(T::pos) poses[] =
			{
				{ -side, -side, -side },
				{ side, -side, -side },
				{ -side, side, -side },
				{ side, side, -side },
				{ -side, -side, side },
				{ side, -side, side },
				{ -side, side, side },
				{ side, side, side }
			};

			std::vector<T> vertices(36);

			auto setNormal = [&vertices](int start, decltype(T::normal) normal)
			{
				for (int i = start; i < start + 6; i++)
					vertices[i].normal = normal;
			};

			// front
			vertices[0].pos = poses[0];
			vertices[1].pos = poses[2];
			vertices[2].pos = poses[1];
			vertices[3].pos = poses[1];
			vertices[4].pos = poses[2];
			vertices[5].pos = poses[3];
			setNormal(0, { 0.f, 0.f, -1.f });

			// left
			vertices[6].pos = poses[4];
			vertices[7].pos = poses[6];
			vertices[8].pos = poses[0];
			vertices[9].pos = poses[0];
			vertices[10].pos = poses[6];
			vertices[11].pos = poses[2];
			setNormal(6, { -1.f, 0.f, 0.f, });

			// right
			vertices[12].pos = poses[1];
			vertices[13].pos = poses[3];
			vertices[14].pos = poses[5];
			vertices[15].pos = poses[5];
			vertices[16].pos = poses[3];
			vertices[17].pos = poses[7];
			setNormal(12, { 1.f, 0.f, 0.f, });

			// back
			vertices[18].pos = poses[5];
			vertices[19].pos = poses[7];
			vertices[20].pos = poses[4];
			vertices[21].pos = poses[4];
			vertices[22].pos = poses[7];
			vertices[23].pos = poses[6];
			setNormal(18, { 0.f, 0.f, 1.f, });

			// up
			vertices[24].pos = poses[2];
			vertices[25].pos = poses[6];
			vertices[26].pos = poses[3];
			vertices[27].pos = poses[3];
			vertices[28].pos = poses[6];
			vertices[29].pos = poses[7];
			setNormal(24, { 0.f, 1.f, 0.f, });

			// down
			vertices[30].pos = poses[4];
			vertices[31].pos = poses[0];
			vertices[32].pos = poses[5];
			vertices[33].pos = poses[5];
			vertices[34].pos = poses[0];
			vertices[35].pos = poses[1];
			setNormal(30, { 0.f, -1.f, 0.f, });

			std::vector<uint16_t> indices;
			for (uint16_t i = 0; i < 36; i++)
				indices.push_back(i);
			
			return { std::move(vertices), std::move(indices) };
		}
	}
}
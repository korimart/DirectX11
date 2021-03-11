#pragma once
#include <vector>
#include <DirectXMath.h>

namespace UniChili
{
	/// <summary>
	/// Template allows for any struct containing member 'pos'
	/// to be stored in this class as vertices.
	/// </summary>
	template<class From>
	class IndexedTriangleList
	{
	public:
		IndexedTriangleList(
			std::vector<From> vertices,
			std::vector<uint16_t> indices) noexcept
			: vertices(std::move(vertices)), indices(std::move(indices))
		{}

		IndexedTriangleList(const IndexedTriangleList&) noexcept = delete;
		IndexedTriangleList& operator=(const IndexedTriangleList&) noexcept = delete;
		IndexedTriangleList(IndexedTriangleList&&) = default;

	public:
#pragma region interface
		/// <summary>
		/// Transforms the vertices stored in this class.
		/// </summary>
		/// <param name="transform">The transform matrix</param>
		void transformVertices(DirectX::FXMMATRIX transform)
		{
			for (auto& vertex : vertices)
			{
				const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&vertex.pos);
				DirectX::XMStoreFloat3(
					&vertex.pos,
					DirectX::XMVector3Transform(pos, transform)
				);
			}
		}
#pragma endregion
	public:
		std::vector<From> vertices;
		std::vector<uint16_t> indices;
	};
}
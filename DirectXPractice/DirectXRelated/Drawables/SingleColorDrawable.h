#pragma once
#include <memory>
#include "DrawableBase.h"
#include "../Mesh/IndexedTriangleList.h"
#include "../Bindables/VertexBuffer.h"
#include "../Bindables/IndexBuffer.h"
#include "../Bindables/ConstantBuffer.h"
#include "../Bindables/InputLayout.h"
#include "../Bindables/VertexShader.h"
#include "../Bindables/PixelShader.h"
#include "../Bindables/Topology.h"

namespace UniChili
{
	/// <summary>
	/// Draw a mesh with a single color applied to all its triangles.
	/// While simple, doesn't seem like the best way to make a drawable.
	/// Or maybe it is. TODO: come back to be later.
	/// </summary>
	template<class T>
	class SingleColorDrawable : public DrawableBase<T>
	{
		using DrawableBase<T>::isStaticInit;
		using DrawableBase<T>::addStaticBindable;
		using DrawableBase<T>::setIndexFromStatic;
		using DrawableBase<T>::setStaticIndex;
		using DrawableBase<T>::addBindable;
	
	protected:
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
		};

	public:
		SingleColorDrawable() = default;
		SingleColorDrawable(const SingleColorDrawable&) noexcept = delete;
		SingleColorDrawable& operator=(const SingleColorDrawable&) noexcept = delete;
		SingleColorDrawable(SingleColorDrawable&&) = default;

	protected:
		void init(Graphics& graphics, IndexedTriangleList<Vertex> mesh,
			float r, float g, float b) noexcept
		{
			namespace dx = DirectX;

			struct ColorArrayCB
			{
				struct
				{
					float r;
					float g;
					float b;
					float a; // memory-alignment
				} faceColor;
			};

			const ColorArrayCB cbColor =
			{
				{ r, g, b, 1.0f },
			};

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{
					"Position", // semantic that we gave in VS
					0, // we can append numbers at the end of semantic. 0 if none.
					DXGI_FORMAT_R32G32B32_FLOAT, // Position is 2 32-bit floats
					0, // slot 0
					0, // offset; This is the first one so just 0 bytes
					D3D11_INPUT_PER_VERTEX_DATA, // no instancing
					0  // advanced
				}
			};

			if (!isStaticInit())
			{
				auto indexBuffer = std::make_unique<IndexBuffer>(graphics, mesh.indices);
				auto vs = std::make_unique<VertexShader>(graphics, L"SimpleVS.cso");
				addStaticBindable(std::make_unique<InputLayout>(graphics, ied, vs->getByteCode()));
				setStaticIndex(indexBuffer.get());

				addStaticBindable(std::move(vs));
				addStaticBindable(std::move(indexBuffer));
				addStaticBindable(std::make_unique<VertexBuffer>(graphics, mesh.vertices));
				addStaticBindable(std::make_unique<ConstantBufferPS<ColorArrayCB>>(graphics, &cbColor));
				addStaticBindable(std::make_unique<PixelShader>(graphics, L"SingleColorPS.cso"));
				addStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			}

			setIndexFromStatic();

			addBindable(std::make_unique<ConstantBufferTransform>(graphics, *this));
		}
	};
}
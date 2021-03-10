#pragma once
#include "DrawableBase.h"
#include "../Mesh/Plane.h"
#include "../Mesh/IndexedTriangleList.h"
#include "../Bindables/VertexBuffer.h"
#include "../Bindables/IndexBuffer.h"
#include "../Bindables/ConstantBuffer.h"
#include "../Bindables/InputLayout.h"
#include "../Bindables/VertexShader.h"
#include "../Bindables/PixelShader.h"
#include "../Bindables/Topology.h"
#include "../Bindables/Texture.h"
#include "../Bindables/Sampler.h"
#include "../Texture/Surface.h"
#include <memory>

namespace UniChili
{
	template<const char *image>
	class TexturePlane : public DrawableBase<TexturePlane<image>>
	{
		using DrawableBase<TexturePlane<image>>::isStaticInit;
		using DrawableBase<TexturePlane<image>>::addStaticBindable;
		using DrawableBase<TexturePlane<image>>::setIndexFromStatic;
		using DrawableBase<TexturePlane<image>>::setStaticIndex;
		using DrawableBase<TexturePlane<image>>::addBindable;

	public:
		TexturePlane(Graphics& graphics, float x, float y, float z)
			: x(x), y(y), z(z)
		{
			namespace dx = DirectX;

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{
					"Position", // semantic that we gave in VS
					0, // we can append numbers at the end of semantic. 0 if none.
					DXGI_FORMAT_R32G32B32_FLOAT, // Position is 3 32-bit floats
					0, // slot 0
					0, // offset; This is the first one so just 0 bytes
					D3D11_INPUT_PER_VERTEX_DATA, // no instancing
					0  // advanced
				},
				{
					"TexCoord", // semantic that we gave in VS
					0, // we can append numbers at the end of semantic. 0 if none.
					DXGI_FORMAT_R32G32_FLOAT, // Texcoord is 2 32-bit floats
					0, // slot 0
					12, // offset;
					D3D11_INPUT_PER_VERTEX_DATA, // no instancing
					0  // advanced
				}
			};

			struct Vertex
			{
				dx::XMFLOAT3 pos;
				dx::XMFLOAT2 tex;
			};

			if (!isStaticInit())
			{
				auto mesh = Plane::make<Vertex>();
				// read Plane::make to see the order of vertices.
				mesh.vertices[0].tex = { 0.0f, 1.0f };
				mesh.vertices[1].tex = { 1.0f, 1.0f };
				mesh.vertices[2].tex = { 0.0f, 0.0f };
				mesh.vertices[3].tex = { 1.0f, 0.0f };

				auto indexBuffer = std::make_unique<IndexBuffer>(graphics, mesh.indices);
				auto vs = std::make_unique<VertexShader>(graphics, L"TextureVS.cso");
				addStaticBindable(std::make_unique<InputLayout>(graphics, ied, vs->getByteCode()));
				setStaticIndex(indexBuffer.get());

				addStaticBindable(std::move(vs));
				addStaticBindable(std::move(indexBuffer));
				addStaticBindable(std::make_unique<VertexBuffer>(graphics, mesh.vertices));
				addStaticBindable(std::make_unique<PixelShader>(graphics, L"TexturePS.cso"));
				addStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

				// texture specifics
				addStaticBindable(std::make_unique<Texture>(graphics, Surface::FromFile(image)));
				addStaticBindable(std::make_unique<Sampler>(graphics));
			}

			setIndexFromStatic();

			addBindable(std::make_unique<ConstantBufferTransform>(graphics, *this));
		}

		TexturePlane(const TexturePlane&) noexcept = delete;
		TexturePlane& operator=(const TexturePlane&) noexcept = delete;
		TexturePlane(TexturePlane&&) = default;

	public:
		DirectX::XMMATRIX getTransform() const noexcept override
		{
			return DirectX::XMMatrixTranslation(x, y, z);
		}

		// nothing
		void update(float elapsed) noexcept override
		{}

	private:
		float x;
		float y;
		float z;
	};
}
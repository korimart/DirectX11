#include "Box.h"
#include "../Bindables/ConstantBuffer.h"
#include "../Bindables/VertexBuffer.h"
#include "../Bindables/IndexBuffer.h"
#include "../Bindables/ConstantBuffer.h"
#include "../Bindables/InputLayout.h"
#include "../Bindables/VertexShader.h"
#include "../Bindables/PixelShader.h"
#include "../Bindables/Topology.h"
#include "../Mesh/Cube.h"
#include "../../Shaders/ShaderPaths.h"

UniChili::Box::Box(Graphics& graphics, 
	float x, float y, float z, 
	float dYaw, float dPitch, float dRoll,
	bool useLighting)
	: 
	x(x), y(y), z(z), 
	dYaw(dYaw), dPitch(dPitch), dRoll(dRoll)
{
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
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
		},
		{
			"Normal", // semantic that we gave in VS
			0, // we can append numbers at the end of semantic. 0 if none.
			DXGI_FORMAT_R32G32B32_FLOAT,
			0, // slot 0
			12, // offset;
			D3D11_INPUT_PER_VERTEX_DATA, // no instancing
			0  // advanced
		}
	};

	auto mesh = Cube::makeWithNormals<Vertex>();

	if (!this->isStaticInit())
	{
		auto indexBuffer = std::make_unique<IndexBuffer>(graphics, mesh.indices);
		auto vs = std::make_unique<VertexShader>(graphics, L"SimpleVS.cso");
		this->addStaticBindable(std::make_unique<InputLayout>(graphics, ied, vs->getByteCode()));
		this->setStaticIndex(indexBuffer.get());

		this->addStaticBindable(std::move(vs));
		this->addStaticBindable(std::move(indexBuffer));
		this->addStaticBindable(std::make_unique<VertexBuffer>(graphics, mesh.vertices));
		this->addStaticBindable(std::make_unique<PixelShader>(graphics, L"PhongPS.cso"));
		this->addStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	this->setIndexFromStatic();

	// default is white
	Material material
	{
		{ 1.f, 1.f, 1.f }
	};

	auto materialCB = std::make_unique<ConstantBufferPS<Material>>(graphics, &material, 1u);
	pMaterialCB = materialCB.get();

	this->addBindable(std::move(materialCB));
	this->addBindable(std::make_unique<ConstantBufferTransform>(graphics, *this));
}

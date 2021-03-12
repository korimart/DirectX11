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
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "../Mesh/Vertex.h"

UniChili::Box::Box(Graphics& graphics,
	float x, float y, float z,
	float dYaw, float dPitch, float dRoll,
	bool useLighting)
	:
	x(x), y(y), z(z),
	dYaw(dYaw), dPitch(dPitch), dRoll(dRoll)
{
	if (!this->isStaticInit())
	{
		//auto mesh = Cube::makeWithNormals<Vertex>();

		// test
		Assimp::Importer importer;
		const auto model = importer.ReadFile("Models\\suzanne.obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

		// we know that there is only one mesh in this model
		const auto rawMesh = model->mMeshes[0];

		VertexLayout layout;
		layout
			.append(VertexLayout::Position3D)
			.append(VertexLayout::Normal);

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = layout.getInputElementDescs();

		VertexArray vertexArray(std::move(layout));

		for (int i = 0; i < rawMesh->mNumVertices; i++)
		{
			vertexArray.emplace_back(
				DirectX::XMFLOAT3{ rawMesh->mVertices[i].x, rawMesh->mVertices[i].y, rawMesh->mVertices[i].z },
				*reinterpret_cast<DirectX::XMFLOAT3*>(&rawMesh->mNormals[i])
			);
		}

		std::vector<uint16_t> indices;
		indices.reserve(rawMesh->mNumFaces * 3);

		for (int i = 0; i < rawMesh->mNumFaces; i++)
		{
			const auto& face = rawMesh->mFaces[i];
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		auto indexBuffer = std::make_unique<IndexBuffer>(graphics, indices);
		auto vs = std::make_unique<VertexShader>(graphics, L"SimpleVS.cso");
		this->addStaticBindable(std::make_unique<InputLayout>(graphics, ied, vs->getByteCode()));
		this->setStaticIndex(indexBuffer.get());

		this->addStaticBindable(std::move(vs));
		this->addStaticBindable(std::move(indexBuffer));
		this->addStaticBindable(std::make_unique<VertexBuffer>(graphics, vertexArray));
		this->addStaticBindable(std::make_unique<PixelShader>(graphics, L"PhongPS.cso"));
		this->addStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

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

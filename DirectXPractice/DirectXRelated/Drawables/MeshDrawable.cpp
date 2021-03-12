#include "MeshDrawable.h"

UniChili::MeshDrawable::MeshDrawable(Graphics& graphics, std::vector<std::unique_ptr<Bindable>> bindables)
{
	// This class does not represent a certain mesh data, like Box.h which represents the box mesh data.
	// Any mesh data can be bound to this class; the instances of this class do not share the same mesh.
	if (!isStaticInit())
	{
		addStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	for (auto& uniqueBindable : bindables)
	{
		if (auto indexBuffer = dynamic_cast<IndexBuffer*>(uniqueBindable.get()))
			setIndexBuffer(indexBuffer);

		addBindable(std::move(uniqueBindable));
	}

	addBindable(std::make_unique<ConstantBufferTransform>(graphics, *this));
}

UniChili::Model::Model(Graphics& graphics, const std::string& filename)
{
	Assimp::Importer importer;

	const auto model = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	for (int i = 0; i < model->mNumMeshes; i++)
	{
		meshes.push_back(createMeshFromAssimp(graphics, *model->mMeshes[i]));
	}

	rootNode = createNodeFromAssimp(*model->mRootNode);
}

std::unique_ptr<UniChili::MeshDrawable> UniChili::Model::createMeshFromAssimp(Graphics& graphics, const aiMesh& assimpMesh) noexcept
{
	VertexLayout layout;

	// assume that the mesh data have these two elements
	layout
		.append(VertexLayout::Position3D)
		.append(VertexLayout::Normal);

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = layout.getInputElementDescs();

	VertexArray vertexArray(std::move(layout));

	for (int i = 0; i < assimpMesh.mNumVertices; i++)
	{
		vertexArray.emplace_back(
			DirectX::XMFLOAT3{ assimpMesh.mVertices[i].x, assimpMesh.mVertices[i].y, assimpMesh.mVertices[i].z },
			*reinterpret_cast<DirectX::XMFLOAT3*>(&assimpMesh.mNormals[i])
		);
	}

	std::vector<uint16_t> indices;
	indices.reserve(assimpMesh.mNumFaces * 3);

	for (int i = 0; i < assimpMesh.mNumFaces; i++)
	{
		const auto& face = assimpMesh.mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	// default is white
	Material material
	{
		{ 1.f, 1.f, 1.f }
	};

	std::vector<std::unique_ptr<Bindable>> bindables;

	auto vertexBuffer = std::make_unique<VertexBuffer>(graphics, vertexArray);
	auto indexBuffer = std::make_unique<IndexBuffer>(graphics, indices);
	auto vs = std::make_unique<VertexShader>(graphics, L"SimpleVS.cso");
	auto ps = std::make_unique<PixelShader>(graphics, L"PhongPS.cso");
	auto inputLayout = std::make_unique<InputLayout>(graphics, ied, vs->getByteCode());
	auto materialCB = std::make_unique<ConstantBufferPS<Material>>(graphics, &material, 1u);

	bindables.push_back(std::move(vertexBuffer));
	bindables.push_back(std::move(indexBuffer));
	bindables.push_back(std::move(vs));
	bindables.push_back(std::move(ps));
	bindables.push_back(std::move(inputLayout));
	bindables.push_back(std::move(materialCB));

	auto meshDrawable = std::make_unique<MeshDrawable>(graphics, std::move(bindables));

	return std::move(meshDrawable);
}

std::unique_ptr<UniChili::TransformTreeNode> UniChili::Model::createNodeFromAssimp(const aiNode& assimpNode) const noexcept
{
	auto transform = reinterpret_cast<const DirectX::XMFLOAT4X4*>(&assimpNode.mTransformation);

	// row, column major mismatch
	const auto transformMat = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(transform));

	auto ret = std::make_unique<TransformTreeNode>(transformMat);

	for (int i = 0; i < assimpNode.mNumMeshes; i++)
	{
		int meshIndex = assimpNode.mMeshes[i];
		ret->bindMesh(meshes[meshIndex].get());
	}

	for (int i = 0; i < assimpNode.mNumChildren; i++)
	{
		ret->addChild(createNodeFromAssimp(*assimpNode.mChildren[i]));
	}

	return std::move(ret);
}

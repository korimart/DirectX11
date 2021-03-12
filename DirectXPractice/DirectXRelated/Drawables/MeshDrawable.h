#pragma once
#include "DrawableBase.h"
#include "Material.h"
#include "../Bindables/ConstantBuffer.h"
#include "../Bindables/VertexBuffer.h"
#include "../Bindables/IndexBuffer.h"
#include "../Bindables/ConstantBuffer.h"
#include "../Bindables/InputLayout.h"
#include "../Bindables/VertexShader.h"
#include "../Bindables/PixelShader.h"
#include "../Bindables/Topology.h"
#include "../../Shaders/ShaderPaths.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace UniChili
{
	class MeshDrawable : public DrawableBase<MeshDrawable>
	{
	public:
		MeshDrawable(Graphics& graphics, std::vector<std::unique_ptr<Bindable>> bindables);

	public:
		void drawWithTransform(Graphics& graphics, DirectX::XMMATRIX transform)
		{
			setTransform(transform);
			draw(graphics);
		}
	};

	class TransformTreeNode
	{
	public:
		TransformTreeNode(DirectX::XMMATRIX relativeTransform)
		{
			DirectX::XMStoreFloat4x4(&this->relativeTransform, relativeTransform);
		}

		void addChild(std::unique_ptr<TransformTreeNode> child) noexcept
		{
			children.push_back(std::move(child));
		}

		void bindMesh(MeshDrawable* meshDrawable)
		{
			meshesBound.push_back(meshDrawable);
		}

		void callDrawDownTheTree(Graphics& graphics, DirectX::XMMATRIX transformsSoFar)
		{
			// shohuld multiply on the left
			const auto worldTransform = DirectX::XMLoadFloat4x4(&relativeTransform) * transformsSoFar;
			
			for (const auto& meshDrawable : meshesBound)
				meshDrawable->drawWithTransform(graphics, worldTransform);

			for (const auto& child : children)
				child->callDrawDownTheTree(graphics, worldTransform);
		}

	private:
		std::vector<std::unique_ptr<TransformTreeNode>> children;
		std::vector<MeshDrawable*> meshesBound;
		DirectX::XMFLOAT4X4 relativeTransform;
	};

	class Model
	{
	public:
		Model(Graphics& graphics, const std::string& filename);

		void draw(Graphics& graphics)
		{
			rootNode->callDrawDownTheTree(graphics, DirectX::XMMatrixIdentity());
		}

	private:
		std::vector<std::unique_ptr<MeshDrawable>> meshes;
		std::unique_ptr<TransformTreeNode> rootNode;

		static std::unique_ptr<MeshDrawable> createMeshFromAssimp(Graphics& graphics, const aiMesh& assimpMesh) noexcept;

		std::unique_ptr<TransformTreeNode> createNodeFromAssimp(const aiNode& assimpNode) const noexcept;
	};
}

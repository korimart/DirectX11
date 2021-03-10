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
#include "../Mesh/Cube.h"
#include "../../Shaders/ShaderPaths.h"

namespace UniChili
{
	class Box : public DrawableBase<Box>
	{
	public:
		Box(Graphics& graphics,
			float x, float y, float z,
			float dYaw, float dPitch, float dRoll,
			bool useLighting = true);
		Box(Box&&) = default;

	public:
		void setMaterial(Graphics &graphics, Material material)
		{
			pMaterialCB->rewrite(graphics, material);
		}

		void setPosition(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		DirectX::XMMATRIX getTransform() const noexcept override
		{
			return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
				* DirectX::XMMatrixTranslation(x, y, z);
		}

		void update(float elapsed) noexcept override
		{
			yaw += dYaw * elapsed;
			pitch += dPitch * elapsed;
			roll += dRoll * elapsed;
		}

	protected:
		float x;
		float y;
		float z;
		float yaw   = 0.f;
		float pitch = 0.f;
		float roll  = 0.f;

		// speeds
		float dYaw;
		float dPitch;
		float dRoll;

		ConstantBufferPS<Material>* pMaterialCB;
	};
}
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <DirectXMath.h>

namespace UniChili
{
	class RotationalCamera
	{
	public:
		RotationalCamera() = default;
		RotationalCamera(const RotationalCamera&) noexcept = delete;
		RotationalCamera& operator=(const RotationalCamera&) noexcept = delete;

	public:
		DirectX::XMMATRIX getView() const noexcept 
		{
			auto pos = DirectX::XMVector3Transform(
				DirectX::XMVectorSet(0.f, rho, 0.f, 1.0f),
				DirectX::XMMatrixRotationX(-phi)
				* DirectX::XMMatrixRotationY(theta));

			return DirectX::XMMatrixLookAtLH(
				pos, 
				DirectX::XMVectorZero(), 
				DirectX::XMVectorSet(0.f, 1.0f, 0.0f, 0.0f));
		}

	public:
		float rho = 5.f;
		float phi = M_PI_2;
		float theta = 0.f;
	};
}
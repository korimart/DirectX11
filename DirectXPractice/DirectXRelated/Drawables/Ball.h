#pragma once
#include "SingleColorDrawable.h"
#include "../Mesh/Sphere.h"

namespace UniChili
{
	class Ball : public SingleColorDrawable<Ball>
	{
	public:
		Ball(Graphics& graphics,
			float x, float y, float z)
			: x(x), y(y), z(z), angle(0.f)
		{
			SingleColorDrawable::init(graphics, Sphere::make<Vertex>(), 1.f, 0.f, 1.f);
		}

		Ball(const Ball&) noexcept = delete;
		Ball& operator=(const Ball&) noexcept = delete;
		Ball(Ball&&) = default;

	public:
		DirectX::XMMATRIX getTransform() const noexcept override
		{
			return DirectX::XMMatrixRotationY(angle) * DirectX::XMMatrixTranslation(x, y, z);
		}

		void update(float elapsed) noexcept override
		{
			angle += elapsed;
		}

	private:
		float x, y, z;
		float angle;
	};
}
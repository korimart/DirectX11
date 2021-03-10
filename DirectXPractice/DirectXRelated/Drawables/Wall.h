#pragma once
#include "SingleColorDrawable.h"
#include "../Mesh/Plane.h"

namespace UniChili
{
	class Wall : public SingleColorDrawable<Wall>
	{
	public:
		/// <summary>
		/// Creates a wall whose center pos is at (x, y, z).
		/// </summary>
		/// <param name="x">The x.</param>
		/// <param name="y">The y.</param>
		/// <param name="z">The z.</param>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		Wall(Graphics& graphics,
			float x, float y, float z,
			float width, float height,
			float r = 0.f, float g = 0.f, float b = 0.f)
			: x(x), y(y), z(z), width(width), height(height), r(r), g(g), b(b)
		{
			SingleColorDrawable::init(graphics, Plane::make<Vertex>(), r, g, b);
		}

		Wall(const Wall&) noexcept = delete;
		Wall& operator=(const Wall&) noexcept = delete;

		// needed to be put into a vector
		Wall(Wall&&) = default;

	public:
		DirectX::XMMATRIX getTransform() const noexcept override
		{
			return DirectX::XMMatrixScaling(width, height, 1.f)
				* DirectX::XMMatrixTranslation(x, y, z);
		}

		void update(float elapsed) noexcept override
		{
			// nothing.
		}

	private:
		float x;
		float y;
		float z;
		float width;
		float height;
		float r, g, b;
	};
}
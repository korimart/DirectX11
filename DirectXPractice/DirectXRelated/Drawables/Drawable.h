#pragma once
#include "../Graphics.h"
#include "../Bindables/Bindable.h"
#include "../Bindables/IndexBuffer.h"
#include <vector>
#include <memory>

namespace UniChili
{
	class Drawable
	{
		template<class From>
		friend class DrawableBase;
		
	public:
		Drawable() = default;
		Drawable(Drawable&&) = default;
		Drawable(const Drawable&) noexcept = delete;
		Drawable& operator=(const Drawable&) noexcept = delete;
		virtual ~Drawable() = default;

#pragma region interface
	public:
		virtual DirectX::XMMATRIX getTransform() const noexcept = 0;

		/// <summary>
		/// Write whatever update logic needed for this Drawable (like transform) in this method.
		/// </summary>
		/// <param name="elapsed">The time elapsed since last frame in miliseconds</param>
		/// <returns></returns>
		virtual void update(float elapsed) noexcept = 0;

	public:
		void setIndexBuffer(const IndexBuffer* indexBuffer) noexcept
		{
			pIndexBuffer = indexBuffer;
		}

		void draw(Graphics& graphics) const noexcept
		{
			for (const auto& bindable : getStaticBindables())
				bindable->bind(graphics);

			for (const auto& bindable : bindables)
				bindable->bind(graphics);

			graphics.drawIndexed(pIndexBuffer->getIndexCount());
		}

		void addBindable(std::unique_ptr<Bindable> bindable) noexcept
		{
			bindables.push_back(std::move(bindable));
		}

#pragma endregion

	private:
		virtual const std::vector<std::unique_ptr<Bindable>>& getStaticBindables() const noexcept = 0;

	private:
		const IndexBuffer* pIndexBuffer = nullptr;
		std::vector<std::unique_ptr<Bindable>> bindables;
	};
}
#include "DrawableBase.h"
#include "../Bindables/ConstantBuffer.h"
#include "../Drawables/Box.h"
#include "../../Shaders/ShaderPaths.h"

namespace UniChili
{
	class LightBulb : public Box
	{
	private:
		struct Light
		{
			alignas(16) DirectX::XMFLOAT3 viewPos;
		};

	public:
		LightBulb(Graphics& graphics)
			: 
			Box(graphics, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, false), 
			lightCB(graphics), 
			lightData{}
		{}

		LightBulb(LightBulb&&) = default;

	public:
		/// <summary>
		/// Must be called when the view matrix in the Graphics object changes.
		/// Internally sets the view space position of the light and binds it
		/// to the pipeline.
		/// </summary>
		/// <param name="graphics">The graphics.</param>
		/// <returns></returns>
		void onViewChange(Graphics& graphics) noexcept
		{
			DirectX::XMFLOAT3 worldPos{ x, y, z };

			// TODO: maybe there is a better interface than onViewChange that takes Graphics.
			auto lightViewPos = DirectX::XMVector3Transform(
				DirectX::XMLoadFloat3(&worldPos), graphics.getView());
			DirectX::XMStoreFloat3(&lightData.viewPos, lightViewPos);

			bind(graphics);
		}

	private:
		void bind(Graphics& graphics) noexcept
		{
			lightCB.rewrite(graphics, lightData);
			lightCB.bind(graphics);
		}

	private:
		Light lightData;
		ConstantBufferPS<Light> lightCB;
	};
}

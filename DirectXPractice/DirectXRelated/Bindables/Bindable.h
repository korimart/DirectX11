#pragma once
#include "../Graphics.h"

namespace UniChili
{
	class Bindable
	{
	public:
		// because derived instances need to be destroyed when
		// we are referencing them using the base class' pointer
		virtual ~Bindable() = default;

	public:
		virtual void bind(Graphics& graphics) noexcept = 0;

	protected:
		// we need the following getters so we don't need to declare
		// all derived classes of Bindable to be a friend of Graphics

		static ID3D11DeviceContext* getContext(Graphics& graphics) noexcept
		{
			return graphics.pDeviceContext.Get();
		}

		static ID3D11Device* getDevice(Graphics& graphics) noexcept
		{
			return graphics.pDevice.Get();
		}
	};
}
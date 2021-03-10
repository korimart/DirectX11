#pragma once
#include "Bindable.h"

namespace UniChili
{
	class Topology : public Bindable
	{
	public:
		Topology(D3D11_PRIMITIVE_TOPOLOGY topology) noexcept
			: topology(topology) {}

		Topology(const Topology&) noexcept = delete;
		Topology& operator=(const Topology&) noexcept = delete;

	public:
		void bind(Graphics& graphics) noexcept override
		{
			getContext(graphics)->IASetPrimitiveTopology(topology);
		}

	private:
		D3D11_PRIMITIVE_TOPOLOGY topology;
	};
}
#pragma once
#include "Bindable.h"

namespace UniChili
{
	class Sampler : public Bindable
	{
	public:
		Sampler(Graphics& graphics);
		Sampler(const Sampler&) noexcept = delete;
		Sampler& operator=(const Sampler&) noexcept = delete;

	public:
		void bind(Graphics& graphics) noexcept override;

	private:		
		/// <summary>
		/// We tell GPU how to look up data from a shader resource.
		/// </summary>
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;
	};
}

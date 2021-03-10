#include "Sampler.h"

UniChili::Sampler::Sampler(Graphics& graphics)
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	auto hr = getDevice(graphics)->CreateSamplerState(&desc, &pSamplerState);
	if (FAILED(hr)) throw MakeExceptionFromHr(hr);
}

void UniChili::Sampler::bind(Graphics& graphics) noexcept
{
	getContext(graphics)->PSSetSamplers(
		0u, // same as Texture.cpp
		1u, // same as Texture.cpp
		pSamplerState.GetAddressOf());
}

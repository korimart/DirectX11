#pragma comment(lib, "D3DCompiler.lib") // for reading .cso file (shader)
#pragma once

#include "Bindable.h"

namespace UniChili
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(Graphics& graphics, const wchar_t* fileName);
		PixelShader(const PixelShader&) = delete;
		PixelShader& operator=(const PixelShader&) = delete;

	public:
		void bind(Graphics& graphics) noexcept override;

	private:
		ComPtr<ID3D11PixelShader> pPixelShader;
	};
}
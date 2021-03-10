#pragma comment(lib, "D3DCompiler.lib") // for reading .cso file (shader)

#include "PixelShader.h"
#include <d3dcompiler.h>

UniChili::PixelShader::PixelShader(Graphics& graphics, const wchar_t fileName[])
{
	ComPtr<ID3DBlob> pBlob;
	HRESULT hr = D3DReadFileToBlob(fileName, &pBlob);
	if (FAILED(hr)) throw MakeExceptionFromHr(hr);

	hr = getDevice(graphics)->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr, // advanced
		pPixelShader.GetAddressOf());

	if (FAILED(hr)) throw MakeExceptionFromHr(hr);
}

void UniChili::PixelShader::bind(Graphics& graphics) noexcept
{
	getContext(graphics)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}

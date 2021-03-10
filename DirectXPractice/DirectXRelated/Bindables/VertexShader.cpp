#pragma comment(lib, "D3DCompiler.lib") // for reading .cso file (shader)

#include "VertexShader.h"
#include <d3dcompiler.h>

UniChili::VertexShader::VertexShader(Graphics& graphics, const wchar_t* fileName)
{
	HRESULT hr = D3DReadFileToBlob(fileName, &pByteCode);
	if (FAILED(hr)) throw MakeExceptionFromHr(hr);

	hr = getDevice(graphics)->CreateVertexShader(
		pByteCode->GetBufferPointer(),
		pByteCode->GetBufferSize(),
		nullptr, // advanced
		pVertexShader.GetAddressOf());
	if (FAILED(hr)) throw MakeExceptionFromHr(hr);
}

void UniChili::VertexShader::bind(Graphics& graphics) noexcept
{
	getContext(graphics)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

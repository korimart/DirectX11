#include "InputLayout.h"

UniChili::InputLayout::InputLayout(Graphics& graphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShader)
{
	HRESULT hr = getDevice(graphics)->CreateInputLayout(
		layout.data(),
		(UINT)layout.size(),
		pVertexShader->GetBufferPointer(), // need shader to validate semantic
		pVertexShader->GetBufferSize(),	   // need shader to validate semantic
		&pInputLayout);
	if (FAILED(hr)) throw MakeExceptionFromHr(hr);
}

void UniChili::InputLayout::bind(Graphics& graphics) noexcept
{
	getContext(graphics)->IASetInputLayout(pInputLayout.Get());
}

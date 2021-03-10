#include "IndexBuffer.h"

UniChili::IndexBuffer::IndexBuffer(Graphics& graphics, const std::vector<uint16_t>& indices)
	: count(indices.size())
{
	D3D11_BUFFER_DESC bdi = {};
	bdi.BindFlags = D3D11_BIND_INDEX_BUFFER; // this is a index buffer
	bdi.Usage = D3D11_USAGE_DEFAULT; // can specify RW access
	bdi.CPUAccessFlags = 0u; // CPU cannot access this buffer
	bdi.MiscFlags = 0u; // miscellaneous flags that we don't need today
	bdi.ByteWidth = (UINT)(sizeof(uint16_t) * indices.size());
	bdi.StructureByteStride = sizeof(uint16_t);

	D3D11_SUBRESOURCE_DATA srdi = {};
	srdi.pSysMem = indices.data(); // other members are advanced

	HRESULT hr = getDevice(graphics)->CreateBuffer(&bdi, &srdi, &pIndexBuffer);
	if (FAILED(hr)) throw MakeExceptionFromHr(hr);
}

void UniChili::IndexBuffer::bind(Graphics& graphics) noexcept
{
	getContext(graphics)->IASetIndexBuffer(
		pIndexBuffer.Get(),
		DXGI_FORMAT_R16_UINT,
		0);
}

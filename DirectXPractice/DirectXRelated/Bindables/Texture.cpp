#include "Texture.h"
#include "../Texture/Surface.h"

namespace wrl = Microsoft::WRL;

UniChili::Texture::Texture(Graphics& graphics, const Surface& s)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = s.GetWidth();
	desc.Height = s.GetHeight();
	desc.MipLevels = 1; // advanced
	desc.ArraySize = 1; // advanced
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // same as our back buffer. Gotta be BGRA not RGBA because we use GDI+ to load images.
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // This is a shader resource.
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = s.GetBufferPtr();
	data.SysMemPitch = s.GetWidth() * sizeof(Surface::Color); // This is the size of a row.

	wrl::ComPtr<ID3D11Texture2D> pTexture;
	auto hr = getDevice(graphics)->CreateTexture2D(&desc, &data, &pTexture);
	if (FAILED(hr)) throw MakeExceptionFromHr(hr);

	// we don't bind textures themselves but views (we have to let GPU know
	// that it is a shader resource.)
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format; // same format (duh)
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1; // no mip mapping

	hr = getDevice(graphics)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView);
	if (FAILED(hr)) throw MakeExceptionFromHr(hr);
}

void UniChili::Texture::bind(Graphics& graphics) noexcept
{
	getContext(graphics)->PSSetShaderResources(
		0u, // we can specify the slot
		1u, // bind only one shader resource
		pTextureView.GetAddressOf());
} 

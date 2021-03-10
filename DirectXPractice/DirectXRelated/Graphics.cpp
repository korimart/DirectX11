#pragma warning( disable : 26812 )
#pragma comment(lib, "d3d11.lib")

#include "Graphics.h"
#include "../Imgui/imgui_impl_dx11.h"

UniChili::Graphics::Graphics(HWND hWnd)
	: view()
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;  // fit window
	sd.BufferDesc.Height = 0; // fit window
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;   // fit display
	sd.BufferDesc.RefreshRate.Denominator = 0; // fit display
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 1:1
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;   // no antialiasing
	sd.SampleDesc.Quality = 0; // no antialiasing
	// we want to render to this buffer
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1; // one front one back buffer (2 in total)
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, // use default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, // no custom software driver
		createDeviceFlags,
		nullptr, // whatever feature level
		0,		 // whatever feature level
		D3D11_SDK_VERSION, // macro; my version
		&sd,
		&pSwapChain, // ComPtr releases content if it has one when & is used
		&pDevice,	 // ComPtr releases content if it has one when & is used
		nullptr, // do not store returned feature level
		&pDeviceContext);

	if (FAILED(hr)) 
		throw MakeExceptionFromHr(hr);

	// we create render target view from swap chain buffer
	ComPtr<ID3D11Resource> pBackBuffer;
	hr = pSwapChain->GetBuffer(
		0, // 0 gives us the back buffer
		__uuidof(ID3D11Resource),  // the COM interface that we want the buffer to be
		&pBackBuffer);

	if (FAILED(hr)) throw MakeExceptionFromHr(hr);

	// now we can create render target view
	hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr, // use default setting
		&pRenderTarget);

	if (FAILED(hr)) throw MakeExceptionFromHr(hr);

	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = true;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	// replace depth buffer value when new value is less than what was in there
	dsd.DepthFunc = D3D11_COMPARISON_LESS;

	ComPtr<ID3D11DepthStencilState> dsState;
	hr = pDevice->CreateDepthStencilState(&dsd, &dsState);
	if (FAILED(hr)) throw MakeExceptionFromHr(hr);

	// second parameter doesn't matter; stencil not used
	pDeviceContext->OMSetDepthStencilState(dsState.Get(), 69u);

	// now that the depth buffer is enabled, we bind the buffer
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = 800u;
	depthDesc.Height = 600u;
	depthDesc.MipLevels = 1u; // advanced
	depthDesc.ArraySize = 1u; // advanced
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count = 1u;   // advanced
	depthDesc.SampleDesc.Quality = 0u; // advanced
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ComPtr<ID3D11Texture2D> pDepthBuffer;
	// nullptr because we are going to fill it later
	hr = pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthBuffer);
	if (FAILED(hr)) throw MakeExceptionFromHr(hr);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
	depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0u; // advanced

	hr = pDevice->CreateDepthStencilView(pDepthBuffer.Get(), &depthViewDesc, &pDepthView);
	if (FAILED(hr)) throw MakeExceptionFromHr(hr);

	pDeviceContext->OMSetRenderTargets(
		1u,
		pRenderTarget.GetAddressOf(),
		pDepthView.Get());

	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0; // advanced
	vp.MaxDepth = 1; // advanced
	vp.TopLeftX = 0; // top left corner
	vp.TopLeftY = 0; // top left corner

	pDeviceContext->RSSetViewports(1u, &vp);

	// init imgui for dx11
	ImGui_ImplDX11_Init(pDevice.Get(), pDeviceContext.Get());
}

UniChili::Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
}

void UniChili::Graphics::present()
{
	// 1 because we target 1 / 1.0x of the refresh rate
	HRESULT hr = pSwapChain->Present(1, 0);
	if (FAILED(hr))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
			throw MakeExceptionFromHr(pDevice->GetDeviceRemovedReason());

		throw MakeExceptionFromHr(hr);
	}
}

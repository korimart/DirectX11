#pragma once
#include "../Exceptions/WindowsException.h"
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h> 

namespace UniChili
{
	using namespace Microsoft::WRL;

	class Graphics
	{
	public:
		friend class Bindable;

		Graphics(HWND hWnd);
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics();

	public:
#pragma region interface
		DirectX::XMMATRIX getProjection()
		{
			return proj;
		}

		DirectX::XMMATRIX getView()
		{
			return view;
		}

		void setView(DirectX::XMMATRIX view)
		{
			this->view = view;
		}

		void clearBackBuffer(float red, float green, float blue) noexcept
		{
			const float color[] = { red, green, blue, 1.0f };
			pDeviceContext->ClearRenderTargetView(pRenderTarget.Get(), color);
			pDeviceContext->ClearDepthStencilView(pDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
		}

		void drawIndexed(UINT indexCount) noexcept
		{
			pDeviceContext->DrawIndexed(indexCount, 0u, 0u);
		}

		void present();
#pragma endregion

	private:
		const DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 100.f);
		DirectX::XMMATRIX view;

		ComPtr<ID3D11Device> pDevice = nullptr;
		ComPtr<IDXGISwapChain> pSwapChain = nullptr;
		ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
		ComPtr<ID3D11RenderTargetView> pRenderTarget = nullptr;
		ComPtr<ID3D11DepthStencilView> pDepthView = nullptr;
	};
}
#pragma once
#include "Bindable.h"
#include <vector>
#include <optional>
#include "../Drawables/Drawable.h"

namespace UniChili
{
	/// <summary>
	/// T is the struct of constant buffer data
	/// </summary>
	template<typename From>
	class ConstantBuffer : public Bindable
	{
	public:
		ConstantBuffer(Graphics& graphics, unsigned slot = 0u)
			: slot(slot)
		{
			D3D11_BUFFER_DESC bdc = {};
			bdc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // this is a constant buffer
			bdc.Usage = D3D11_USAGE_DYNAMIC; // CPU is going to write to this buffer
			bdc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU should have access to this buffer
			bdc.MiscFlags = 0u; // miscellaneous flags that we don't need today
			bdc.ByteWidth = sizeof(From);
			bdc.StructureByteStride = 0u; // constant buffer is not an array like VB or IB

			HRESULT hr = getDevice(graphics)->CreateBuffer(&bdc, nullptr, &pConstantBuffer);
			if (FAILED(hr)) throw MakeExceptionFromHr(hr);
		}

		ConstantBuffer(Graphics& graphics, const From* data, unsigned slot = 0u)
			: slot(slot)
		{
			D3D11_BUFFER_DESC bdc = {};
			bdc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // this is a constant buffer
			bdc.Usage = D3D11_USAGE_DYNAMIC; // CPU is going to write to this buffer
			bdc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU should have access to this buffer
			bdc.MiscFlags = 0u; // miscellaneous flags that we don't need today
			bdc.ByteWidth = sizeof(From);
			bdc.StructureByteStride = 0u; // constant buffer is not an array like VB or IB

			D3D11_SUBRESOURCE_DATA srdc = {};
			srdc.pSysMem = data; // other members are advanced

			HRESULT hr = getDevice(graphics)->CreateBuffer(&bdc, &srdc, &pConstantBuffer);
			if (FAILED(hr)) throw MakeExceptionFromHr(hr);
		}

		ConstantBuffer(const ConstantBuffer&) = delete;
		ConstantBuffer& operator=(const ConstantBuffer&) = delete;

	public:
		void rewrite(Graphics& graphics, const From& data)
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			// lock resource
			HRESULT hr = getContext(graphics)->Map(
				pConstantBuffer.Get(),
				0u,						 // advanced
				D3D11_MAP_WRITE_DISCARD, // advanced
				0u,						 // advanced
				&msr);

			if (FAILED(hr)) throw MakeExceptionFromHr(hr);

			memcpy(msr.pData, &data, sizeof(data));
			// unlock resource
			getContext(graphics)->Unmap(pConstantBuffer.Get(), 0u);
		}

	protected:
		ComPtr<ID3D11Buffer> pConstantBuffer;
		unsigned slot;
	};

	template<typename From>
	class ConstantBufferVS : public ConstantBuffer<From>
	{
		using ConstantBuffer<From>::ConstantBuffer;
		using Bindable::getContext;
		using ConstantBuffer<From>::pConstantBuffer;

	public:
		void bind(Graphics& graphics) noexcept override
		{
			getContext(graphics)->VSSetConstantBuffers(
				this->slot,
				1u,
				pConstantBuffer.GetAddressOf());
		}
	};

	template<typename From>
	class ConstantBufferPS : public ConstantBuffer<From>
	{
		using ConstantBuffer<From>::ConstantBuffer;
		using Bindable::getContext;
		using ConstantBuffer<From>::pConstantBuffer;

	public:
		void bind(Graphics& graphics) noexcept override
		{
			getContext(graphics)->PSSetConstantBuffers(
				this->slot,
				1u,
				pConstantBuffer.GetAddressOf());
		}
	};

	class ConstantBufferTransform : public Bindable
	{
	private:
		struct Transform
		{
			DirectX::XMFLOAT4X4 worldViewProj;
			DirectX::XMFLOAT4X4 worldView;
		};

	public:
		ConstantBufferTransform(Graphics& graphics, const Drawable& drawable) noexcept
			: parent(drawable)
		{
			if (!cb.has_value())
				cb.emplace(graphics);
		}

		ConstantBufferTransform(const ConstantBufferTransform&) noexcept = delete;
		ConstantBufferTransform& operator=(const ConstantBufferTransform&) noexcept = delete;

	public:
		void bind(Graphics& graphics) noexcept override
		{
			// TODO: view and projection need to be bound
			// only once per frame. Not once per object. Refactor.
			Transform transform;

			DirectX::XMStoreFloat4x4(&transform.worldViewProj, 
				DirectX::XMMatrixTranspose(
				parent.getTransform() * graphics.getView() * graphics.getProjection()));

			DirectX::XMStoreFloat4x4(&transform.worldView,
				DirectX::XMMatrixTranspose(
				parent.getTransform() * graphics.getView()));

			cb->rewrite(graphics, transform);
			cb->bind(graphics);
		}

	private:
		static std::optional<ConstantBufferVS<Transform>> cb;

		const Drawable& parent;
	};
}

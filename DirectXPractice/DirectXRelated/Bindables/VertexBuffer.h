#pragma once
#include "Bindable.h"
#include <vector>
#include "../Mesh/Vertex.h"

namespace UniChili
{
	class VertexBuffer : public Bindable
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="VertexBuffer"/> class.
		/// </summary>
		/// <param name="graphics">The graphics.</param>
		/// <param name="vertices">The vertices of type T where T is Vertex</param>
		VertexBuffer(Graphics& graphics, const VertexArray& vertexArray)
			: stride(vertexArray.layout.getByteSize())
		{
			D3D11_BUFFER_DESC bd = {};
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // this is a vertex buffer
			bd.Usage = D3D11_USAGE_DEFAULT; // can specify RW access
			bd.CPUAccessFlags = 0u; // CPU cannot access this buffer
			bd.MiscFlags = 0u; // miscellaneous flags that we don't need today
			bd.ByteWidth = (UINT)vertexArray.buffer.size();
			bd.StructureByteStride = stride;

			D3D11_SUBRESOURCE_DATA srd = {};
			srd.pSysMem = vertexArray.buffer.data(); // other members are advanced

			HRESULT hr = getDevice(graphics)->CreateBuffer(&bd, &srd, &pVertexBuffer);
			if (FAILED(hr)) throw MakeExceptionFromHr(hr);
		}

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

	public:
		void bind(Graphics& graphics) noexcept override;

	private:
		ComPtr<ID3D11Buffer> pVertexBuffer;
		const UINT stride;
	};
}
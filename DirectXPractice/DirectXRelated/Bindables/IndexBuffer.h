#pragma once
#include "Bindable.h"
#include <vector>

namespace UniChili
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(Graphics& graphics, const std::vector<uint16_t>& indices);
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

	public:
		UINT getIndexCount() const noexcept
		{
			return count;
		}

		void bind(Graphics& graphics) noexcept override;

	private:
		ComPtr<ID3D11Buffer> pIndexBuffer;
		UINT count;
	};
}
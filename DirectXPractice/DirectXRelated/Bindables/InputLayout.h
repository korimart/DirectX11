#pragma once
#include "Bindable.h"
#include <vector>

namespace UniChili
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(Graphics& graphics, 
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
			ID3DBlob* pVertexShader);

		InputLayout(const InputLayout&) = delete;
		InputLayout& operator=(const InputLayout&) = delete;

	public:
		void bind(Graphics& graphics) noexcept override;

	private:
		ComPtr<ID3D11InputLayout> pInputLayout;
	};
}
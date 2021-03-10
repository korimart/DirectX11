#pragma comment(lib, "D3DCompiler.lib") // for reading .cso file (shader)
#pragma once

#include "Bindable.h"

namespace UniChili
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(Graphics& graphics, const wchar_t* fileName);
		VertexShader(const VertexShader&) = delete;
		VertexShader& operator=(const VertexShader&) = delete;

	public:
		void bind(Graphics& graphics) noexcept override;

	public:
		ID3DBlob* getByteCode()
		{
			return pByteCode.Get();
		}

	private:
		ComPtr<ID3D11VertexShader> pVertexShader;
		ComPtr<ID3DBlob> pByteCode;
	};
}
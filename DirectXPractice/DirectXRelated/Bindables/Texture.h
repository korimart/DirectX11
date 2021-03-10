#pragma once
#include "Bindable.h"

namespace UniChili
{
	class Texture : public Bindable
	{
	public:
		Texture(Graphics& graphics, const class Surface& s);
		Texture(const Texture&) noexcept = delete;
		Texture& operator=(const Texture&) noexcept = delete;

	public:
		void bind(Graphics& graphics) noexcept override;

	private:		
		/// <summary>
		/// Texture is loaded to GPU as a shader resource cause it's used
		/// by shaders (duh) but needs to identify itself as a texture
		/// so we make a view with the appropriate settings.
		/// </summary>
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}

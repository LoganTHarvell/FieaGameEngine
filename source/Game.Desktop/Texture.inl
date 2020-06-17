#pragma once

namespace Library
{
	inline winrt::com_ptr<ID3D11ShaderResourceView> Texture::ShaderResourceView() const
	{
		return mShaderResourceView;
	}
}
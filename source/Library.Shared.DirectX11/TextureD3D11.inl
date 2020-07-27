#pragma once

#include "TextureD3D11.h"

namespace Library
{
	inline TextureD3D11::TextureD3D11(const winrt::com_ptr<ID3D11ShaderResourceView>& shaderResourceView) :
		mShaderResourceView(shaderResourceView)
	{
	}
	
	inline winrt::com_ptr<ID3D11ShaderResourceView> TextureD3D11::ShaderResourceView() const
	{
		return mShaderResourceView;
	}

	inline Texture2DD3D11::Texture2DD3D11(const winrt::com_ptr<ID3D11ShaderResourceView>& shaderResourceView, const std::uint32_t width, const std::uint32_t height) :
		Texture2D(width, height),
		mShaderResourceView(shaderResourceView)
	{
	}

	inline winrt::com_ptr<ID3D11ShaderResourceView> Texture2DD3D11::ShaderResourceView() const
	{
		return mShaderResourceView;
	}

	inline TextureCubeD3D11::TextureCubeD3D11(const winrt::com_ptr<ID3D11ShaderResourceView>& shaderResourceView) :
		mShaderResourceView(shaderResourceView)
	{
	}

	inline winrt::com_ptr<ID3D11ShaderResourceView> TextureCubeD3D11::ShaderResourceView() const
	{
		return mShaderResourceView;
	}
}
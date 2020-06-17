#include "pch.h"
#include "Texture2DReader.h"
#include "Game.h"
#include "GameException.h"
#include "StringHelper.h"
#include "TextureHelper.h"

namespace Library
{
	Texture2DReader::Texture2DReader(Game& game) :
		ContentTypeReader(game, Texture2D::TypeIdClass())
	{
	}

	std::shared_ptr<Texture2D> Texture2DReader::_Read(const std::wstring& assetName)
	{
		winrt::com_ptr<ID3D11Resource> resource;
		winrt::com_ptr<ID3D11ShaderResourceView> shaderResourceView;
		if (StringHelper::EndsWith(assetName, L".dds"))
		{
			ThrowIfFailed(DirectX::CreateDDSTextureFromFile(mGame->Direct3DDevice(), assetName.c_str(), resource.put(), shaderResourceView.put()), "CreateDDSTextureFromFile() failed.");
		}
		else
		{
			ThrowIfFailed(DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), assetName.c_str(), resource.put(), shaderResourceView.put()), "CreateWICTextureFromFile() failed.");
		}

		const winrt::com_ptr<ID3D11Texture2D> texture = resource.as<ID3D11Texture2D>();
		Point textureSize = TextureHelper::GetTextureSize(gsl::not_null<ID3D11Texture2D*>(texture.get()));

		return std::make_shared<Texture2D>(std::move(shaderResourceView), textureSize.X, textureSize.Y);
	}
}
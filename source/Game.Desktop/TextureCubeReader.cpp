#include "pch.h"
#include "TextureCubeReader.h"
#include "Game.h"

// TODO: make rendering manager call to create
#include "TextureD3D11.h"

namespace Library
{
	TextureCubeReader::TextureCubeReader(Game& game) :
		ContentTypeReader(game, TextureCube::TypeIdClass())
	{
	}

	std::shared_ptr<TextureCube> TextureCubeReader::_Read(const std::wstring& assetName)
	{
		winrt::com_ptr<ID3D11ShaderResourceView> shaderResourceView;	
		ThrowIfFailed(DirectX::CreateDDSTextureFromFile(mGame->Direct3DDevice(), assetName.c_str(), nullptr, shaderResourceView.put()), "CreateDDSTextureFromFile() failed.");

		return std::make_shared<TextureCubeD3D11>(std::move(shaderResourceView));
	}
}

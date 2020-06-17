#include "pch.h"
#include "PixelShaderReader.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"
namespace Library
{
	PixelShaderReader::PixelShaderReader(Game& game) :
		ContentTypeReader(game, PixelShader::TypeIdClass())
	{
	}

	std::shared_ptr<PixelShader> PixelShaderReader::_Read(const std::wstring& assetName)
	{
		winrt::com_ptr<ID3D11PixelShader> pixelShader;
		Vector<char> compiledPixelShader;
		File::LoadBinary(String::ToString(assetName), compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.Size(), nullptr, pixelShader.put()), "ID3D11Device::CreatedPixelShader() failed.");
		
		return std::make_shared<PixelShader>(std::move(pixelShader));
	}

	PixelShaderWithClassLinkageReader::PixelShaderWithClassLinkageReader(Game& game, winrt::com_ptr<ID3D11ClassLinkage> classLinkage) :
		mGame(&game), mClassLinkage(classLinkage)
	{
	}

	std::shared_ptr<PixelShader> PixelShaderWithClassLinkageReader::operator()(const std::wstring& assetName) const
	{
		assert(mClassLinkage != nullptr);

		winrt::com_ptr<ID3D11PixelShader> pixelShader;
		Vector<char> compiledPixelShader;
		File::LoadBinary(String::ToString(assetName), compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.Size(), mClassLinkage.get(), pixelShader.put()), "ID3D11Device::CreatedPixelShader() failed.");

		return std::make_shared<PixelShader>(std::move(pixelShader));
	}
}
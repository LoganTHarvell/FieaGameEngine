#include "pch.h"
#include "DomainShaderReader.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"

// TODO: Remove dependency through render manager create call
#include "ShaderD3D11.h"

namespace Library
{
	DomainShaderReader::DomainShaderReader(Game& game) :
		ContentTypeReader(game, DomainShader::TypeIdClass())
	{
	}

	std::shared_ptr<DomainShader> DomainShaderReader::_Read(const std::wstring& assetName)
	{
		winrt::com_ptr<ID3D11DomainShader> hullShader;
		Vector<char> compiledDomainShader;
		File::LoadBinary(String::ToString(assetName), compiledDomainShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateDomainShader(&compiledDomainShader[0], compiledDomainShader.Size(), nullptr, hullShader.put()), "ID3D11Device::CreatedDomainShader() failed.");
		
		return std::make_shared<DomainShaderD3D11>(std::move(hullShader));
	}
}
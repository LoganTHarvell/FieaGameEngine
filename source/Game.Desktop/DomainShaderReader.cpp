#include "pch.h"
#include "DomainShaderReader.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"


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
		
		return std::make_shared<DomainShader>(std::move(hullShader));
	}
}
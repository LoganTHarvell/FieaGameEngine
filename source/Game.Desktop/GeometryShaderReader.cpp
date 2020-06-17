#include "pch.h"
#include "GeometryShaderReader.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"

namespace Library
{
	GeometryShaderReader::GeometryShaderReader(Game& game) :
		ContentTypeReader(game, GeometryShader::TypeIdClass())
	{
	}

	std::shared_ptr<GeometryShader> GeometryShaderReader::_Read(const std::wstring& assetName)
	{
		winrt::com_ptr<ID3D11GeometryShader> hullShader;
		Vector<char> compiledGeometryShader;
		File::LoadBinary(String::ToString(assetName), compiledGeometryShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateGeometryShader(&compiledGeometryShader[0], compiledGeometryShader.Size(), nullptr, hullShader.put()), "ID3D11Device::CreatedGeometryShader() failed.");
		
		return std::make_shared<GeometryShader>(std::move(hullShader));
	}
}
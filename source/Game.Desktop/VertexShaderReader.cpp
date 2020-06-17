#include "pch.h"
#include "VertexShaderReader.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"

namespace Library
{
	VertexShaderReader::VertexShaderReader(Game& game) :
		ContentTypeReader(game, VertexShader::TypeIdClass())
	{
	}

	std::shared_ptr<VertexShader> VertexShaderReader::_Read(const std::wstring& assetName)
	{
		winrt::com_ptr<ID3D11VertexShader> vertexShader;
		Vector<char> compiledVertexShader;
		File::LoadBinary(String::ToString(assetName), compiledVertexShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.Size(), nullptr, vertexShader.put()), "ID3D11Device::CreatedVertexShader() failed.");
		
		return std::shared_ptr<VertexShader>(new VertexShader(std::move(compiledVertexShader), std::move(vertexShader)));
	}
}
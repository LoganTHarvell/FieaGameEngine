#include "pch.h"
#include "ComputeShaderReader.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"

// TODO: Remove dependency through render manager create call
#include "ShaderD3D11.h"

namespace Library
{
	ComputeShaderReader::ComputeShaderReader(Game& game) :
		ContentTypeReader(game, ComputeShader::TypeIdClass())
	{
	}

	std::shared_ptr<ComputeShader> ComputeShaderReader::_Read(const std::wstring& assetName)
	{
		winrt::com_ptr<ID3D11ComputeShader> hullShader;
		Vector<char> compiledComputeShader;
		File::LoadBinary(String::ToString(assetName), compiledComputeShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateComputeShader(&compiledComputeShader[0], compiledComputeShader.Size(), nullptr, hullShader.put()), "ID3D11Device::CreatedComputeShader() failed.");
		
		return std::make_shared<ComputeShaderD3D11>(std::move(hullShader));
	}
}
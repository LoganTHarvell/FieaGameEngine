#include "pch.h"
#include "ComputeShaderReader.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"

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
		
		return std::make_shared<ComputeShader>(std::move(hullShader));
	}
}
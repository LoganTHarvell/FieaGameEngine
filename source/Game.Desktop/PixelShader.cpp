#include "pch.h"
#include "PixelShader.h"

using namespace std;
using namespace DirectX;
using namespace winrt;

namespace Library
{
	PixelShader::PixelShader(const com_ptr<ID3D11PixelShader>& pixelShader) :
		mShader(pixelShader)
	{
	}

	com_ptr<ID3D11PixelShader> PixelShader::Shader() const
	{
		return mShader;
	}
}
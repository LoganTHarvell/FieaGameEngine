#include "pch.h"
#include "HullShader.h"

using namespace std;
using namespace DirectX;
using namespace winrt;

namespace Library
{
	HullShader::HullShader(const com_ptr<ID3D11HullShader>& hullShader) :
		mShader(hullShader)
	{
	}

	com_ptr<ID3D11HullShader> HullShader::Shader() const
	{
		return mShader;
	}
}
#include "pch.h"
#include "ComputeShader.h"

using namespace std;
using namespace DirectX;
using namespace winrt;

namespace Library
{
	ComputeShader::ComputeShader(const com_ptr<ID3D11ComputeShader>& computeShader) :
		mShader(computeShader)
	{
	}

	com_ptr<ID3D11ComputeShader> ComputeShader::Shader() const
	{
		return mShader;
	}
}
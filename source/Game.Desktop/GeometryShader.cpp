#include "pch.h"
#include "GeometryShader.h"

using namespace std;
using namespace DirectX;
using namespace winrt;

namespace Library
{
	GeometryShader::GeometryShader(const com_ptr<ID3D11GeometryShader>& geometryShader) :
		mShader(geometryShader)
	{
	}

	com_ptr<ID3D11GeometryShader> GeometryShader::Shader() const
	{
		return mShader;
	}
}
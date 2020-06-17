#include "pch.h"
#include "Shader.h"
#include "GameException.h"

using namespace winrt;

namespace Library
{
	com_ptr<ID3D11ClassLinkage> Shader::CreateClassLinkage(gsl::not_null<ID3D11Device*> device)
	{
		com_ptr<ID3D11ClassLinkage> classLinkage;
		ThrowIfFailed(device->CreateClassLinkage(classLinkage.put()), "ID3D11Device::CreateClassLinkage() failed.");

		return classLinkage;
	}
}
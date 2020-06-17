#include "pch.h"
#include "TextureCube.h"

namespace Library
{
	TextureCube::TextureCube(const winrt::com_ptr<ID3D11ShaderResourceView>& shaderResourceView) :
		Texture(shaderResourceView)
	{
	}
}
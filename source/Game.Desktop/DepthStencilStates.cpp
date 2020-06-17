#include "pch.h"
#include "DepthStencilStates.h"
#include "GameException.h"

namespace Library
{
	void DepthStencilStates::Initialize(gsl::not_null<ID3D11Device*> direct3DDevice)
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = false;
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace = depthStencilDesc.FrontFace;

		ThrowIfFailed(direct3DDevice->CreateDepthStencilState(&depthStencilDesc, DefaultDepthCulling.put()), "ID3D11Device::CreateDepthStencilState() failed.");
		
		depthStencilDesc.DepthEnable = false;
		ThrowIfFailed(direct3DDevice->CreateDepthStencilState(&depthStencilDesc, NoDepthCulling.put()), "ID3D11Device::CreateDepthStencilState() failed.");
	}

	void DepthStencilStates::Shutdown()
	{
		DefaultDepthCulling = nullptr;
		NoDepthCulling = nullptr;
	}
}
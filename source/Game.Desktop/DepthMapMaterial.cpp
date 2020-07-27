#include "pch.h"
#include "DepthMapMaterial.h"
#include "Game.h"
#include "VertexDeclarations.h"
#include "Shader.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	DepthMapMaterial::DepthMapMaterial(ContentManager& contentManager, RenderingManager& renderingManager) :
		Material(contentManager, renderingManager)
	{
	}	

	uint32_t DepthMapMaterial::VertexSize() const
	{
		return sizeof(VertexPosition);
	}

	void DepthMapMaterial::Initialize()
	{
		Material::Initialize();

		auto vertexShader = mContentManager.Load<VertexShader>(L"Shaders\\DepthMapVS.cso");
		vertexShader->CreateInputLayout<VertexPosition>(mRenderingManager);
		SetShader(vertexShader);
		SetShader<PixelShader>(nullptr);
		
		mVSConstantBuffer = mRenderingManager.CreateConstantBuffer(sizeof(XMFLOAT4X4));
		assert(mVSConstantBuffer);
		AddShaderResource(ShaderStages::VS, *mVSConstantBuffer);
	}

	void DepthMapMaterial::UpdateTransform(CXMMATRIX worldLightViewProjectionMatrix)
	{
		assert(mVSConstantBuffer);
		mRenderingManager.UpdateBuffer(*mVSConstantBuffer, &worldLightViewProjectionMatrix.r, sizeof(CXMMATRIX));
	}
}
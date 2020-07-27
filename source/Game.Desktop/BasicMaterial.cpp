#include "pch.h"
#include "BasicMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "Shader.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	BasicMaterial::BasicMaterial(ContentManager& contentManager, RenderingManager& renderingManager) :
		Material(contentManager, renderingManager)
	{
	}	

	void BasicMaterial::SetSurfaceColor(const DirectX::XMFLOAT4& color)
	{
		SetSurfaceColor(reinterpret_cast<const float*>(&color));
	}

	uint32_t BasicMaterial::VertexSize() const
	{
		return sizeof(VertexPosition);
	}

	void BasicMaterial::Initialize()
	{
		Material::Initialize();

		auto vertexShader = mContentManager.Load<VertexShader>(L"Shaders\\BasicVS.cso");
		vertexShader->CreateInputLayout<VertexPosition>(mRenderingManager);
		SetShader(vertexShader);

		const auto pixelShader = mContentManager.Load<PixelShader>(L"Shaders\\BasicPS.cso");
		SetShader(pixelShader);

		mVSConstantBuffer = mRenderingManager.CreateConstantBuffer(sizeof(XMFLOAT4X4));
		mPSConstantBuffer = mRenderingManager.CreateConstantBuffer(sizeof(XMFLOAT4));

		assert(mVSConstantBuffer);
		assert(mPSConstantBuffer);

		AddShaderResource(ShaderStages::VS, *mVSConstantBuffer);
		AddShaderResource(ShaderStages::PS, *mPSConstantBuffer);

		SetSurfaceColor(Colors::White.f);
	}

	void BasicMaterial::UpdateTransform(CXMMATRIX worldViewProjectionMatrix)
	{
		assert(mVSConstantBuffer);
		mRenderingManager.UpdateBuffer(*mVSConstantBuffer, &worldViewProjectionMatrix.r, sizeof(CXMMATRIX));
	}

	void BasicMaterial::SetSurfaceColor(const float* color)
	{
		assert(mPSConstantBuffer);
		mRenderingManager.UpdateBuffer(*mPSConstantBuffer, color, sizeof(XMVECTORF32));
	}
}
#include "pch.h"

#include "SolarBodyMaterial.h"
#include "Game.h"
#include "VertexDeclarations.h"
#include "Shader.h"
#include "Texture.h"

using namespace std::string_literals;
using namespace Library;

namespace Demo
{
	SolarBodyMaterial::SolarBodyMaterial(ContentManager& contentManager, RenderingManager& renderingManager, std::shared_ptr<Texture2D> colorMap, std::shared_ptr<Texture2D> specularMap) :
		Material(contentManager, renderingManager), mColorMap(move(colorMap)), mSpecularMap(move(specularMap)), mSamplerState(renderingManager.GetSamplerState(Sampler::Type::TrilinearClamp))
	{
	}

	Sampler* SolarBodyMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void SolarBodyMaterial::SetSamplerState(const gsl::not_null<Sampler*> samplerState)
	{
		mSamplerState = samplerState;
		AddShaderResource(ShaderStages::PS, *mSamplerState);
	}

	std::shared_ptr<Texture2D> SolarBodyMaterial::ColorMap() const
	{
		return mColorMap;
	}

	void SolarBodyMaterial::SetColorMap(std::shared_ptr<Texture2D> texture)
	{
		assert(texture != nullptr);
		mColorMap = move(texture);
		ResetPixelShaderResources();
	}

	std::shared_ptr<Texture2D> SolarBodyMaterial::SpecularMap() const
	{
		return mSpecularMap;
	}

	void SolarBodyMaterial::SetSpecularMap(std::shared_ptr<Texture2D> texture)
	{
		assert(texture != nullptr);
		mSpecularMap = move(texture);
		ResetPixelShaderResources();
	}

	const DirectX::XMFLOAT4& SolarBodyMaterial::AmbientColor() const
	{
		return mPixelCBufferPerFrameData.AmbientColor;
	}

	void SolarBodyMaterial::SetAmbientColor(const DirectX::XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.AmbientColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const DirectX::XMFLOAT3& SolarBodyMaterial::LightPosition() const
	{
		return mPixelCBufferPerFrameData.LightPosition;
	}

	void SolarBodyMaterial::SetLightPosition(const DirectX::XMFLOAT3& position)
	{
		mPixelCBufferPerFrameData.LightPosition = position;
		mPixelCBufferPerFrameDataDirty = true;

		mVertexCBufferPerFrameData.LightPosition = position;
		mVertexCBufferPerFrameDataDirty = true;
	}

	const float SolarBodyMaterial::LightRadius() const
	{
		return mVertexCBufferPerFrameData.LightRadius;
	}

	void SolarBodyMaterial::SetLightRadius(float radius)
	{
		mVertexCBufferPerFrameData.LightRadius = radius;
		mVertexCBufferPerFrameDataDirty = true;
	}

	const DirectX::XMFLOAT4& SolarBodyMaterial::LightColor() const
	{
		return mPixelCBufferPerFrameData.LightColor;
	}

	void SolarBodyMaterial::SetLightColor(const DirectX::XMFLOAT4& color)
	{
		mPixelCBufferPerFrameData.LightColor = color;
		mPixelCBufferPerFrameDataDirty = true;
	}

	const DirectX::XMFLOAT3& SolarBodyMaterial::SpecularColor() const
	{
		return mPixelCBufferPerObjectData.SpecularColor;
	}

	void SolarBodyMaterial::SetSpecularColor(const DirectX::XMFLOAT3& color)
	{
		mPixelCBufferPerObjectData.SpecularColor = color;
		mPixelCBufferPerObjectDataDirty = true;
	}

	float SolarBodyMaterial::SpecularPower() const
	{
		return mPixelCBufferPerObjectData.SpecularPower;
	}

	void SolarBodyMaterial::SetSpecularPower(const float power)
	{
		mPixelCBufferPerObjectData.SpecularPower = power;
		mPixelCBufferPerObjectDataDirty = true;
	}

	uint32_t SolarBodyMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTextureNormal);
	}

	void SolarBodyMaterial::Initialize()
	{
		Material::Initialize();

		auto vertexShader = mContentManager.Load<VertexShader>(L"Shaders\\PointLightDemoVS.cso"s);
		SetShader(vertexShader);

		const auto pixelShader = mContentManager.Load<PixelShader>(L"Shaders\\PointLightDemoPS.cso");
		vertexShader->CreateInputLayout<VertexPositionTextureNormal>(mRenderingManager);
		SetShader(pixelShader);

		mVertexCBufferPerFrame = mRenderingManager.CreateConstantBuffer(sizeof(VertexCBufferPerFrame));
		mVertexCBufferPerObject = mRenderingManager.CreateConstantBuffer(sizeof(VertexCBufferPerObject));

		assert(mVertexCBufferPerFrame);
		assert(mVertexCBufferPerObject);

		AddShaderResource(ShaderStages::VS, *mVertexCBufferPerFrame);
		AddShaderResource(ShaderStages::VS, *mVertexCBufferPerObject);

		mPixelCBufferPerFrame = mRenderingManager.CreateConstantBuffer(sizeof(PixelCBufferPerFrame));
		mPixelCBufferPerObject = mRenderingManager.CreateConstantBuffer(sizeof(PixelCBufferPerObject));

		assert(mPixelCBufferPerFrame);
		assert(mPixelCBufferPerObject);

		AddShaderResource(ShaderStages::PS, *mPixelCBufferPerFrame);
		AddShaderResource(ShaderStages::PS, *mPixelCBufferPerObject);

		mRenderingManager.UpdateBuffer(*mVertexCBufferPerFrame, &mVertexCBufferPerFrame, sizeof(VertexCBufferPerFrame));
		mRenderingManager.UpdateBuffer(*mVertexCBufferPerObject, &mVertexCBufferPerObject, sizeof(VertexCBufferPerObject));
		mRenderingManager.UpdateBuffer(*mPixelCBufferPerFrame, &mPixelCBufferPerFrame, sizeof(PixelCBufferPerFrame));
		mRenderingManager.UpdateBuffer(*mPixelCBufferPerObject, &mPixelCBufferPerObject, sizeof(PixelCBufferPerObject));

		ResetPixelShaderResources();
		AddShaderResource(ShaderStages::PS, *mSamplerState);
	}

	void SolarBodyMaterial::UpdateCameraPosition(const DirectX::XMFLOAT3& position)
	{
		mPixelCBufferPerFrameData.CameraPosition = position;
		mPixelCBufferPerFrameDataDirty = true;
	}

	void SolarBodyMaterial::UpdateTransforms(DirectX::FXMMATRIX worldViewProjectionMatrix, DirectX::CXMMATRIX worldMatrix)
	{
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, worldViewProjectionMatrix);
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.World, worldMatrix);

		assert(mVertexCBufferPerObject);
		mRenderingManager.UpdateBuffer(*mVertexCBufferPerObject, &mVertexCBufferPerObjectData, sizeof(VertexCBufferPerObject));
	}

	void SolarBodyMaterial::BeginDraw()
	{
		Material::BeginDraw();

		if (mVertexCBufferPerFrameDataDirty)
		{
			assert(mVertexCBufferPerFrame);
			mRenderingManager.UpdateBuffer(*mVertexCBufferPerFrame, &mVertexCBufferPerFrameData, sizeof(VertexCBufferPerFrame));

			mVertexCBufferPerFrameDataDirty = false;
		}

		if (mPixelCBufferPerFrameDataDirty)
		{
			assert(mPixelCBufferPerFrame);
			mRenderingManager.UpdateBuffer(*mPixelCBufferPerFrame, &mPixelCBufferPerFrameData, sizeof(PixelCBufferPerFrame));

			mPixelCBufferPerFrameDataDirty = false;
		}

		if (mPixelCBufferPerObjectDataDirty)
		{
			assert(mPixelCBufferPerFrame);
			mRenderingManager.UpdateBuffer(*mPixelCBufferPerObject, &mPixelCBufferPerObjectData, sizeof(PixelCBufferPerObject));

			mPixelCBufferPerObjectDataDirty = false;
		}
	}

	void SolarBodyMaterial::ResetPixelShaderResources()
	{
		ClearShaderResources(ShaderStages::PS);

		gsl::not_null<Resource*> shaderResources[] = { mColorMap.get(), mSpecularMap.get() };
		AddShaderResources(ShaderStages::PS, shaderResources);
	}
}
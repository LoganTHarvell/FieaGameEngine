#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Material.h"

// First Party
#include "RenderingManager.h"
#include "Buffer.h"
#include "Shader.h"
#pragma endregion Includes

using namespace std::placeholders;

namespace Library
{
	Material::Material(ContentManager& contentManager, RenderingManager& renderingManager, const PrimitiveTopology topology) :
		mContentManager(contentManager), mRenderingManager(renderingManager), mTopology(topology)
	{
	}

	std::shared_ptr<Shader> Material::GetShader(const ShaderStages shaderStage)
	{
		assert(ShaderStageIsProgrammable(shaderStage));

		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		return (found ? shaderStageData->Shader : nullptr);
	}

	//ID3D11ClassInstance* Material::GetShaderClassInstance(const ShaderStages shaderStage)
	//{
	//	assert(ShaderStageIsProgrammable(shaderStage));

	//	auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
	//	return (found ? shaderStageData->ShaderClassInstance : nullptr);
	//}

	void Material::RemoveShaderResource(const ShaderStages shaderStage, Resource& shaderResource)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);

		if (found)
		{
			auto it = shaderStageData->Resources.Find(shaderResource.ResourceType());

			if (it != shaderStageData->Resources.end())
			{
				it->second.Remove(&shaderResource);
			}
		}
	}

	void Material::ClearShaderResources(const ShaderStages shaderStage, const Resource::Type type)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);

		if (found)
		{
			if (type == Resource::Type::Undefined)
			{
				shaderStageData->Resources.Clear();
			}
			else
			{
				auto it = shaderStageData->Resources.Find(type);

				if (it != shaderStageData->Resources.end())
				{
					it->second.Clear();
				}
			}
		}
	}

	void Material::Initialize()
	{	
	}

	void Material::Draw()
	{
		BeginDraw();

		if (mDrawCallback != nullptr)
		{
			mDrawCallback();
		}

		EndDraw();
	}

	void Material::Draw(Buffer& vertexBuffer, const uint32_t vertexCount, const uint32_t startVertexLocation)
	{
		BeginDraw();

		mRenderingManager.SetVertexBuffer(vertexBuffer);
		mRenderingManager.Draw(vertexCount, startVertexLocation);

		EndDraw();
	}

	void Material::DrawIndexed(Buffer& vertexBuffer, Buffer& indexBuffer, const std::uint32_t indexCount, const Format& format, const std::uint32_t startIndexLocation, const std::int32_t baseVertexLocation, const std::uint32_t indexOffset)
	{
		BeginDraw();

		mRenderingManager.SetVertexBuffer(vertexBuffer);
		mRenderingManager.SetIndexBuffer(indexBuffer, format, indexOffset);
		mRenderingManager.DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);

		EndDraw();
	}

	uint32_t Material::VertexSize() const
	{
		return 0;
	}

	void Material::BeginDraw()
	{
		mRenderingManager.SetPrimitiveTopology(mTopology);

		for (const auto& entry : mShaderStageData)
		{
			const ShaderStageData& shaderStageData = entry.second;

			assert(shaderStageData.Shader != nullptr);
			mRenderingManager.SetShader(shaderStageData.Shader.get());

			for (const auto& resourceList : shaderStageData.Resources)
			{
				// TODO: Might need 0 check?
				mRenderingManager.SetShaderResources(entry.first, resourceList.first, resourceList.second);
			}
		}

		if (mUpdateMaterialCallback != nullptr)
		{
			mUpdateMaterialCallback();
		}
	}

	void Material::EndDraw()
	{
		if (mAutoUnbindShaderResourcesEnabled)
		{
			for (const auto& entry : mShaderStageData)
			{
				const ShaderStageData& shaderStageData = entry.second;

				if (shaderStageData.Resources[Resource::Type::Texture].Size() > 0)
				{
					assert(EmptySRVs.Size() >= shaderStageData.Resources[Resource::Type::Texture].Size());
					mRenderingManager.SetShaderResources(entry.first, Resource::Type::Texture, EmptySRVs);
				}
			}
		}
	}

	std::pair<bool, Material::ShaderStageData*> Material::GetDataForShaderStage(const ShaderStages shaderStage)
	{
		const auto it = mShaderStageData.Find(shaderStage);
		bool found = (it != mShaderStageData.end());
		ShaderStageData* shaderStageData = (found ? shaderStageData = &(it->second) : nullptr);

		return std::make_pair(found, shaderStageData);
	}
}

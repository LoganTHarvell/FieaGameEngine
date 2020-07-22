#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Material.h"

// First Party
#include "RenderingManager.h"
#include "Buffer.h"
#include "VertexShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#pragma endregion Includes

using namespace std::placeholders;

namespace Library
{
	const HashMap<ShaderStages, RTTI::IdType> Material::ShaderStageTypeMap
	{
		{ ShaderStages::VS, VertexShader::TypeIdClass() },
		{ ShaderStages::HS, HullShader::TypeIdClass() },
		{ ShaderStages::DS, DomainShader::TypeIdClass() },
		{ ShaderStages::GS, GeometryShader::TypeIdClass() },
		{ ShaderStages::PS, PixelShader::TypeIdClass() },
		{ ShaderStages::CS, ComputeShader::TypeIdClass() },
	};

	const HashMap<RTTI::IdType, ShaderStages> Material::TypeShaderStageMap
	{
		{ VertexShader::TypeIdClass(), ShaderStages::VS },
		{ HullShader::TypeIdClass(), ShaderStages::HS },
		{ DomainShader::TypeIdClass(), ShaderStages::DS },
		{ GeometryShader::TypeIdClass(), ShaderStages::GS },
		{ PixelShader::TypeIdClass(), ShaderStages::PS },
		{ ComputeShader::TypeIdClass(), ShaderStages::CS },
	};

	const HashMap<ShaderStages, Material::ShaderStageCallInfo> Material::ShaderStageCalls
	{		
		{
			ShaderStages::VS,
			{
				SetVSShader,
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->VSSetConstantBuffers(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->VSSetShaderResources(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->VSSetSamplers(PH2, PH3, PH4); },
			}
		},
		{
			ShaderStages::HS,
			{
				SetHSShader,
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->HSSetConstantBuffers(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->HSSetShaderResources(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->HSSetSamplers(PH2, PH3, PH4); },
			}
		},
		{
			ShaderStages::DS,
			{
				SetDSShader,
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->DSSetConstantBuffers(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->DSSetShaderResources(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->DSSetSamplers(PH2, PH3, PH4); },
			}
		},
		{
			ShaderStages::GS,
			{
				SetGSShader,
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->GSSetConstantBuffers(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->GSSetShaderResources(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->GSSetSamplers(PH2, PH3, PH4); },
			}
		},
		{
			ShaderStages::PS,
			{
				SetPSShader,
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->PSSetConstantBuffers(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->PSSetShaderResources(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->PSSetSamplers(PH2, PH3, PH4); },
			}
		},
		{
			ShaderStages::CS,
			{
				SetCSShader,
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->CSSetConstantBuffers(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->CSSetShaderResources(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->CSSetSamplers(PH2, PH3, PH4); },
			}
		},
	};

	Vector<ID3D11ShaderResourceView*> Material::EmptySRVs;

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

	ID3D11ClassInstance* Material::GetShaderClassInstance(const ShaderStages shaderStage)
	{
		assert(ShaderStageIsProgrammable(shaderStage));

		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		return (found ? shaderStageData->ShaderClassInstance : nullptr);
	}

	void Material::RemoveConstantBuffer(const ShaderStages shaderStage, ID3D11Buffer* constantBuffer)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		if (found)
		{
			auto& constantBuffers = shaderStageData->ConstantBuffers;
			constantBuffers.Erase(std::find(constantBuffers.begin(), constantBuffers.end(), constantBuffer));
		}
	}

	void Material::ClearConstantBuffers(const ShaderStages shaderStage)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		if (found)
		{
			shaderStageData->ConstantBuffers.Clear();
		}
	}

	void Material::RemoveShaderResource(const ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		if (found)
		{
			auto& shaderResources = shaderStageData->ShaderResources;
			shaderResources.Erase(std::find(shaderResources.begin(), shaderResources.end(), shaderResource));
		}
	}

	void Material::ClearShaderResources(const ShaderStages shaderStage)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		if (found)
		{
			shaderStageData->ShaderResources.Clear();
		}
	}

	void Material::RemoveSamplerState(const ShaderStages shaderStage, ID3D11SamplerState* samplerState)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		if (found)
		{
			auto& samplerStates = shaderStageData->SamplerStates;
			samplerStates.Erase(std::find(samplerStates.begin(), samplerStates.end(), samplerState));
		}
	}

	void Material::ClearSamplerStates(const ShaderStages shaderStage)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		if (found)
		{
			shaderStageData->SamplerStates.Clear();
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

		auto* context = mRenderingManager.AssertAs<RenderingManagerD3D11>()->Context();
		
		assert(mInputLayout != nullptr);
		context->IASetInputLayout(mInputLayout.get());

		for (const auto& entry : mShaderStageData)
		{
			const ShaderStageData& shaderStageData = entry.second;
			const auto& shaderStageCallInfo = ShaderStageCalls.At(entry.first);

			shaderStageCallInfo.SetShader(*context, shaderStageData);

			if (shaderStageData.ConstantBuffers.Size() > 0)
			{
				shaderStageCallInfo.SetConstantBuffers(context, 0, gsl::narrow_cast<uint32_t>(shaderStageData.ConstantBuffers.Size()), shaderStageData.ConstantBuffers.Data());
			}

			if (shaderStageData.ShaderResources.Size() > 0)
			{
				shaderStageCallInfo.SetShaderResources(context, 0, gsl::narrow_cast<uint32_t>(shaderStageData.ShaderResources.Size()), shaderStageData.ShaderResources.Data());
			}

			if (shaderStageData.SamplerStates.Size() > 0)
			{
				shaderStageCallInfo.SetSamplers(context, 0, gsl::narrow_cast<uint32_t>(shaderStageData.SamplerStates.Size()), shaderStageData.SamplerStates.Data());
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
				const auto& shaderStageCallInfo = ShaderStageCalls.At(entry.first);

				if (shaderStageData.ShaderResources.Size() > 0)
				{
					assert(EmptySRVs.Size() >= shaderStageData.ShaderResources.Size());
					shaderStageCallInfo.SetShaderResources(static_cast<RenderingManagerD3D11&>(mRenderingManager).Context(), 0, gsl::narrow_cast<uint32_t>(shaderStageData.ShaderResources.Size()), EmptySRVs.Data());
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

	void Material::SetVSShader(ID3D11DeviceContext& context, const ShaderStageData& shaderStageData)
	{
		assert(shaderStageData.Shader != nullptr);
		const auto vertexShader = std::static_pointer_cast<VertexShader>(shaderStageData.Shader);
		context.VSSetShader(vertexShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
	}

	void Material::SetHSShader(ID3D11DeviceContext& context, const ShaderStageData& shaderStageData)
	{
		if (shaderStageData.Shader != nullptr)
		{
			const auto hullShader = std::static_pointer_cast<HullShader>(shaderStageData.Shader);
			context.HSSetShader(hullShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
		}
		else
		{
			context.HSSetShader(nullptr, nullptr, 0);
		}
	}

	void Material::SetDSShader(ID3D11DeviceContext& context, const ShaderStageData& shaderStageData)
	{
		if (shaderStageData.Shader != nullptr)
		{
			const auto domainShader = std::static_pointer_cast<DomainShader>(shaderStageData.Shader);
			context.DSSetShader(domainShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
		}
		else
		{
			context.DSSetShader(nullptr, nullptr, 0);
		}
	}

	void Material::SetGSShader(ID3D11DeviceContext& context, const ShaderStageData& shaderStageData)
	{
		if (shaderStageData.Shader != nullptr)
		{
			const auto geometryShader = std::static_pointer_cast<GeometryShader>(shaderStageData.Shader);
			context.GSSetShader(geometryShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
		}
		else
		{
			context.GSSetShader(nullptr, nullptr, 0);
		}
	}

	void Material::SetPSShader(ID3D11DeviceContext& context, const ShaderStageData& shaderStageData)
	{
		if (shaderStageData.Shader != nullptr)
		{
			const auto pixelShader = std::static_pointer_cast<PixelShader>(shaderStageData.Shader);
			context.PSSetShader(pixelShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
		}
		else
		{
			context.PSSetShader(nullptr, nullptr, 0);
		}
	}

	void Material::SetCSShader(ID3D11DeviceContext& context, const ShaderStageData& shaderStageData)
	{
		if (shaderStageData.Shader != nullptr)
		{
			const auto pixelShader = std::static_pointer_cast<ComputeShader>(shaderStageData.Shader);
			context.CSSetShader(pixelShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
		}
		else
		{
			context.CSSetShader(nullptr, nullptr, 0);
		}
	}
}

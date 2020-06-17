#include "Material.h"
#include "pch.h"
#include "Material.h"
#include "Game.h"
#include "VertexShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"

using namespace std;
using namespace std::placeholders;
using namespace gsl;
using namespace winrt;
using namespace DirectX;

namespace Library
{
	const map<ShaderStages, RTTI::IdType> Material::ShaderStageTypeMap
	{
		{ ShaderStages::VS, VertexShader::TypeIdClass() },
		{ ShaderStages::HS, HullShader::TypeIdClass() },
		{ ShaderStages::DS, DomainShader::TypeIdClass() },
		{ ShaderStages::GS, GeometryShader::TypeIdClass() },
		{ ShaderStages::PS, PixelShader::TypeIdClass() },
		{ ShaderStages::CS, ComputeShader::TypeIdClass() },
	};

	const map<RTTI::IdType, ShaderStages> Material::TypeShaderStageMap
	{
		{ VertexShader::TypeIdClass(), ShaderStages::VS },
		{ HullShader::TypeIdClass(), ShaderStages::HS },
		{ DomainShader::TypeIdClass(), ShaderStages::DS },
		{ GeometryShader::TypeIdClass(), ShaderStages::GS },
		{ PixelShader::TypeIdClass(), ShaderStages::PS },
		{ ComputeShader::TypeIdClass(), ShaderStages::CS },
	};

	const map<ShaderStages, Material::ShaderStageCallInfo> Material::ShaderStageCalls
	{		
		{
			ShaderStages::VS,
			{
				SetVSShader,
				bind(&ID3D11DeviceContext::VSSetConstantBuffers, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::VSSetShaderResources, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::VSSetSamplers, _1, _2, _3, _4),
			}
		},
		{
			ShaderStages::HS,
			{
				SetHSShader,
				bind(&ID3D11DeviceContext::HSSetConstantBuffers, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::HSSetShaderResources, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::HSSetSamplers, _1, _2, _3, _4),
			}
		},
		{
			ShaderStages::DS,
			{
				SetDSShader,
				bind(&ID3D11DeviceContext::DSSetConstantBuffers, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::DSSetShaderResources, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::DSSetSamplers, _1, _2, _3, _4),
			}
		},
		{
			ShaderStages::GS,
			{
				SetGSShader,
				bind(&ID3D11DeviceContext::GSSetConstantBuffers, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::GSSetShaderResources, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::GSSetSamplers, _1, _2, _3, _4),
			}
		},
		{
			ShaderStages::PS,
			{
				SetPSShader,
				bind(&ID3D11DeviceContext::PSSetConstantBuffers, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::PSSetShaderResources, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::PSSetSamplers, _1, _2, _3, _4),
			}
		},
		{
			ShaderStages::CS,
			{
				SetCSShader,
				bind(&ID3D11DeviceContext::CSSetConstantBuffers, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::CSSetShaderResources, _1, _2, _3, _4),
				bind(&ID3D11DeviceContext::CSSetSamplers, _1, _2, _3, _4),
			}
		},
	};

	std::vector<ID3D11ShaderResourceView*> Material::EmptySRVs;

	Material::Material(ContentManager& contentManager, RenderingManager& renderingManager, D3D_PRIMITIVE_TOPOLOGY topology) :
		mContentManager(contentManager), mRenderingManager(renderingManager), mTopology(topology)
	{
	}

	shared_ptr<Shader> Material::GetShader(ShaderStages shaderStage)
	{
		assert(ShaderStageIsProgrammable(shaderStage));

		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		return (found ? shaderStageData->Shader : nullptr);
	}

	ID3D11ClassInstance* Material::GetShaderClassInstance(ShaderStages shaderStage)
	{
		assert(ShaderStageIsProgrammable(shaderStage));

		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		return (found ? shaderStageData->ShaderClassInstance : nullptr);
	}

	void Material::RemoveConstantBuffer(ShaderStages shaderStage, ID3D11Buffer* constantBuffer)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		if (found)
		{
			auto& constantBuffers = shaderStageData->ConstantBuffers;
			constantBuffers.erase(find(constantBuffers.begin(), constantBuffers.end(), constantBuffer));
		}
	}

	void Material::ClearConstantBuffers(ShaderStages shaderStage)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		if (found)
		{
			shaderStageData->ConstantBuffers.clear();
		}
	}

	void Material::RemoveShaderResource(ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		if (found)
		{
			auto& shaderResources = shaderStageData->ShaderResources;
			shaderResources.erase(find(shaderResources.begin(), shaderResources.end(), shaderResource));
		}
	}

	void Material::ClearShaderResources(ShaderStages shaderStage)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		if (found)
		{
			shaderStageData->ShaderResources.clear();
		}
	}

	void Material::RemoveSamplerState(ShaderStages shaderStage, ID3D11SamplerState* samplerState)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		if (found)
		{
			auto& samplerStates = shaderStageData->SamplerStates;
			samplerStates.erase(find(samplerStates.begin(), samplerStates.end(), samplerState));
		}
	}

	void Material::ClearSamplerStates(ShaderStages shaderStage)
	{
		auto [found, shaderStageData] = GetDataForShaderStage(shaderStage);
		if (found)
		{
			shaderStageData->SamplerStates.clear();
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

	void Material::Draw(not_null<ID3D11Buffer*> vertexBuffer, uint32_t vertexCount, uint32_t startVertexLocation, uint32_t offset)
	{
		auto direct3DDeviceContext = mRenderingManager.GetDevice().ContextPtr;

		BeginDraw();

		const uint32_t stride = VertexSize();
		ID3D11Buffer* const vertexBuffers[]{ vertexBuffer };
		direct3DDeviceContext->IASetVertexBuffers(0, narrow_cast<uint32_t>(size(vertexBuffers)), vertexBuffers, &stride, &offset);

		direct3DDeviceContext->Draw(vertexCount, startVertexLocation);

		EndDraw();
	}

	void Material::DrawIndexed(not_null<ID3D11Buffer*> vertexBuffer, not_null<ID3D11Buffer*> indexBuffer, uint32_t indexCount, DXGI_FORMAT format, uint32_t startIndexLocation, uint32_t baseVertexLocation, uint32_t vertexOffset, uint32_t indexOffset)
	{
		auto direct3DDeviceContext = mRenderingManager.GetDevice().ContextPtr;

		BeginDraw();

		const uint32_t stride = VertexSize();
		const uint32_t offset = 0;
		ID3D11Buffer* const vertexBuffers[]{ vertexBuffer };
		direct3DDeviceContext->IASetVertexBuffers(0, narrow_cast<uint32_t>(size(vertexBuffers)), vertexBuffers, &stride, &vertexOffset);
		direct3DDeviceContext->IASetIndexBuffer(indexBuffer, format, indexOffset);

		direct3DDeviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);

		EndDraw();
	}

	uint32_t Material::VertexSize() const
	{
		return 0;
	}

	void Material::BeginDraw()
	{
		auto direct3DDeviceContext = mRenderingManager.GetDevice().ContextPtr;
		direct3DDeviceContext->IASetPrimitiveTopology(mTopology);

		assert(mInputLayout != nullptr);
		direct3DDeviceContext->IASetInputLayout(mInputLayout.get());

		for (const auto& entry : mShaderStageData)
		{
			const ShaderStageData& shaderStageData = entry.second;
			auto& shaderStageCallInfo = ShaderStageCalls.at(entry.first);

			shaderStageCallInfo.SetShader(*direct3DDeviceContext, shaderStageData);

			if (shaderStageData.ConstantBuffers.size() > 0)
			{
				shaderStageCallInfo.SetConstantBuffers(direct3DDeviceContext, 0, narrow_cast<uint32_t>(shaderStageData.ConstantBuffers.size()), shaderStageData.ConstantBuffers.data());
			}

			if (shaderStageData.ShaderResources.size() > 0)
			{
				shaderStageCallInfo.SetShaderResources(direct3DDeviceContext, 0, narrow_cast<uint32_t>(shaderStageData.ShaderResources.size()), shaderStageData.ShaderResources.data());
			}

			if (shaderStageData.SamplerStates.size() > 0)
			{
				shaderStageCallInfo.SetSamplers(direct3DDeviceContext, 0, narrow_cast<uint32_t>(shaderStageData.SamplerStates.size()), shaderStageData.SamplerStates.data());
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
				auto& shaderStageCallInfo = ShaderStageCalls.at(entry.first);

				if (shaderStageData.ShaderResources.size() > 0)
				{
					assert(EmptySRVs.size() >= shaderStageData.ShaderResources.size());
					shaderStageCallInfo.SetShaderResources(mRenderingManager.GetDevice().ContextPtr, 0, narrow_cast<uint32_t>(shaderStageData.ShaderResources.size()), EmptySRVs.data());
				}
			}
		}
	}

	pair<bool, Material::ShaderStageData*> Material::GetDataForShaderStage(ShaderStages shaderStage)
	{
		auto it = mShaderStageData.find(shaderStage);
		bool found = (it != mShaderStageData.end());
		ShaderStageData* shaderStageData = (found ? shaderStageData = &(it->second) : nullptr);

		return make_pair(found, shaderStageData);
	}

	void Material::SetVSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData)
	{
		assert(shaderStageData.Shader != nullptr);
		auto vertexShader = static_pointer_cast<VertexShader>(shaderStageData.Shader);
		direct3DDeviceContext.VSSetShader(vertexShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
	}

	void Material::SetHSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData)
	{
		if (shaderStageData.Shader != nullptr)
		{
			auto hullShader = static_pointer_cast<HullShader>(shaderStageData.Shader);
			direct3DDeviceContext.HSSetShader(hullShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
		}
		else
		{
			direct3DDeviceContext.HSSetShader(nullptr, nullptr, 0);
		}
	}

	void Material::SetDSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData)
	{
		if (shaderStageData.Shader != nullptr)
		{
			auto domainShader = static_pointer_cast<DomainShader>(shaderStageData.Shader);
			direct3DDeviceContext.DSSetShader(domainShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
		}
		else
		{
			direct3DDeviceContext.DSSetShader(nullptr, nullptr, 0);
		}
	}

	void Material::SetGSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData)
	{
		if (shaderStageData.Shader != nullptr)
		{
			auto geometryShader = static_pointer_cast<GeometryShader>(shaderStageData.Shader);
			direct3DDeviceContext.GSSetShader(geometryShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
		}
		else
		{
			direct3DDeviceContext.GSSetShader(nullptr, nullptr, 0);
		}
	}

	void Material::SetPSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData)
	{
		if (shaderStageData.Shader != nullptr)
		{
			auto pixelShader = static_pointer_cast<PixelShader>(shaderStageData.Shader);
			direct3DDeviceContext.PSSetShader(pixelShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
		}
		else
		{
			direct3DDeviceContext.PSSetShader(nullptr, nullptr, 0);
		}
	}

	void Material::SetCSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData)
	{
		if (shaderStageData.Shader != nullptr)
		{
			auto pixelShader = static_pointer_cast<ComputeShader>(shaderStageData.Shader);
			direct3DDeviceContext.CSSetShader(pixelShader->Shader().get(), &shaderStageData.ShaderClassInstance, shaderStageData.ShaderClassInstance != nullptr ? 1 : 0);
		}
		else
		{
			direct3DDeviceContext.CSSetShader(nullptr, nullptr, 0);
		}
	}
}

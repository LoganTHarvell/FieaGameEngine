#pragma once

namespace Library
{
	inline PrimitiveTopology Material::Topology() const
	{
		return mTopology;
	}

	inline void Material::SetTopology(const PrimitiveTopology topology)
	{
		mTopology = topology;
	}

	template <typename T>
	std::shared_ptr<T> Material::GetShader()
	{
		const ShaderStages shaderStage = Shader::TypeStageMap.At(T::TypeIdClass());
		const std::shared_ptr<Shader> shader = GetShader(shaderStage);
		return std::static_pointer_cast<T>(shader);
	}

	template <typename T>
	void Material::SetShader(std::shared_ptr<T> shader)
	{
		const ShaderStages shaderStage = Shader::TypeStageMap.At(T::TypeIdClass());
		assert(ShaderStageIsProgrammable(shaderStage));
		mShaderStageData[shaderStage].Shader = std::move(shader);
	}

	//inline void Material::SetShaderClassInstance(const ShaderStages shaderStage, ID3D11ClassInstance* classInstance)
	//{
	//	assert(ShaderStageIsProgrammable(shaderStage));
	//	mShaderStageData[shaderStage].ShaderClassInstance = classInstance;
	//}

	inline void Material::SetShaderResource(const ShaderStages shaderStage, Resource& shaderResource)
	{
		ClearShaderResources(shaderStage);
		AddShaderResource(shaderStage, shaderResource);
	}

	inline void Material::AddShaderResource(const ShaderStages shaderStage, Resource& shaderResource)
	{
		auto& shaderResources = mShaderStageData[shaderStage].Resources[shaderResource.ResourceType()];
		shaderResources.EmplaceBack(&shaderResource);

		if (shaderResource.ResourceType() == Resource::Type::Texture)
		{			
			if (EmptySRVs.Size() < shaderResources.Size())
			{
				EmptySRVs.Resize(shaderResources.Size(), nullptr);
			}
		}
	}

	inline void Material::AddShaderResources(const ShaderStages shaderStage, const gsl::span<gsl::not_null<Resource*>>& shaderResources)
	{
		if (shaderResources.empty()) return;

		const auto type = (*shaderResources.begin())->ResourceType();
		auto& resourceList = mShaderStageData[shaderStage].Resources[type];
		resourceList.Insert(resourceList.end(), shaderResources.begin(), shaderResources.end());

		if (type == Resource::Type::Texture)
		{
			if (EmptySRVs.Size() < resourceList.Size())
			{
				EmptySRVs.Resize(resourceList.Size(), nullptr);
			}
		}
	}

	inline std::function<void()> Material::DrawCallback() const
	{
		return mDrawCallback;
	}

	inline void Material::SetDrawCallback(std::function<void()> callback)
	{
		mDrawCallback = std::move(callback);
	}

	inline std::function<void()> Material::UpdateMaterialCallback() const
	{
		return mUpdateMaterialCallback;
	}

	inline void Material::SetUpdateMaterialCallback(std::function<void()> callback)
	{
		mUpdateMaterialCallback = std::move(callback);
	}

	template <size_t _Count>
	void Material::UnbindShaderResources(const RenderingManager& renderingManager, const ShaderStages shaderStage, std::uint32_t startSlot)
	{
		static std::array<ID3D11ShaderResourceView*, _Count> emptySRVs{ nullptr };

		renderingManager.SetShaderResources(startSlot, _Count, emptySRVs.data());
	}

	template <size_t _Count>
	void Material::UnbindShaderResources(const ShaderStages shaderStage, const std::uint32_t startSlot)
	{
		Material::UnbindShaderResources<_Count>(mRenderingManager, shaderStage, startSlot);
	}

	inline bool Material::AutoUnbindShaderResourcesEnabled() const
	{
		return mAutoUnbindShaderResourcesEnabled;
	}

	inline void Material::SetAutoUnbindShaderResourcesEnabled(const bool enabled)
	{
		mAutoUnbindShaderResourcesEnabled = enabled;
	}
}
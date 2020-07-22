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

	inline winrt::com_ptr<ID3D11InputLayout> Material::InputLayout() const
	{
		return mInputLayout;
	}

	inline void Material::SetInputLayout(winrt::com_ptr<ID3D11InputLayout> inputLayout)
	{
		assert(inputLayout != nullptr);
		mInputLayout = inputLayout;
	}

	template <typename T>
	std::shared_ptr<T> Material::GetShader()
	{
		const ShaderStages shaderStage = TypeShaderStageMap.At(T::TypeIdClass());
		const std::shared_ptr<Shader> shader = GetShader(shaderStage);
		return std::static_pointer_cast<T>(shader);
	}

	template <typename T>
	void Material::SetShader(std::shared_ptr<T> shader)
	{
		const ShaderStages shaderStage = TypeShaderStageMap.At(T::TypeIdClass());
		SetShader(shaderStage, shader);
	}

	inline void Material::SetShader(const ShaderStages shaderStage, std::shared_ptr<Shader> shader)
	{
		assert(ShaderStageIsProgrammable(shaderStage));
		mShaderStageData[shaderStage].Shader = std::move(shader);
	}

	inline void Material::SetShaderClassInstance(const ShaderStages shaderStage, ID3D11ClassInstance* classInstance)
	{
		assert(ShaderStageIsProgrammable(shaderStage));
		mShaderStageData[shaderStage].ShaderClassInstance = classInstance;
	}

	inline void Material::SetConstantBuffer(const ShaderStages shaderStage, ID3D11Buffer* constantBuffer)
	{
		ClearConstantBuffers(shaderStage);
		AddConstantBuffer(shaderStage, constantBuffer);
	}

	inline void Material::AddConstantBuffer(const ShaderStages shaderStage, ID3D11Buffer* constantBuffer)
	{
		mShaderStageData[shaderStage].ConstantBuffers.EmplaceBack(constantBuffer);
	}

	inline void Material::AddConstantBuffers(const ShaderStages shaderStage, const gsl::span<ID3D11Buffer*>& constantBuffers)
	{
		auto& registeredConstantBuffers = mShaderStageData[shaderStage].ConstantBuffers;
		registeredConstantBuffers.Insert(registeredConstantBuffers.cend(), constantBuffers.cbegin(), constantBuffers.cend());
	}

	inline void Material::SetShaderResource(const ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource)
	{
		ClearShaderResources(shaderStage);
		AddShaderResource(shaderStage, shaderResource);
	}

	inline void Material::AddShaderResource(const ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource)
	{
		auto& shaderResources = mShaderStageData[shaderStage].ShaderResources;
		shaderResources.EmplaceBack(shaderResource);

		if (EmptySRVs.Size() < shaderResources.Size())
		{
			EmptySRVs.EmplaceBack(nullptr);
		}
	}

	inline void Material::AddShaderResources(const ShaderStages shaderStage, const gsl::span<ID3D11ShaderResourceView*>& shaderResources)
	{
		auto& registeredShaderResources = mShaderStageData[shaderStage].ShaderResources;
		registeredShaderResources.Insert(registeredShaderResources.end(), shaderResources.begin(), shaderResources.end());

		if (EmptySRVs.Size() < registeredShaderResources.Size())
		{
			EmptySRVs.Resize(registeredShaderResources.Size(), nullptr);
		}
	}

	inline void Material::SetSamplerState(const ShaderStages shaderStage, ID3D11SamplerState* samplerState)
	{
		ClearSamplerStates(shaderStage);
		AddSamplerState(shaderStage, samplerState);
	}

	inline void Material::AddSamplerState(const ShaderStages shaderStage, ID3D11SamplerState* samplerState)
	{
		mShaderStageData[shaderStage].SamplerStates.EmplaceBack(samplerState);
	}

	inline void Material::AddSamplerStates(const ShaderStages shaderStage, const gsl::span<ID3D11SamplerState*>& samplerStates)
	{
		auto& registeredSamplerStates = mShaderStageData[shaderStage].SamplerStates;
		registeredSamplerStates.Insert(registeredSamplerStates.end(), samplerStates.begin(), samplerStates.end());
	}

	inline std::function<void()> Material::DrawCallback() const
	{
		return mDrawCallback;
	}

	inline void Material::SetDrawCallback(std::function<void()> callback)
	{
		mDrawCallback = callback;
	}

	inline std::function<void()> Material::UpdateMaterialCallback() const
	{
		return mUpdateMaterialCallback;
	}

	inline void Material::SetUpdateMaterialCallback(std::function<void()> callback)
	{
		mUpdateMaterialCallback = callback;
	}

	template <size_t _Count>
	void Material::UnbindShaderResources(gsl::not_null<ID3D11DeviceContext*> direct3DDeviceContext, const ShaderStages shaderStage, std::uint32_t startSlot)
	{
		static std::array<ID3D11ShaderResourceView*, _Count> emptySRVs{ nullptr };

		auto& shaderStageCallInfo = ShaderStageCalls.At(shaderStage);
		shaderStageCallInfo.SetShaderResources(direct3DDeviceContext.get(), startSlot, _Count, emptySRVs.data());
	}

	template <size_t _Count>
	void Material::UnbindShaderResources(const ShaderStages shaderStage, std::uint32_t startSlot)
	{
		Material::UnbindShaderResources<_Count>(static_cast<RenderingManagerD3D11&>(mRenderingManager).Context(), shaderStage, startSlot);
	}

	inline bool Material::AutoUnbindShaderResourcesEnabled() const
	{
		return mAutoUnbindShaderResourcesEnabled;
	}

	inline void Material::SetAutoUnbindShaderResourcesEnabled(bool enabled)
	{
		mAutoUnbindShaderResourcesEnabled = enabled;
	}
}
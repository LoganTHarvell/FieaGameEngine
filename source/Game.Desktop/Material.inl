#pragma once

namespace Library
{
	inline D3D_PRIMITIVE_TOPOLOGY Material::Topology() const
	{
		return mTopology;
	}

	inline void Material::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)
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
		ShaderStages shaderStage = TypeShaderStageMap.at(T::TypeIdClass());
		std::shared_ptr<Shader> shader = GetShader(shaderStage);
		return std::static_pointer_cast<T>(shader);
	}

	template <typename T>
	void Material::SetShader(std::shared_ptr<T> shader)
	{
		ShaderStages shaderStage = TypeShaderStageMap.at(T::TypeIdClass());
		SetShader(shaderStage, shader);
	}

	inline void Material::SetShader(ShaderStages shaderStage, std::shared_ptr<Shader> shader)
	{
		assert(ShaderStageIsProgrammable(shaderStage));
		mShaderStageData[shaderStage].Shader = shader;
	}

	inline void Material::SetShaderClassInstance(ShaderStages shaderStage, ID3D11ClassInstance* classInstance)
	{
		assert(ShaderStageIsProgrammable(shaderStage));
		mShaderStageData[shaderStage].ShaderClassInstance = classInstance;
	}

	inline void Material::SetConstantBuffer(ShaderStages shaderStage, ID3D11Buffer* constantBuffer)
	{
		ClearConstantBuffers(shaderStage);
		AddConstantBuffer(shaderStage, constantBuffer);
	}

	inline void Material::AddConstantBuffer(ShaderStages shaderStage, ID3D11Buffer* constantBuffer)
	{
		mShaderStageData[shaderStage].ConstantBuffers.push_back(constantBuffer);
	}

	inline void Material::AddConstantBuffers(ShaderStages shaderStage, const gsl::span<ID3D11Buffer*>& constantBuffers)
	{
		auto& registeredConstantBuffers = mShaderStageData[shaderStage].ConstantBuffers;
		registeredConstantBuffers.insert(registeredConstantBuffers.end(), constantBuffers.begin(), constantBuffers.end());
	}

	inline void Material::SetShaderResource(ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource)
	{
		ClearShaderResources(shaderStage);
		AddShaderResource(shaderStage, shaderResource);
	}

	inline void Material::AddShaderResource(ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource)
	{
		auto& shaderResources = mShaderStageData[shaderStage].ShaderResources;
		shaderResources.push_back(shaderResource);

		if (EmptySRVs.size() < shaderResources.size())
		{
			EmptySRVs.push_back(nullptr);
		}
	}

	inline void Material::AddShaderResources(ShaderStages shaderStage, const gsl::span<ID3D11ShaderResourceView*>& shaderResources)
	{
		auto& registeredShaderResources = mShaderStageData[shaderStage].ShaderResources;
		registeredShaderResources.insert(registeredShaderResources.end(), shaderResources.begin(), shaderResources.end());

		if (EmptySRVs.size() < registeredShaderResources.size())
		{
			EmptySRVs.resize(registeredShaderResources.size(), nullptr);
		}
	}

	inline void Material::SetSamplerState(ShaderStages shaderStage, ID3D11SamplerState* samplerState)
	{
		ClearSamplerStates(shaderStage);
		AddSamplerState(shaderStage, samplerState);
	}

	inline void Material::AddSamplerState(ShaderStages shaderStage, ID3D11SamplerState* samplerState)
	{
		mShaderStageData[shaderStage].SamplerStates.push_back(samplerState);
	}

	inline void Material::AddSamplerStates(ShaderStages shaderStage, const gsl::span<ID3D11SamplerState*>& samplerStates)
	{
		auto& registeredSamplerStates = mShaderStageData[shaderStage].SamplerStates;
		registeredSamplerStates.insert(registeredSamplerStates.end(), samplerStates.begin(), samplerStates.end());
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
	void Material::UnbindShaderResources(gsl::not_null<ID3D11DeviceContext*> direct3DDeviceContext, ShaderStages shaderStage, std::uint32_t startSlot)
	{
		static std::array<ID3D11ShaderResourceView*, _Count> emptySRVs{ nullptr };

		auto& shaderStageCallInfo = ShaderStageCalls.at(shaderStage);
		shaderStageCallInfo.SetShaderResources(direct3DDeviceContext.get(), startSlot, _Count, emptySRVs.data());
	}

	template <size_t _Count>
	void Material::UnbindShaderResources(ShaderStages shaderStage, std::uint32_t startSlot)
	{
		Material::UnbindShaderResources<_Count>(mRenderingManager.GetDevice().ContextPtr, shaderStage, startSlot);
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
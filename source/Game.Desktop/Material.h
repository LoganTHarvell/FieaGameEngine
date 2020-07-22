#pragma once

#pragma region Includes
// Standard
#include <memory>
#include <functional>

// First Party
#include "Vector.h"
#include "HashMap.h"
#include "Resource.h"
#include "Shader.h"
#pragma endregion Includes

// TODO: Remove
#include "RenderingManagerD3D11.h"

namespace Library
{
	// Forward Declarations
	class RenderingManager;
	class ContentManager;
	class Buffer;

	/**
	 * @brief Managers the visual attributes
	*/
	class Material : public RTTI
	{
		RTTI_DECLARATIONS_ABSTRACT(Material, RTTI)

	public:
		using CallbackFunctor = std::function<void()>;
		
		struct ShaderStageData
		{
			std::shared_ptr<Shader> Shader;
			ID3D11ClassInstance* ShaderClassInstance{ nullptr };
			Vector<ID3D11Buffer*> ConstantBuffers;
			Vector<ID3D11ShaderResourceView*> ShaderResources;			
			Vector<ID3D11SamplerState*> SamplerStates;
		};

		explicit Material(ContentManager& contentManager, RenderingManager& renderingManager, const PrimitiveTopology topology = PrimitiveTopology::TriangleList);

		PrimitiveTopology Topology() const;
		void SetTopology(const PrimitiveTopology topology);

		winrt::com_ptr<ID3D11InputLayout> InputLayout() const;
		void SetInputLayout(winrt::com_ptr<ID3D11InputLayout> inputLayout);

		std::shared_ptr<Shader> GetShader(const ShaderStages shaderStage);
		void SetShader(const ShaderStages shaderStage, std::shared_ptr<Shader> shader);

		template <typename T>
		std::shared_ptr<T> GetShader();

		template <typename T>
		void SetShader(std::shared_ptr<T> shader);

		ID3D11ClassInstance* GetShaderClassInstance(const ShaderStages shaderStage);
		void SetShaderClassInstance(const ShaderStages shaderStage, ID3D11ClassInstance* classInstance);

		void SetConstantBuffer(const ShaderStages shaderStage, ID3D11Buffer* constantBuffer);
		void AddConstantBuffer(const ShaderStages shaderStage, ID3D11Buffer* constantBuffer);
		void AddConstantBuffers(const ShaderStages shaderStage, const gsl::span<ID3D11Buffer*>& constantBuffers);
		void RemoveConstantBuffer(const ShaderStages shaderStage, ID3D11Buffer* constantBuffer);
		void ClearConstantBuffers(const ShaderStages shaderStage);

		void SetShaderResource(const ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource);
		void AddShaderResource(const ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource);
		void AddShaderResources(const ShaderStages shaderStage, const gsl::span<ID3D11ShaderResourceView*>& shaderResources);
		void RemoveShaderResource(const ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource);
		void ClearShaderResources(const ShaderStages shaderStage);

		void SetSamplerState(const ShaderStages shaderStage, ID3D11SamplerState* samplerState);
		void AddSamplerState(const ShaderStages shaderStage, ID3D11SamplerState* samplerState);
		void AddSamplerStates(const ShaderStages shaderStage, const gsl::span<ID3D11SamplerState*>& samplerStates);
		void RemoveSamplerState(const ShaderStages shaderStage, ID3D11SamplerState* samplerState);
		void ClearSamplerStates(const ShaderStages shaderStage);

		CallbackFunctor DrawCallback() const;
		void SetDrawCallback(CallbackFunctor callback);

		CallbackFunctor UpdateMaterialCallback() const;
		void SetUpdateMaterialCallback(CallbackFunctor callback);

		std::pair<bool, ShaderStageData*> GetDataForShaderStage(const ShaderStages shaderStage);
		bool AutoUnbindShaderResourcesEnabled() const;
		void SetAutoUnbindShaderResourcesEnabled(bool enabled);
		
		template <size_t _Count>
		static void UnbindShaderResources(gsl::not_null<ID3D11DeviceContext*> direct3DDeviceContext, const ShaderStages shaderStage, std::uint32_t startSlot = 0);

		template <size_t _Count>
		void UnbindShaderResources(const ShaderStages shaderStage, std::uint32_t startSlot = 0);

		virtual void Initialize();
		virtual void Draw();
		virtual void Draw(Buffer& vertexBuffer, std::uint32_t vertexCount, std::uint32_t startVertexLocation = 0);
		virtual void DrawIndexed(Buffer& vertexBuffer, Buffer& indexBuffer, const std::uint32_t indexCount, const Format& format = Format::R32UInt, const std::uint32_t startIndexLocation=0, const std::int32_t baseVertexLocation=0, const std::uint32_t indexOffset=0);
		virtual std::uint32_t VertexSize() const;

	private:
		struct ShaderStageCallInfo
		{
			std::function<void(ID3D11DeviceContext&, const ShaderStageData&)> SetShader;
			std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11Buffer* const *)> SetConstantBuffers;
			std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11ShaderResourceView* const *)> SetShaderResources;
			std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11SamplerState* const *)> SetSamplers;			
		};

		static void SetVSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData);		
		static void SetHSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData);
		static void SetDSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData);
		static void SetGSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData);
		static void SetPSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData);
		static void SetCSShader(ID3D11DeviceContext& direct3DDeviceContext, const ShaderStageData& shaderStageData);

	protected:
		static const HashMap<ShaderStages, IdType> ShaderStageTypeMap;
		static const HashMap<IdType, ShaderStages> TypeShaderStageMap;
		static const HashMap<ShaderStages, ShaderStageCallInfo> ShaderStageCalls;
		static Vector<ID3D11ShaderResourceView*> EmptySRVs;

		virtual void BeginDraw();
		virtual void EndDraw();

		ContentManager& mContentManager;
		RenderingManager& mRenderingManager;
		PrimitiveTopology mTopology;
		
		winrt::com_ptr<ID3D11InputLayout> mInputLayout;

		HashMap<ShaderStages, ShaderStageData> mShaderStageData;
		
		CallbackFunctor mDrawCallback;
		CallbackFunctor mUpdateMaterialCallback;
		bool mAutoUnbindShaderResourcesEnabled{ false };
	};
}

#include "Material.inl"
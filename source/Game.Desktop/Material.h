#pragma once

#include <map>
#include <vector>
#include <functional>

#include "CoreD3D.h"

#include "Shader.h"
#include "Game.h"

namespace Library
{
	class Material : public RTTI
	{
		RTTI_DECLARATIONS_ABSTRACT(Material, RTTI)

	public:
		struct ShaderStageData
		{
			std::shared_ptr<Shader> Shader;
			ID3D11ClassInstance* ShaderClassInstance{ nullptr };
			std::vector<ID3D11Buffer*> ConstantBuffers;
			std::vector<ID3D11ShaderResourceView*> ShaderResources;			
			std::vector<ID3D11SamplerState*> SamplerStates;
		};

		explicit Material(ContentManager& contentManager, RenderingManager& renderingManager, PrimitiveTopology topology = PrimitiveTopology::TriangleList);

		PrimitiveTopology Topology() const;
		void SetTopology(PrimitiveTopology topology);

		winrt::com_ptr<ID3D11InputLayout> InputLayout() const;
		void SetInputLayout(winrt::com_ptr<ID3D11InputLayout> inputLayout);

		std::shared_ptr<Shader> GetShader(ShaderStages shaderStage);
		void SetShader(ShaderStages shaderStage, std::shared_ptr<Shader> shader);

		template <typename T>
		std::shared_ptr<T> GetShader();

		template <typename T>
		void SetShader(std::shared_ptr<T> shader);

		ID3D11ClassInstance* GetShaderClassInstance(ShaderStages shaderStage);
		void SetShaderClassInstance(ShaderStages shaderStage, ID3D11ClassInstance* classInstance);

		void SetConstantBuffer(ShaderStages shaderStage, ID3D11Buffer* constantBuffer);
		void AddConstantBuffer(ShaderStages shaderStage, ID3D11Buffer* constantBuffer);
		void AddConstantBuffers(ShaderStages shaderStage, const gsl::span<ID3D11Buffer*>& constantBuffers);
		void RemoveConstantBuffer(ShaderStages shaderStage, ID3D11Buffer* constantBuffer);
		void ClearConstantBuffers(ShaderStages shaderStage);

		void SetShaderResource(ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource);
		void AddShaderResource(ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource);
		void AddShaderResources(ShaderStages shaderStage, const gsl::span<ID3D11ShaderResourceView*>& shaderResources);
		void RemoveShaderResource(ShaderStages shaderStage, ID3D11ShaderResourceView* shaderResource);
		void ClearShaderResources(ShaderStages shaderStage);

		void SetSamplerState(ShaderStages shaderStage, ID3D11SamplerState* samplerState);
		void AddSamplerState(ShaderStages shaderStage, ID3D11SamplerState* samplerState);
		void AddSamplerStates(ShaderStages shaderStage, const gsl::span<ID3D11SamplerState*>& samplerStates);
		void RemoveSamplerState(ShaderStages shaderStage, ID3D11SamplerState* samplerState);
		void ClearSamplerStates(ShaderStages shaderStage);

		std::function<void()> DrawCallback() const;
		void SetDrawCallback(std::function<void()> callback);

		std::function<void()> UpdateMaterialCallback() const;
		void SetUpdateMaterialCallback(std::function<void()> callback);

		std::pair<bool, ShaderStageData*> GetDataForShaderStage(ShaderStages shaderStage);
		bool AutoUnbindShaderResourcesEnabled() const;
		void SetAutoUnbindShaderResourcesEnabled(bool enabled);
		
		template <size_t _Count>
		static void UnbindShaderResources(gsl::not_null<ID3D11DeviceContext*> direct3DDeviceContext, ShaderStages shaderStage, std::uint32_t startSlot = 0);

		template <size_t _Count>
		void UnbindShaderResources(ShaderStages shaderStage, std::uint32_t startSlot = 0);

		virtual void Initialize();
		virtual void Draw();
		virtual void Draw(gsl::not_null<ID3D11Buffer*> vertexBuffer, std::uint32_t vertexCount, std::uint32_t startVertexLocation = 0, std::uint32_t offset = 0);
		virtual void DrawIndexed(gsl::not_null<ID3D11Buffer*> vertexBuffer, gsl::not_null<ID3D11Buffer*> indexBuffer, std::uint32_t indexCount, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t vertexOffset = 0, std::uint32_t indexOffset = 0);
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
		static const std::map<ShaderStages, RTTI::IdType> ShaderStageTypeMap;
		static const std::map<RTTI::IdType, ShaderStages> TypeShaderStageMap;
		static const std::map<ShaderStages, ShaderStageCallInfo> ShaderStageCalls;
		static std::vector<ID3D11ShaderResourceView*> EmptySRVs;

		virtual void BeginDraw();
		virtual void EndDraw();

		ContentManager& mContentManager;
		RenderingManager& mRenderingManager;
		PrimitiveTopology mTopology;
		winrt::com_ptr<ID3D11InputLayout> mInputLayout;
		std::map<ShaderStages, ShaderStageData> mShaderStageData;
		std::function<void()> mDrawCallback;
		std::function<void()> mUpdateMaterialCallback;
		bool mAutoUnbindShaderResourcesEnabled{ false };
	};
}

#include "Material.inl"
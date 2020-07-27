#pragma once

#pragma region Includes
// Standard
#include <memory>
#include <functional>

// First Party
#include "RenderingManager.h"
#include "Shader.h"
#pragma endregion Includes

namespace Library
{
	// Forward Declarations
	class ContentManager;

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

			// TODO: Support ResourceArrays per Type so you can call SetConstantBuffers, etc.
			HashMap<Resource::Type, Vector<Resource*>> Resources;
		};

		explicit Material(ContentManager& contentManager, RenderingManager& renderingManager, const PrimitiveTopology topology = PrimitiveTopology::TriangleList);

		PrimitiveTopology Topology() const;
		void SetTopology(const PrimitiveTopology topology);

		std::shared_ptr<Shader> GetShader(const ShaderStages shaderStage);

		template <typename T>
		std::shared_ptr<T> GetShader();

		template <typename T>
		void SetShader(std::shared_ptr<T> shader);

		//ID3D11ClassInstance* GetShaderClassInstance(const ShaderStages shaderStage);
		//void SetShaderClassInstance(const ShaderStages shaderStage, ID3D11ClassInstance* classInstance);

		void SetShaderResource(const ShaderStages shaderStage, Resource& shaderResource);
		void AddShaderResource(const ShaderStages shaderStage, Resource& shaderResource);
		void AddShaderResources(const ShaderStages shaderStage, const gsl::span<gsl::not_null<Resource*>>& shaderResources);
		void RemoveShaderResource(const ShaderStages shaderStage, Resource& shaderResource);
		void ClearShaderResources(const ShaderStages shaderStage, const Resource::Type type=Resource::Type::Undefined);
		
		CallbackFunctor DrawCallback() const;
		void SetDrawCallback(CallbackFunctor callback);

		CallbackFunctor UpdateMaterialCallback() const;
		void SetUpdateMaterialCallback(CallbackFunctor callback);

		std::pair<bool, ShaderStageData*> GetDataForShaderStage(const ShaderStages shaderStage);
		bool AutoUnbindShaderResourcesEnabled() const;
		void SetAutoUnbindShaderResourcesEnabled(bool enabled);
		
		template <size_t _Count>
		static void UnbindShaderResources(const RenderingManager& renderingManager, const ShaderStages shaderStage, std::uint32_t startSlot = 0);

		template <size_t _Count>
		void UnbindShaderResources(const ShaderStages shaderStage, std::uint32_t startSlot = 0);

		virtual void Initialize();
		virtual void Draw();
		virtual void Draw(Buffer& vertexBuffer, std::uint32_t vertexCount, std::uint32_t startVertexLocation = 0);
		virtual void DrawIndexed(Buffer& vertexBuffer, Buffer& indexBuffer, const std::uint32_t indexCount, const Format& format = Format::R32UInt, const std::uint32_t startIndexLocation=0, const std::int32_t baseVertexLocation=0, const std::uint32_t indexOffset=0);
		virtual std::uint32_t VertexSize() const;

	protected:
		inline static Vector<Resource*> EmptySRVs;

		virtual void BeginDraw();
		virtual void EndDraw();

		// TODO: Replace with gsl::not_null
		ContentManager& mContentManager;
		RenderingManager& mRenderingManager;
		PrimitiveTopology mTopology;
		
		HashMap<ShaderStages, ShaderStageData> mShaderStageData;
		
		CallbackFunctor mDrawCallback;
		CallbackFunctor mUpdateMaterialCallback;
		bool mAutoUnbindShaderResourcesEnabled{ false };
	};
}

#include "Material.inl"
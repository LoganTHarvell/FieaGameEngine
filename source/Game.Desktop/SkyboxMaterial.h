#pragma once

#include "Material.h"
#include "Sampler.h"
#include "SamplerStatesD3D11.h"

namespace Library
{
	class TextureCube;

	class SkyboxMaterial final : public Material
	{
		RTTI_DECLARATIONS_ABSTRACT(SkyboxMaterial, Material)

	public:
		SkyboxMaterial(ContentManager& contentManager, RenderingManager& renderingManager,
		               std::shared_ptr<TextureCube> texture, const Sampler::Type samplerType = Sampler::Type::TrilinearClamp);
		SkyboxMaterial(const SkyboxMaterial&) = default;
		SkyboxMaterial& operator=(const SkyboxMaterial&) = default;
		SkyboxMaterial(SkyboxMaterial&&) = default;
		SkyboxMaterial& operator=(SkyboxMaterial&&) = default;
		~SkyboxMaterial() = default;

		Sampler* SamplerState() const;
		void SetSamplerState(gsl::not_null<Sampler*> samplerState);

		std::shared_ptr<TextureCube> Texture() const;
		void SetTexture(std::shared_ptr<TextureCube> texture);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

		void UpdateTransforms(DirectX::CXMMATRIX worldViewProjectionMatrix);

	private:
		virtual void BeginDraw() override;
		virtual void EndDraw() override;

		Buffer* mConstantBuffer{ nullptr };
		std::shared_ptr<TextureCube> mTexture;
		Sampler* mSamplerState{ nullptr };
	};
}
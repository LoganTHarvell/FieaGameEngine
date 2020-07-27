#pragma once

#include "Material.h"
#include "SamplerStatesD3D11.h"
#include "Texture.h"

namespace Library
{
	class FullScreenQuadMaterial : public Material
	{
		RTTI_DECLARATIONS_ABSTRACT(FullScreenQuadMaterial, Material)

	public:
		explicit FullScreenQuadMaterial(ContentManager& contentManager, RenderingManager& renderingManager, const Sampler::Type samplerType = Sampler::Type::TrilinearClamp);
		FullScreenQuadMaterial(const FullScreenQuadMaterial&) = default;
		FullScreenQuadMaterial& operator=(const FullScreenQuadMaterial&) = default;
		FullScreenQuadMaterial(FullScreenQuadMaterial&&) = default;
		FullScreenQuadMaterial& operator=(FullScreenQuadMaterial&&) = default;
		virtual ~FullScreenQuadMaterial() = default;

		Sampler* SamplerState() const;
		void SetSamplerState(const gsl::not_null<Sampler*> samplerState);

		void SetTexture(gsl::not_null<Texture*> texture);
		void SetTextures(gsl::span<gsl::not_null<Texture*>> textures);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

	private:
		Vector<gsl::not_null<Resource*>> mTextures;
		Sampler* mSamplerState{ nullptr };
	};
}

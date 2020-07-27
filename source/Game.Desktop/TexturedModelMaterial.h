#pragma once

#include "Material.h"
#include "Sampler.h"
#include "SamplerStatesD3D11.h"

namespace Library
{
	class Texture2D;

	class TexturedModelMaterial : public Material
	{
		RTTI_DECLARATIONS_ABSTRACT(TexturedModelMaterial, Material)

	public:
		explicit TexturedModelMaterial(ContentManager& contentManager, RenderingManager& renderingManager, std::shared_ptr<Texture2D> texture = nullptr);
		TexturedModelMaterial(const TexturedModelMaterial&) = default;
		TexturedModelMaterial& operator=(const TexturedModelMaterial&) = default;
		TexturedModelMaterial(TexturedModelMaterial&&) = default;
		TexturedModelMaterial& operator=(TexturedModelMaterial&&) = default;
		virtual ~TexturedModelMaterial() = default;

		Sampler* SamplerState() const;
		void SetSamplerState(gsl::not_null<Sampler*> samplerState);

		std::shared_ptr<Texture2D> Texture() const;
		void SetTexture(std::shared_ptr<Texture2D> texture);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

		void UpdateTransform(DirectX::CXMMATRIX worldViewProjectionMatrix);

	private:
		Buffer* mVertexCBufferPerObject;
		std::shared_ptr<Texture2D> mTexture;
		Sampler* mSamplerState;
	};
}

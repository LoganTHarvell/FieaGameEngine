#pragma once

#include "Material.h"
#include "SamplerStates.h"
#include <vector>

namespace Library
{
	class FullScreenQuadMaterial : public Material
	{
		RTTI_DECLARATIONS_ABSTRACT(FullScreenQuadMaterial, Material)

	public:
		explicit FullScreenQuadMaterial(ContentManager& contentManager, RenderingManager& renderingManager, const winrt::com_ptr<ID3D11SamplerState>& samplerState = SamplerStates::TrilinearClamp);
		FullScreenQuadMaterial(const FullScreenQuadMaterial&) = default;
		FullScreenQuadMaterial& operator=(const FullScreenQuadMaterial&) = default;
		FullScreenQuadMaterial(FullScreenQuadMaterial&&) = default;
		FullScreenQuadMaterial& operator=(FullScreenQuadMaterial&&) = default;
		virtual ~FullScreenQuadMaterial() = default;

		winrt::com_ptr<ID3D11SamplerState> SamplerState() const;
		void SetSamplerState(const winrt::com_ptr<ID3D11SamplerState>& samplerState);

		void SetTexture(ID3D11ShaderResourceView* texture);
		void SetTextures(gsl::span<ID3D11ShaderResourceView*> textures);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

	private:
		std::vector<ID3D11ShaderResourceView*> mTextures;
		winrt::com_ptr<ID3D11SamplerState> mSamplerState;
	};
}
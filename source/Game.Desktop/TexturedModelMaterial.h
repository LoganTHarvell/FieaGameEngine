#pragma once

#include <winrt\Windows.Foundation.h>
#include "Material.h"
#include "SamplerStates.h"

namespace Library
{
	class Texture2D;

	class TexturedModelMaterial : public Material
	{
		RTTI_DECLARATIONS_ABSTRACT(TexturedModelMaterial, Material)

	public:
		explicit TexturedModelMaterial(ContentManager& contentManager, RenderingManager& renderingManager, std::shared_ptr<Library::Texture2D> texture = nullptr);
		TexturedModelMaterial(const TexturedModelMaterial&) = default;
		TexturedModelMaterial& operator=(const TexturedModelMaterial&) = default;
		TexturedModelMaterial(TexturedModelMaterial&&) = default;
		TexturedModelMaterial& operator=(TexturedModelMaterial&&) = default;
		virtual ~TexturedModelMaterial() = default;

		winrt::com_ptr<ID3D11SamplerState> SamplerState() const;
		void SetSamplerState(winrt::com_ptr<ID3D11SamplerState> samplerState);

		std::shared_ptr<Library::Texture2D> Texture() const;
		void SetTexture(std::shared_ptr<Library::Texture2D> texture);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

		void UpdateTransform(DirectX::CXMMATRIX worldViewProjectionMatrix);

	private:
		winrt::com_ptr<ID3D11Buffer> mVertexCBufferPerObject;
		std::shared_ptr<Library::Texture2D> mTexture;
		winrt::com_ptr<ID3D11SamplerState> mSamplerState{ Library::SamplerStates::TrilinearClamp };
	};
}

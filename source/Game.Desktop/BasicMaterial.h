#pragma once

#include "Material.h"

namespace Library
{
	class BasicMaterial : public Material
	{
		RTTI_DECLARATIONS_ABSTRACT(BasicMaterial, Material)

	public:
		explicit BasicMaterial(ContentManager& contentManager, RenderingManager& renderingManager);
		BasicMaterial(const BasicMaterial&) = default;
		BasicMaterial& operator=(const BasicMaterial&) = default;
		BasicMaterial(BasicMaterial&&) = default;
		BasicMaterial& operator=(BasicMaterial&&) = default;
		virtual ~BasicMaterial() = default;

		void SetSurfaceColor(const DirectX::XMFLOAT4& color);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

		void UpdateTransform(DirectX::CXMMATRIX worldViewProjectionMatrix);

	private:
		void SetSurfaceColor(const float* color);

		Buffer* mVSConstantBuffer{ nullptr };
		Buffer* mPSConstantBuffer{ nullptr };
	};
}

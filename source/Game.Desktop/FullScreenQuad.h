#pragma once

#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include "DrawableGameComponent.h"

namespace Library
{
	class FullScreenQuadMaterial;

	class FullScreenQuad final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS_ABSTRACT(FullScreenQuad, DrawableGameComponent)

	public:
		FullScreenQuad(Game& game);
		FullScreenQuad(const FullScreenQuad&) = delete;
		FullScreenQuad(FullScreenQuad&&) = default;
		FullScreenQuad& operator=(const FullScreenQuad&) = delete;
		FullScreenQuad& operator=(FullScreenQuad&&) = default;
		~FullScreenQuad() = default;

		std::shared_ptr<FullScreenQuadMaterial> Material() const;

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
		winrt::com_ptr<ID3D11Buffer> mIndexBuffer;
		std::shared_ptr<FullScreenQuadMaterial> mMaterial;
		std::uint32_t mIndexCount{ 0 };
	};
}
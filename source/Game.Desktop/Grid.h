#pragma once

#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "DrawableGameComponent.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "BasicMaterial.h"

namespace Library
{
	class Grid final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS_ABSTRACT(Grid, DrawableGameComponent)

	public:
		Grid(Game& game, const std::shared_ptr<Camera>& camera, std::uint32_t size = DefaultSize, std::uint32_t scale = DefaultScale, const DirectX::XMFLOAT4& color = DefaultColor);
		Grid(const Grid&) = delete;
		Grid(Grid&&) = default;
		Grid& operator=(const Grid&) = delete;
		Grid& operator=(Grid&&) = default;
		~Grid() = default;

		const DirectX::XMFLOAT3& Position() const;
		void SetPosition(const DirectX::XMFLOAT3& position);
		void SetPosition(float x, float y, float z);

		void SetColor(const DirectX::XMFLOAT4& color);

		const std::uint32_t Size() const;
		void SetSize(std::uint32_t size);

		const std::uint32_t Scale() const;
		void SetScale(std::uint32_t scale);

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

		inline static const std::uint32_t DefaultSize{ 16 };
		inline static const std::uint32_t DefaultScale{ 16 };
		inline static const DirectX::XMFLOAT4 DefaultColor{ 0.961f, 0.871f, 0.702f, 1.0f };

	private:
		void InitializeGrid();

		BasicMaterial mMaterial;
		winrt::com_ptr<ID3D11Buffer> mVertexBuffer;		
		DirectX::XMFLOAT3 mPosition{ Vector3Helper::Zero };
		std::uint32_t mSize;
		std::uint32_t mScale;
		DirectX::XMFLOAT4 mColor;
		DirectX::XMFLOAT4X4 mWorldMatrix{ MatrixHelper::Identity };
		bool mUpdateMaterial{ true };
	};
}
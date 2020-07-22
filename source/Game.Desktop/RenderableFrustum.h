#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "DrawableGameComponent.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "BasicMaterial.h"

namespace Library
{
	class Frustum;
	class BasicMaterial;

    class RenderableFrustum final : public DrawableGameComponent
    {
        RTTI_DECLARATIONS_ABSTRACT(RenderableFrustum, DrawableGameComponent)

    public:
		RenderableFrustum(Game& game, const std::shared_ptr<Camera>& camera, const DirectX::XMFLOAT4& color = DefaultColor);
		RenderableFrustum(const RenderableFrustum&) = delete;
		RenderableFrustum(RenderableFrustum&&) = default;
		RenderableFrustum& operator=(const RenderableFrustum&) = delete;
		RenderableFrustum& operator=(RenderableFrustum&&) = default;
		~RenderableFrustum() = default;

		const DirectX::XMFLOAT3& Position() const;
        const DirectX::XMFLOAT3& Direction() const;
        const DirectX::XMFLOAT3& Up() const;
        const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR PositionVector() const;
		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;

		void SetPosition(float x, float y, float z);
		void SetPosition(DirectX::FXMVECTOR position);
        void SetPosition(const DirectX::XMFLOAT3& position);

		void ApplyRotation(DirectX::CXMMATRIX transform);
        void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

		void SetColor(const DirectX::XMFLOAT4& color);

		void InitializeGeometry(const Frustum& frustum);

        virtual void Initialize() override;
        virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		inline static const DirectX::XMFLOAT4 DefaultColor{ DirectX::Colors::Green.f };

    private:
		inline static const std::uint32_t FrustumVertexCount{ 8 };
		inline static const std::uint32_t FrustumPrimitiveCount{ 12 };
		inline static const std::uint32_t FrustumIndicesPerPrimitive{ 2 };
		inline static const std::uint32_t FrustumIndexCount{ FrustumPrimitiveCount * FrustumIndicesPerPrimitive };
		inline static const std::uint16_t FrustumIndices[]
		{
			// Near plane lines
			0, 1,
			1, 2,
			2, 3,
			3, 0,

			// Sides
			0, 4,
			1, 5,
			2, 6,
			3, 7,

			// Far plane lines
			4, 5,
			5, 6,
			6, 7,
			7, 4
		};

		DirectX::XMFLOAT4X4 mWorldMatrix{ MatrixHelper::Identity };
		DirectX::XMFLOAT4 mColor;
		DirectX::XMFLOAT3 mPosition{ Vector3Helper::Zero };
		DirectX::XMFLOAT3 mDirection{ Vector3Helper::Forward };
		DirectX::XMFLOAT3 mUp{ Vector3Helper::Up };
		DirectX::XMFLOAT3 mRight{ Vector3Helper::Right };

		BasicMaterial mMaterial;
    	
		Buffer* mVertexBuffer{ nullptr };
		Buffer* mIndexBuffer{ nullptr };		

    	bool mUpdateWorldMatrix{ true };
		bool mUpdateMaterial{ true };
    };
}
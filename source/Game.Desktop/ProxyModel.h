#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <gsl/gsl>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"
#include "VectorHelper.h"
#include "BasicMaterial.h"

namespace Library
{
	class Mesh;

	class ProxyModel final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS_ABSTRACT(ProxyModel, DrawableGameComponent)

	public:
		ProxyModel(Game& game, const std::shared_ptr<Camera>& camera, const std::string& modelFileName, float scale = 1.0f);
		ProxyModel(const ProxyModel&) = delete;
		ProxyModel(ProxyModel&&) = default;
		ProxyModel& operator=(const ProxyModel&) = delete;		
		ProxyModel& operator=(ProxyModel&&) = default;
		~ProxyModel() = default;

		const DirectX::XMFLOAT3& Position() const;
		const DirectX::XMFLOAT3& Direction() const;
		const DirectX::XMFLOAT3& Up() const;
		const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR PositionVector() const;
		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;

		bool& DisplayWireframe();

		void SetPosition(float x, float y, float z);
		void SetPosition(DirectX::FXMVECTOR position);
		void SetPosition(const DirectX::XMFLOAT3& position);

		void ApplyRotation(DirectX::CXMMATRIX transform);
		void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

		void SetColor(const DirectX::XMFLOAT4& color);

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;		
		virtual void Draw(const GameTime& gameTime) override;

	private:
		DirectX::XMFLOAT4X4 mWorldMatrix{ MatrixHelper::Identity };
		DirectX::XMFLOAT3 mPosition{ Vector3Helper::Zero };
		DirectX::XMFLOAT3 mDirection{ Vector3Helper::Forward };
		DirectX::XMFLOAT3 mUp{ Vector3Helper::Up };
		DirectX::XMFLOAT3 mRight{ Vector3Helper::Right };
		BasicMaterial mMaterial;
		std::string mModelFileName;
		float mScale;
		
		Buffer* mVertexBuffer{ nullptr };
		Buffer* mIndexBuffer{ nullptr };		

		std::uint32_t mIndexCount{ 0 };
		bool mDisplayWireframe{ true };
		bool mUpdateWorldMatrix{ true };
		bool mUpdateMaterial{ true };
	};
}
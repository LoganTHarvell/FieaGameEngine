#pragma once

#include "Light.h"
#include "VectorHelper.h"

namespace Library
{
	class DirectionalLight : public Light
	{
		RTTI_DECLARATIONS(DirectionalLight, Light)

	public:
		DirectionalLight() = default;
		DirectionalLight(const DirectionalLight&) = default;
		DirectionalLight& operator=(const DirectionalLight&) = default;
		DirectionalLight& operator=(DirectionalLight&&) = default;
		DirectionalLight(DirectionalLight&&) = default;
		virtual ~DirectionalLight() = default;

		const DirectX::XMFLOAT3& Direction() const;
		DirectX::XMFLOAT3 DirectionToLight() const;
		const DirectX::XMFLOAT3& Up() const;
		const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR DirectionToLightVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;

		void ApplyRotation(DirectX::CXMMATRIX transform);
		void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

	protected:
		DirectX::XMFLOAT3 mDirection{ Vector3Helper::Forward };
		DirectX::XMFLOAT3 mUp{ Vector3Helper::Up };
		DirectX::XMFLOAT3 mRight{ Vector3Helper::Right };
	};
}
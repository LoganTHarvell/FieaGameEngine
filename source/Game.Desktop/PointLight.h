#pragma once

#include "Light.h"
#include "VectorHelper.h"

namespace Library
{
	class PointLight : public Light
	{
		RTTI_DECLARATIONS(PointLight, Light)

	public:
		explicit PointLight(const DirectX::XMFLOAT3& position = Vector3Helper::Zero, float radius = DefaultRadius);
		PointLight(const PointLight&) = default;
		PointLight& operator=(const PointLight&) = default;
		PointLight& operator=(PointLight&&) = default;
		PointLight(PointLight&&) = default;
		virtual ~PointLight() = default;

		const DirectX::XMFLOAT3& Position() const;
		DirectX::XMVECTOR PositionVector() const;
		float Radius() const;

		virtual void SetPosition(float x, float y, float z);
		virtual void SetPosition(DirectX::FXMVECTOR position);
		virtual void SetPosition(const DirectX::XMFLOAT3& position);
		virtual void SetRadius(float value);

		inline static const float DefaultRadius{ 50.0f };

	protected:
		DirectX::XMFLOAT3 mPosition;
		float mRadius;
	};
}
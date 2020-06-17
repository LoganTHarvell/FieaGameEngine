#pragma once

#include "PointLight.h"
#include "VectorHelper.h"

namespace Library
{
	class SpotLight : public PointLight
	{
		RTTI_DECLARATIONS(SpotLight, PointLight)

	public:
		explicit SpotLight(const DirectX::XMFLOAT3& position = Vector3Helper::Zero, float radius = DefaultRadius);
		virtual ~SpotLight() = default;
	
		const DirectX::XMFLOAT3& Direction() const;
		DirectX::XMFLOAT3 DirectionToLight() const;
		const DirectX::XMFLOAT3& Up() const;
		const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR DirectionToLightVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;

		float InnerAngle();
		void SetInnerAngle(float value);
		
		float OuterAngle();
		void SetOuterAngle(float value);

		void ApplyRotation(DirectX::CXMMATRIX transform);
		void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

		inline static const float DefaultInnerAngle{ 0.75f };
		inline static const float DefaultOuterAngle{ 0.25f };

	protected:
		DirectX::XMFLOAT3 mDirection{ Vector3Helper::Forward };
		DirectX::XMFLOAT3 mUp{ Vector3Helper::Up };
		DirectX::XMFLOAT3 mRight{ Vector3Helper::Right };
		float mInnerAngle{ DefaultInnerAngle };
		float mOuterAngle{ DefaultOuterAngle };
	};
}
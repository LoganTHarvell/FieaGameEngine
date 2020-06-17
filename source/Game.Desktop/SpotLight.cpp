#include "pch.h"
#include "SpotLight.h"

using namespace DirectX;

namespace Library
{
	SpotLight::SpotLight(const XMFLOAT3& position, float radius) :
		PointLight(position, radius)
	{
	}

	const XMFLOAT3& SpotLight::Direction() const
	{
		return mDirection;
	}

	XMFLOAT3 SpotLight::DirectionToLight() const
	{
		return XMFLOAT3(-mDirection.x, -mDirection.y, -mDirection.z);
	}
	
	const XMFLOAT3& SpotLight::Up() const
	{
		return mUp;
	}

	const XMFLOAT3& SpotLight::Right() const
	{
		return mRight;
	}

	XMVECTOR SpotLight::DirectionVector() const
	{
		return XMLoadFloat3(&mDirection);
	}

	XMVECTOR SpotLight::DirectionToLightVector() const
	{
		return XMVectorNegate(XMLoadFloat3(&mDirection));
	}

	XMVECTOR SpotLight::UpVector() const
	{
		return XMLoadFloat3(&mUp);
	}
	
	XMVECTOR SpotLight::RightVector() const
	{
		return XMLoadFloat3(&mRight);
	}

	float SpotLight::InnerAngle()
	{
		return mInnerAngle;
	}

	void SpotLight::SetInnerAngle(float value)
	{
		mInnerAngle = value;
	}

	float SpotLight::OuterAngle()
	{
		return mOuterAngle;
	}

	void SpotLight::SetOuterAngle(float value)
	{
		mOuterAngle = value;
	}

	void SpotLight::ApplyRotation(CXMMATRIX transform)
    {
        XMVECTOR direction = XMLoadFloat3(&mDirection);
        XMVECTOR up = XMLoadFloat3(&mUp);
        
        direction = XMVector3TransformNormal(direction, transform);
        direction = XMVector3Normalize(direction);

        up = XMVector3TransformNormal(up, transform);
        up = XMVector3Normalize(up);

        XMVECTOR right = XMVector3Cross(direction, up);
        up = XMVector3Cross(right, direction);

        XMStoreFloat3(&mDirection, direction);
        XMStoreFloat3(&mUp, up);
        XMStoreFloat3(&mRight, right);
    }

    void SpotLight::ApplyRotation(const XMFLOAT4X4& transform)
    {
        XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
        ApplyRotation(transformMatrix);
    }
}
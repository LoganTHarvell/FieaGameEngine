#include "pch.h"
#include "DirectionalLight.h"

using namespace DirectX;

namespace Library
{
	const XMFLOAT3& DirectionalLight::Direction() const
	{
		return mDirection;
	}

	XMFLOAT3 DirectionalLight::DirectionToLight() const
	{
		return XMFLOAT3(-mDirection.x, -mDirection.y, -mDirection.z);
	}
	
	const XMFLOAT3& DirectionalLight::Up() const
	{
		return mUp;
	}

	const XMFLOAT3& DirectionalLight::Right() const
	{
		return mRight;
	}

	XMVECTOR DirectionalLight::DirectionVector() const
	{
		return XMLoadFloat3(&mDirection);
	}

	XMVECTOR DirectionalLight::DirectionToLightVector() const
	{
		return XMVectorNegate(XMLoadFloat3(&mDirection));
	}

	XMVECTOR DirectionalLight::UpVector() const
	{
		return XMLoadFloat3(&mUp);
	}
	
	XMVECTOR DirectionalLight::RightVector() const
	{
		return XMLoadFloat3(&mRight);
	}

	void DirectionalLight::ApplyRotation(CXMMATRIX transform)
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

    void DirectionalLight::ApplyRotation(const XMFLOAT4X4& transform)
    {
        XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
        ApplyRotation(transformMatrix);
    }
}
#include "pch.h"
#include "PointLight.h"

using namespace DirectX;

namespace Library
{
	PointLight::PointLight(const DirectX::XMFLOAT3& position, float radius) :
		mPosition(position), mRadius(radius)
	{
	}

	const XMFLOAT3& PointLight::Position() const
	{
		return mPosition;
	}

	XMVECTOR PointLight::PositionVector() const
	{
		return XMLoadFloat3(&mPosition);
	}

	float PointLight::Radius() const
	{
		return mRadius;
	}

	void PointLight::SetPosition(float x, float y, float z)
    {
        XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
        SetPosition(position);
    }

    void PointLight::SetPosition(FXMVECTOR position)
    {
        XMStoreFloat3(&mPosition, position);
    }

    void PointLight::SetPosition(const XMFLOAT3& position)
    {
        mPosition = position;
    }

	void PointLight::SetRadius(float value)
	{
		mRadius = value;
	}
}
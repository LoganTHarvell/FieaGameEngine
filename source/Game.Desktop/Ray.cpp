#include "pch.h"
#include "Ray.h"

using namespace DirectX;

namespace Library
{
    Ray::Ray(FXMVECTOR position, FXMVECTOR direction)
	{
		XMStoreFloat3(&mPosition, position);
		XMStoreFloat3(&mDirection, direction);
	}

	Ray::Ray(const XMFLOAT3& position, const XMFLOAT3& direction) :
		mPosition(position), mDirection(direction)
	{
	}

	const XMFLOAT3& Ray::Position() const
    {
        return mPosition;
    }

    const XMFLOAT3& Ray::Direction() const
    {
        return mDirection;
    }

	XMVECTOR Ray::PositionVector() const
    {
        return XMLoadFloat3(&mPosition);
    }

    XMVECTOR Ray::DirectionVector() const
    {
        return XMLoadFloat3(&mDirection);
    }

	void Ray::SetPosition(float x, float y, float z)
    {
        XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
        SetPosition(position);
    }

    void Ray::SetPosition(FXMVECTOR position)
    {
        XMStoreFloat3(&mPosition, position);
    }

    void Ray::SetPosition(const XMFLOAT3& position)
    {
        mPosition = position;
    }

	void Ray::SetDirection(float x, float y, float z)
    {
        XMVECTOR direction = XMVectorSet(x, y, z, 0.0f);
        SetDirection(direction);
    }

    void Ray::SetDirection(FXMVECTOR direction)
    {
        XMStoreFloat3(&mDirection, direction);
    }

    void Ray::SetDirection(const XMFLOAT3& direction)
    {
        mDirection = direction;
    }
}
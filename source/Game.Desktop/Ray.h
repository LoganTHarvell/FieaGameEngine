#pragma once

#include <DirectXMath.h>
#include "RTTI.h"

namespace Library
{
    class Ray final : public RTTI
    {
		RTTI_DECLARATIONS(Ray, RTTI)

    public:
		Ray() = default;
		Ray(DirectX::FXMVECTOR position, DirectX::FXMVECTOR direction);
		Ray(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& direction);
		Ray(const Ray&) = default;
		Ray& operator=(const Ray&) = default;
		Ray(Ray&&) = default;
		Ray& operator=(Ray&&) = default;
		~Ray() = default;

		const DirectX::XMFLOAT3& Position() const;
        const DirectX::XMFLOAT3& Direction() const;

		DirectX::XMVECTOR PositionVector() const;
		DirectX::XMVECTOR DirectionVector() const;

		virtual void SetPosition(float x, float y, float z);
		virtual void SetPosition(DirectX::FXMVECTOR position);
        virtual void SetPosition(const DirectX::XMFLOAT3& position);

		virtual void SetDirection(float x, float y, float z);
		virtual void SetDirection(DirectX::FXMVECTOR direction);
        virtual void SetDirection(const DirectX::XMFLOAT3& direction);
    
	private:
		DirectX::XMFLOAT3 mPosition{ 0, 0, 0 };
		DirectX::XMFLOAT3 mDirection{ 0, 0, 0 };
    };
}
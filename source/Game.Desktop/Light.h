#pragma once

#include "RTTI.h"
#include "ColorHelper.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

namespace Library
{
	class Light : public RTTI
	{
		RTTI_DECLARATIONS(Light, RTTI)

	public:
		Light();
		Light(const Light&) = default;
		Light& operator=(const Light&) = default;
		Light& operator=(Light&&) = default;
		Light(Light&&) = default;
		virtual ~Light() = default;

		const DirectX::XMFLOAT4& Color() const;
		DirectX::PackedVector::XMCOLOR XmColor() const;
		DirectX::XMVECTOR ColorVector() const;
		void SetColor(float r, float g, float b, float a);
		void SetColor(const DirectX::XMFLOAT4& color);
		void SetColor(const DirectX::PackedVector::XMCOLOR& color);
		void SetColor(DirectX::FXMVECTOR color);

	protected:
		DirectX::XMFLOAT4 mColor;
	};
}

#include "Light.inl"
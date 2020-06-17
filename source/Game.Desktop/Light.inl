#pragma once

namespace Library
{
	inline Light::Light() :
		mColor(DirectX::Colors::White)
	{
	}

	inline const DirectX::XMFLOAT4& Light::Color() const
	{
		return mColor;
	}

	inline DirectX::PackedVector::XMCOLOR Light::XmColor() const
	{
		return DirectX::PackedVector::XMCOLOR(reinterpret_cast<const float*>(&mColor));
	}

	inline DirectX::XMVECTOR Light::ColorVector() const
	{
		return XMLoadFloat4(&mColor);
	}

	inline void Light::SetColor(float r, float g, float b, float a)
	{
		mColor = DirectX::XMFLOAT4(r, g, b, a);
	}

	inline void Light::SetColor(const DirectX::XMFLOAT4& color)
	{
		mColor = color;
	}

	inline void Light::SetColor(const DirectX::PackedVector::XMCOLOR& color)
	{
		mColor = ColorHelper::ToFloat4(color, true);
	}

	inline void Light::SetColor(DirectX::FXMVECTOR color)
	{
		XMStoreFloat4(&mColor, color);
	}
}
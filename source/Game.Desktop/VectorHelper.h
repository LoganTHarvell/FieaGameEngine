#pragma once

#include <DirectXMath.h>

namespace Library
{
	class Vector2Helper final
	{
	public:
		static const DirectX::XMFLOAT2 Zero;
		static const DirectX::XMFLOAT2 One;

		static std::string ToString(const DirectX::XMFLOAT2& vector);
		static bool Equals(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs);

		Vector2Helper() = delete;
		Vector2Helper(const Vector2Helper&) = delete;
		Vector2Helper& operator=(const Vector2Helper&) = delete;
		Vector2Helper(Vector2Helper&&) = delete;
		Vector2Helper& operator=(Vector2Helper&&) = delete;
		~Vector2Helper() = default;
	};

	class Vector3Helper final
	{
	public:
		static const DirectX::XMFLOAT3 Zero;
		static const DirectX::XMFLOAT3 One;
		static const DirectX::XMFLOAT3 Forward;
		static const DirectX::XMFLOAT3 Backward;
		static const DirectX::XMFLOAT3 Up;
		static const DirectX::XMFLOAT3 Down;
		static const DirectX::XMFLOAT3 Right;
		static const DirectX::XMFLOAT3 Left;

		static std::string ToString(const DirectX::XMFLOAT3& vector);
		static DirectX::XMFLOAT3 ToFloat3(const DirectX::XMFLOAT4& vector);
		static bool Equals(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);

		Vector3Helper() = delete;
		Vector3Helper(const Vector3Helper&) = delete;
		Vector3Helper& operator=(const Vector3Helper&) = delete;
		Vector3Helper(Vector3Helper&&) = delete;
		Vector3Helper& operator=(Vector3Helper&&) = delete;
		~Vector3Helper() = default;
	};

	class Vector4Helper final
	{
	public:
		static const DirectX::XMFLOAT4 Zero;
		static const DirectX::XMFLOAT4 One;

		static std::string ToString(const DirectX::XMFLOAT4& vector);
		static bool Equals(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs);

		Vector4Helper() = delete;
		Vector4Helper(const Vector3Helper&) = delete;
		Vector4Helper& operator=(const Vector3Helper&) = delete;
		Vector4Helper(Vector4Helper&&) = delete;
		Vector4Helper& operator=(Vector4Helper&&) = delete;
		~Vector4Helper() = default;
	};
}

#include "VectorHelper.inl"
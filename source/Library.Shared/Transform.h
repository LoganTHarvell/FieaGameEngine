#pragma once

namespace Library
{
	/// <summary>
	/// Represents a 3D transform matrix.
	/// </summary>
	class Transform final
	{
#pragma region Constants
	public:
		static const Transform Identity;
		static const Transform Zero;
#pragma endregion Constants

#pragma region Special Members
	public:
		Transform() = default;
		~Transform() = default;
		Transform(const Transform& rhs) = default;
		Transform& operator=(const Transform& rhs) = default;
		Transform(Transform&& rhs) = default;
		Transform& operator=(Transform&& rhs) = default;
		
		explicit Transform(const float value);
		
		Transform(	const float& x0, const float& y0, const float& z0, const float& w0,
		            const float& x1, const float& y1, const float& z1, const float& w1,
		            const float& x2, const float& y2, const float& z2, const float& w2,
		            const float& x3, const float& y3, const float& z3, const float& w3	);

		Transform(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3, const glm::vec4& v4);
		
		Transform(const glm::vec3& translation, const glm::quat& quaternion, const glm::vec3& scaling);
		
		explicit Transform(const glm::mat4x4& matrix);
#pragma endregion Special Members

#pragma region Boolean Operators
		bool operator==(const Transform& rhs) const noexcept;
		bool operator!=(const Transform& rhs) const noexcept;
#pragma endregion Boolean Operators
		
#pragma region Arithmetic Operators
	public:
		Transform operator+(const Transform& rhs) const;
		Transform operator+(const float rhs) const;
		friend Transform operator+(const float lhs, const Transform& rhs);

		Transform& operator+=(const Transform& rhs);
		Transform& operator+=(const float rhs);

		Transform operator-(const Transform& rhs) const;
		Transform operator-(const float rhs) const;
		friend Transform operator-(const float lhs, const Transform& rhs);

		Transform& operator-=(const Transform& rhs);
		Transform& operator-=(const float rhs);

		Transform operator*(const Transform& rhs) const;
		Transform operator*(const float rhs) const;
		glm::vec4 operator*(const glm::vec4& rhs) const;
		friend glm::vec4 operator*(const glm::vec4& lhs, const Transform& rhs);

		Transform& operator*=(const Transform& rhs);
		Transform& operator*=(const float rhs);

		Transform operator/(const Transform& rhs) const;
		Transform operator/(const float rhs) const;
		glm::vec4 operator/(const glm::vec4& rhs) const;
		friend glm::vec4 operator/(const glm::vec4& lhs, const Transform& rhs);

		Transform& operator/=(const Transform& rhs);
		Transform& operator/=(const float rhs);
#pragma endregion Arithmetic Operators

#pragma region Accessors
		glm::vec4& operator[](const int i);
		const glm::vec4& operator[](const int i) const;

		float Determinant() const;
		Transform Inverse() const;
#pragma endregion Accessors

#pragma region Transformations
	public:
		Transform& Translate(const glm::vec3& translation);
		static Transform Translate(const Transform& transform, const glm::vec3& translation);
		
		Transform& Rotate(const float angle, const glm::vec3& axis);
		static Transform Rotate(const Transform& transform, const float angle, const glm::vec3& axis);

		Transform& Scale(const glm::vec3& scaling);
		static Transform Scale(const Transform& transform, const glm::vec3& scaling);

		Transform& Transformation(const glm::vec3& translation, const glm::quat& quaternion, const glm::vec3& scaling);
		static Transform Transformation(const Transform& transform, const glm::vec3& translation, const glm::quat& quaternion, const glm::vec3& scaling);
#pragma endregion Transformations

	private:
		glm::mat4x4 mMatrix{ 0 };
	};
}

#include "Transform.inl"
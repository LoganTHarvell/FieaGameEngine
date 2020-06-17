#pragma once

// Third Party
#pragma warning(disable : 4201)
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#pragma warning(default : 4201)

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
		
		Transform(const glm::vec3& translation, const glm::quat& quaternion, const glm::vec3& scaling);
		Transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scaling);
#pragma endregion Special Members

#pragma region Boolean Operators
	public:
		bool operator==(const Transform& rhs) const noexcept;
		bool operator!=(const Transform& rhs) const noexcept;
#pragma endregion Boolean Operators
		
#pragma region Arithmetic Operators
	public:
		Transform operator+(const Transform& rhs) const;
		Transform& operator+=(const Transform& rhs);

		Transform operator-(const Transform& rhs) const;
		Transform& operator-=(const Transform& rhs);

		Transform operator*(const Transform& rhs) const;
		Transform& operator*=(const Transform& rhs);
		glm::vec4 operator*(const glm::vec4& rhs) const;
		Transform operator*(const float rhs) const;
		friend glm::vec4 operator*(const glm::vec4& lhs, const Transform& rhs);
		Transform& operator*=(const float rhs);

		Transform operator/(const float rhs) const;
		Transform& operator/=(const float rhs);
		glm::vec4 operator/(const glm::vec4& rhs) const;
		friend glm::vec4 operator/(const glm::vec4& lhs, const Transform& rhs);
#pragma endregion Arithmetic Operators

#pragma region Accessors
	public:
		const glm::vec4& operator[](const int i) const;

		float Determinant() const;

		glm::vec3& Translation();
		const glm::vec3& Translation() const;

		glm::quat& Rotation();
		const glm::quat& Rotation() const;
		glm::vec3 EulerRotation() const;

		glm::vec3& Scale();
		const glm::vec3& Scale() const;

		const glm::mat4x4& Matrix() const;
#pragma endregion Accessors

#pragma region Transformations
	public:
		glm::mat4x4 Inverse() const;
		glm::mat4x4 Transpose() const;

		Transform& Translate(const glm::vec3& translation);
		static Transform Translate(Transform transform, const glm::vec3& translation);
		
		Transform& Rotate(const float angle, const glm::vec3& axis);
		static Transform Rotate(Transform transform, const float angle, const glm::vec3& axis);
		
		Transform& Rotate(const glm::quat& quaternion);
		static Transform Rotate(Transform transform, const glm::quat& quaternion);

		Transform& Scale(const glm::vec3& scaling);
		static Transform Scale(Transform transform, const glm::vec3& scaling);

		Transform& Transformation(const glm::vec3& translation, const glm::quat& quaternion, const glm::vec3& scaling);
		static Transform Transformation(Transform transform, const glm::vec3& translation, const glm::quat& quaternion, const glm::vec3& scaling);
#pragma endregion Transformations

	private:
		glm::vec3 mTranslation{ 0 };
		glm::quat mRotation{ 0, 0, 0, 1 };
		glm::vec3 mScale{ 1 };
		
		mutable glm::mat4x4 mMatrix{ 0 };
		mutable bool mUpdateMatrix{ true };
	};
}

// Inline File
#include "Transform.inl"
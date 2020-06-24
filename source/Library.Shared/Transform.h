#pragma once

#pragma region Includes
// Third Party
#pragma warning(disable : 4201)
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#pragma warning(default : 4201)
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents a three-dimensional transformation comprising translation, rotation, and scale.
	/// </summary>
	class Transform final
	{
#pragma region Constants
	public:
		/// <summary>
		/// Identity Transform.
		/// </summary>
		static const Transform Identity;

		/// <summary>
		/// Zero transform.
		/// </summary>
		static const Transform Zero;
#pragma endregion Constants

#pragma region Special Members
	public:
		/* Defaulted Special Members*/
		Transform() = default;
		~Transform() = default;
		Transform(const Transform& rhs) = default;
		Transform& operator=(const Transform& rhs) = default;
		Transform(Transform&& rhs) = default;
		Transform& operator=(Transform&& rhs) = default;

		/// <summary>
		/// Specialized constructor for a transformation with the given translation, rotation, and scale.
		/// </summary>
		/// <param name="translation">Translation of the new Transform.</param>
		/// <param name="quaternion">Rotation of the new Transform.</param>
		/// <param name="scaling">Scale of the new Transform</param>
		Transform(const glm::vec3& translation, const glm::quat& quaternion, const glm::vec3& scaling);

		/// <summary>
		/// Specialized constructor for a transformation with the given translation, rotation, and scale.
		/// </summary>
		/// <param name="translation">Translation of the new Transform.</param>
		/// <param name="rotation">Rotation of the new Transform, as Euler angles in radians.</param>
		/// <param name="scaling">Scale of the new Transform</param>
		Transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scaling);
#pragma endregion Special Members

#pragma region Boolean Operators
	public:
		/// <summary>
		/// Boolean equal operator.
		/// </summary>
		/// <param name="rhs">Transform to be compared.</param>
		/// <returns>True if the Transforms have the same translation, rotation, and scale. Otherwise, false.</returns>
		bool operator==(const Transform& rhs) const noexcept;

		/// <summary>
		/// Boolean non-equal operator.
		/// </summary>
		/// <param name="rhs">Transform to be compared.</param>
		/// <returns>True if the Transforms have the same translation, rotation, and scale. Otherwise, false.</returns>
		bool operator!=(const Transform& rhs) const noexcept;
#pragma endregion Boolean Operators
		
#pragma region Arithmetic Operators
	public:
		/// <summary>
		/// Addition operator.
		/// </summary>
		/// <param name="rhs">Transform to be added.</param>
		/// <returns>Transform result from the addition.</returns>
		Transform operator+(const Transform& rhs) const;

		/// <summary>
		/// Addition assignment operator.
		/// </summary>
		/// <param name="rhs">Transform to be added.</param>
		/// <returns>Reference to the newly modified Transform.</returns>
		Transform& operator+=(const Transform& rhs);

		/// <summary>
		/// Subtraction operator.
		/// </summary>
		/// <param name="rhs">Transform to be subtracted.</param>
		/// <returns>Transform result from the subtraction.</returns>
		Transform operator-(const Transform& rhs) const;

		/// <summary>
		/// Subtraction assignment operator.
		/// </summary>
		/// <param name="rhs">Transform to be subtracted.</param>
		/// <returns>Reference to the newly modified Transform.</returns>
		Transform& operator-=(const Transform& rhs);

		/// <summary>
		/// Multiplication operator.
		/// </summary>
		/// <param name="rhs">Transform to be multiplied.</param>
		/// <returns>Transform result from the multiplication.</returns>
		Transform operator*(const Transform& rhs) const;

		/// <summary>
		/// Multiplication assignment operator.
		/// </summary>
		/// <param name="rhs">Transform to be multiplied.</param>
		/// <returns>Reference to the newly modified Transform.</returns>
		Transform& operator*=(const Transform& rhs);

		/// <summary>
		/// Scalar multiplication operator.
		/// </summary>
		/// <param name="rhs">Scaling value as a float.</param>
		/// <returns>Transform scaled by a scalar float.</returns>
		Transform operator*(const float rhs) const;

		/// <summary>
		/// Scalar multiplication assignment operator.
		/// </summary>
		/// <param name="rhs">Scaling value as a float.</param>
		/// <returns>Reference to the newly modified Transform.</returns>
		Transform& operator*=(const float rhs);

		/// <summary>
		/// Vector transformation operator. (column major)
		/// </summary>
		/// <param name="rhs">Vector to be transformed.</param>
		/// <returns>Transformed vector.</returns>
		glm::vec4 operator*(const glm::vec4& rhs) const;

		/// <summary>
		/// Vector transformation operator. (row major)
		/// </summary>
		/// <param name="lhs">Vector to be transformed.</param>
		/// <param name="rhs">Transform representing the transformation.</param>
		/// <returns>Transformed vector.</returns>
		friend glm::vec4 operator*(const glm::vec4& lhs, const Transform& rhs);

		/// <summary>
		/// Scalar division operator.
		/// </summary>
		/// <param name="rhs">Scaling value as a float.</param>
		/// <returns>Transform scaled by a scalar float.</returns>
		Transform operator/(const float rhs) const;

		/// <summary>
		/// Scalar division assignment operator.
		/// </summary>
		/// <param name="rhs">Scaling value as a float.</param>
		/// <returns>Reference to the newly modified Transform.</returns>
		Transform& operator/=(const float rhs);

		/// <summary>
		/// Vector transformation operator. (column major)
		/// </summary>
		/// <param name="rhs">Vector to be transformed.</param>
		/// <returns>Transformed vector.</returns>
		glm::vec4 operator/(const glm::vec4& rhs) const;

		/// <summary>
		/// Vector transformation operator. (row major)
		/// </summary>
		/// <param name="lhs">Vector to be transformed.</param>
		/// <param name="rhs">Transform representing the transformation.</param>
		/// <returns>Transformed vector.</returns>
		friend glm::vec4 operator/(const glm::vec4& lhs, const Transform& rhs);
#pragma endregion Arithmetic Operators

#pragma region Accessors
	public:
		/// <summary>
		/// Gets the Transform translation component.
		/// </summary>
		/// <returns>Translation vector of the Transform.</returns>
		glm::vec3& Translation();
		
		/// <summary>
		/// Gets the Transform translation component.
		/// </summary>
		/// <returns>Translation vector of the Transform.</returns>
		const glm::vec3& Translation() const;

		/// <summary>
		/// Gets the Transform rotation component.
		/// </summary>
		/// <returns>Rotation quaternion of the Transform.</returns>
		glm::quat& Rotation();
		
		/// <summary>
		/// Gets the Transform rotation component.
		/// </summary>
		/// <returns>Rotation quaternion of the Transform.</returns>
		const glm::quat& Rotation() const;

		/// <summary>
		/// Gets the rotation of the Transform as a vector of Euler angles.
		/// </summary>
		/// <returns>Vector representing the Transform rotation as Euler angles.</returns>
		glm::vec3 EulerRotation() const;

		/// <summary>
		/// Gets the Transform scale component.
		/// </summary>
		/// <returns>Scale vector of the Transform.</returns>
		glm::vec3& Scale();

		/// <summary>
		/// Gets the Transform scale component.
		/// </summary>
		/// <returns>Scale vector of the Transform.</returns>
		const glm::vec3& Scale() const;

		/// <summary>
		/// Gets the inverse Transform.
		/// </summary>
		/// <returns>Inverse Transform.</returns>
		Transform Inverse() const;

		/// <summary>
		/// Gets the inverse Transform.
		/// </summary>
		/// <returns>Inverse Transform.</returns>
		static Transform Inverse(Transform transform);
		
		/// <summary>
		/// Gets the transformation matrix represented by this Transform.
		/// </summary>
		/// <returns>Transform as a glm::mat4x4.</returns>
		const glm::mat4x4& Matrix() const;

		/// <summary>
		/// Dereference operator for accessing the columns of the matrix.
		/// </summary>
		/// <param name="i">Column index.</param>
		/// <returns>Column vector at the given index.</returns>
		const glm::vec4& operator[](const int i) const;

		/// <summary>
		/// Gets the determinant of the transformation matrix.
		/// </summary>
		/// <returns>Determinant of the transformation matrix.</returns>
		float Determinant() const;

		/// <summary>
		/// Gets the inverse transformation matrix.
		/// </summary>
		/// <returns>Inverse transformation matrix.</returns>
		glm::mat4x4 InverseMatrix() const;
		
		/// <summary>
		/// Gets the transposed transformation matrix.
		/// </summary>
		/// <returns>Transposed transformation matrix.</returns>
		glm::mat4x4 Transpose() const;
#pragma endregion Accessors

#pragma region Transformations
	public:
		/// <summary>
		/// Translates the Transform.
		/// </summary>
		/// <param name="translation">Vector used to translate the Transform.</param>
		/// <returns>Reference to the modified Transform.</returns>
		Transform& Translate(const glm::vec3& translation);

		/// <summary>
		/// Gets a translated Transform.
		/// </summary>
		/// <param name="transform">Input Transform.</param>
		/// <param name="translation">Vector used to translate the Transform.</param>
		/// <returns>A copy of the given transform with the translation applied.</returns>
		static Transform Translate(Transform transform, const glm::vec3& translation);

		/// <summary>
		/// Rotates the Transform.
		/// </summary>
		/// <param name="angle">Rotation angle in radians.</param>
		/// <param name="axis">Rotation axis vector, recommended to be normalized.</param>
		/// <returns>Reference to the modified Transform.</returns>
		Transform& Rotate(const float angle, const glm::vec3& axis);

		/// <summary>
		/// Gets a rotated Transform.
		/// </summary>
		/// <param name="transform">Input Transform.</param>
		/// <param name="angle">Rotation angle in radians.</param>
		/// <param name="axis">Rotation axis vector, recommended to be normalized.</param>
		/// <returns>Reference to the modified Transform.</returns>
		/// <returns>A copy of the given transform with the rotation applied.</returns>
		static Transform Rotate(Transform transform, const float angle, const glm::vec3& axis);

		/// <summary>
		/// Rotates the Transform.
		/// </summary>
		/// <param name="quaternion">Rotation as a quaternion.</param>
		/// <returns>Reference to the modified Transform.</returns>
		Transform& Rotate(const glm::quat& quaternion);

		/// <summary>
		/// Gets a rotated Transform.
		/// </summary>
		/// <param name="transform">Input Transform.</param>
		/// <param name="quaternion">Rotation as a quaternion.</param>
		/// <returns>A copy of the given transform with the rotation applied.</returns>
		static Transform Rotate(Transform transform, const glm::quat& quaternion);

		/// <summary>
		/// Scales the Transform.
		/// </summary>
		/// <param name="scaling">Vector used to scale the Transform.</param>
		/// <returns>Reference to the modified Transform.</returns>
		Transform& Scale(const glm::vec3& scaling);

		/// <summary>
		/// Gets a scaled Transform.
		/// </summary>
		/// <param name="transform">Input Transform.</param>
		/// <param name="scaling">Vector used to scale the Transform.</param>
		/// <returns>A copy of the given transform with the scale applied.</returns>
		static Transform Scale(Transform transform, const glm::vec3& scaling);

		/// <summary>
		/// Transforms the Transform.
		/// </summary>
		/// <param name="transformation">Transform representing a transformation.</param>
		Transform& Transformation(const Transform& transformation);

		/// <summary>
		/// Gets a transformed Transform.
		/// </summary>
		/// <param name="transform">Input Transform.</param>
		/// <param name="transformation">Transform representing a transformation.</param>
		/// <returns>A copy of the given transform with the transformation applied.</returns>
		static Transform Transformation(Transform transform, const Transform& transformation);
#pragma endregion Transformations

	private:
		/// <summary>
		/// Translation vector.
		/// </summary>
		glm::vec3 mTranslation{ 0 };

		/// <summary>
		/// Rotation quaternion.
		/// </summary>
		glm::quat mRotation{ 0, 0, 0, 1 };

		/// <summary>
		/// Scale vector.
		/// </summary>
		glm::vec3 mScale{ 1 };

		/// <summary>
		/// Cached transformation matrix.
		/// </summary>
		mutable glm::mat4x4 mMatrix{ 0 };

		/// <summary>
		/// Dirty flag for the transformation matrix cache.
		/// </summary>
		mutable bool mDirtyMatrix{ true };
	};
}

// Inline File
#include "Transform.inl"
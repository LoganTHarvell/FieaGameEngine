#pragma once

// Header
#include "Transform.h"

namespace Library
{
#pragma region Special Members
	inline Transform::Transform(const glm::vec3& translation, const glm::quat& quaternion, const glm::vec3& scaling) :
		mTranslation(translation), mRotation(glm::eulerAngles(quaternion)), mScale(scaling)
	{
	}

	inline Transform::Transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scaling) :
		mTranslation(translation), mRotation(rotation), mScale(scaling)
	{
	}
#pragma endregion Special Members

#pragma region Boolean Operators
	inline bool Transform::operator==(const Transform& rhs) const noexcept
	{
		return Matrix() == rhs.Matrix();
	}

	inline bool Transform::operator!=(const Transform& rhs) const noexcept
	{
		return !operator==(rhs);
	}
#pragma endregion Boolean Operators

#pragma region Arithmetic Operators
	inline Transform Transform::operator+(const Transform& rhs) const
	{
		return Transform(mTranslation + rhs.mTranslation, mRotation + rhs.mRotation, mScale + rhs.mScale);
	}

	inline Transform& Transform::operator+=(const Transform& rhs)
	{
		mDirtyMatrix = true;
		return *this = Transform(*this) + rhs;
	}
	
	inline Transform Transform::operator-(const Transform& rhs) const
	{
		return Transform(mTranslation - rhs.mTranslation, mRotation - rhs.mRotation, mScale - rhs.mScale);
	}

	inline Transform& Transform::operator-=(const Transform& rhs)
	{
		mDirtyMatrix = true;
		return *this = Transform(*this) + rhs;
	}
	
	inline Transform Transform::operator*(const Transform& rhs) const
	{
		// RotationResult rhs.Rotation * lhs.Rotation
		const glm::quat rotation = mRotation * rhs.mRotation;

		// TranslateResult = rhs.Rotate(rhs.Scale * lhs.Translation) + rhs.Translate
		const glm::vec4 scaledTranslation = { mTranslation * rhs.mScale, 0 };
		const glm::vec4 rotatedTranslate = glm::rotate(rhs.mRotation, scaledTranslation);
		const glm::vec3 translation = glm::vec3(rotatedTranslate) + rhs.mTranslation;

		// ScaleResult = rhs.Scale * lhs.Scale
		const glm::vec3 scale = mScale * rhs.mScale;
		
		return Transform(translation, rotation, scale);
	}

	inline Transform& Transform::operator*=(const Transform& rhs)
	{
		mDirtyMatrix = true;
		return *this = Transform(*this) * rhs;
	}

	inline Transform Transform::operator*(const float rhs) const
	{
		return Transform(mTranslation * rhs, mRotation * rhs, mScale * rhs);
	}

	inline Transform& Transform::operator*=(const float rhs)
	{
		mDirtyMatrix = true;
		return *this = Transform(*this) * rhs;
	}

	inline glm::vec4 Transform::operator*(const glm::vec4& rhs) const
	{
		return Matrix() * rhs;
	}

	inline glm::vec4 operator*(const glm::vec4& lhs, const Transform& rhs)
	{
		return lhs * rhs.Matrix();
	}

	inline Transform Transform::operator/(const float rhs) const
	{
		return Transform(mTranslation / rhs, mRotation / rhs, mScale / rhs);
	}

	inline Transform& Transform::operator/=(const float rhs)
	{
		mDirtyMatrix = true;
		return *this = Transform(*this) / rhs;
	}

	inline glm::vec4 Transform::operator/(const glm::vec4& rhs) const
	{
		return Matrix() / rhs;
	}

	inline glm::vec4 operator/(const glm::vec4& lhs, const Transform& rhs)
	{
		return lhs / rhs.Matrix();
	}
#pragma endregion Arithmetic Operators

#pragma region Accessors
	inline glm::vec3& Transform::Translation()
	{
		return mTranslation;
	}

	inline const glm::vec3& Transform::Translation() const
	{
		return mTranslation;
	}

	inline glm::quat& Transform::Rotation()
	{
		return mRotation;
	}

	inline const glm::quat& Transform::Rotation() const
	{
		return mRotation;
	}

	inline glm::vec3 Transform::EulerRotation() const
	{
		return glm::eulerAngles(mRotation);
	}

	inline glm::vec3& Transform::Scale()
	{
		return mScale;
	}

	inline const glm::vec3& Transform::Scale() const
	{
		return mScale;
	}

	inline Transform Transform::Inverse() const
	{
		Transform inverse;

		inverse.mScale = glm::vec3(1) / mScale;
		inverse.mRotation = glm::inverse(mRotation);
		inverse.mTranslation = inverse.mRotation * (inverse.mScale * -mTranslation);

		return inverse;
	}

	inline Transform Transform::Inverse(Transform transform)
	{
		transform.mScale = glm::vec3(1) / transform.mScale;
		transform.mRotation = glm::inverse(transform.mRotation);
		transform.mTranslation = transform.mRotation * (transform.mScale * -transform.mTranslation);

		return transform;
	}

	inline const glm::mat4x4& Transform::Matrix() const
	{
		if (mDirtyMatrix)
		{
			mMatrix = glm::mat4x4(glm::translate(mTranslation) * glm::mat4_cast(mRotation) * glm::scale(mScale));
			mDirtyMatrix = false;
		}

		return mMatrix;
	}

	inline const glm::vec4& Transform::operator[](const int i) const
	{
		return Matrix()[i];
	}

	inline float Transform::Determinant() const
	{
		return glm::determinant(Matrix());
	}

	inline glm::mat4x4 Transform::InverseMatrix() const
	{
		return glm::inverse(Matrix());
	}

	inline glm::mat4x4 Transform::Transpose() const
	{
		return glm::transpose(Matrix());
	}
#pragma endregion Accessors

#pragma region Transformations
	inline Transform& Transform::Translate(const glm::vec3& translation)
	{
		mTranslation += translation;
		mDirtyMatrix = true;
		return *this;
	}

	inline Transform Transform::Translate(Transform transform, const glm::vec3& translation)
	{
		return transform.Translate(translation);
	}

	inline Transform& Transform::Rotate(const float angle, const glm::vec3& axis)
	{
		mRotation = mRotation * glm::angleAxis(angle, axis);
		mDirtyMatrix = true;
		return *this;
	}

	inline Transform Transform::Rotate(Transform transform, const float angle, const glm::vec3& axis)
	{
		return transform.Rotate(angle, axis);
	}

	inline Transform& Transform::Rotate(const glm::quat& quaternion)
	{
		mRotation = mRotation * quaternion;
		mDirtyMatrix = true;
		return *this;
	}

	inline Transform Transform::Rotate(Transform transform, const glm::quat& quaternion)
	{
		return transform.Rotate(quaternion);
	}
	
	inline Transform& Transform::Scale(const glm::vec3& scaling)
	{
		mScale *= scaling;
		mDirtyMatrix = true;
		return *this;
	}

	inline Transform Transform::Scale(Transform transform, const glm::vec3& scaling)
	{
		return transform.Scale(scaling);
	}

	inline Transform& Transform::Transformation(const Transform& transformation)
	{
		mDirtyMatrix = true;
		return *this *= transformation;
	}

	inline Transform Transform::Transformation(Transform transform, const Transform& transformation)
	{
		return transform.Transformation(transformation);
	}
#pragma endregion Transformations
}

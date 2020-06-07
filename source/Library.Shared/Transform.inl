// Header
#include "Transform.h"

namespace Library
{
#pragma region Special Members
	inline Transform::Transform(const float value) :
		mMatrix(value)
	{
	}

	inline Transform::Transform(float const& x0, float const& y0, float const& z0, float const& w0,
	                            float const& x1, float const& y1, float const& z1, float const& w1,
	                            float const& x2, float const& y2, float const& z2, float const& w2,
	                            float const& x3, float const& y3, float const& z3, float const& w3) :
		mMatrix(x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3)
	{
	}

	inline Transform::Transform(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3, const glm::vec4& v4) :
		mMatrix(v1, v2, v3, v4)
	{
	}

	inline Transform::Transform(const glm::vec3& translation, const glm::quat& quaternion, const glm::vec3& scaling) :
		mMatrix(glm::translate(translation) * glm::mat4_cast(quaternion) * glm::scale(scaling))
	{
	}

	inline Transform::Transform(const glm::mat4x4& matrix) : mMatrix(matrix)
	{
	}
#pragma endregion Special Members

#pragma region Boolean Operators
	inline bool Transform::operator==(const Transform& rhs) const noexcept
	{
		return mMatrix == rhs.mMatrix;
	}

	inline bool Transform::operator!=(const Transform& rhs) const noexcept
	{
		return !operator==(rhs);
	}
#pragma endregion Boolean Operators

#pragma region Arithmetic Operators
	inline Transform Transform::operator+(const Transform& rhs) const
	{
		return Transform(mMatrix + rhs.mMatrix);
	}

	inline Transform Transform::operator+(const float rhs) const
	{
		return Transform(mMatrix + rhs);
	}

	inline Transform operator+(const float lhs, const Transform& rhs)
	{
		return Transform(lhs + rhs.mMatrix);
	}

	inline Transform& Transform::operator+=(const Transform& rhs)
	{
		this->mMatrix += rhs.mMatrix;
		return *this;
	}

	inline Transform& Transform::operator+=(const float rhs)
	{
		this->mMatrix += rhs;
		return *this;
	}
	
	inline Transform Transform::operator-(const Transform& rhs) const
	{
		return Transform(mMatrix - rhs.mMatrix);
	}

	inline Transform Transform::operator-(const float rhs) const
	{
		return Transform(mMatrix - rhs);
	}

	inline Transform operator-(const float lhs, const Transform& rhs)
	{
		return Transform(lhs - rhs.mMatrix);
	}

	inline Transform& Transform::operator-=(const Transform& rhs)
	{
		this->mMatrix -= rhs.mMatrix;
		return *this;
	}

	inline Transform& Transform::operator-=(const float rhs)
	{
		this->mMatrix -= rhs;
		return *this;
	}
	
	inline Transform Transform::operator*(const Transform& rhs) const
	{
		return Transform(mMatrix * rhs.mMatrix);
	}

	inline Transform Transform::operator*(const float rhs) const
	{
		return Transform(mMatrix * rhs);
	}

	inline glm::vec4 Transform::operator*(const glm::vec4& rhs) const
	{
		return mMatrix * rhs;
	}

	inline glm::vec4 operator*(const glm::vec4& lhs, const Transform& rhs)
	{
		return lhs * rhs.mMatrix;
	}

	inline Transform& Transform::operator*=(const Transform& rhs)
	{
		this->mMatrix *= rhs.mMatrix;
		return *this;
	}

	inline Transform& Transform::operator*=(const float rhs)
	{
		this->mMatrix *= rhs;
		return *this;
	}
	
	inline Transform Transform::operator/(const Transform& rhs) const
	{
		return Transform(mMatrix / rhs.mMatrix);
	}

	inline Transform Transform::operator/(const float rhs) const
	{
		return Transform(mMatrix / rhs);
	}

	inline glm::vec4 Transform::operator/(const glm::vec4& rhs) const
	{
		return mMatrix / rhs;
	}

	inline glm::vec4 operator/(const glm::vec4& lhs, const Transform& rhs)
	{
		return lhs / rhs.mMatrix;
	}

	inline Transform& Transform::operator/=(const Transform& rhs)
	{
		this->mMatrix /= rhs.mMatrix;
		return *this;
	}

	inline Transform& Transform::operator/=(const float rhs)
	{
		this->mMatrix /= rhs;
		return *this;
	}
#pragma endregion Arithmetic Operators

#pragma region Accessors
	inline glm::vec4& Transform::operator[](const int i)
	{
		return mMatrix[i];
	}
	
	inline const glm::vec4& Transform::operator[](const int i) const
	{
		return mMatrix[i];
	}

	inline float Transform::Determinant() const
	{
		return glm::determinant(mMatrix);
	}

	inline Transform Transform::Inverse() const
	{
		return Transform(glm::inverse(mMatrix));
	}
#pragma endregion Accessors

#pragma region Transformations
	inline Transform& Transform::Translate(const glm::vec3& translation)
	{
		return *this = Transform(glm::translate(mMatrix, translation));
	}

	inline Transform Transform::Translate(const Transform& transform, const glm::vec3& translation)
	{
		return Transform(glm::translate(transform.mMatrix, translation));
	}

	inline Transform& Transform::Rotate(const float angle, const glm::vec3& axis)
	{
		return *this = Transform(glm::rotate(mMatrix, angle, axis));
	}

	inline Transform Transform::Rotate(const Transform& transform, const float angle, const glm::vec3& axis)
	{
		return Transform(glm::rotate(transform.mMatrix, angle, axis));
	}

	inline Transform& Transform::Scale(const glm::vec3& scaling)
	{
		return *this = Transform(glm::scale(mMatrix, scaling));
	}

	inline Transform Transform::Scale(const Transform& transform, const glm::vec3& scaling)
	{
		return Transform(glm::scale(transform.mMatrix, scaling));
	}

	inline Transform& Transform::Transformation(const glm::vec3& translation, const glm::quat& quaternion, const glm::vec3& scaling)
	{
		mMatrix *= glm::translate(translation) * glm::mat4_cast(quaternion) * glm::scale(scaling);
		return *this;
	}

	inline Transform Transform::Transformation(const Transform& transform, const glm::vec3& translation, const glm::quat& quaternion, const glm::vec3& scaling)
	{
		return Transform(transform.mMatrix * glm::translate(translation) * glm::mat4_cast(quaternion) * glm::scale(scaling));
	}
#pragma endregion Transformations
}

#include "pch.h"
#include "Camera.h"
#include "VectorHelper.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	Camera::Camera(Game& game, float nearPlaneDistance, float farPlaneDistance) :
		GameComponent(game),
		mNearPlaneDistance(nearPlaneDistance), mFarPlaneDistance(farPlaneDistance)
	{
	}

	const XMFLOAT3& Camera::Position() const
	{
		return mPosition;
	}

	const XMFLOAT3& Camera::Direction() const
	{
		return mDirection;
	}

	const XMFLOAT3& Camera::Up() const
	{
		return mUp;
	}

	const XMFLOAT3& Camera::Right() const
	{
		return mRight;
	}

	XMVECTOR Camera::PositionVector() const
	{
		return XMLoadFloat3(&mPosition);
	}

	XMVECTOR Camera::DirectionVector() const
	{
		return XMLoadFloat3(&mDirection);
	}

	XMVECTOR Camera::UpVector() const
	{
		return XMLoadFloat3(&mUp);
	}

	XMVECTOR Camera::RightVector() const
	{
		return XMLoadFloat3(&mRight);
	}

	float Camera::NearPlaneDistance() const
	{
		return mNearPlaneDistance;
	}

	void Camera::SetNearPlaneDistance(float distance)
	{
		mNearPlaneDistance = distance;
		mProjectionMatrixDataDirty = true;
	}

	float Camera::FarPlaneDistance() const
	{
		return mFarPlaneDistance;
	}

	void Camera::SetFarPlaneDistance(float distance)
	{
		mFarPlaneDistance = distance;
		mProjectionMatrixDataDirty = true;
	}

	XMMATRIX Camera::ViewMatrix() const
	{
		return XMLoadFloat4x4(&mViewMatrix);
	}

	XMMATRIX Camera::ProjectionMatrix() const
	{
		return XMLoadFloat4x4(&mProjectionMatrix);
	}

	XMMATRIX Camera::ViewProjectionMatrix() const
	{
		XMMATRIX viewMatrix = XMLoadFloat4x4(&mViewMatrix);
		XMMATRIX projectionMatrix = XMLoadFloat4x4(&mProjectionMatrix);

		return XMMatrixMultiply(viewMatrix, projectionMatrix);
	}

	const vector<function<void()>>& Camera::ViewMatrixUpdatedCallbacks() const
	{
		return mViewMatrixUpdatedCallbacks;
	}

	void Camera::AddViewMatrixUpdatedCallback(function<void()> callback)
	{
		mViewMatrixUpdatedCallbacks.push_back(callback);
	}

	const vector<function<void()>>& Camera::ProjectionMatrixUpdatedCallbacks() const
	{
		return mProjectionMatrixUpdatedCallbacks;
	}

	void Camera::AddProjectionMatrixUpdatedCallback(function<void()> callback)
	{
		mProjectionMatrixUpdatedCallbacks.push_back(callback);
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		SetPosition(position);
	}

	void Camera::SetPosition(FXMVECTOR position)
	{
		XMStoreFloat3(&mPosition, position);
		mViewMatrixDataDirty = true;
	}

	void Camera::SetPosition(const XMFLOAT3& position)
	{
		mPosition = position;
		mViewMatrixDataDirty = true;
	}

	void Camera::Reset()
	{
		mPosition = Vector3Helper::Zero;
		mDirection = Vector3Helper::Forward;
		mUp = Vector3Helper::Up;
		mRight = Vector3Helper::Right;
		mViewMatrixDataDirty = true;

		UpdateViewMatrix();
	}

	void Camera::Initialize()
	{
		UpdateProjectionMatrix();
		Reset();
	}

	void Camera::Update(const GameTime&)
	{
		if (mViewMatrixDataDirty)
		{
			UpdateViewMatrix();
		}
	}

	void Camera::UpdateViewMatrix()
	{
		XMVECTOR eyePosition = XMLoadFloat3(&mPosition);
		XMVECTOR direction = XMLoadFloat3(&mDirection);
		XMVECTOR upDirection = XMLoadFloat3(&mUp);

		XMMATRIX viewMatrix = XMMatrixLookToRH(eyePosition, direction, upDirection);
		XMStoreFloat4x4(&mViewMatrix, viewMatrix);

		for (auto& callback : mViewMatrixUpdatedCallbacks)
		{
			callback();
		}

		mViewMatrixDataDirty = false;
	}

	void Camera::ApplyRotation(CXMMATRIX transform)
	{
		XMVECTOR direction = XMLoadFloat3(&mDirection);
		XMVECTOR up = XMLoadFloat3(&mUp);

		direction = XMVector3TransformNormal(direction, transform);
		direction = XMVector3Normalize(direction);

		up = XMVector3TransformNormal(up, transform);
		up = XMVector3Normalize(up);

		XMVECTOR right = XMVector3Cross(direction, up);
		up = XMVector3Cross(right, direction);

		XMStoreFloat3(&mDirection, direction);
		XMStoreFloat3(&mUp, up);
		XMStoreFloat3(&mRight, right);

		mViewMatrixDataDirty = true;
	}

	void Camera::ApplyRotation(const XMFLOAT4X4& transform)
	{
		XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transformMatrix);
	}
}

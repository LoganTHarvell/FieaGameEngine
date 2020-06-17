#include "pch.h"
#include "OrthographicCamera.h"

using namespace DirectX;

namespace Library
{
	OrthographicCamera::OrthographicCamera(Game& game, float viewWidth, float viewHeight, float nearPlaneDistance, float farPlaneDistance) :
		Camera(game, nearPlaneDistance, farPlaneDistance),
		mViewWidth(viewWidth), mViewHeight(viewHeight)
    {
    }
    
	float OrthographicCamera::ViewWidth() const
	{
		return mViewWidth;
	}

	void OrthographicCamera::SetViewWidth(float viewWidth)
	{
		if (viewWidth > 0.0f)
		{
			mViewWidth = viewWidth;
			mProjectionMatrixDataDirty = true;
		}
	}

	float OrthographicCamera::ViewHeight() const
	{
		return mViewHeight;
	}

	void OrthographicCamera::SetViewHeight(float viewHeight)
	{
		if (viewHeight > 0.0f)
		{
			mViewHeight = viewHeight;
			mProjectionMatrixDataDirty = true;
		}
	}

    void OrthographicCamera::UpdateProjectionMatrix()
    {
		if (mProjectionMatrixDataDirty)
		{
			XMMATRIX projectionMatrix = XMMatrixOrthographicRH(mViewWidth, mViewHeight, mNearPlaneDistance, mFarPlaneDistance);
			XMStoreFloat4x4(&mProjectionMatrix, projectionMatrix);

			for (auto& callback : mProjectionMatrixUpdatedCallbacks)
			{
				callback();
			}
		}
    }
}

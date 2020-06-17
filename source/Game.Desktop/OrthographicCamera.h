#pragma once

#include "Camera.h"

namespace Library
{
    class GameTime;

    class OrthographicCamera : public Camera
    {
        RTTI_DECLARATIONS_ABSTRACT(OrthographicCamera, Camera)

    public:
		explicit OrthographicCamera(Game& game, float viewWidth = DefaultViewWidth, float viewHeight = DefaultViewHeight, float nearPlaneDistance = DefaultNearPlaneDistance, float farPlaneDistance = DefaultFarPlaneDistance);
		OrthographicCamera(const OrthographicCamera&) = default;
		OrthographicCamera(OrthographicCamera&&) = default;
		OrthographicCamera& operator=(const OrthographicCamera&) = default;
		OrthographicCamera& operator=(OrthographicCamera&&) = default;
		virtual ~OrthographicCamera() = default;

		float ViewWidth() const;
		void SetViewWidth(float viewWidth);

		float ViewHeight() const;
		void SetViewHeight(float viewHeight);

        virtual void UpdateProjectionMatrix() override;

		inline static const float DefaultViewWidth{ 100.0f };
		inline static const float DefaultViewHeight{ 100.0f };

    protected:
		float mViewWidth;
		float mViewHeight;
    };
}
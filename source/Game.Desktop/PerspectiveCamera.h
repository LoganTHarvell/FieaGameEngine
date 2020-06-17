#pragma once

#include "Camera.h"

namespace Library
{
    class PerspectiveCamera : public Camera
    {
		RTTI_DECLARATIONS_ABSTRACT(PerspectiveCamera, Camera)

    public:
		explicit PerspectiveCamera(Game& game, float fieldOfView = DefaultFieldOfView, float aspectRatio = DefaultAspectRatio, float nearPlaneDistance = DefaultNearPlaneDistance, float farPlaneDistance = DefaultFarPlaneDistance);
		PerspectiveCamera(const PerspectiveCamera&) = default;
		PerspectiveCamera& operator=(const PerspectiveCamera&) = default;
		PerspectiveCamera(PerspectiveCamera&&) = default;
		PerspectiveCamera& operator=(PerspectiveCamera&&) = default;
		virtual ~PerspectiveCamera() = default;
       
        float AspectRatio() const;
		void SetAspectRatio(float aspectRatio);

        float FieldOfView() const;
		void SetFieldOfView(float fieldOfView);

		virtual void UpdateProjectionMatrix() override;
        
		inline static const float DefaultFieldOfView{ DirectX::XM_PIDIV4 };
		inline static const float DefaultAspectRatio{ 4.0f / 3.0f };

    protected:
        float mFieldOfView;
        float mAspectRatio;
    };
}
#pragma once

#include <gsl/gsl>
#include <winrt/Windows.Foundation.h>
#include <d3d11.h>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"
#include "PointLight.h"
#include "SolarSystem.h"
#include "Vector.h"
#include "WorldState.h"

namespace Library
{
	class ProxyModel;
}

namespace Demo
{
	class SolarBodyMaterial;

	class SolarSystemDemo final : public Library::DrawableGameComponent
	{
	public:
		SolarSystemDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);
		SolarSystemDemo(const SolarSystemDemo&) = delete;
		SolarSystemDemo(SolarSystemDemo&&) = default;
		SolarSystemDemo& operator=(const SolarSystemDemo&) = default;		
		SolarSystemDemo& operator=(SolarSystemDemo&&) = default;
		~SolarSystemDemo();

		SolarSystem& GetSolarSystem();
		const SolarSystem& GetSolarSystem() const;
		
		bool AnimationEnabled() const;
		void SetAnimationEnabled(bool enabled);
		void ToggleAnimation();

		float AmbientLightIntensity() const;
		void SetAmbientLightIntensity(float intensity);

		float PointLightIntensity() const;
		void SetPointLightIntensity(float intensity);

		const DirectX::XMFLOAT3& LightPosition() const;
		const DirectX::XMVECTOR LightPositionVector() const;
		void SetLightPosition(const DirectX::XMFLOAT3& position);
		void SetLightPosition(DirectX::FXMVECTOR position);

		float LightRadius() const;
		void SetLightRadius(float radius);

		float SpecularIntensity() const;
		void SetSpecularIntensity(float intensity);

		float SpecularPower() const;
		void SetSpecularPower(float power);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		void DrawChildren(Library::Entity& parent);

	private:
		SolarSystem mSolarSystem{ "SolarSystem" };
		
		winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
		winrt::com_ptr<ID3D11Buffer> mIndexBuffer;
		std::uint32_t mIndexCount{ 0 };
		Library::PointLight mPointLight;
		
		bool mAnimationEnabled{ true };
		bool mUpdateMaterial{ true };
	};
}
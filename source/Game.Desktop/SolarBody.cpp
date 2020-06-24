#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "SolarBody.h"

// First Party
#include "SolarSystem.h"
#include "Texture2D.h"
#include "Utility.h"
#pragma endregion Includes

using namespace std::string_literals;

using namespace Library;

namespace Demo
{
	const SignatureList& SolarBody::Signatures()
	{
		static const SignatureList signatures =
		{
			{ ColorMapKey, Types::String, false, 1, offsetof(SolarBody, mDescription.ColorMap) },
			{ SpecularMapKey, Types::String, false, 1, offsetof(SolarBody, mDescription.SpecularMap) },
			{ DiameterKey, Types::Float, false, 1, offsetof(SolarBody, mDescription.Diameter) },
			{ OrbitalDistanceKey, Types::Float, false, 1, offsetof(SolarBody, mDescription.OrbitalDistance) },
			{ OrbitalPeriodKey, Types::Float, false, 1, offsetof(SolarBody, mDescription.OrbitalPeriod) },
			{ RotationPeriodKey, Types::Float, false, 1, offsetof(SolarBody, mDescription.RotationPeriod) },
			{ TiltKey, Types::Float, false, 1, offsetof(SolarBody, mDescription.Tilt) },
			{ AmbientColorKey, Types::Vector, false, 1, offsetof(SolarBody, mDescription.AmbientColor) },
			{ SpecularColorKey, Types::Vector, false, 1, offsetof(SolarBody, mDescription.SpecularColor) },
			{ SpecularPowerKey, Types::Float, false, 1, offsetof(SolarBody, mDescription.SpecularPower) }
		};

		return signatures;
	}
	
	SolarBody::SolarBody(std::string name, Description description) : Actor(TypeIdClass(), std::move(name)),
		mDescription(std::move(description))
	{
	}

	SolarBody::SolarBody(Description description) : Actor(TypeIdClass(), std::string()),
		mDescription(std::move(description))
	{
	}

	SolarBody::Description& SolarBody::GetDescription()
	{
		return mDescription;
	}

	const SolarBody::Description& SolarBody::GetDescription() const
	{
		return mDescription;
	}

	SolarBodyMaterial& SolarBody::GetMaterial()
	{
		return *mMaterial.get();
	}

	const SolarBodyMaterial& SolarBody::GetMaterial() const
	{
		return *mMaterial.get();
	}

	void SolarBody::Initialize(WorldState& worldState)
	{
		if (worldState.ContentManager && worldState.RenderingManager)
		{
			std::wstring filename = String::ToWideString(mDescription.ColorMap);
			auto colorMap = worldState.ContentManager->Load<Texture2D>(!filename.empty() ? filename : L"Textures\\sun.jpg"s);

			filename = String::ToWideString(mDescription.SpecularMap);
			auto specularMap = worldState.ContentManager->Load<Texture2D>(!filename.empty() ? filename : L"Textures\\NoSpecular.png"s);

			mMaterial = std::make_shared<SolarBodyMaterial>(*worldState.ContentManager, *worldState.RenderingManager, colorMap, specularMap);
			mMaterial->Initialize();
			mMaterial->SetAmbientColor(DirectX::XMFLOAT4(&mDescription.AmbientColor[0]));
		}

		Actor::Initialize(worldState);
	}

	void SolarBody::Update(Library::WorldState& worldState)
	{
		assert(worldState.Sector->Is(SolarSystem::TypeIdClass()));
		SolarSystem* solarSystem = static_cast<SolarSystem*>(worldState.Sector);
		
		mAxialRotation += worldState.GameTime->ElapsedGameTimeSeconds().count() * mDescription.RotationPeriod * solarSystem->TimeScale();
		mOrbitRotation += worldState.GameTime->ElapsedGameTimeSeconds().count() * mDescription.OrbitalPeriod * solarSystem->TimeScale();
		
		Transform& transform = GetTransform();
		transform.Scale() = glm::vec3(mDescription.Diameter);
		transform.Rotation() = glm::angleAxis(mAxialRotation, glm::vec3( 0, 1, 0 ));
		transform.Rotate(mDescription.Tilt, glm::vec3(1, 0, 0));
		transform.Translation() = glm::vec3(1.0f * mDescription.OrbitalDistance * solarSystem->DistanceScale(), 0.0f, 0.0f);
		transform *= Transform(glm::vec3(0), glm::angleAxis(mOrbitRotation, glm::vec3(0, 1, 0)), glm::vec3(1));
		
		SolarBody* parent = GetParent()->As<SolarBody>();
		if (parent) transform.Translate(parent->GetTransform().Translation());

		Actor::Update(worldState);
	}
}

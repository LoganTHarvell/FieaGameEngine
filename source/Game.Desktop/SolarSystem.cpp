#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "SolarSystem.h"
#pragma endregion Includes

using namespace Library;

namespace Demo
{
	const SignatureList& SolarSystem::Signatures()
	{
		static const SignatureList signatures =
		{
			{ DistanceScaleKey, Types::Float, false, 1,  offsetof(SolarSystem, mDistanceScale) },
			{ TimeScaleKey, Types::Float, false, 1,  offsetof(SolarSystem, mTimeScale) }
		};

		return signatures;
	}

	SolarSystem::SolarSystem(std::string name) : Entity(TypeIdClass(), std::move(name))
	{
	}
	
	float& SolarSystem::DistanceScale()
	{
		return mDistanceScale;
	}

	float SolarSystem::DistanceScale() const
	{
		return mDistanceScale;
	}

	float& SolarSystem::TimeScale()
	{
		return mTimeScale;
	}

	float SolarSystem::TimeScale() const
	{
		return mTimeScale;
	}

	float SolarSystem::AmbientLightIntensity() const
	{
		return mAmbientLightIntensity;
	}

	void SolarSystem::SetAmbientLightIntensity(const float intensity)
	{
		std::function<void(Entity&)> updateChildren;
		updateChildren = [&intensity, &updateChildren](Entity& parent)
		{
			parent.ForEachChild([&intensity, &updateChildren](Entity& entity)
			{
				SolarBody* solarBody = entity.As<SolarBody>();

				if (solarBody)
				{
					solarBody->GetMaterial().SetAmbientColor({ intensity, intensity, intensity, 1 });
				}

				updateChildren(entity);
			});
		};

		updateChildren(*this);
	}

	float SolarSystem::SunIntensity() const
	{
		return mSunIntensity;
	}

	void SolarSystem::SetSunIntensity(const float intensity)
	{
		std::function<void(Entity&)> updateChildren;
		updateChildren = [&intensity, &updateChildren](Entity& parent)
		{
			parent.ForEachChild([&intensity, &updateChildren](Entity& entity)
			{
				SolarBody* solarBody = entity.As<SolarBody>();

				if (solarBody)
				{
					solarBody->GetMaterial().SetLightColor({ intensity, intensity, intensity, 1 });
				}

				updateChildren(entity);
			});
		};

		updateChildren(*this);
	}

	float SolarSystem::LightRadius() const
	{
		return mLightRadius;
	}

	void SolarSystem::SetLightRadius(const float radius)
	{
		std::function<void(Entity&)> updateChildren;
		updateChildren = [&radius, &updateChildren](Entity& parent)
		{
			parent.ForEachChild([&radius, &updateChildren](Entity& entity)
			{
				SolarBody* solarBody = entity.As<SolarBody>();

				if (solarBody)
				{
					solarBody->GetMaterial().SetLightRadius(radius);
				}

				updateChildren(entity);
			});

		};

		updateChildren(*this);
	}

	float SolarSystem::SpecularIntensity() const
	{
		return mSpecularIntensity;
	}

	void SolarSystem::SetSpecularIntensity(const float intensity)
	{
		std::function<void(Entity&)> updateChildren;
		updateChildren = [&intensity, &updateChildren](Entity& parent)
		{
			parent.ForEachChild([&intensity, &updateChildren](Entity& entity)
			{
				SolarBody* solarBody = entity.As<SolarBody>();

				if (solarBody)
				{
					solarBody->GetMaterial().SetSpecularColor({ intensity, intensity, intensity });
				}

				updateChildren(entity);
			});
		};
		
		updateChildren(*this);
	}

	float SolarSystem::SpecularPower() const
	{
		return mSpecularPower;
	}

	void SolarSystem::SetSpecularPower(const float power)
	{
		std::function<void(Entity&)> updateChildren;
		updateChildren = [&power, &updateChildren](Entity& parent)
		{
			parent.ForEachChild([&power, &updateChildren](Entity& entity)
			{
				SolarBody* solarBody = entity.As<SolarBody>();

				if (solarBody)
				{
					solarBody->GetMaterial().SetSpecularPower(power);
				}

				updateChildren(entity);
			});
		};

		updateChildren(*this);
	}

	glm::vec3 SolarSystem::SunPosition() const
	{
		return mSunPosition;
	}

	void SolarSystem::SetSunPosition(const glm::vec3 position)
	{
		mSunPosition = { position, 1 };
		
		SolarBody* sun = FindChild<SolarBody>(SunKey);
		if (sun) sun->GetTransform().Translation() = position;
	}
}

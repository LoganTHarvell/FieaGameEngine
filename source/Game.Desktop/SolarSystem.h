#pragma once

#include "Entity.h"
#include "SolarBody.h"

namespace Demo
{
	class SolarSystem final : public Library::Entity
	{
		RTTI_DECLARATIONS(SolarSystem, Library::Entity)

#pragma region Property Keys
	public:
		inline static const Key SunKey = "Sun";
		inline static const Key DistanceScaleKey = "DistanceScale";
		inline static const Key TimeScaleKey = "TimeScale";
#pragma endregion Property Keys
		
#pragma region Static Members
	public:
		/// <summary>
		/// Getter for the class TypeInfo, used for registration with the TypeManager.
		/// </summary>
		static const Library::TypeManager::TypeInfo& TypeInfo();
#pragma endregion Static Members

	public:
		explicit SolarSystem(std::string name=std::string());

		public:
		float& DistanceScale();
		float DistanceScale() const;
		float& TimeScale();
		float TimeScale() const;

		float AmbientLightIntensity() const;
		void SetAmbientLightIntensity(const float intensity);

		float SunIntensity() const;
		void SetSunIntensity(const float intensity);
		
		float LightRadius() const;
		void SetLightRadius(const float radius);

		float SpecularIntensity() const;
		void SetSpecularIntensity(const float intensity);
		
		float SpecularPower() const;
		void SetSpecularPower(const float power);

		glm::vec3 SunPosition() const;
		void SetSunPosition(glm::vec3 position);
		
	private:		
		float mDistanceScale{ 150 };
		float mTimeScale{ 1 };
		
		float mAmbientLightIntensity{ 0.5 };
		glm::vec4 mSunPosition{ 0 };
		float mSunIntensity{ 0.5 };
		float mLightRadius{ 100 };
		float mSpecularIntensity{ 0.5 };
		float mSpecularPower{ 0.5 };
	};

#pragma region Factory
	/// <summary>
	/// SolarSystemFactory class declaration.
	/// </summary>
	ConcreteFactory(SolarSystem, Library::Entity)
#pragma endregion Factory
}


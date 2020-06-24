#pragma once

#include "Entity.h"
#include "SolarBody.h"

namespace Demo
{
	/// <summary>
	/// Represents a solar system, managing the update of any SolarBody children.
	/// </summary>
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
		/// Getter for the class SignatureList, used for registration with the TypeManager.
		/// </summary>
		static const Library::SignatureList& Signatures();
#pragma endregion Static Members

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="name">Name of the SolarSystem Entity.</param>
		explicit SolarSystem(std::string name=std::string());
#pragma endregion Special Members

#pragma region Accessors
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
#pragma endregion Accessors

#pragma region Data Members
	private:		
		float mDistanceScale{ 150 };
		float mTimeScale{ 1 };
		
		float mAmbientLightIntensity{ 0.5 };
		glm::vec4 mSunPosition{ 0 };
		float mSunIntensity{ 0.5 };
		float mLightRadius{ 100 };
		float mSpecularIntensity{ 0.5 };
		float mSpecularPower{ 0.5 };
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// SolarSystemFactory class declaration.
	/// </summary>
	ConcreteFactory(SolarSystem, Library::Entity)
#pragma endregion Factory
}


#pragma once

#include "Actor.h"
#include "SolarBodyMaterial.h"

namespace Library
{
	// Forward Declarations
	class Model;
}

namespace Demo
{	
	class SolarBody final : public Library::Actor
	{
		RTTI_DECLARATIONS(SolarBody, Library::Actor)

	public:
		struct Description
		{
			std::string ColorMap;
			std::string SpecularMap;
			float Diameter{ 0 };
			float OrbitalDistance{ 0 };
			float OrbitalPeriod{ 0 };
			float RotationPeriod{ 0 };
			float Tilt{ 0 };
			glm::vec4 AmbientColor{ 1 };
			glm::vec4 SpecularColor{ 0.5f };
			float SpecularPower{ 1 };
		};

		inline static const Key ColorMapKey = "ColorMap";
		inline static const Key SpecularMapKey = "SpecularMap";
		inline static const Key DiameterKey = "Diameter";
		inline static const Key OrbitalDistanceKey = "OrbitalDistance";
		inline static const Key OrbitalPeriodKey = "OrbitalPeriod";
		inline static const Key RotationPeriodKey = "RotationPeriod";
		inline static const Key TiltKey = "Tilt";
		inline static const Key AmbientColorKey = "AmbientColor";
		inline static const Key SpecularColorKey = "SpecularColor";
		inline static const Key SpecularPowerKey = "SpecularPower";

#pragma region Static Members
	public:
		/// <summary>
		/// Getter for the class SignatureList, used for registration with the TypeManager.
		/// </summary>
		static const Library::SignatureList& Signatures();
#pragma endregion Static Members

	public:
		explicit SolarBody(std::string name=std::string(), Description description=Description());
		explicit SolarBody(Description description);

		Description& GetDescription();
		const Description& GetDescription() const;

		SolarBodyMaterial& GetMaterial();
		const SolarBodyMaterial& GetMaterial() const;

		virtual void Initialize(Library::WorldState& worldState) override;
		virtual void Update(Library::WorldState& worldState) override;
		
	private:
		Description mDescription;

		std::shared_ptr<SolarBodyMaterial> mMaterial;

		float mAxialRotation{ 0 };
		float mOrbitRotation{ 0 };
	};

#pragma region Factory
	/// <summary>
	/// EntityFactory class declaration.
	/// </summary>
	ConcreteFactory(SolarBody, Library::Entity)
#pragma endregion Factory
}


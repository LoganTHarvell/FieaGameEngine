#pragma once
#include <cmath>

#include "Attributed.h"
#include "Factory.h"

using namespace Library;

namespace GameDesktop 
{
	class Transform final: public Attributed
	{
		RTTI_DECLARATIONS(Transform, Attributed)
	public:
		inline static const std::string LocationKey = "Location";
		inline static const std::string RotationKey = "Rotation";
		inline static const std::string ScaleKey = "Scale";
		static const TypeManager::TypeInfo& TypeInfo();

	public:
		explicit Transform(const glm::vec4& location = glm::vec4(), float rotation = 0.0f, const glm::vec4& scale = glm::vec4(1.0f));
		Transform(const Transform&) = default;
		Transform(Transform&&) = default;
		Transform& operator=(const Transform&) = default;
		Transform& operator=(Transform&&) = default;
		~Transform() = default;

		glm::vec3 GetLocation() const;
		glm::vec4& GetLocation();
		void SetLocation(const glm::vec2& location);
		void SetLocation(const glm::vec4& location);

		float GetRotation() const;
		void SetRotation(float rotation);

		glm::vec2 GetScale() const;
		void SetScale(const glm::vec2& scale);

		glm::vec2 Forward() const;
		glm::vec2 Right() const;
		virtual gsl::owner<Transform*> Clone() const override;
	protected:
	private:
		glm::vec4 mLocation;
		float mRotation;
		glm::vec4 mScale;
	};

	ConcreteFactory(Transform, Scope)
}
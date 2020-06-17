#pragma once

#pragma region Includes
// First Party
#include "Action.h"
#include "WorldState.h"
#include "Factory.h"

// Third Party
#include <glm/glm.hpp>
#pragma endregion Includes

using namespace Library;

namespace GameDesktop
{
	/// <summary>
	/// Sets the character velocity based on reaction input
	/// Input // vector4 representing input {x, y, z, w} // Only x/y are used
	/// </summary>

	class ActionCharacterMovementInput final : public Action
	{
		RTTI_DECLARATIONS(ActionCharacterMovementInput, Action)

	public:
		inline static const std::string GameObjectNameKey = "Name";
		inline static const std::string InputVec4Key = "Input";

		static const TypeManager::TypeInfo& TypeInfo();

		explicit ActionCharacterMovementInput(const std::string& name = std::string());

		~ActionCharacterMovementInput() = default;
		ActionCharacterMovementInput(const ActionCharacterMovementInput& rhs) = default;
		ActionCharacterMovementInput& operator=(const ActionCharacterMovementInput& rhs) = default;
		ActionCharacterMovementInput(ActionCharacterMovementInput&& rhs) noexcept = default;
		ActionCharacterMovementInput& operator=(ActionCharacterMovementInput&& rhs) noexcept = default;

		virtual gsl::owner<Scope*> Clone() const override;

		virtual void Update(WorldState&);

		virtual std::string ToString() const override;

	public:
		std::string mCharacterName = "default";
		glm::vec4 mInputVec4{0,0,0,0}; //Up Down Left Right
	};

	ConcreteFactory(ActionCharacterMovementInput, Scope)
}


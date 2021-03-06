#pragma once

#pragma region Includes
// First Party
#include "Entity.h"
#include "Factory.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents an Action for incrementing an integer Attribute.
	/// </summary>
	class ActionIncrement final : public Entity
	{
		RTTI_DECLARATIONS(ActionIncrement, Entity)

#pragma region Type Definitions, Constants
	public:
		/// <summary>
		/// Key for the name of the integer Attribute to increment.
		/// </summary>
		inline static const std::string OperandKey = "Operand";

		/// <summary>
		/// Key for the increment step amount.
		/// </summary>
		inline static const std::string IncrementStepKey = "IncrementStep";

	public:
		/// <summary>
		/// Getter for the class SignatureList, used for registration with the TypeManager.
		/// </summary>
		static const SignatureList& Signatures();
#pragma endregion Type Definitions, Constants

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		explicit ActionIncrement(std::string name=std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~ActionIncrement() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">ActionIncrement to be copied.</param>
		ActionIncrement(const ActionIncrement& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">ActionIncrement to be copied.</param>
		/// <returns>Newly copied into left hand side ActionIncrement.</returns>
		ActionIncrement& operator=(const ActionIncrement& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">ActionIncrement to be moved.</param>
		ActionIncrement(ActionIncrement&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">ActionIncrement to be moved.</param>
		/// <returns>Newly moved into left hand side ActionIncrement.</returns>
		ActionIncrement& operator=(ActionIncrement&& rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the ActionIncrement.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Game Loop
	public:
		/// <summary>
		/// Virtual update method called by the containing object.
		/// </summary>
		virtual void Update(WorldState&) override;
#pragma endregion Game Loop

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// Virtual override for representing the Action as a std::string.
		/// </summary>
		/// <returns></returns>
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides

#pragma region Data Members
	private:
		/// <summary>
		/// Name for the integer Attribute to increment.
		/// </summary>
		std::string mOperand;

		/// <summary>
		/// Amount to increment the integer Attribute.
		/// </summary>
		int mIncrementStep{ 1 };
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// ActionIncrementFactory class declaration.
	/// </summary>
	ConcreteFactory(ActionIncrement, Entity)
#pragma endregion Factory
}


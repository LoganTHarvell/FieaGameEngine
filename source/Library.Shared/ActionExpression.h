#pragma once

#pragma region Includes
// First Party
#include "Entity.h"
#include "WorldState.h"
#include "Factory.h"
#include "Stack.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents an Action that evaluates an algebraic expression.
	/// </summary>
	class ActionExpression final : public Entity
	{
		RTTI_DECLARATIONS(ActionExpression, Entity)

#pragma region Type Definitions, Constants
	public:
		/// <summary>
		/// Key for the name of the string Attribute representing the expression to evaluate.
		/// </summary>
		inline static const std::string ExpressionKey = "Expression";

		/// <summary>
		/// Key for the name of the string Attribute representing the resulting evaluation of the expression.
		/// </summary>
		inline static const std::string ResultKey = "Result";

	public:
		/// <summary>
		/// Getter for the class TypeInfo, used for registration with the TypeManager.
		/// </summary>
		static const TypeManager::TypeInfo& TypeInfo();
#pragma endregion Type Definitions, Constants

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		explicit ActionExpression(std::string name=std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~ActionExpression() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">ActionExpression to be copied.</param>
		ActionExpression(const ActionExpression& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">ActionExpression to be copied.</param>
		/// <returns>Newly copied into left hand side ActionExpression.</returns>
		ActionExpression& operator=(const ActionExpression& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">ActionExpression to be moved.</param>
		ActionExpression(ActionExpression&& rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">ActionExpression to be moved.</param>
		/// <returns>Newly moved into left hand side ActionExpression.</returns>
		ActionExpression& operator=(ActionExpression&& rhs) noexcept;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the ActionExpression.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Game Loop
	public:
		/// <summary>
		/// Virtual update method called by the containing object.
		/// </summary>
		/// <param name="worldState">WorldState context for the current processing step.</param>
		virtual void Update(WorldState& worldState) override;
#pragma endregion Game Loop

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// Virtual override for representing the Action as a std::string.
		/// </summary>
		/// <returns></returns>
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides

#pragma region Helper Methods
	private:
		void TokenizeExpression();
#pragma endregion Helper Methods

#pragma region Data Members
	private:
		/// <summary>
		/// Algebraic expression evaluation result.
		/// </summary>
		Data mResult;

		Stack<std::string> mTokenStack;

#pragma region Prescribed Attributes
	private:
		/// <summary>
		/// Algebraic expression represented as a string.
		/// </summary>
		std::string mExpression;

		/// <summary>
		/// Algebraic expression evaluation result pointer.
		/// </summary>
		Data* mResultPtr{ &mResult };
#pragma endregion Prescribed Attributes
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// ActionExpressionFactory class declaration.
	/// </summary>
	ConcreteFactory(ActionExpression, Entity)
#pragma endregion Factory
}


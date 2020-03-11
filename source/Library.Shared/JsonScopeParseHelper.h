#pragma once

#pragma region Includes
// Standard
#include <string>

// First Party
#include "IJsonParseHelper.h"
#include "HashMap.h"
#include "Stack.h"
#include "Scope.h"
#pragma endregion Includes

namespace Library
{
	class JsonScopeParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonScopeParseHelper, IJsonParseHelper)

#pragma region Type Definitions and Constants
	private:
		/// <summary>
		/// Represents the context of the parser for a given value.
		/// </summary>
		struct StackFrame
		{
			const std::string& Key;
			const Json::Value* Value;
			Scope::Types Type;
			Scope& Context;
		};

	public:
		/// <summary>
		/// Mapping of string values to Scope data types.
		/// </summary>
		static const HashMap<std::string, Scope::Types> TypeStringMap;
#pragma endregion Type Definitions and Constants

#pragma region Shared Data
	public:
		/// <summary>
		/// Represents the SharedData needed to parse a Scope.
		/// </summary>
		class SharedData final : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseMaster::SharedData)

			friend JsonScopeParseHelper;

#pragma region Shared Data Special Members
		public:
			/// <summary>
			/// Default constructor.
			/// </summary>
			SharedData() = default;

			/// <summary>
			/// Specialized constructor for initializing with an external Scope.
			/// </summary>
			/// <param name="scope">Scope used to initialize the SharedData.</param>
			SharedData(Scope& scope);

			/// <summary>
			/// Default destructor.
			/// </summary>
			virtual ~SharedData();

			/// <summary>
			/// Copy constructor.
			/// </summary>
			SharedData(const SharedData& rhs) = default;

			/// <summary>
			/// Copy assignment operator.
			/// </summary>
			SharedData& operator=(const SharedData& rhs) = default;

			/// <summary>
			/// Move constructor.
			/// </summary>
			/// <param name="rhs">Shared to be moved.</param>
			/// <remarks>Sets SharedData for moved instance's master to this instance, if it exists.</remarks>
			SharedData(SharedData&& rhs) noexcept;

			/// <summary>
			/// Move assignment operator.
			/// </summary>
			/// <param name="rhs">Shared to be moved.</param>
			/// <remarks>Sets SharedData for moved instance's master to this instance, if it exists.</remarks>
			SharedData& operator=(SharedData&& rhs) noexcept;
#pragma endregion Shared Data Special Members

#pragma region Parser Methods
		public:
			/// <summary>
			/// Called before every parse to initialize SharedData.
			/// If there is no Scope associated with the SharedData, a base Scope class instance is heap allocated to use.
			/// </summary>
			virtual void Initialize() override;
#pragma endregion Parser Methods

#pragma region Scope Accessors
		public:
			/// <summary>
			/// Gets the Scope associated with the SharedData.
			/// </summary>
			/// <returns>Reference to the SharedData Scope.</returns>
			const Scope& GetScope() const;

			/// <summary>
			/// Sets the associated Scope with a given external Scope.
			/// </summary>
			/// <param name="scope">External scope to set into the SharedData.</param>
			void SetScope(Scope& scope);

			/// <summary>
			/// Removes ownership of an owned Scope, returning a pointer that now owns the Scope.
			/// </summary>
			/// <returns>
			/// Owner pointer to the Scope previously owned by SharedData. 
			/// If the SharedData did not own its associated Scope, then it returns nullptr.
			/// </returns>
			gsl::owner<Scope*> TransferScope();
#pragma endregion Scope Accessors

#pragma region Shared Data Data Members
		private:
			/// <summary>
			/// Scope associated with the SharedData. Filled during parsing.
			/// </summary>
			Scope* mRootScope{ nullptr };

			/// <summary>
			/// Represents if the SharedData owns the associated Scope.
			/// </summary>
			bool mOwnsScope{ false };

			/// <summary>
			/// Context stack frame used during parsing.
			/// </summary>
			Stack<StackFrame> mStack;
#pragma endregion Shared Data Data Members
		};
#pragma endregion Shared Data

#pragma region Parse Handlers
	public:
		/// <summary>
		/// Called by the associated JsonParseMaster during initial parsing of a value.
		/// Verifies a value can be handled and then sets the SharedData context for a subsequent EndHandler call.
		/// </summary>
		/// <returns>True if handled. Otherwise, false.</returns>
		virtual bool StartHandler(JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value) override;

		/// <summary>
		/// Called by the associated JsonParseMaster during final parsing of a value.
		/// Sets the SharedData with the values from the SharedData context setup by a previous StartHandler call.
		/// </summary>
		virtual bool EndHandler(JsonParseMaster::SharedData& data, const std::string& key) override;
#pragma endregion Parse Handlers
	};
}



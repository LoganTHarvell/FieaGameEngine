#pragma once

#pragma region Includes
// Standard
#include <string>

// First Party
#include "IJsonParseHelper.h"
#include "HashMap.h"
#include "Stack.h"
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Helper that handles parsing JSON into a Entity.
	/// </summary>
	class JsonEntityParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonEntityParseHelper, IJsonParseHelper)

#pragma region Type Definitions and Constants
	public:
		/// <summary>
		/// Mapping of string values to Entity data types.
		/// </summary>
		static const HashMap<std::string, Entity::Types> TypeStringMap;

	private:
		/// <summary>
		/// Represents the context of the parser for a given value.
		/// </summary>
		struct StackFrame final
		{
			const std::string& Key;
			Entity::Types Type;
			std::string ClassName;
			const Json::Value* Value;
			Entity& Context;
		};
#pragma endregion Type Definitions and Constants

#pragma region Shared Data
	public:
		/// <summary>
		/// Represents the SharedData needed to parse a Entity.
		/// </summary>
		class SharedData final : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseMaster::SharedData)

			friend JsonEntityParseHelper;

#pragma region Shared Data Special Members
		public:
			/// <summary>
			/// Default constructor.
			/// </summary>
			SharedData() = default;

			/// <summary>
			/// Specialized constructor for initializing with an external Entity.
			/// </summary>
			/// <param name="scope">Entity used to initialize the SharedData.</param>
			SharedData(Entity& scope);

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
			/// If there is no Entity associated with the SharedData, a base Entity class instance is heap allocated to use.
			/// </summary>
			virtual void PreParse() override;
#pragma endregion Parser Methods

#pragma region Entity Accessors
		public:
			/// <summary>
			/// Gets the Entity associated with the SharedData.
			/// </summary>
			/// <returns>Reference to the SharedData Entity.</returns>
			const Entity& GetEntity() const;

			/// <summary>
			/// Sets the associated Entity with a given external Entity.
			/// </summary>
			/// <param name="scope">External scope to set into the SharedData.</param>
			void SetEntity(Entity& scope);

			/// <summary>
			/// Removes ownership of an owned Entity, returning a pointer that now owns the Entity.
			/// </summary>
			/// <returns>
			/// Owner pointer to the Entity previously owned by SharedData. 
			/// If the SharedData did not own its associated Entity, then it returns nullptr.
			/// </returns>
			gsl::owner<Entity*> TransferEntity();
#pragma endregion Entity Accessors

#pragma region Shared Data Data Members
		private:
			/// <summary>
			/// Entity associated with the SharedData. Filled during parsing.
			/// </summary>
			Entity* mRootEntity{ nullptr };

			/// <summary>
			/// Represents if the SharedData owns the associated Entity.
			/// </summary>
			bool mOwnsEntity{ false };

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
		/// <exception cref="std::runtime_error"><string> is not a valid type.</exception>
		/// <exception cref="std::runtime_error">Invalid value type.</exception>
		/// <exception cref="std::runtime_error">Invalid array value type.</exception>
		/// <exception cref="std::runtime_error">Invalid mismatched array value types.</exception>
		virtual bool StartHandler(JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value) override;

		/// <summary>
		/// Called by the associated JsonParseMaster during final parsing of a value.
		/// Sets the SharedData with the values from the SharedData context setup by a previous StartHandler call.
		/// </summary>
		/// <exception cref="std::runtime_error">"" array has too many elements.</exception>
		virtual bool EndHandler(JsonParseMaster::SharedData& data, const std::string& key) override;
#pragma endregion Parse Handlers
	};
}



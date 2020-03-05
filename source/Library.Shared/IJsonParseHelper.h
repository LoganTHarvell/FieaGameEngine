#pragma once

#pragma region Includes
// Third Party
#pragma warning(disable : 26812)
#include <json/json.h>
#pragma warning(default : 26812)

// First Party
#include "RTTI.h"
#include "JsonParseMaster.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Parse helper interface class for JSON to data conversions.
	/// </summary>
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS_ABSTRACT(IJsonParseHelper, RTTI)

#pragma region Special Member Functions
	protected:
		/// <summary>
		/// Default constructor.
		/// </summary>
		IJsonParseHelper() = default;

	public:
		/// <summary>
		/// Virtual default destructor.
		/// </summary>
		virtual ~IJsonParseHelper() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		IJsonParseHelper(const IJsonParseHelper&) = delete;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		IJsonParseHelper& operator=(const IJsonParseHelper&) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		IJsonParseHelper(IJsonParseHelper&&) noexcept = delete;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		IJsonParseHelper& operator=(IJsonParseHelper&&) noexcept = delete;
#pragma endregion Special Member Functions

#pragma region Virtual Methods
	public:
		/// <summary>
		/// Virtual handler method for determining how the helper will respond to a given JSON::Value.
		/// </summary>
		/// <param name="data">Reference to the SharedData instance to be filled with parsed data.</param>
		/// <param name="key">A const reference to the key string associated witht the JSON::Value.</param>
		/// <param name="value">The JSON::Value to be parsed.</param>
		/// <param name="isArray">Boolean describing if the value is an array.</param>
		/// <returns>True if the value is handled by the helper. Otherwise, false.</returns>
		virtual bool StartHandler(JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value, bool isArray) = 0;

		/// <summary>
		/// Virtual handler method for determining how the helper will react to a handled JSON::Value.
		/// </summary>
		/// <param name="data">Reference to the SharedData instance to be filled with parsed data.</param>
		/// <param name="key">A const reference to the key string associated witht the JSON::Value.</param>
		/// <returns>True if the value is handled by the helper. Otherwise, false.</returns>
		virtual bool EndHandler(JsonParseMaster::SharedData& data, const std::string& key) = 0;

		/// <summary>
		/// Virtual method for any initialization required by the helper before a parse.
		/// </summary>
		virtual void Initialize() {};
#pragma endregion Virtual Methods
	};
}


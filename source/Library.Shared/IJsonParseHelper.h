#pragma once

#pragma region Includes
// Third Party
#pragma warning(disable : 26812)
#include "json/json.h"
#pragma warning(default : 26812)

// First Party
#include "JsonParseMaster.h"
#pragma endregion Includes


namespace Library
{
	/// <summary>
	/// Parse helper interface class for JSON to data conversions.
	/// </summary>
	class IJsonParseHelper
	{
	protected:
		IJsonParseHelper() = default;
		virtual ~IJsonParseHelper() = default;

		IJsonParseHelper(const IJsonParseHelper& rhs) = delete;
		IJsonParseHelper& operator=(const IJsonParseHelper& rhs) = delete;
		IJsonParseHelper(IJsonParseHelper&& rhs) = delete;
		IJsonParseHelper& operator=(IJsonParseHelper&& rhs) = delete;

	public:
		void Initialize();

		virtual void Create() = 0;
		virtual bool StartHandler(JsonParseMaster::SharedData& data, const std::string& key, Json::Value& value, bool isArray) = 0;
		virtual bool EndHandler(JsonParseMaster::SharedData& data, const std::string& key) = 0;
	};
}


#pragma once

#pragma region Includes
// Standard
#include <stdint.h>

// Third Party
#pragma warning(disable : 26812)
#include "json/json.h"
#pragma warning(default : 26812)

// First Party
#include "RTTI.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// JSON parser master class for managing IJsonParserHelper handles and parsing JSON data.
	/// </summary>
	class JsonParseMaster final
	{
		class IJsonParseHelper;

#pragma region Shared Data
public:
		class SharedData : RTTI
		{
			friend JsonParseMaster;

		public:
			SharedData() = default;
			virtual ~SharedData() = default;

			SharedData(const SharedData& rhs) = delete;
			SharedData& operator=(const SharedData& rhs) = delete;
			SharedData(SharedData&& rhs) = delete;
			SharedData& operator=(SharedData&& rhs) = delete;

		public:
			virtual SharedData* Create() = 0;
			
			JsonParseMaster* GetJsonParseMaster();
			const JsonParseMaster* GetJsonParseMaster() const;

			std::uint16_t Depth() const;
			void IncrementDepth();
			void DecrementDepth();
		
		private:
			void SetJsonParseMaster(JsonParseMaster* master);
			
		private:
			JsonParseMaster* mMaster{ nullptr };
			std::uint16_t mDepth{ 0 };
		};
#pragma endregion Shared Data

	public:
		JsonParseMaster();
		~JsonParseMaster();

		JsonParseMaster(const JsonParseMaster& rhs) = delete;
		JsonParseMaster& operator=(const JsonParseMaster& rhs) = delete;
		JsonParseMaster(JsonParseMaster&& rhs) = default;
		JsonParseMaster& operator=( JsonParseMaster&& rhs) = default;

		JsonParseMaster& Clone() const;

	public:
		void AddHelper(IJsonParseHelper& helper);
		bool RemoveHelper(IJsonParseHelper& helper);
		
		bool Parse(const std::string& string);
		bool ParseFromFile(const std::string& filename);
		bool Parse(const std::istream& inputStream);

		const std::string& GetFilename();

		void SetSharedData(SharedData* sharedData);
		SharedData* GetSharedData();

	private:
		bool ParseMembers(const Json::Value& value);
		bool Parse(const std::string& key, Json::Value& value, bool isArray);

	private:
		SharedData* mSharedData{ nullptr };
		const std::string& mFilename;
	};
}

#include "JsonParseMaster.inl"

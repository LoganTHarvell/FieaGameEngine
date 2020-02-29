#pragma once

#pragma region Includes
// Standard
#include <stdint.h>

// Third Party
#pragma warning(disable : 26812)
#include <json/json.h>
#pragma warning(default : 26812)

#include <gsl/gsl>

// First Party
#include "RTTI.h"
#include "Vector.h"
#pragma endregion Includes

namespace Library
{
	class IJsonParseHelper;
	
	/// <summary>
	/// JSON parser master class for managing IJsonParserHelper handles and parsing JSON data.
	/// </summary>
	class JsonParseMaster final
	{
#pragma region Shared Data
public:
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI)

			friend JsonParseMaster;

		public:
			SharedData() = default;
			virtual ~SharedData() = default;

			SharedData(const SharedData& rhs) = delete;
			SharedData& operator=(const SharedData& rhs) = delete;
			SharedData(SharedData&& rhs) = delete;
			SharedData& operator=(SharedData&& rhs) = delete;

		public:
			virtual gsl::owner<SharedData*> Create() const = 0;
			
			virtual void Initialize() {};

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
		JsonParseMaster(SharedData* sharedData=nullptr);
		~JsonParseMaster();

		JsonParseMaster(const JsonParseMaster& rhs) = delete;
		JsonParseMaster& operator=(const JsonParseMaster& rhs) = delete;
		JsonParseMaster(JsonParseMaster&& rhs) = default;
		JsonParseMaster& operator=( JsonParseMaster&& rhs) = default;

		gsl::owner<JsonParseMaster*> Clone() const;

	public:
		void AddHelper(IJsonParseHelper& helper);
		bool RemoveHelper(IJsonParseHelper& helper);
		
		void Parse(const std::string& string);
		void Parse(std::istream& inputStream);
		void ParseFromFile(const std::string& filename);

		const std::string& GetFilename();

		void SetSharedData(SharedData& sharedData);
		SharedData* GetSharedData();

	private:
		void ParseMembers(const Json::Value& value);
		void Parse(const std::string& key, const Json::Value& value, bool isArray);

	private:
		SharedData* mSharedData;
		Vector<IJsonParseHelper*> mHelpers;
		bool mIsClone{ false };

		const std::string* mFilename{ nullptr };
	};
}

#include "JsonParseMaster.inl"

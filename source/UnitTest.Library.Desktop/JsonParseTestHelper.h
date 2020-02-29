#pragma once

#include "IJsonParseHelper.h"
#include "Stack.h"

namespace UnitTests
{
	class JsonParseTestHelper : public Library::IJsonParseHelper
	{ 

	public:
		class SharedData : public Library::JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, Library::JsonParseMaster::SharedData)

			friend JsonParseTestHelper;

		public:
			virtual gsl::owner<Library::JsonParseMaster::SharedData*> Create() const override;
			virtual void Initialize() override;

		public:
			int GetInt();
			float GetFloat();
			const std::string& GetString();
			const gsl::span<int> GetArray();

		private:
			int mInt{ 0 };
			float mFloat{ 0 };
			std::string mString;
			int mArray[3]{ 0 };

			std::size_t arrayFillCount{ 0 };
		};

	public:
		virtual gsl::owner<IJsonParseHelper*> Create() const override;
		virtual bool StartHandler(Library::JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value, bool isArray) override;
		virtual bool EndHandler(Library::JsonParseMaster::SharedData& data, const std::string& key) override;

	private:
		Library::Stack<std::pair<const std::string&, const Json::Value&>> mStack;
	};
}


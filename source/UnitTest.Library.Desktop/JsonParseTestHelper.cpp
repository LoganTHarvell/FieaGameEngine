#include "pch.h"
#include "JsonParseTestHelper.h"

using namespace Library;

namespace UnitTests
{
	RTTI_DEFINITIONS(JsonParseTestHelper::SharedData)

	JsonParseTestHelper::SharedData::SharedData(SharedData&& rhs) : JsonParseMaster::SharedData(std::move(rhs))
	{
	}

	JsonParseTestHelper::SharedData& JsonParseTestHelper::SharedData::operator=(SharedData&& rhs)
	{
		JsonParseMaster::SharedData::operator=(std::move(rhs));
		return *this;
	}

	gsl::owner<JsonParseMaster::SharedData*> JsonParseTestHelper::SharedData::Create() const
	{
		return new SharedData();
	}

	int JsonParseTestHelper::SharedData::GetInt() const
	{
		return mInt;
	}

	float JsonParseTestHelper::SharedData::GetFloat() const
	{
		return mFloat;
	}

	const std::string& JsonParseTestHelper::SharedData::GetString() const
	{
		return mString;
	}

	const gsl::span<const int> JsonParseTestHelper::SharedData::GetArray() const
	{
		return gsl::span(mArray);
	}

	gsl::owner<IJsonParseHelper*> JsonParseTestHelper::Create() const
	{
		return new JsonParseTestHelper();
	}
	
	bool JsonParseTestHelper::StartHandler(Library::JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value, bool isArray)
	{
		SharedData* testHelperData = data.As<SharedData>();
		if (testHelperData == nullptr) return false;
		
		bool handled = false;

		if (key == "mArray" && isArray && value.size() == 3)
		{
			for (Json::ArrayIndex i = 0; i < value.size(); ++i)
			{
				if (value[i].isInt())
				{
					mStack.Push({ key, value[i] });
					++testHelperData->arrayFillCount;
					handled = true;
				}
			}
		}

		if ((key == "mInt" && value.isInt()) || (key == "mFloat" && value.isDouble()) || (key == "mString" && value.isString()))
		{
			mStack.Push({ key, value });
			handled = true;
		}

		return handled;
	}
	
	bool JsonParseTestHelper::EndHandler(Library::JsonParseMaster::SharedData& data, const std::string& key)
	{
		SharedData* testHelperData = data.As<SharedData>();
		assert(testHelperData != nullptr);
		
		auto& pair = mStack.Top();
		assert(&pair.first == &key);

		bool handled = true;

		if (key == "mInt" && pair.second.isInt())
		{
			testHelperData->mInt = pair.second.asInt();
		}
		else if (key == "mFloat" && pair.second.isDouble())
		{
			testHelperData->mFloat = static_cast<float>(pair.second.asDouble());
		}
		else if (key == "mString" && pair.second.isString()) 
		{
			testHelperData->mString = pair.second.asString();
		}
		else if (key == "mArray")
		{
			while (testHelperData->arrayFillCount > 0)
			{
				auto& element = mStack.Top();
				testHelperData->mArray[testHelperData->arrayFillCount-1] = element.second.asInt();
				--testHelperData->arrayFillCount;

				if (testHelperData->arrayFillCount > 0) mStack.Pop();
			}
		}
		else
		{
			handled = false;
		}

		mStack.Pop();

		return handled;
	}	

	void JsonParseTestHelper::SharedData::Initialize()
	{
		JsonParseMaster::SharedData::Initialize();

		mInt = 0;
		mFloat = 0;
		mString.clear();
		mArray[0] = mArray[1] = mArray[2] = 0;

		arrayFillCount = 0;
	}
}

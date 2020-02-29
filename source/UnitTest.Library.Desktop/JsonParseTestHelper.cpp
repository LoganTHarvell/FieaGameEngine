#include "pch.h"
#include "JsonParseTestHelper.h"

using namespace Library;

namespace UnitTests
{
	RTTI_DEFINITIONS(JsonParseTestHelper::SharedData)

	gsl::owner<JsonParseMaster::SharedData*> JsonParseTestHelper::SharedData::Create() const
	{
		return new SharedData();
	}

	void JsonParseTestHelper::SharedData::Initialize()
	{
		arrayFillCount = 0;
	}

	int JsonParseTestHelper::SharedData::GetInt()
	{
		return mInt;
	}

	float JsonParseTestHelper::SharedData::GetFloat()
	{
		return mFloat;
	}

	const std::string& JsonParseTestHelper::SharedData::GetString()
	{
		return mString;
	}

	const gsl::span<int> JsonParseTestHelper::SharedData::GetArray()
	{
		return gsl::span(mArray);
	}

	gsl::owner<IJsonParseHelper*> JsonParseTestHelper::Create() const
	{
		return new JsonParseTestHelper();
	}
	
	bool JsonParseTestHelper::StartHandler(Library::JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value, bool isArray)
	{
		if (!data.Is(SharedData::TypeIdClass())) return false;
		
		bool handled = false;

		if (isArray)
		{
			for (Json::ArrayIndex i = value.size(); i > 0; --i)
			{
				if (value[i-1].isInt())
				{
					mStack.Push({ key, value[i-1] });
					handled = true;
				}
			}
		}

		if (value.isInt() || value.isDouble() || value.isString())
		{
			mStack.Push({ key, value });
			handled = true;
		}

		return handled;
	}
	
	bool JsonParseTestHelper::EndHandler(Library::JsonParseMaster::SharedData& data, const std::string& key)
	{
		assert(data.Is(SharedData::TypeIdClass()));
		SharedData& testHelperData = *data.As<SharedData>();
		
		auto& pair = mStack.Top();
		assert(pair.first == key);

		bool handled = true;

		if (key == "mInt" && pair.second.isInt())
		{
			testHelperData.mInt = pair.second.asInt();
		}
		else if (key == "mFloat" && pair.second.isDouble())
		{
			testHelperData.mFloat = static_cast<float>(pair.second.asDouble());
		}
		else if (key == "mString" && pair.second.isString()) 
		{
			testHelperData.mString = pair.second.asString();
		}
		else if (key == "mArray" && pair.second.isInt() && testHelperData.arrayFillCount < 3)
		{
			testHelperData.mArray[testHelperData.arrayFillCount] = pair.second.asInt();
		}
		else
		{
			handled = false;
		}

		mStack.Pop();

		return handled;
	}	
}

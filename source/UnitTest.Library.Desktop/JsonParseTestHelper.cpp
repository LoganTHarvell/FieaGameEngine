#include "pch.h"
#include "JsonParseTestHelper.h"

using namespace Library;

namespace UnitTests
{
	RTTI_DEFINITIONS(JsonParseTestHelper)
		RTTI_DEFINITIONS(JsonParseTestHelper::SharedData)

		JsonParseTestHelper::SharedData::SharedData(SharedData&& rhs) noexcept : JsonParseMaster::SharedData(std::move(rhs))
	{
	}

	JsonParseTestHelper::SharedData& JsonParseTestHelper::SharedData::operator=(SharedData&& rhs) noexcept
	{
		JsonParseMaster::SharedData::operator=(std::move(rhs));
		return *this;
	}

	gsl::owner<JsonParseMaster::SharedData*> JsonParseTestHelper::SharedData::Create() const
	{
		return new SharedData();
	}

	void JsonParseTestHelper::SharedData::Initialize()
	{
		JsonParseMaster::SharedData::Initialize();

		mDataSizes.Clear();
	}

	std::size_t JsonParseTestHelper::SharedData::GetSize(const std::string& key) const
	{
		auto it = mDataSizes.Find(key);
		return it != mDataSizes.end() ? it->second : 0;
	}

	gsl::owner<IJsonParseHelper*> JsonParseTestHelper::Create() const
	{
		return new JsonParseTestHelper();
	}

	bool JsonParseTestHelper::StartHandler(Library::JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value, bool)
	{
		SharedData* testHelperData = data.As<SharedData>();
		if (testHelperData == nullptr) return false;

		bool handled;

		if (value.isObject())
		{
			mStack.Push({ key, value });
			handled = true;
		}
		else if (value.isArray())
		{
			mStack.Push({ key, value });
			handled = true;
		}
		else if (value.isInt() || value.isDouble() || value.isString())
		{
			mStack.Push({ key, value });
			handled = true;
		}
		else
		{
			handled = false;
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
		
		if (pair.second.isObject())
		{
			for (const auto& member : pair.second.getMemberNames())
			{
				testHelperData->mDataSizes[pair.first] += testHelperData->GetSize(member);
			}
		}
		if (pair.second.isArray())
		{
			for (const auto& v : pair.second)
			{
				if (v.isObject())
				{
					for (const auto& member : v.getMemberNames())
					{
						testHelperData->mDataSizes[pair.first] += testHelperData->GetSize(member);
					}
				}
				else if (!v.isArray())
				{
					testHelperData->mDataSizes[pair.first]++;
				}
			}
		}
		else if (pair.second.isInt() || pair.second.isDouble() || pair.second.isString())
		{
			testHelperData->mDataSizes[pair.first]++;
		}
		else
		{
			handled = false;
		}

		mStack.Pop();

		return handled;
	}
}

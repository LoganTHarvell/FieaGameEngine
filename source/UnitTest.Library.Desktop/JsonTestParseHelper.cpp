#include "pch.h"
#include "JsonTestParseHelper.h"

using namespace Library;

namespace UnitTests
{
	JsonTestParseHelper::SharedData::SharedData(SharedData&& rhs) noexcept : JsonParseMaster::SharedData(std::move(rhs))
	{
	}

	JsonTestParseHelper::SharedData& JsonTestParseHelper::SharedData::operator=(SharedData&& rhs) noexcept
	{
		JsonParseMaster::SharedData::operator=(std::move(rhs));
		return *this;
	}

	void JsonTestParseHelper::SharedData::Initialize()
	{
		JsonParseMaster::SharedData::Initialize();

		mDataSizes.Clear();
	}

	std::size_t JsonTestParseHelper::SharedData::GetSize(const std::string& key) const
	{
		auto it = mDataSizes.Find(key);
		return it != mDataSizes.end() ? it->second : 0;
	}

	bool JsonTestParseHelper::StartHandler(Library::JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value, bool)
	{
		if (!data.Is(SharedData::TypeIdClass())) return false;

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

	bool JsonTestParseHelper::EndHandler(Library::JsonParseMaster::SharedData& data, const std::string& key)
	{
		assert(data.Is(SharedData::TypeIdClass()));

		SharedData* testHelperData = data.As<SharedData>();

		auto& pair = mStack.Top();
		key; // UNREFERENCED PARAMETER
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

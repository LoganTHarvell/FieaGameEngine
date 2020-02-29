#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "JsonParseMaster.h"

// Standard
#include <istream>
#include <fstream>

// First Party
#include "IJsonParseHelper.h"
#pragma endregion Includes

namespace Library
{
	RTTI_DEFINITIONS(JsonParseMaster::SharedData)

	JsonParseMaster::~JsonParseMaster()
	{
		if (!mIsClone) return;

		if (mSharedData)
		{
			delete mSharedData;
			mSharedData = nullptr;
		}

		for (auto helper : mHelpers)
		{
			delete helper;
		}

		mHelpers.Clear();

		mIsClone = false;

		if (mFilename)
		{
			delete mFilename;
			mFilename = nullptr;
		}
	}

	gsl::owner<JsonParseMaster*> JsonParseMaster::Clone() const
	{
		JsonParseMaster* clone = new JsonParseMaster(mSharedData->Create());
		
		for (auto& helper : mHelpers)
		{
			clone->AddHelper(*helper->Create());
		}

		return clone;
	}

	void JsonParseMaster::AddHelper(IJsonParseHelper& helper)
	{
		mHelpers.PushBack(&helper);
	}

	bool JsonParseMaster::RemoveHelper(IJsonParseHelper& helper)
	{
		return mHelpers.Remove(&helper);
	}

	void JsonParseMaster::Parse(const std::string& string)
	{
		mSharedData->Initialize();

		for (auto helper : mHelpers)
		{
			helper->Initialize();
		}

		Json::Value root;
		std::istringstream input(string);
		input >> root;

		return ParseMembers(root);
	}

	void JsonParseMaster::Parse(std::istream& inputStream)
	{
		mSharedData->Initialize();

		for (auto helper : mHelpers)
		{
			helper->Initialize();
		}

		Json::Value root;
		inputStream >> root;

		return ParseMembers(root);
	}

	void JsonParseMaster::ParseFromFile(const std::string& filename)
	{
		std::filebuf fb;
		if (fb.open(filename, std::ios::in))
		{
			std::istream input(&fb);
			Parse(input);
		}
	}

	void Library::JsonParseMaster::ParseMembers(const Json::Value& value)
	{
		for (const auto& member : value.getMemberNames())
		{
			Parse(member, value[member], value[member].isArray());
		}
	}

	void JsonParseMaster::Parse(const std::string& key, const Json::Value& value, bool isArray)
	{
		for (auto helper : mHelpers)
		{
			if (helper->StartHandler(*mSharedData, key, value, isArray))
			{
				helper->EndHandler(*mSharedData, key);
				break;
			}
		}
	}
}

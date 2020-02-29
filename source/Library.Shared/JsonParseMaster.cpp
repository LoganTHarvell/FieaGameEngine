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

#pragma region Destructor
	JsonParseMaster::~JsonParseMaster()
	{
		if (!mOwnsSharedData && mOwnedHelperIndices.IsEmpty()) return;

		if (mOwnsSharedData)
		{
			delete mSharedData;
			mSharedData = nullptr;
			mOwnsSharedData = false;
		}

		for (auto index : mOwnedHelperIndices)
		{
			delete mHelpers[index];
		}

		mHelpers.Clear();
		mOwnedHelperIndices.Clear();
	}
#pragma endregion Destructor

#pragma region Virtual Constructor
	gsl::owner<JsonParseMaster*> JsonParseMaster::Clone() const
	{
		JsonParseMaster* clone = new JsonParseMaster();
		
		if (mSharedData)
		{
			clone->SetSharedData(*mSharedData->Create());
			clone->mOwnsSharedData = true;
		}

		clone->mOwnedHelperIndices.Reserve(mHelpers.Size());

		for (std::size_t i = 0; i < mHelpers.Size(); ++i)
		{
			clone->AddHelper(*mHelpers[i]->Create());
			clone->mOwnedHelperIndices.PushBack(i);
		}

		return clone;
	}
#pragma endregion Virtual Constructor

#pragma region Modifiers
	void JsonParseMaster::AddHelper(IJsonParseHelper& helper)
	{
		mHelpers.PushBack(&helper);
	}

	bool JsonParseMaster::RemoveHelper(IJsonParseHelper& helper)
	{
		for (auto index : mOwnedHelperIndices)
		{
			if (mHelpers[index] == &helper)
			{
				delete mHelpers[index];
				return mHelpers.Remove(mHelpers.begin()[index]);
			}
		}

		return mHelpers.Remove(&helper);
	}
#pragma endregion Modifiers

#pragma region Parse Methods
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
			mFilename = &filename;

			std::istream input(&fb);
			Parse(input);
		}
	}
#pragma endregion Parse Methods

#pragma region Parse Helper Methods
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
				mSharedData->IncrementDepth();
				helper->EndHandler(*mSharedData, key);
				mSharedData->DecrementDepth();
				break;
			}
		}
	}
#pragma endregion Parse Helper Methods
}

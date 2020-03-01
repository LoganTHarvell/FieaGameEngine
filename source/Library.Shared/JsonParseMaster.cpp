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
#pragma region Shared Data
	JsonParseMaster::SharedData::SharedData(SharedData&& rhs) : mMaster(rhs.mMaster), mDepth(rhs.mDepth)
	{
		if (mMaster) mMaster->SetSharedData(*this);

		rhs.mMaster = nullptr;
		rhs.mDepth = 0;
	}

	JsonParseMaster::SharedData& JsonParseMaster::SharedData::operator=(SharedData&& rhs)
	{
		mMaster = rhs.mMaster;
		mDepth = rhs.mDepth;
		
		if (mMaster) mMaster->SetSharedData(*this);

		rhs.mMaster = nullptr;
		rhs.mDepth = 0;

		return *this;
	}
#pragma endregion Shared Data

#pragma region Special Member Functions
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

	JsonParseMaster::JsonParseMaster(JsonParseMaster&& rhs) : 
		mSharedData(rhs.mSharedData), mHelpers(std::move(rhs.mHelpers)), mFilename(rhs.mFilename), 
		mOwnsSharedData(rhs.mOwnsSharedData), mOwnedHelperIndices(std::move(rhs.mOwnedHelperIndices))
	{
		mSharedData->SetJsonParseMaster(this);

		rhs.mSharedData = nullptr;
		rhs.mFilename = nullptr;
		rhs.mOwnsSharedData = false;
	}

	JsonParseMaster& JsonParseMaster::operator=(JsonParseMaster&& rhs)
	{
		mSharedData = rhs.mSharedData;
		mHelpers = std::move(rhs.mHelpers);
		mFilename = rhs.mFilename;
		mOwnsSharedData = rhs.mOwnsSharedData;
		mOwnedHelperIndices = std::move(rhs.mOwnedHelperIndices);
	
		mSharedData->SetJsonParseMaster(this);

		rhs.mSharedData = nullptr;
		rhs.mFilename = nullptr;
		rhs.mOwnsSharedData = false;

		return *this;
	}
#pragma endregion Special Member Functions

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
		return mHelpers.Remove(&helper);
	}
#pragma endregion Modifiers

#pragma region Parse Methods
	void JsonParseMaster::Parse(std::istream& inputStream)
	{
		mSharedData->Initialize();

		for (auto helper : mHelpers)
		{
			helper->Initialize();
		}

		if (!mSharedData || mHelpers.Size() == 0) return;

		Json::Value root;
		inputStream >> root;

		ParseMembers(root);
	}

	void JsonParseMaster::Parse(const std::string& string)
	{
		std::istringstream input(string);
		Parse(input);
	}

	void JsonParseMaster::ParseFromFile(const std::string& filename)
	{
		std::ifstream fb;
		fb.open(filename);

		if (fb.is_open())
		{
			mFilename = &filename;
			Parse(fb);
			fb.close();
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

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
	JsonParseMaster::SharedData::SharedData(SharedData&& rhs) noexcept : mMaster(rhs.mMaster), mDepth(rhs.mDepth)
	{
		if (mMaster) mMaster->SetSharedData(*this);

		rhs.mMaster = nullptr;
		rhs.mDepth = 0;
	}

	JsonParseMaster::SharedData& JsonParseMaster::SharedData::operator=(SharedData&& rhs) noexcept
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

	JsonParseMaster::JsonParseMaster(JsonParseMaster&& rhs) noexcept : 
		mSharedData(rhs.mSharedData), mHelpers(std::move(rhs.mHelpers)), mFilename(rhs.mFilename), 
		mOwnsSharedData(rhs.mOwnsSharedData), mOwnedHelperIndices(std::move(rhs.mOwnedHelperIndices))
	{
		mSharedData->SetJsonParseMaster(this);

		rhs.mSharedData = nullptr;
		rhs.mFilename.clear();
		rhs.mOwnsSharedData = false;
	}

	JsonParseMaster& JsonParseMaster::operator=(JsonParseMaster&& rhs) noexcept
	{
		mSharedData = rhs.mSharedData;
		mHelpers = std::move(rhs.mHelpers);
		mFilename = rhs.mFilename;
		mOwnsSharedData = rhs.mOwnsSharedData;
		mOwnedHelperIndices = std::move(rhs.mOwnedHelperIndices);
	
		mSharedData->SetJsonParseMaster(this);

		rhs.mSharedData = nullptr;
		rhs.mFilename.clear();
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
			clone->SetSharedData(*mSharedData->CreateAs<JsonParseMaster::SharedData>());
			clone->mOwnsSharedData = true;
		}

		clone->mOwnedHelperIndices.Reserve(mHelpers.Size());

		for (std::size_t i = 0; i < mHelpers.Size(); ++i)
		{
			clone->AddHelper(*mHelpers[i]->CreateAs<IJsonParseHelper>());
			clone->mOwnedHelperIndices.EmplaceBack(i);
		}

		return clone;
	}
#pragma endregion Virtual Constructor

#pragma region Modifiers
	void JsonParseMaster::AddHelper(IJsonParseHelper& helper)
	{
		auto helperTypeCheck = [&helper](IJsonParseHelper* currentHelper) 
		{
			return helper.TypeIdInstance() == currentHelper->TypeIdInstance(); 
		};

		if (std::find_if(mHelpers.begin(), mHelpers.end(), helperTypeCheck) != mHelpers.end())
		{
			throw std::runtime_error("Multiple helpers of the same type.");
		}

		mHelpers.EmplaceBack(&helper);
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

		mSharedData->IncrementDepth();
		ParseMembers(root);
		mSharedData->DecrementDepth();
	}

	void JsonParseMaster::Parse(const std::string& string)
	{
		std::istringstream input(string);
		Parse(input);
	}

	void JsonParseMaster::ParseFromFile(std::string filename)
	{
		std::ifstream filestream;
		filestream.open(filename);

		if (filestream.good())
		{
			mFilename = std::move(filename);
			Parse(filestream);
			filestream.close();
		}
	}
#pragma endregion Parse Methods

#pragma region Parse Helper Methods
	void Library::JsonParseMaster::ParseMembers(const Json::Value& value)
	{
		for (const auto& member : value.getMemberNames())
		{
			Parse(member, value[member]);
		}
	}

	void JsonParseMaster::Parse(const std::string& key, const Json::Value& value)
	{		
		for (auto helper : mHelpers)
		{
			if (value.isObject())
			{
				if (helper->StartHandler(*mSharedData, key, value))
				{
					mSharedData->IncrementDepth();
					ParseMembers(value);
					helper->EndHandler(*mSharedData, key);
					mSharedData->DecrementDepth();
					break;
				}
			}
			else if (value.isArray())
			{
				if (helper->StartHandler(*mSharedData, key, value))
				{
					for (const auto& element : value)
					{
						if (element.isObject())
						{
							mSharedData->IncrementDepth();
							ParseMembers(element);
							mSharedData->DecrementDepth();
						}
						else if (element.isArray())
						{
							Parse(key, element);
						}
						else
						{
							break;
						}
					}

					helper->EndHandler(*mSharedData, key);
					break;
				}
			}
			else
			{
				if (helper->StartHandler(*mSharedData, key, value))
				{
					helper->EndHandler(*mSharedData, key);
					break;
				}
			}
		}
	}
#pragma endregion Parse Helper Methods
}

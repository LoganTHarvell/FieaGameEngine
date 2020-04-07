#include "JsonParseMaster.h"

namespace Library
{
#pragma region Shared Data
#pragma region Accessors
	inline const JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster() const
	{
		return mMaster;
	}

	inline std::size_t JsonParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}
#pragma endregion Accessors

#pragma region Modifiers
	inline void JsonParseMaster::SharedData::SetJsonParseMaster(JsonParseMaster* master)
	{
		mMaster = master;
	}

	inline void JsonParseMaster::SharedData::IncrementDepth()
	{
		++mDepth;
	}
	
	inline void JsonParseMaster::SharedData::DecrementDepth()
	{
		--mDepth;
	}
#pragma endregion Modifiers
#pragma endregion Shared Data

#pragma region Constructor
	inline JsonParseMaster::JsonParseMaster(SharedData* sharedData)
	{
		mSharedData = sharedData;
	}
#pragma endregion Constructor

#pragma region Accessors
	inline const std::string& JsonParseMaster::GetFilename() const
	{
		return mFilename;
	}

	inline const JsonParseMaster::SharedData* JsonParseMaster::GetSharedData() const
	{
		return mSharedData;
	}
#pragma endregion Accessors

#pragma region Modifiers
	inline void JsonParseMaster::SetSharedData(SharedData& sharedData)
	{
		if (mSharedData)
		{
			mSharedData->SetJsonParseMaster(nullptr);

			if (mOwnsSharedData) delete mSharedData;
		}

		mSharedData = &sharedData;
		mSharedData->SetJsonParseMaster(this);
	}
#pragma endregion Modifiers
}
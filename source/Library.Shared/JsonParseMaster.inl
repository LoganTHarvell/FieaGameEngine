#pragma once

#include "JsonParseMaster.h"

namespace Library
{
#pragma region Shared Data
#pragma region Accessors
	inline JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster()
	{
		return mMaster;
	}

	inline const JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster() const
	{
		return mMaster;
	}

	inline std::uint16_t JsonParseMaster::SharedData::Depth() const
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
		return *mFilename;
	}

	inline JsonParseMaster::SharedData* JsonParseMaster::GetSharedData()
	{
		return mSharedData;
	}

	inline const JsonParseMaster::SharedData* JsonParseMaster::GetSharedData() const
	{
		return mSharedData;
	}
#pragma endregion Accessors

#pragma region Modifiers
	inline void JsonParseMaster::SetSharedData(SharedData& sharedData)
	{
		mSharedData = &sharedData;
	}
#pragma endregion Modifiers
}
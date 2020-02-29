#pragma once

#include "JsonParseMaster.h"

namespace Library
{
#pragma region Shared Data
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

	inline void JsonParseMaster::SharedData::IncrementDepth()
	{
		++mDepth;
	}
	
	inline void JsonParseMaster::SharedData::DecrementDepth()
	{
		--mDepth;
	}

	inline void JsonParseMaster::SharedData::SetJsonParseMaster(JsonParseMaster* master)
	{
		mMaster = master;
	}
#pragma endregion Shared Data

#pragma region Constructor
	inline JsonParseMaster::JsonParseMaster(SharedData* sharedData)
	{
		mSharedData = sharedData;
	}
#pragma endregion Constructor

#pragma region Accessors
	inline const std::string& JsonParseMaster::GetFilename()
	{
		return *mFilename;
	}

	inline void JsonParseMaster::SetSharedData(SharedData& sharedData)
	{
		mSharedData = &sharedData;
	}

	inline JsonParseMaster::SharedData* JsonParseMaster::GetSharedData()
	{
		return mSharedData;
	}
#pragma endregion Accessors
}
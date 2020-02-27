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
}
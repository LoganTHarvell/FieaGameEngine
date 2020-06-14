#include "pch.h"
#include "FooEntity.h"
#include "WorldState.h"

namespace UnitTests
{
	const Library::TypeManager::TypeInfo& FooEntity::TypeInfo()
	{
		static const Library::TypeManager::TypeInfo typeInfo
		{
			{
				{ "Data", Types::Float, false, 1, offsetof(FooEntity, mData) }
			},

			Entity::TypeIdClass()
		};

		return typeInfo;
	}

	FooEntity::FooEntity(const std::string& name) : Entity(TypeIdClass(), name)
	{
	}

	FooEntity::FooEntity(float data, const std::string& name) : Entity(TypeIdClass(), name),
		mData(data)
	{
	}

	gsl::owner<Library::Scope*> FooEntity::Clone() const
	{
		return new FooEntity(*this);
	}

	void FooEntity::Update(Library::WorldState& worldState)
	{
		Entity::Update(worldState);

		worldState.Entity = this;
		if (!mIsUpdated) mIsUpdated = true;
		worldState.Entity = nullptr;
	}

	std::string FooEntity::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (FooEntity)";
		return oss.str();
	}
	
	bool FooEntity::IsUpdated()
	{
		return mIsUpdated;
	}

	float& FooEntity::Data()
	{
		return mData;
	}

	float FooEntity::Data() const
	{
		return mData;
	}
}

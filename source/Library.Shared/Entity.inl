#pragma once

// Header
#include "Entity.h"

namespace Library
{
#pragma region Accessors
	inline const std::string& Entity::Name() const
	{
		return mName;
	}

	inline void Entity::SetName(const std::string& name)
	{
		mName = name;
	}

	inline bool Entity::Enabled() const
	{
		return mEnabled;
	}

	inline void Entity::SetEnabled(const bool enabled)
	{
		mEnabled = enabled;
	}

	inline std::size_t Entity::ChildCount() const
	{
		return mChildren.Size();
	}

	template<typename T>
	inline T* Entity::FindChild(const std::string& name)
	{		
		T* child = nullptr;

		Data* childData = Find(name);

		if (childData && childData->Type() == Types::Scope && childData->Size() > 0)
		{
			child = childData->Get<Scope*>()->As<T>();
		}

		return child;
	}

	template<typename T>
	inline const T* Entity::FindChild(const std::string& name) const
	{
		return const_cast<Entity*>(this)->FindChild<T>(name);
	}

	template<typename T>
	inline gsl::span<T*> Entity::FindChildArray(const std::string& name)
	{
		Data* childData = Find(name);

		if (childData && childData->Type() == Types::Scope && childData->Size() > 0 && childData->Get<Scope*>()->Is(T::TypeIdClass()))
		{
			return { reinterpret_cast<T**>(childData->Data<Scope*>()), static_cast<std::ptrdiff_t>(childData->Size()) };
		}

		return gsl::span<T*>(nullptr, nullptr);
	}

	template<typename T>
	inline gsl::span<const T* const> Entity::FindChildArray(const std::string& name) const
	{
		const Data* childData = Find(name);

		if (childData && childData->Type() == Types::Scope && childData->Size() > 0 && childData->Get<Scope*>()->Is(T::TypeIdClass()))
		{
			return { reinterpret_cast<T* const *>(childData->Data<Scope*>()), static_cast<std::ptrdiff_t>(childData->Size()) };
		}

		return gsl::span<const T* const>(nullptr, nullptr);
	}
#pragma endregion Accessors
}

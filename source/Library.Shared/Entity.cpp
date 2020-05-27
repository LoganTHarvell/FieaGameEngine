#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& Entity::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ NameKey, Types::String, false, 1, offsetof(Entity, mName) },
				{ ChildrenKey, Types::Scope, true, 1, 0 }
			},

			Attributed::TypeIdClass()
		};

		return typeInfo;
	}

	Entity::Entity(std::string name) : Attributed(TypeIdClass()), 
		mName(std::move(name)), mChildren(mPairPtrs[ChildrenIndex]->second)
	{
	}

	Entity::Entity(const Entity& rhs) : Attributed(rhs),
		mName(rhs.mName), mChildren(mPairPtrs[ChildrenIndex]->second)
	{
	}

	Entity& Entity::operator=(const Entity& rhs)
	{
		if (this == &rhs) return *this;

		mName = rhs.mName;
		Attributed::operator=(rhs);
		return *this;
	}

	Entity::Entity(Entity&& rhs) noexcept : Attributed(std::move(rhs)),
		mName(std::move(rhs.mName)), mChildren(mPairPtrs[ChildrenIndex]->second)
	{
	}

	Entity& Entity::operator=(Entity&& rhs) noexcept
	{
		if (this == &rhs) return *this;

		mName = std::move(rhs.mName);
		Attributed::operator=(std::move(rhs));
		return *this;
	}

	Entity::Entity(const RTTI::IdType typeId, std::string name) : Attributed(typeId),
		mName(std::move(name)), mChildren(mPairPtrs[ChildrenIndex]->second)
	{
	}

	gsl::owner<Scope*> Entity::Clone() const
	{
		return new Entity(*this);
	}

	const std::string& Entity::Name() const
	{
		return mName;
	}

	void Entity::SetName(const std::string& name)
	{
		mName = name;
	}

	Entity* Entity::GetParent() const
	{
		Scope* parent = Scope::GetParent();
		if (!parent) return nullptr;

		assert(parent->Is(Entity::TypeIdClass()));
		return static_cast<Entity*>(parent);
	}

	void Entity::SetParent(Entity* entity)
	{
		if (entity == nullptr)
		{
			Entity* parent = GetParent();
			if (parent) parent->Orphan(*this);
		}
		else
		{
			entity->Adopt(*this, mName);
		}
	}

	Entity::Data& Entity::Children()
	{
		return mChildren;
	}

	const Entity::Data& Entity::Children() const
	{
		return mChildren;
	}

	Entity& Entity::AddChild(Entity& child)
	{
		if (mUpdatingChildren)
		{
			PendingChild pendingChild
			{
				child,
				PendingChild::State::ToAdd,
			};

			mPendingChildren.EmplaceBack(pendingChild);
		}
		else
		{
			Adopt(child, ChildrenKey);
		}

		return child;
	}

	void Entity::DestroyChild(Entity& child)
	{
		if (mUpdatingChildren)
		{
			PendingChild pendingChild
			{
				child,
				PendingChild::State::ToRemove,
			};

			mPendingChildren.EmplaceBack(pendingChild);
		}
		else
		{
			delete Orphan(child);
		}
	}

	Entity& Entity::CreateChild(const std::string& className, const std::string& name)
	{
		Scope* newScope = Factory<Scope>::Create(className);
		if (!newScope) throw std::runtime_error("Create failed.");

		assert(newScope->Is(Entity::TypeIdClass()));

		Entity* newEntity = static_cast<Entity*>(newScope);
		newEntity->SetName(name);
		
		if (mUpdatingChildren)
		{
			PendingChild pendingChild
			{
				*newEntity,
				PendingChild::State::ToAdd,
			};

			mPendingChildren.EmplaceBack(pendingChild);
		}
		else
		{
			Adopt(*newScope, ChildrenKey);
		}

		return *newEntity;
	}

	const Entity::PendingChildList& Entity::PendingChildren() const
	{
		return mPendingChildren;
	}
	
	void Entity::Update(WorldState& worldState)
	{
		for (std::size_t i = 0; i < mChildren.Size(); ++i)
		{
			assert(mChildren[i].Is(Entity::TypeIdClass()));

			worldState.Entity = static_cast<Entity*>(mChildren.Get<Scope*>(i));
			worldState.Entity->Update(worldState);
		}

		worldState.Entity = nullptr;
	}

	std::string Entity::ToString() const
	{
		std::ostringstream oss;
		oss << mName << " (Entity)";
		return oss.str();
	}

	void Entity::UpdatePendingChildren()
	{
		for (PendingChild& pendingChild : mPendingChildren)
		{
			switch (pendingChild.ChildState)
			{
			case PendingChild::State::ToAdd:
				Adopt(pendingChild.Child, ChildrenKey);
				break;

			case PendingChild::State::ToRemove:
				delete Orphan(pendingChild.Child);
				break;

			default:
				break;
			}
		}

		mPendingChildren.Clear();
	}
}
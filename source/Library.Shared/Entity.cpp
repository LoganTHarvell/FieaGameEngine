#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Entity.h"
#include "WorldState.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& Entity::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			SignatureListType(),
			Attributed::TypeIdClass()
		};

		return typeInfo;
	}

	Entity::Entity(std::string name) : Attributed(TypeIdClass()), 
		mName(std::move(name))
	{
	}

	Entity::Entity(const Entity& rhs) : Attributed(rhs),
		mName(rhs.mName)
	{
		rhs.ForEachChild([this](const Entity& rhsChild)
		{
			assert(Find(rhsChild.mName) && Find(rhsChild.mName)->Size() > 0 
				&& Find(rhsChild.mName)->Type() == Types::Scope 
				&& Find(rhsChild.mName)->Get<Scope*>()->Is(Entity::TypeIdClass()));
			
			mChildren.EmplaceBack(Find(rhsChild.Name())->Get<Scope*>()->As<Entity>());
		});
	}

	Entity& Entity::operator=(const Entity& rhs)
	{
		if (this == &rhs) return *this;

		mName = rhs.mName;
		Attributed::operator=(rhs);

		rhs.ForEachChild([this](const Entity& rhsChild)
		{
			assert(Find(rhsChild.mName));
			assert(Find(rhsChild.mName)->Size() > 0);
			assert(Find(rhsChild.mName)->Type() == Types::Scope);
			assert(Find(rhsChild.mName)->Get<Scope*>()->Is(Entity::TypeIdClass()));

			mChildren.EmplaceBack(Find(rhsChild.Name())->Get<Scope*>()->As<Entity>());
		});
		
		return *this;
	}

	Entity::Entity(Entity&& rhs) noexcept : Attributed(std::move(rhs)),
		mName(std::move(rhs.mName)), mChildren(std::move(rhs.mChildren))
	{
	}

	Entity& Entity::operator=(Entity&& rhs) noexcept
	{
		if (this == &rhs) return *this;

		mName = std::move(rhs.mName);
		mChildren = std::move(rhs.mChildren);
		
		Attributed::operator=(std::move(rhs));
		
		return *this;
	}

	Entity::Entity(const RTTI::IdType typeId, std::string name) : Attributed(typeId),
		mName(std::move(name))
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

	std::size_t Entity::ChildCount() const
	{
		return mChildren.Size();
	}

	Entity* Entity::FindChild(const std::string& name)
	{
		Entity* child = nullptr;

		Data* childData = Find(name);

		if (childData && childData->Size() > 0)
		{
			assert(childData->Get<Scope*>()->Is(Entity::TypeIdClass()));
			child = static_cast<Entity*>(childData->Get<Scope*>());
		}

		return child;
	}

	const Entity* Entity::FindChild(const std::string& name) const
	{
		return const_cast<Entity*>(this)->FindChild(name);
	}

	void Entity::ForEachChild(const std::function<void(Entity&)>& functor)
	{
		for (auto* child : mChildren)
		{
			assert(child != nullptr);
			functor(*child);
		};
	}

	void Entity::ForEachChild(const std::function<void(const Entity&)>& functor) const
	{
		for (auto* child : mChildren)
		{
			assert(child != nullptr);
			functor(*child);
		};
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
			Adopt(child, child.Name());
			mChildren.EmplaceBack(&child);
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
			mChildren.Remove(&child);
			delete Orphan(child);
		}
	}

	Entity& Entity::CreateChild(const std::string& className, const std::string& name)
	{
		Entity* child = Factory<Entity>::Create(className);

		if (child == nullptr)
		{
			throw std::runtime_error("Create failed.");
		}
		
		child->SetName(name);
		
		if (mUpdatingChildren)
		{
			PendingChild pendingChild
			{
				*child,
				PendingChild::State::ToAdd,
			};

			mPendingChildren.EmplaceBack(pendingChild);
		}
		else
		{
			Adopt(*child, name);
			mChildren.EmplaceBack(child);
		}

		return *child;
	}
	
	void Entity::Update(WorldState& worldState)
	{
		mUpdatingChildren = true;
		
		ForEachChild([&worldState](Entity& entity)
		{
			worldState.Entity = &entity;
			worldState.Entity->Update(worldState);
		});
		
		worldState.Entity = nullptr;

		mUpdatingChildren = false;

		UpdatePendingChildren();
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
				AddChild(pendingChild.Child);
				break;

			case PendingChild::State::ToRemove:
				DestroyChild(pendingChild.Child);
				break;

			default:
				break;
			}
		}

		mPendingChildren.Clear();
	}
}
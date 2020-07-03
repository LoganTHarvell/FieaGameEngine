#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Entity.h"
#include "WorldState.h"
#pragma endregion Includes

namespace Library
{
	const SignatureList& Entity::Signatures()
	{
		static const SignatureList signatures
		{
			SignatureList(),
		};

		return signatures;
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

	Entity::Entity(const IdType typeId, std::string name) : Attributed(typeId),
		mName(std::move(name))
	{
	}

	gsl::owner<Scope*> Entity::Clone() const
	{
		return new Entity(*this);
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

	void Entity::ForEachChild(const std::function<void(Entity&)>& functor)
	{
		mUpdatingChildren = true;
		
		for (auto* child : mChildren)
		{
			assert(child != nullptr);
			functor(*child);
		};

		mUpdatingChildren = false;
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

	void Entity::Initialize(WorldState& worldState)
	{
		if (!mEnabled) return;

		ForEachChild([&worldState](Entity& entity)
		{
			worldState.Entity = &entity;
			worldState.Entity->Initialize(worldState);
		});

		worldState.Entity = nullptr;

		UpdatePendingChildren();
	}
	
	void Entity::Update(WorldState& worldState)
	{
		if (!mEnabled) return;
				
		ForEachChild([&worldState](Entity& entity)
		{
			worldState.Entity = &entity;
			worldState.Entity->Update(worldState);
		});
		
		worldState.Entity = nullptr;

		UpdatePendingChildren();
	}

	void Entity::Shutdown(WorldState& worldState)
	{
		if (!mEnabled) return;
				
		ForEachChild([&worldState](Entity& entity)
		{
			worldState.Entity = &entity;
			worldState.Entity->Shutdown(worldState);
		});
		
		worldState.Entity = nullptr;

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
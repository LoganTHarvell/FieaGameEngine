#include "pch.h"
#include "DestructibleWall.h"

#include "ActionRender.h"
#include "ActionAnimate.h"
#include "Sector.h"

#include "ActionExpireEntity.h"

#include "WorldHelpers.h"

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace Library;

namespace GameDesktop
{
	using namespace Library;

	const TypeManager::TypeInfo& DestructibleWall::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ SizeKey, Types::Integer, false, 1, offsetof(DestructibleWall, mSize) },
			},

			GameObject::TypeIdClass()
		};

		return typeInfo;
	}

	DestructibleWall::DestructibleWall(std::string name)
		: GameObject(TypeIdClass(), name)
	{
		if (Find("Transform")->Size() == 0)
		{
			Transform* transform = new Transform();
			transform->SetScale(glm::vec2(mSize, mSize));
			SetTransform(*transform);
		}
		AddRenderComponent(*new RenderComponent(CurrentSprite, ColumnCount, RowCount, StartColumnIndex, StartRowIndex));
		Adopt(*(new ActionRender()), "Actions");
	}

	namespace Helpers 
	{
		struct Point
		{
			float x;
			float y;
		};

		bool doOverlap(Point l1, Point r1, Point l2, Point r2)
		{
			// If one rectangle is on left side of other 
			if (l1.x >= r2.x || l2.x >= r1.x)
				return false;

			// If one rectangle is above other 
			if (l1.y >= r2.y || l2.y >= r1.y)
				return false;

			return true;
		}
	}

	void DestructibleWall::CheckPlayerCollision(WorldState& worldState) 
	{
		// Check collision with characters 
		auto characters = WorldHelpers::GetCharacters(worldState);
		for (auto& character : characters)
		{
			Transform* characterTransform = &character->GetTransform();
			Transform* myTransform = &GetTransform();

			bool overlap = Helpers::doOverlap(
				{ myTransform->GetLocation().x, myTransform->GetLocation().y },
				{ myTransform->GetLocation().x + myTransform->GetScale().x, myTransform->GetLocation().y + myTransform->GetScale().y },
				{ characterTransform->GetLocation().x, characterTransform->GetLocation().y },
				{ characterTransform->GetLocation().x + characterTransform->GetScale().x, characterTransform->GetLocation().y + characterTransform->GetScale().y });

			if (overlap)
				Die();
		}
	}

	void DestructibleWall::Update(Library::WorldState& worldState)
	{
		//CheckPlayerCollision(worldState);
		GameObject::Update(worldState);
	}

	void DestructibleWall::Die()
	{
		if (!mIsDying)
		{
			mIsDying = true;
			Adopt(*(new ActionAnimate(mAnimationFrameDelay)), "Actions");
			Adopt(*(new ActionExpireEntity(mDieTime)), "Actions");
		}
	}

	gsl::owner<GameObject*> DestructibleWall::Clone() const
	{
		return new DestructibleWall(*this);
	}
}

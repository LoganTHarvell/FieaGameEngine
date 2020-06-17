#include "pch.h"
#include "Explosion.h"

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

	const TypeManager::TypeInfo& Explosion::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ SizeKey, Types::Integer, false, 1, offsetof(Explosion, mSize) },
			},

			GameObject::TypeIdClass()
		};

		return typeInfo;
	}

	int Explosion::ExplosionChunkToInt(EExplosionChunk chunk)
	{
		return static_cast<int>(chunk);
	}

	Explosion::Explosion(std::string name)
		: GameObject(TypeIdClass(), name)
	{
		Transform* transform = new Transform();
		transform->SetScale(glm::vec2(mSize, mSize));
		SetTransform(*transform);

		AddRenderComponent(*new RenderComponent(CurrentSprite, ColumnCount, RowCount, StartColumnIndex, StartRowIndex));
		Adopt(*(new ActionRender()), "Actions");
		Adopt(*(new ActionRender("Render", 10)), "Actions"); 

		Die();
	}

	void Explosion::Update(Library::WorldState& worldState)
	{
		GameObject::Update(worldState);
	}

	void Explosion::SetExplosionChunk(EExplosionChunk explosionChunk)
	{
		mExplosionChunk = explosionChunk;

		RenderComponent& renderComp = GetRenderComponent();
		renderComp.SetColumnIndex(0);
		renderComp.SetRowIndex(static_cast<int>(explosionChunk));
	}

	void Explosion::Die()
	{
		Adopt(*(new ActionAnimate(mAnimationFrameDelay)), "Actions");
		Adopt(*(new ActionExpireEntity(mDieTime)), "Actions");
	}

	gsl::owner<GameObject*> Explosion::Clone() const
	{
		return new Explosion(*this);
	}
}

#pragma once

// Header
#include "Actor.h"

namespace Library
{
	inline const Actor* Actor::RootActor() const
	{
		return mRootActor;
	}

	inline void Actor::SetRootActor(Actor* actor)
	{
		mRootActor = actor;

		if (mRootActor == nullptr)
		{
			mWorldTransform = mLocalTransform;
		}
	}
	
	inline const Transform& Actor::LocalTransform() const
	{
		return mLocalTransform;
	}

	inline const Transform& Actor::WorldTransform() const
	{
		UpdateWorldTransform();
		return mWorldTransform;
	}

	inline void Actor::SetLocalTranslation(const glm::vec3& translation)
	{
		mLocalTransform.Translation() = translation;
		mDirtyWorldTransform = true;
	}

	inline void Actor::SetWorldTranslation(const glm::vec3& translation)
	{
		glm::vec3 newTranslation = translation;
		
		if (mRootActor)
		{
			mRootActor->UpdateWorldTransform();
			newTranslation = mRootActor->WorldTransform().Inverse() * glm::vec4(translation, 0);
		}
		
		mLocalTransform.Translation() = newTranslation;
		mDirtyWorldTransform = true;
	}

	inline void Actor::SetLocalRotation(const float angle, const glm::vec3& axis)
	{
		mLocalTransform.Rotation() = glm::angleAxis(angle, axis);
		mDirtyWorldTransform = true;
	}

	inline void Actor::SetWorldRotation(const float angle, const glm::vec3& axis)
	{
		SetWorldRotation(glm::angleAxis(angle, axis));
	}

	inline void Actor::SetLocalRotation(const glm::quat& quaternion)
	{
		mLocalTransform.Rotation() = quaternion;
		mDirtyWorldTransform = true;
	}

	inline void Actor::SetWorldRotation(const glm::quat& quaternion)
	{
		glm::quat newRotation = quaternion;

		if (mRootActor)
		{
			mRootActor->UpdateWorldTransform();
			newRotation = glm::inverse(mRootActor->WorldTransform().Rotation()) * newRotation;
		}

		mLocalTransform.Rotation() = newRotation;
		mDirtyWorldTransform = true;
	}

	inline void Actor::SetLocalScale(const glm::vec3& scale)
	{
		mLocalTransform.Scale() = scale;
		mDirtyWorldTransform = true;
	}

	inline void Actor::SetWorldScale(const glm::vec3& scale)
	{
		glm::vec3 newScale = scale;

		if (mRootActor)
		{
			mRootActor->UpdateWorldTransform();
			newScale /= mRootActor->WorldTransform().Scale();
		}

		mLocalTransform.Scale() = newScale;
		mDirtyWorldTransform = true;
	}

	inline void Actor::SetLocalTransform(const Transform& transform)
	{
		mLocalTransform = transform;
		mDirtyWorldTransform = true;
	}

	inline void Actor::SetWorldTransform(const Transform& transform)
	{
		Transform newTransform = transform;

		if (mRootActor)
		{
			mRootActor->UpdateWorldTransform();
			
			newTransform.Translation() = mRootActor->WorldTransform().Inverse() * glm::vec4(transform.Translation(), 0);
			newTransform.Rotation() = glm::inverse(mRootActor->WorldTransform().Rotation()) * transform.Rotation();
			newTransform.Scale() /= mRootActor->WorldTransform().Scale();
		}

		mLocalTransform = newTransform;
		mDirtyWorldTransform = true;
	}

	inline void Actor::AddLocalTranslation(const glm::vec3& translation)
	{
		mLocalTransform.Translate(translation);
		mDirtyWorldTransform = true;
	}

	inline void Actor::AddWorldTranslation(const glm::vec3& translation)
	{
		UpdateWorldTransform();
		SetWorldTranslation(mWorldTransform.Translation() + translation);
	}

	inline void Actor::AddLocalRotation(const float angle, const glm::vec3& axis)
	{
		mLocalTransform.Rotate(angle, axis);
		mDirtyWorldTransform = true;
	}

	inline void Actor::AddWorldRotation(const float angle, const glm::vec3& axis)
	{
		UpdateWorldTransform();
		SetWorldRotation(glm::angleAxis(angle, axis) * mWorldTransform.Rotation());
	}

	inline void Actor::AddLocalRotation(const glm::quat& quaternion)
	{
		mLocalTransform.Rotate(quaternion);
		mDirtyWorldTransform = true;
	}

	inline void Actor::AddWorldRotation(const glm::quat& quaternion)
	{
		UpdateWorldTransform();
		SetWorldRotation(quaternion * mWorldTransform.Rotation());
	}

	inline void Actor::LocalTransformation(const Transform& transformation)
	{
		mLocalTransform *= transformation;
		mDirtyWorldTransform = true;
	}

	inline void Actor::WorldTransformation(const Transform& transformation)
	{
		UpdateWorldTransform();
		SetWorldTransform(mWorldTransform * transformation);
	}

	inline void Actor::UpdateWorldTransform() const
	{
		if (mDirtyWorldTransform)
		{
			if (mRootActor)
			{
				mRootActor->UpdateWorldTransform();
				mWorldTransform = mLocalTransform * mRootActor->mWorldTransform;
			}
			else
			{
				mWorldTransform = mLocalTransform;
			}

			mDirtyWorldTransform = false;
		}
	}
}

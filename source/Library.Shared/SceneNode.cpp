#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "SceneNode.h"

// First Party
#include "StreamHelper.h"
#pragma endregion Includes

namespace Library
{
	SceneNode::SceneNode(std::string name) :
		SceneNode(std::move(name), Transform::Identity)
	{
	}

	SceneNode::SceneNode(std::string name, const Transform& transform) :
		mName(std::move(name)), mTransform(transform)
    {
    }

	const std::string& SceneNode::Name() const
	{
		return mName;
	}

	std::weak_ptr<SceneNode> SceneNode::GetParent() const
	{
		return mParent;
	}

	Vector<std::shared_ptr<const SceneNode>>& SceneNode::Children()
	{
		return mChildren;
	}

	const Vector<std::shared_ptr<const SceneNode>>& SceneNode::Children() const
	{
		return mChildren;
	}

	const Transform& SceneNode::GetTransform() const
	{
		return mTransform;
	}

	void SceneNode::SetParent(std::shared_ptr<SceneNode> parent)
	{
		mParent = std::move(parent);
	}

	void SceneNode::SetTransform(const Transform& transform)
	{
		mTransform = transform;
	}

	void SceneNode::Save(OutputStreamHelper& streamHelper)
	{
		streamHelper << mName << mTransform;
	}

	void SceneNode::Load(InputStreamHelper& streamHelper)
	{
		streamHelper >> mName >> mTransform;
	}
}

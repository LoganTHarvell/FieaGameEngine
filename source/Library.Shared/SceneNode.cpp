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
		SceneNode(std::move(name), glm::identity<glm::mat4x4>())
	{
	}

	SceneNode::SceneNode(std::string name, const glm::mat4x4& transform) :
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

	const glm::mat4x4& SceneNode::GetTransform() const
	{
		return mTransform;
	}

	void SceneNode::SetParent(std::shared_ptr<SceneNode> parent)
	{
		mParent = std::move(parent);
	}

	void SceneNode::SetTransform(const glm::mat4x4& transform)
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

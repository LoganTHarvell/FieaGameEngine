#pragma once

#pragma region Includes
// First Party
#include "RTTI.h"
#include "Vector.h"
#include "Transform.h"
#pragma endregion Includes

namespace Library
{
	// Forwarded Classes
	class OutputStreamHelper;
	class InputStreamHelper;

	class SceneNode : public RTTI
	{
		RTTI_DECLARATIONS(SceneNode, RTTI)

	public:
		explicit SceneNode(std::string name = std::string());
		SceneNode(std::string name, const Transform& transform);
		SceneNode(const SceneNode&) = default;
		SceneNode(SceneNode&&) = default;
		SceneNode& operator=(const SceneNode&) = default;
		SceneNode& operator=(SceneNode&&) = default;
		virtual ~SceneNode() = default;
		
		const std::string& Name() const;
		std::weak_ptr<SceneNode> GetParent() const;
		Vector<std::shared_ptr<const SceneNode>>& Children();
		const Vector<std::shared_ptr<const SceneNode>>& Children() const;
		const Transform& GetTransform() const;

		void SetParent(std::shared_ptr<SceneNode> parent);

		void SetTransform(const Transform& transform);

		virtual void Save(OutputStreamHelper& streamHelper);

	protected:
		virtual void Load(InputStreamHelper& streamHelper);

		std::string mName;
		std::weak_ptr<SceneNode> mParent;
		Vector<std::shared_ptr<const SceneNode>> mChildren;
		Transform mTransform;
	};
}

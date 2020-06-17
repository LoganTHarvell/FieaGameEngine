#pragma once

namespace Library
{
	// Forward Declarations
	class OutputStreamHelper;
	class InputStreamHelper;

    class Keyframe final
    {
    public:
		Keyframe() = delete;
    	~Keyframe() = default;
		Keyframe(const Keyframe&) = default;
		Keyframe& operator=(const Keyframe&) = default;
		Keyframe(Keyframe&&) = default;
		Keyframe& operator=(Keyframe&&) = default;

    	explicit Keyframe(InputStreamHelper& streamHelper);
		Keyframe(const float time, const glm::vec3& translation, const glm::quat& rotationQuaternion, const glm::vec3& scale);

		float Time() const;
		const glm::vec3& Translation() const;
		const glm::quat& RotationQuaternion() const;
		const glm::vec3& Scale() const;

		glm::vec3 TranslationVector() const;
		glm::quat RotationQuaternionVector() const;
		glm::vec3 ScaleVector() const;
		class Transform TransformationMatrix() const;

		void Save(OutputStreamHelper& streamHelper) const;

    private:
		void Load(InputStreamHelper& streamHelper);

		float mTime{ 0.0f };
		glm::vec3 mTranslation{ 0, 0, 0 };
		glm::quat mRotationQuaternion{ 0, 0, 0, 1 };
		glm::vec3 mScale{ 1, 1, 1 };
    };
}

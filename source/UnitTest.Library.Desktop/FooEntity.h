#pragma once

#include "Entity.h"

namespace UnitTests
{
	class FooEntity final : public Library::Entity
	{
		RTTI_DECLARATIONS(FooEntity, Library::Entity)

	public:
		static const Library::TypeManager::TypeInfo& TypeInfo();

		explicit FooEntity(const std::string& name=std::string());
		virtual ~FooEntity() = default;
		FooEntity(const FooEntity & rhs) = default;
		FooEntity& operator=(const FooEntity & rhs) = default;
		FooEntity(FooEntity && rhs) noexcept = default;
		FooEntity& operator=(FooEntity && rhs) noexcept = default;

		virtual gsl::owner<Library::Scope*> FooEntity::Clone() const override;

		virtual void Update(Library::WorldState& worldState) override;

		virtual std::string ToString() const override;

		bool IsUpdated();

	private:
		int mData{ 0 };

		bool mIsUpdated{ false };
	};

	ConcreteFactory(FooEntity, Library::Entity)
}

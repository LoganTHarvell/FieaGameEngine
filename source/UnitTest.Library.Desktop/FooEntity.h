#pragma once

#include "Entity.h"

namespace UnitTests
{
	class FooEntity final : public Library::Entity
	{
		RTTI_DECLARATIONS(FooEntity, Library::Entity)

	public:
		static const Library::SignatureList& Signatures();

		explicit FooEntity(const std::string& name=std::string());
		explicit FooEntity(float data, const std::string& name=std::string());
		virtual ~FooEntity() = default;
		FooEntity(const FooEntity & rhs) = default;
		FooEntity& operator=(const FooEntity & rhs) = default;
		FooEntity(FooEntity && rhs) noexcept = default;
		FooEntity& operator=(FooEntity && rhs) noexcept = default;

		virtual gsl::owner<Library::Scope*> Clone() const override;

		virtual void Update(Library::WorldState& worldState) override;

		virtual std::string ToString() const override;

		bool IsUpdated();

		float& Data();
		float Data() const;

	private:
		float mData{ 0 };

		bool mIsUpdated{ false };
	};

	ConcreteFactory(FooEntity, Library::Entity)
}

#pragma once
#include "Attributed.h"
#include "World.h"
#include "Reaction.h"
#include "Factory.h"
#include "Stack.h"

namespace GameDesktop
{
	class PowerPickupReaction : public Library::Reaction
	{
		RTTI_DECLARATIONS_ABSTRACT(PowerPickupReaction, Library::Reaction)

	public:
		static const Library::TypeManager::TypeInfo& TypeInfo();
		explicit PowerPickupReaction(const std::string& name = std::string());
		virtual ~PowerPickupReaction() override = default;
		PowerPickupReaction(const PowerPickupReaction& rhs) = default;
		PowerPickupReaction& operator=(const PowerPickupReaction& rhs) = default;
		PowerPickupReaction(PowerPickupReaction&& rhs) noexcept = default;
		PowerPickupReaction& operator=(PowerPickupReaction&& rhs) noexcept = default;

	protected:

		explicit PowerPickupReaction(const RTTI::IdType typeId, const std::string& name = std::string());

	public:

		virtual gsl::owner<Scope*> Clone() const override = 0;

	public:

		virtual void Notify(Library::EventPublisher& eventPublisher);

	public:

		virtual Data* Find(const Key& key) override;

	public:
		Library::Stack<Scope> mParameterStack;
		Data& mSpawnBombActions;
#pragma endregion Data Members
	};
}


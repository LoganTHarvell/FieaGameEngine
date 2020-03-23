#pragma once

// First Party
#include "Attributed.h"
#include "WorldState.h"

namespace Library
{
	class Entity;

	class Sector final : public Attributed
	{
	public:
		static const Library::TypeManager::TypeInfo& TypeInfo();

	public:
		Sector();
		~Sector();
		Sector(const Sector& rhs);
		Sector& operator=(const Sector& rhs);
		Sector(Sector&& rhs);
		Sector& operator=(Sector&& rhs);

		virtual gsl::owner<Library::Scope*> Clone() const override;

	public:
		const std::string& Name() const;
		void SetName(const std::string& name);

		const World* GetWorld() const;
		void SetWorld(World& sector);

		DataType& Entities();
		const DataType& Entities() const;

		Entity* CreateEntity(const std::string& className, const std::string& name);

		void Update(WorldState& worldState);

		virtual std::string ToString() const override;

	private:
		class World* mWorld{ nullptr };

		std::string mName;
	};
}


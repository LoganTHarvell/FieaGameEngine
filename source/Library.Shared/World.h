#pragma once

// First Party
#include "Attributed.h"
#include "WorldState.h"

namespace Library
{
	class Sector;

	class World final : public Attributed
	{
	public:
		static const Library::TypeManager::TypeInfo& TypeInfo();
	
	public:
		World();
		~World();
		World(const World& rhs);
		World& operator=(const World& rhs);
		World(World&& rhs);
		World& operator=(World&& rhs);

		virtual gsl::owner<Library::Scope*> Clone() const override;

	public:		
		const std::string& Name() const;
		void SetName(const std::string& name);

		DataType& Sectors();
		const DataType& Sectors() const;

		Sector* CreateSector(const std::string& name);

		void Update();

		virtual std::string ToString() const override;

	private:
		GameClock mGameClock;
		WorldState mWorldState;
		
		std::string mName;
	};
}


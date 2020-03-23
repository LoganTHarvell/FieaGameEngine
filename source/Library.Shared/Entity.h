#pragma once

#pragma region Includes
// First Party
#include "Attributed.h"
#include "WorldState.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents a base object within the reflection system.
	/// </summary>
	class Entity : public Attributed
	{
	public:
		static const Library::TypeManager::TypeInfo& TypeInfo();

	public:
		Entity();
		~Entity();
		Entity(const Entity& rhs);
		Entity& operator=(const Entity& rhs);
		Entity(Entity&& rhs);
		Entity& operator=(Entity&& rhs);

		virtual gsl::owner<Library::Scope*> Clone() const override;

	public:
		const std::string& Name() const;
		void SetName(const std::string& name);

		const Sector* GetSector() const;
		void SetSector(Sector& sector);

		void Update(WorldState& worldState);

		virtual std::string ToString() const override;

	private:
		class Sector* mSector{ nullptr };

		std::string mName;
	};
}


#include "pch.h"
#include "WorldHelpers.h"

#include "World.h"
#include "Sector.h"
#include "Entity.h"

namespace GameDesktop
{
	namespace WorldHelpers
	{
		using namespace Library;

		Character* GameDesktop::WorldHelpers::WorldSearchCharacter(WorldState& worldState, const std::string& name)
		{
			Scope* character = nullptr;
			auto& sectors = worldState.World->Sectors();
			for (int x = 0; static_cast<size_t>(x) < sectors.Size(); ++x)
			{
				auto& entities = sectors[x].As<Sector>()->Entities();
				for (auto y = 0; static_cast<size_t>(y) < entities.Size(); ++y)
				{
					if (entities[y]["Name"] == name)
						character = &entities[y];
				}
			}
			return static_cast<Character*>(character);
		}


		static Vector<Character*> sCharacters = Vector<Character*>();
		Vector<Character*> GetCharacters(WorldState& worldState)
		{
			if (sCharacters.Size() != 0)
				return sCharacters;

			Vector<Character*> mCharacters;
			for (int i = 1; i < 5; ++i)
			{
				std::string characterName = "Character" + std::to_string(i);

				Character* character = WorldHelpers::WorldSearchCharacter(worldState, characterName);
				if (character != nullptr)
					mCharacters.PushBack(character);
			}

			sCharacters = mCharacters;
			return sCharacters;
		}
	}
}
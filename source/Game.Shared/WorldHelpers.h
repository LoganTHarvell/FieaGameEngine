#pragma once
#include "WorldState.h"
#include "Character.h"
#include "Vector.h"

namespace GameDesktop
{
	namespace WorldHelpers
	{
		Character* WorldSearchCharacter(Library::WorldState& worldState, const std::string& name);
		
		Vector<Character*> GetCharacters(WorldState& worldState);
	}
}
#include "Reaction.h"

namespace Library
{
#pragma region Special Members
	inline Reaction::Reaction(const RTTI::IdType typeId, const std::string& name) : ActionList(typeId, name)
	{
	}
#pragma endregion Special Members

#pragma region RTTI Overrides
	inline std::string Reaction::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (Reaction)";
		return oss.str();
	}
#pragma endregion RTTI Overrides
}
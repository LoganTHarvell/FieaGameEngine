#include "pch.h"
#include "PlayerInputMessage.h"

using namespace Library;

namespace GameDesktop
{

#pragma region Static Members
	const TypeManager::TypeInfo& PlayerInputMessage::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			SignatureListType(),
			Attributed::TypeIdClass()
		};

		return typeInfo;
	}
#pragma endregion Static Members

#pragma region Special Members
	PlayerInputMessage::PlayerInputMessage(MessageType type) :
		Attributed(TypeIdClass()), mType(type)
	{
	}
#pragma endregion Special Members
	

#pragma region Virtual Copy Constructor
	gsl::owner<Scope*> PlayerInputMessage::Clone() const
	{
		return new PlayerInputMessage(*this);
	}
#pragma endregion Virtual Copy Constructor
}
#include "pch.h"
#include "MenuInputMessage.h"

using namespace Library;

namespace GameDesktop
{
#pragma region Static Members
	const TypeManager::TypeInfo& MenuInputMessage::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			SignatureListType(),
			Attributed::TypeIdClass()
		};

		return typeInfo;
	}
#pragma endregion Static Members

	MenuInputMessage::MenuInputMessage(MessageType type) : Attributed(TypeIdClass()), mType(type)
	{
	}

	gsl::owner<Scope*> MenuInputMessage::Clone() const
	{
		return new MenuInputMessage(*this);
	}

	MenuInputMessage::MessageType MenuInputMessage::GetType()
	{
		return mType;
	}

	const MenuInputMessage::MessageType MenuInputMessage::GetType() const
	{
		return mType;
	}

	void MenuInputMessage::SetType(MessageType type)
	{
		mType = type;
	}

	Library::World* MenuInputMessage::GetWorld() const
	{
		return mWorld;
	}

	void MenuInputMessage::SetWorld(Library::World* world)
	{
		mWorld = world;
	}
}
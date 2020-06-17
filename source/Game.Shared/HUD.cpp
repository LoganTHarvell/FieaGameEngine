#include "pch.h"
#include "HUD.h"

#include "HUDNumber.h"
#include "Game.h"

using namespace std::chrono;
namespace GameDesktop
{
	const TypeManager::TypeInfo& HUD::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{HUDNumbersKey, Types::Scope, true, 1, 0}
			},
			GameObject::TypeIdClass()
		};
		return typeInfo;
	}
	HUD::HUD(const std::string& name) :
		GameObject(TypeIdClass(), name)
	{
	}
	void HUD::Update(WorldState& worldState)
	{
		if (!mInitialized)
		{
			Initialize(worldState);
		}

		RTTI* rtti = worldState.World->operator[](Game::GameKey).Get<RTTI*>();
		assert(rtti->Is(Game::TypeIdClass()));
		Game* game = static_cast<Game*>(rtti);
		seconds timeLeft = duration_cast<seconds>(game->GetEndTime() - worldState.GameTime->CurrentTime());
		mSecondsLeft = static_cast<unsigned int>(timeLeft.count());
		UpdateNumbers();

		GameObject::Update(worldState);
		for (size_t i = 0; i < HUDNumbersSize(); ++i)
		{
			GetHUDNumber(i).Update(worldState);
		}
	}
	gsl::owner<HUD*> HUD::Clone() const
	{
		return new HUD(*this);
	}
	HUDNumber& HUD::GetHUDNumber(size_t index)
	{
		Data& datum = (*this)[HUDNumbersKey];
		assert(datum.Size() > 0 && index < datum.Size());
		Scope& scope = datum[index];
		assert(scope.Is(HUDNumber::TypeIdClass()));
		return static_cast<HUDNumber&>(scope);
		
	}
	size_t HUD::HUDNumbersSize()
	{
		return (*this)[HUDNumbersKey].Size();
	}
	void HUD::Initialize(WorldState& worldState)
	{
		UpdateNumbers();

		// Updating the transform
		RTTI* rtti = worldState.World->operator[](Game::GameKey).Get<RTTI*>();
		assert(rtti->Is(Game::TypeIdClass()));
		Game* game = static_cast<Game*>(rtti);
		auto& windowManager = game->GetWindowManager();

		Transform& transform = GetTransform();
		transform.SetLocation(glm::vec2(windowManager.Width() / 2.0f, windowManager.Height() - 16.0f * Map::scalingFactorX));
		transform.SetScale(glm::vec2(windowManager.Width() / Map::scalingFactorX, windowManager.Height() / Map::scalingFactorY / 15.0f * 2));

		GetHUDNumber(0).GetTransform().SetLocation(glm::vec2(windowManager.Width() / 2.0f - 16 * Map::scalingFactorX, windowManager.Height() - 16 * Map::scalingFactorY));
		GetHUDNumber(1).GetTransform().SetLocation(glm::vec2(windowManager.Width() / 2.0f + 8 * Map::scalingFactorX, windowManager.Height() - 16 * Map::scalingFactorY));
		GetHUDNumber(2).GetTransform().SetLocation(glm::vec2(windowManager.Width() / 2.0f + 24 * Map::scalingFactorX, windowManager.Height() - 16 * Map::scalingFactorY));
		GetHUDNumber(0).GetTransform().SetScale(glm::vec2(3.0 * Map::scalingFactorX, 3.0f * Map::scalingFactorY));
		GetHUDNumber(1).GetTransform().SetScale(glm::vec2(3.0 * Map::scalingFactorX, 3.0f * Map::scalingFactorY));
		GetHUDNumber(2).GetTransform().SetScale(glm::vec2(3.0 * Map::scalingFactorX, 3.0f * Map::scalingFactorY));

		mInitialized = true;
	}
	void HUD::UpdateNumbers()
	{
		unsigned int seconds = mSecondsLeft;

		HUDNumber& number1 = GetHUDNumber(0);
		HUDNumber& number2 = GetHUDNumber(1);
		HUDNumber& number3 = GetHUDNumber(2);

		number1.UpdateNumber(seconds / 60);
		seconds %= 60;
		number2.UpdateNumber(seconds / 10);
		seconds %= 10;
		number3.UpdateNumber(seconds);
	}
}
#include "pch.h"
#include "RenderManager.h"

#include "Transform.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "Map.h"

namespace GameDesktop
{
	HashMap<std::string, std::shared_ptr<Texture>> RenderManager::sSpriteMap;
	Shader RenderManager::sShader;
	glm::mat4 RenderManager::sProjectionMatrix(1);
	void RenderManager::Initialize()
	{
		sShader.CreateFromFiles("Content\\Shader.vert", "Content\\Shader.frag");

		Vector<std::pair<std::string, std::string>> spriteItems = {
			{"TestBrick", "Content\\Image\\testBrick.png"},
			{"TestDirt", "Content\\Image\\testDirt.png"},
			{"WhiteBomberman", "Content\\Image\\bomberman_white.png"},
			{"WhiteBombermanDeath", "Content\\Image\\bomberman_white_death.png"},
			{"BlackBomberman", "Content\\Image\\bomberman_black.png"},
			{"BlackBombermanDeath", "Content\\Image\\bomberman_black_death.png"},
			{"RedBomberman", "Content\\Image\\bomberman_red.png"},
			{"RedBombermanDeath", "Content\\Image\\bomberman_red_death.png"},
			{"BlueBomberman", "Content\\Image\\bomberman_blue.png"},
			{"BlueBombermanDeath", "Content\\Image\\bomberman_blue_death.png"},
			{"Bomb", "Content\\Image\\bomb.png"},
			{"ExplodingBlock", "Content\\Image\\exploding_block.png"},
			{"BreakableBlock", "Content\\Image\\breakable_block.png"},
			{"Gray", "Content\\Image\\gray.png"},
			{"Green", "Content\\Image\\green.png"},
			{"GreenShadow", "Content\\Image\\green_with_shadow.png"},
			{"PermanentBlock", "Content\\Image\\permanent_block.png"},
			{"PowerUps", "Content\\Image\\powerups.png"},
			{"ExplodingPowerup", "Content\\Image\\exploding_powerup.png"},
			{"Level1", "Content\\Image\\level1.png"},
			{"MainMenu", "Content\\Image\\MainMenu.png"},
			{"MenuDisplay", "Content\\Image\\MenuDisplay.png"},
			{"DestructibleWall", "Content\\Image\\DestructibleWall.png"},
			{"Explosion", "Content\\Image\\Explosion.png"},
			{"HUD", "Content\\Image\\HUDBackground.png"},
			{"HUDNumber", "Content\\Image\\Numbers.png"}
		};

		sSpriteMap.Rehash(spriteItems.Size() * 2);
		for (auto& sprite : spriteItems)
		{
			AddSprite(sprite.first, sprite.second);
		}
	}

	void RenderManager::AddSprite(const std::string& spriteName, const std::string& filePath)
	{
		auto [it, inserted] = sSpriteMap.Insert(std::make_pair(spriteName, std::make_shared<Texture>(filePath)));
		if (!inserted)
		{
			throw std::exception((spriteName+" already exists in the sprite map!").c_str());
		}
		(*it).second->LoadTexture();
	}
	void RenderManager::RehashSpritesMap(size_t newSize)
	{
		sSpriteMap.Rehash(newSize);
	}

	std::shared_ptr<Texture>& RenderManager::GetTexture(const std::string& spriteName)
	{
		return sSpriteMap.At(spriteName);
	}
	const Shader& RenderManager::GetShader()
	{
		return sShader;
	}
	const glm::mat4& RenderManager::GetProjectionMatrix()
	{
		return sProjectionMatrix;
	}
	void RenderManager::SetProjectionMatrix(const glm::mat4& projectionMatrix)
	{
		sProjectionMatrix = projectionMatrix;
	}
	void RenderManager::Render(RenderQueue& renderQueue)
	{
		while (!renderQueue.IsEmpty())
		{
			const GameObject* gameObject = renderQueue.Top();
			
			sShader.UseShader();

			glm::mat4 model(1);
			const Transform& transform = gameObject->GetTransform();
			model = glm::translate(model, transform.GetLocation());
			model = glm::rotate(model, glm::radians(transform.GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f));

			glm::vec2 scale = transform.GetScale();
			scale.x *= Map::scalingFactorX;
			scale.y *= Map::scalingFactorY;

			model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

			glUniformMatrix4fv(sShader.GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(sShader.GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(RenderManager::GetProjectionMatrix()));

			const RenderComponent& renderComponent = gameObject->GetRenderComponent();
			RenderManager::GetTexture(renderComponent.GetCurrentSprite())->UseTexture();
			gameObject->GetQuad().UpdateUV(renderComponent.GetColumnCount(), renderComponent.GetRowCount(), renderComponent.GetColumnIndex(), renderComponent.GetRowIndex());
			gameObject->GetQuad().Render();

			renderQueue.Pop();
		}
	}
}

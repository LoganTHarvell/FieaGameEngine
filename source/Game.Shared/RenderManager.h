#pragma once
#include <GL/gl3w.h>
#include <glm/glm.hpp>

#include "HashMap.h"
#include "Texture.h"
#include "Shader.h"
#include "Vector.h"
#include "RenderQueue.h"

using namespace Library;

namespace GameDesktop
{
	class RenderManager
	{
	public:
		static void Initialize();
		/// <summary>
		/// Add and load texture into the sprites map
		/// </summary>
		/// <param name="spriteName">Name of the sprite</param>
		/// <param name="filePath">Path to the image</param>
		static void AddSprite(const std::string& spriteName, const std::string& filePath);
		/// <summary>
		/// Rehash the sprites map
		/// </summary>
		/// <param name="newSize">New bucket size</param>
		static void RehashSpritesMap(size_t newSize);
		/// <summary>
		/// Get the texture object from the sprites map
		/// </summary>
		/// <param name="spriteName">Name of the sprite stored in the texture map</param>
		/// <returns>A shared_ptr to the texture object</returns>
		static std::shared_ptr<Texture>& GetTexture(const std::string& spriteName);
		static const Shader& GetShader();
		static const glm::mat4& GetProjectionMatrix();
		static void SetProjectionMatrix(const glm::mat4& projectionMatrix);
		static void Render(RenderQueue& renderQueue);
	private:
		static HashMap<std::string, std::shared_ptr<Texture>> sSpriteMap;
		static Shader sShader;
		static glm::mat4 sProjectionMatrix;
	};
}
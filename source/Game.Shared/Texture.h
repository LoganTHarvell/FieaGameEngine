#pragma once
#include <GL/gl3w.h>

namespace GameDesktop
{
	class Texture
	{
	public:
		Texture(const std::string& fileLocation);
		Texture(const Texture&) = default;
		Texture(Texture&&) = default;
		Texture& operator=(const Texture&) = default;
		Texture& operator=(Texture&&) = default;
		~Texture();

		/// <summary>
		/// Load the texture and stores the texture ID
		/// </summary>
		void LoadTexture();
		/// <summary>
		/// Use the texture for next draw call
		/// </summary>
		void UseTexture() const;
		/// <summary>
		/// Deletes the texture
		/// </summary>
		void ClearTexture();
	private:
		GLuint mTextureId = 0;
		int mWidth = 0, mHeight = 0, mBitDepth = 0;
		std::string mFileLocation;
	};
}

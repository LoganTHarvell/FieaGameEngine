#include "pch.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std::string_literals;
namespace GameDesktop
{
	Texture::Texture(const std::string& fileLocation) :
		mFileLocation(fileLocation)
	{
	}
	Texture::~Texture()
	{
		ClearTexture();
	}
	void Texture::LoadTexture()
	{
		unsigned char* textureData = stbi_load(mFileLocation.c_str(), &mWidth, &mHeight, &mBitDepth, 0);
        if (!textureData)
        {
            throw std::exception(("Failed to locate " + mFileLocation).c_str());
        }
        glGenTextures(1, &mTextureId);
        glBindTexture(GL_TEXTURE_2D, mTextureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, /*legacy value*/ 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(textureData);
	}
    void Texture::UseTexture() const
    {
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTextureId);
    }
    void Texture::ClearTexture()
	{
		glDeleteTextures(1, &mTextureId);
		mTextureId = 0;
		mWidth = 0;
		mHeight = 0;
		mBitDepth = 0;
		mFileLocation = std::string();
    }
}
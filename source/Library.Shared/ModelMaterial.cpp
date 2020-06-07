#include "pch.h"

#include "ModelMaterial.h"

#include "StreamHelper.h"

namespace Library
{

	ModelMaterial::ModelMaterial(Model& model, InputStreamHelper& streamHelper) :
		mModel(&model)
	{
		Load(streamHelper);
	}

	ModelMaterial::ModelMaterial(Model& model, ModelMaterialData&& modelMaterialData) :
		mModel(&model), mData(std::move(modelMaterialData))
	{
	}

	Model& ModelMaterial::GetModel()
	{
		return *mModel;
	}

	const Model& ModelMaterial::GetModel() const
	{
		return *mModel;
	}

	const std::string& ModelMaterial::Name() const
	{
		return mData.Name;
	}

	const HashMap<TextureType, Vector<std::string>>& ModelMaterial::Textures() const
	{
		return mData.Textures;
	}

	void ModelMaterial::Save(OutputStreamHelper& streamHelper) const
	{
		streamHelper << mData.Name;

		streamHelper << gsl::narrow_cast<uint32_t>(mData.Textures.Size());
		for (const auto& texturePair : mData.Textures)
		{
			streamHelper << gsl::narrow_cast<int32_t>(texturePair.first);
			streamHelper << gsl::narrow_cast<uint32_t>(texturePair.second.Size());
			for (const auto& texture : texturePair.second)
			{
				streamHelper << texture;
			}
		}
	}

	void ModelMaterial::Load(InputStreamHelper& streamHelper)
	{
		streamHelper >> mData.Name;

		uint32_t texturesCount;
		streamHelper >> texturesCount;
		for (uint32_t i = 0; i < texturesCount; i++)
		{
			int32_t textureType;
			streamHelper >> textureType;

			Vector<std::string> textures;
			uint32_t textureListCount;
			streamHelper >> textureListCount;
			textures.Reserve(textureListCount);
			for (uint32_t j = 0; j < textureListCount; j++)
			{
				std::string texture;
				streamHelper >> texture;
				textures.EmplaceBack(std::move(texture));
			}

			mData.Textures.Emplace(TextureType(textureType), std::move(textures));
		}
	}
}
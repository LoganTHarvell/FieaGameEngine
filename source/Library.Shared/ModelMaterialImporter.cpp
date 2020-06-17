#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "AssetImporter.h"

// Third Party
#pragma warning(disable : 26812)
#pragma warning(disable : 26451)
#include <assimp/material.h>
#pragma warning(default : 26451)
#pragma warning(default : 26812)

// First Party
#include "ModelMaterial.h"
#include "HashMap.h"
#pragma endregion Includes

namespace Library
{
    const HashMap<TextureType, uint32_t> AssetImporter::sTextureTypeMappings =
    {
        { TextureType::Diffuse, aiTextureType_DIFFUSE },
        { TextureType::SpecularMap, aiTextureType_SPECULAR },
        { TextureType::Ambient, aiTextureType_AMBIENT },
        { TextureType::Emissive, aiTextureType_EMISSIVE },
        { TextureType::Heightmap, aiTextureType_HEIGHT },
        { TextureType::NormalMap, aiTextureType_NORMALS },
        { TextureType::SpecularPowerMap, aiTextureType_SHININESS },
        { TextureType::DisplacementMap, aiTextureType_DISPLACEMENT },
        { TextureType::LightMap, aiTextureType_LIGHTMAP }
    };

    std::shared_ptr<ModelMaterial> AssetImporter::LoadModelMaterial(Model& model, aiMaterial& material)
    {
        ModelMaterialData modelMaterialData;

        aiString name;
        material.Get(AI_MATKEY_NAME, name);
        modelMaterialData.Name = name.C_Str();

        for (TextureType textureType = static_cast<TextureType>(0); textureType < TextureType::End; textureType = static_cast<TextureType>(static_cast<int>(textureType) + 1))
        {
            const aiTextureType mappedTextureType = static_cast<aiTextureType>(sTextureTypeMappings.At(textureType));

            const uint32_t textureCount = material.GetTextureCount(mappedTextureType);

        	if (textureCount > 0)
            {
                Vector<std::string> textures;
                textures.Reserve(textureCount);
                for (uint32_t textureIndex = 0; textureIndex < textureCount; textureIndex++)
                {
                    aiString path;
                    if (material.GetTexture(mappedTextureType, textureIndex, &path) == AI_SUCCESS)
                    {
                        textures.EmplaceBack(path.C_Str());
                    }
                }
                modelMaterialData.Textures.Emplace(textureType, std::move(textures));
            }
        }

        return std::make_shared<ModelMaterial>(model, std::move(modelMaterialData));
    }

}

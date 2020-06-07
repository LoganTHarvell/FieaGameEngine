#pragma once

#pragma region Includes
// First Party
#include "Vector.h"
#include "HashMap.h"
#pragma endregion Includes

namespace Library
{
	// Forwarded Classes
	class Model;
	class OutputStreamHelper;
	class InputStreamHelper;

    enum class TextureType
    {
        Diffuse = 0,
        SpecularMap,
        Ambient,
        Emissive,
        Heightmap,
        NormalMap,
        SpecularPowerMap,
        DisplacementMap,
        LightMap,
        End
    };

	struct ModelMaterialData final
	{
		std::string Name;
		HashMap<TextureType, Vector<std::string>> Textures;
	};

    class ModelMaterial
    {
    public:
		ModelMaterial(Model& model, InputStreamHelper& streamHelper);
		ModelMaterial(Model& model, ModelMaterialData&& modelMaterialData);
		ModelMaterial(const ModelMaterial&) = default;
		ModelMaterial(ModelMaterial&&) = default;
		ModelMaterial& operator=(const ModelMaterial&) = default;
		ModelMaterial& operator=(ModelMaterial&&) = default;
		~ModelMaterial() = default;

        Model& GetModel();
        const Model& GetModel() const;
        const std::string& Name() const;
        const HashMap<TextureType, Vector<std::string>>& Textures() const;

		void Save(OutputStreamHelper& streamHelper) const;

    private:
		void Load(InputStreamHelper& streamHelper);

        gsl::not_null<Model*> mModel;
		ModelMaterialData mData;
    };
}

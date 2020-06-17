#include "pch.h"
#include "ModelReader.h"
#include "Utility.h"

namespace Library
{
	ModelReader::ModelReader(Game& game) :
		ContentTypeReader(game, Model::TypeIdClass())
	{
	}

	std::shared_ptr<Model> ModelReader::_Read(const std::wstring& assetName)
	{
		return std::make_shared<Model>(String::ToString(assetName), std::string());
	}
}
#include "pch.h"
#include "ContentTypeReaderManager.h"
#include "Texture2DReader.h"
#include "TextureCubeReader.h"
#include "VertexShaderReader.h"
#include "PixelShaderReader.h"
#include "HullShaderReader.h"
#include "DomainShaderReader.h"
#include "GeometryShaderReader.h"
#include "ComputeShaderReader.h"
#include "ModelReader.h"

using namespace std;

namespace Library
{
	map<uint64_t, shared_ptr<AbstractContentTypeReader>> ContentTypeReaderManager::sContentTypeReaders;
	bool ContentTypeReaderManager::sInitialized;

	const map<uint64_t, shared_ptr<AbstractContentTypeReader>>& ContentTypeReaderManager::ContentTypeReaders()
	{
		return sContentTypeReaders;
	}

	bool ContentTypeReaderManager::AddContentTypeReader(shared_ptr<AbstractContentTypeReader> reader)
	{
		return sContentTypeReaders.emplace(reader->TargetTypeId(), move(reader)).second;
	}

	void ContentTypeReaderManager::Initialize(Game& game)
	{
		if (sInitialized == false)
		{
			// Add known content type readers
			AddContentTypeReader(make_shared<Texture2DReader>(game));
			AddContentTypeReader(make_shared<TextureCubeReader>(game));
			AddContentTypeReader(make_shared<VertexShaderReader>(game));
			AddContentTypeReader(make_shared<PixelShaderReader>(game));
			AddContentTypeReader(make_shared<HullShaderReader>(game));
			AddContentTypeReader(make_shared<DomainShaderReader>(game));
			AddContentTypeReader(make_shared<GeometryShaderReader>(game));
			AddContentTypeReader(make_shared<ComputeShaderReader>(game));
			AddContentTypeReader(make_shared<ModelReader>(game));

			sInitialized = true;
		}
	}

	void ContentTypeReaderManager::Shutdown()
	{
		sContentTypeReaders.clear();
		sInitialized = false;
	}
}
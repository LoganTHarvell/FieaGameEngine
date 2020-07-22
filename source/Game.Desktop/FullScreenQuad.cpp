#include "pch.h"
#include "FullScreenQuad.h"
#include "FullScreenQuadMaterial.h"
#include "Game.h"
#include "VertexDeclarations.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	FullScreenQuad::FullScreenQuad(Game& game) :
		DrawableGameComponent(game)
	{
	}

	shared_ptr<FullScreenQuadMaterial> FullScreenQuad::Material() const
	{
		return mMaterial;
	}

	void FullScreenQuad::Initialize()
	{
		const VertexPositionTexture sourceVertices[] =
		{
			VertexPositionTexture(glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
			VertexPositionTexture(glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
			VertexPositionTexture(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
			VertexPositionTexture(glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
		};

		const span<const VertexPositionTexture> vertices{ sourceVertices };
		mVertexBuffer = VertexPositionTexture::CreateVertexBuffer(mGame->GetWorldState().RenderingManager, vertices);

		const uint16_t sourceIndices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		const span<const uint16_t> indices{ sourceIndices };
		mIndexCount = narrow_cast<uint32_t>(indices.size());

		mIndexBuffer = mGame->GetWorldState().RenderingManager->CreateIndexBuffer(indices);

		mMaterial = make_shared<FullScreenQuadMaterial>(*mGame->GetWorldState().ContentManager, *mGame->GetWorldState().RenderingManager);
		mMaterial->Initialize();
	}

	void FullScreenQuad::Draw(const GameTime&)
	{
		if (mVertexBuffer != nullptr && mIndexBuffer != nullptr)
		{
			mMaterial->DrawIndexed(*mVertexBuffer, *mIndexBuffer, mIndexCount, Format::R16UInt);
		}
	}
}
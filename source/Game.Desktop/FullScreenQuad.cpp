#include "pch.h"
#include "FullScreenQuad.h"
#include "FullScreenQuadMaterial.h"
#include "Game.h"
#include "VertexDeclarations.h"
#include "DirectXHelper.h"

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
			VertexPositionTexture(XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)),
			VertexPositionTexture(XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)),
			VertexPositionTexture(XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)),
			VertexPositionTexture(XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)),
		};

		auto direct3DDevice = mGame->Direct3DDevice();
		const span<const VertexPositionTexture> vertices{ sourceVertices };
		VertexPositionTexture::CreateVertexBuffer(direct3DDevice, vertices, not_null<ID3D11Buffer**>(mVertexBuffer.put()));

		const uint16_t sourceIndices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		const span<const uint16_t> indices{ sourceIndices };
		mIndexCount = narrow_cast<uint32_t>(indices.size());
		CreateIndexBuffer(direct3DDevice, indices, not_null<ID3D11Buffer**>(mIndexBuffer.put()));

		mMaterial = make_shared<FullScreenQuadMaterial>(*mGame->GetWorldState().ContentManager, *mGame->GetWorldState().RenderingManager);
		mMaterial->Initialize();
	}

	void FullScreenQuad::Draw(const GameTime&)
	{
		mMaterial->DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount, DXGI_FORMAT_R16_UINT);
	}
}
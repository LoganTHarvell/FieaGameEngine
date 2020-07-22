#include "pch.h"

#include "RenderingManagerD3D11.h"

#include "Skybox.h"

#include "BufferD3D11.h"
#include "Game.h"
#include "FirstPersonCamera.h"
#include "Model.h"
#include "Mesh.h"
#include "SkyboxMaterial.h"
#include "VertexDeclarations.h"
#include "TextureCube.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	Skybox::Skybox(Game& game, const shared_ptr<Camera>& camera, const wstring& cubeMapFileName, float scale) :
		DrawableGameComponent(game, camera),
		mCubeMapFileName(cubeMapFileName), mScale(scale)
	{
	}

	void Skybox::Initialize()
	{
		const auto model = mGame->Content().Load<Model>(L"Models\\Sphere.obj.bin");
		Mesh* mesh = model->Meshes().At(0).get();
		mVertexBuffer = VertexPosition::CreateVertexBuffer(mGame->GetWorldState().RenderingManager, *mesh);

		WorldState& worldState = mGame->GetWorldState();
		assert(worldState.ContentManager && worldState.RenderingManager);
		
		mIndexBuffer = worldState.RenderingManager->CreateMeshIndexBuffer(*mesh);
		
		mIndexCount = narrow<uint32_t>(mesh->Indices().Size());

		auto textureCube = mGame->Content().Load<TextureCube>(mCubeMapFileName);
		mMaterial = make_shared<SkyboxMaterial>(*worldState.ContentManager, *worldState.RenderingManager, textureCube);
		mMaterial->Initialize();

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		auto* firstPersonCamera = mCamera->As<FirstPersonCamera>();
		if (firstPersonCamera != nullptr)
		{
			firstPersonCamera->AddPositionUpdatedCallback([this]() {
				const XMFLOAT3& currentPosition = mCamera->Position();
				XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale, mScale, mScale) * XMMatrixTranslation(currentPosition.x, currentPosition.y, currentPosition.z));
			});
		}
	}
	
	void Skybox::Draw(const GameTime&)
	{
		if (mUpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial->UpdateTransforms(wvp);
			mUpdateMaterial = false;
		}

		mMaterial->DrawIndexed(not_null<ID3D11Buffer*>(static_cast<BufferD3D11*>(mVertexBuffer)->Native()), static_cast<BufferD3D11*>(mIndexBuffer)->Native(), mIndexCount);
	}
}
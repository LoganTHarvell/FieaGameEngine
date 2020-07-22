#include "pch.h"

#include "ProxyModel.h"

#include "Game.h"
#include "Utility.h"
#include "Camera.h"
#include "VertexDeclarations.h"
#include "RasterizerStates.h"
#include "Model.h"
#include "Mesh.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	ProxyModel::ProxyModel(Game& game, const shared_ptr<Camera>& camera, const std::string& modelFileName, float scale) :
		DrawableGameComponent(game, camera),
		mModelFileName(modelFileName), mScale(scale),
		mMaterial(*game.GetWorldState().ContentManager, *game.GetWorldState().RenderingManager)
	{
	}

	const XMFLOAT3& ProxyModel::Position() const
	{
		return mPosition;
	}

	const XMFLOAT3& ProxyModel::Direction() const
	{
		return mDirection;
	}

	const XMFLOAT3& ProxyModel::Up() const
	{
		return mUp;
	}

	const XMFLOAT3& ProxyModel::Right() const
	{
		return mRight;
	}

	XMVECTOR ProxyModel::PositionVector() const
	{
		return XMLoadFloat3(&mPosition);
	}

	XMVECTOR ProxyModel::DirectionVector() const
	{
		return XMLoadFloat3(&mDirection);
	}

	XMVECTOR ProxyModel::UpVector() const
	{
		return XMLoadFloat3(&mUp);
	}

	XMVECTOR ProxyModel::RightVector() const
	{
		return XMLoadFloat3(&mRight);
	}

	bool& ProxyModel::DisplayWireframe()
	{
		return mDisplayWireframe;
	}

	void ProxyModel::SetPosition(float x, float y, float z)
	{
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		SetPosition(position);
	}

	void ProxyModel::SetPosition(FXMVECTOR position)
	{
		XMStoreFloat3(&mPosition, position);
		mUpdateWorldMatrix = true;
	}

	void ProxyModel::SetPosition(const XMFLOAT3& position)
	{
		mPosition = position;
		mUpdateWorldMatrix = true;
	}

	void ProxyModel::ApplyRotation(CXMMATRIX transform)
	{
		XMVECTOR direction = XMLoadFloat3(&mDirection);
		XMVECTOR up = XMLoadFloat3(&mUp);

		direction = XMVector3TransformNormal(direction, transform);
		direction = XMVector3Normalize(direction);

		up = XMVector3TransformNormal(up, transform);
		up = XMVector3Normalize(up);

		XMVECTOR right = XMVector3Cross(direction, up);
		up = XMVector3Cross(right, direction);

		XMStoreFloat3(&mDirection, direction);
		XMStoreFloat3(&mUp, up);
		XMStoreFloat3(&mRight, right);
		
		mUpdateWorldMatrix = true;
	}

	void ProxyModel::ApplyRotation(const XMFLOAT4X4& transform)
	{
		XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transformMatrix);
	}

	void ProxyModel::SetColor(const DirectX::XMFLOAT4& color)
	{
		mMaterial.SetSurfaceColor(color);
	}

	void ProxyModel::Initialize()
	{
		const auto model = mGame->Content().Load<Model>(String::ToWideString(mModelFileName));
		Mesh* mesh = model->Meshes().At(0).get();
		mVertexBuffer = VertexPosition::CreateVertexBuffer(mGame->GetWorldState().RenderingManager, *mesh);

		mIndexBuffer = mGame->GetWorldState().RenderingManager->CreateMeshIndexBuffer(*mesh);
		mIndexCount = narrow<uint32_t>(mesh->Indices().Size());

		mMaterial.Initialize();

		auto updateMaterialFunc = [this]() { mUpdateWorldMatrix = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);
	}

	void ProxyModel::Update(const GameTime&)
	{
		if (mUpdateWorldMatrix)
		{
			XMMATRIX worldMatrix = XMMatrixIdentity();
			MatrixHelper::SetForward(worldMatrix, mDirection);
			MatrixHelper::SetUp(worldMatrix, mUp);
			MatrixHelper::SetRight(worldMatrix, mRight);
			MatrixHelper::SetTranslation(worldMatrix, mPosition);

			XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale, mScale, mScale) * worldMatrix);
			mUpdateWorldMatrix = false;
			mUpdateMaterial = true;
		}
	}

	void ProxyModel::Draw(const GameTime&)
	{
		if (mUpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial.UpdateTransform(wvp);
			mUpdateMaterial = false;
		}

		if (mDisplayWireframe)
		{
			mGame->Direct3DDeviceContext()->RSSetState(RasterizerStates::Wireframe.get());
			
			if (mVertexBuffer != nullptr && mIndexBuffer != nullptr)
			{
				mMaterial.DrawIndexed(*mVertexBuffer, *mIndexBuffer, mIndexCount);
			}

			mGame->Direct3DDeviceContext()->RSSetState(nullptr);
		}
		else
		{
			if (mVertexBuffer != nullptr && mIndexBuffer != nullptr)
			{
				mMaterial.DrawIndexed(*mVertexBuffer, *mIndexBuffer, mIndexCount);
			}
		}
	}
}
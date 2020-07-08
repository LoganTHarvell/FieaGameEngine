#include "pch.h"
#include "RenderableFrustum.h"
#include "GameException.h"
#include "Game.h"
#include "Frustum.h"
#include "Camera.h"
#include "VertexDeclarations.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	RenderableFrustum::RenderableFrustum(Game& game, const shared_ptr<Camera>& camera, const XMFLOAT4& color) :
		DrawableGameComponent(game, camera),
		mColor(color),
		mMaterial(*game.GetWorldState().ContentManager, *game.GetWorldState().RenderingManager)
	{
	}

	const XMFLOAT3& RenderableFrustum::Position() const
    {
        return mPosition;
    }

    const XMFLOAT3& RenderableFrustum::Direction() const
    {
        return mDirection;
    }
    
    const XMFLOAT3& RenderableFrustum::Up() const
    {
        return mUp;
    }

    const XMFLOAT3& RenderableFrustum::Right() const
    {
        return mRight;
    }

    XMVECTOR RenderableFrustum::PositionVector() const
    {
        return XMLoadFloat3(&mPosition);
    }

    XMVECTOR RenderableFrustum::DirectionVector() const
    {
        return XMLoadFloat3(&mDirection);
    }

    XMVECTOR RenderableFrustum::UpVector() const
    {
        return XMLoadFloat3(&mUp);
    }
    
    XMVECTOR RenderableFrustum::RightVector() const
    {
        return XMLoadFloat3(&mRight);
    }

	void RenderableFrustum::SetPosition(float x, float y, float z)
    {
        XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
        SetPosition(position);
    }

    void RenderableFrustum::SetPosition(FXMVECTOR position)
    {
        XMStoreFloat3(&mPosition, position);
		mUpdateWorldMatrix = true;
    }

    void RenderableFrustum::SetPosition(const XMFLOAT3& position)
    {
        mPosition = position;
		mUpdateWorldMatrix = true;
    }

	void RenderableFrustum::ApplyRotation(CXMMATRIX transform)
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

    void RenderableFrustum::ApplyRotation(const XMFLOAT4X4& transform)
    {
        XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
        ApplyRotation(transformMatrix);
    }

	void RenderableFrustum::SetColor(const XMFLOAT4& color)
	{
		mMaterial.SetSurfaceColor(color);
	}

	void RenderableFrustum::InitializeGeometry(const Frustum& frustum)
	{
		array<VertexPosition, FrustumVertexCount> vertices;
		auto& corners = frustum.Corners();
		assert(corners.size() == vertices.size());
		for (uint32_t i = 0; i < FrustumVertexCount; ++i)
		{
			vertices[i].Position = XMFLOAT4(corners[i].x, corners[i].y, corners[i].z, 1.0f);
		}

		mVertexBuffer = nullptr;
		VertexPosition::CreateVertexBuffer(mGame->Direct3DDevice(), vertices, not_null<ID3D11Buffer**>(mVertexBuffer.put()));
	}

    void RenderableFrustum::Initialize()
	{		
		mMaterial.SetTopology(PrimitiveTopology::LineList);
		mMaterial.Initialize();
		SetColor(mColor);

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		mIndexBuffer = nullptr;
		CreateIndexBuffer(mGame->Direct3DDevice(), FrustumIndices, not_null<ID3D11Buffer**>(mIndexBuffer.put()));
	}

	void RenderableFrustum::Update(const GameTime&)
	{
		if (mUpdateWorldMatrix)
		{
			XMMATRIX worldMatrix = XMMatrixIdentity();
			MatrixHelper::SetForward(worldMatrix, mDirection);
			MatrixHelper::SetUp(worldMatrix, mUp);
			MatrixHelper::SetRight(worldMatrix, mRight);
			MatrixHelper::SetTranslation(worldMatrix, mPosition);

			XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
			mUpdateWorldMatrix = false;
			mUpdateMaterial = true;
		}
	}

	void RenderableFrustum::Draw(const GameTime&)
	{
		if (mUpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial.UpdateTransform(wvp);
			mUpdateMaterial = false;
		}

		mMaterial.DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), FrustumIndexCount, DXGI_FORMAT_R16_UINT);
	}
}
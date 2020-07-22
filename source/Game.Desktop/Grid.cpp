#include "pch.h"
#include "Grid.h"
#include "GameException.h"
#include "Game.h"
#include "Camera.h"
#include "Utility.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "VertexDeclarations.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	Grid::Grid(Game& game, const std::shared_ptr<Camera>& camera, std::uint32_t size, std::uint32_t scale, const XMFLOAT4& color) :
		DrawableGameComponent(game, camera),
		mMaterial(*game.GetWorldState().ContentManager, *game.GetWorldState().RenderingManager),
		mSize(size), mScale(scale), mColor(color)
	{
	}
	
	const XMFLOAT3& Grid::Position() const
	{
		return mPosition;
	}

	void Grid::SetPosition(const XMFLOAT3& position)
	{
		mPosition = position;

		XMMATRIX translation = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
		XMStoreFloat4x4(&mWorldMatrix, translation);
		mUpdateMaterial = true;
	}

	void Grid::SetPosition(float x, float y, float z)
	{
		SetPosition(XMFLOAT3(x, y, z));
	}

	void Grid::SetColor(const XMFLOAT4& color)
	{
		mMaterial.SetSurfaceColor(color);
	}

	const uint32_t Grid::Size() const
	{
		return mSize;
	}

	void Grid::SetSize(uint32_t size)
	{
		mSize = size;
		InitializeGrid();
	}

	const uint32_t Grid::Scale() const
	{
		return mScale;
	}

	void Grid::SetScale(uint32_t scale)
	{
		mScale = scale;
		InitializeGrid();
	}

	void Grid::Initialize()
	{
		mMaterial.SetTopology(PrimitiveTopology::LineList);
		mMaterial.Initialize();
		SetColor(mColor);

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		InitializeGrid();
	}

	void Grid::Draw(const GameTime&)
	{
		if (mUpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial.UpdateTransform(wvp);
			mUpdateMaterial = false;
		}

		if (mVertexBuffer != nullptr)
		{
			mMaterial.Draw(*mVertexBuffer, (mSize + 1) * 4, 0);
		}
	}

	void Grid::InitializeGrid()
	{
		RenderingManager* renderingManager = GetGame()->GetWorldState().RenderingManager;
		
		const int length = 4 * (mSize + 1);
		const int size = sizeof(VertexPosition) * length;
		
		std::unique_ptr<VertexPosition> vertexData(new VertexPosition[length]);		
		VertexPosition* vertices = vertexData.get();

		const float adjustedScale = mScale * 0.1f;
		const float maxPosition = mSize * adjustedScale / 2;

        for (unsigned int i = 0, j = 0; i < mSize + 1; i++, j = 4 * i)
        {
            const float position = maxPosition - (i * adjustedScale);

            // Vertical line
			vertices[j] = VertexPosition(glm::vec4(position, 0.0f, maxPosition, 1.0f));
            vertices[j + 1] = VertexPosition(glm::vec4(position, 0.0f, -maxPosition, 1.0f));

            // Horizontal line
            vertices[j + 2] = VertexPosition(glm::vec4(maxPosition, 0.0f, position, 1.0f));
            vertices[j + 3] = VertexPosition(glm::vec4(-maxPosition, 0.0f, position, 1.0f));
        }

		BufferDesc vertexBufferDesc;
		vertexBufferDesc.Size = size;
		vertexBufferDesc.BindFlagsValue.BufferTypeValue = BufferType::Vertex;
		mVertexBuffer = renderingManager->CreateBuffer(vertexBufferDesc, vertices);
	}
}
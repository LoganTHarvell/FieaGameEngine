#include "pch.h"

#include "SolarSystemDemo.h"

#include "BufferD3D11.h"
#include "FirstPersonCamera.h"
#include "VertexDeclarations.h"
#include "Game.h"
#include "JsonEntityParseHelper.h"
#include "Model.h"
#include "Mesh.h"
#include "ProxyModel.h"
#include "SolarBodyMaterial.h"
#include "RenderingManagerD3D11.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace Library;
using namespace DirectX;

namespace Demo
{
	SolarSystemDemo::SolarSystemDemo(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}

	SolarSystemDemo::~SolarSystemDemo()
	{
	}

	SolarSystem& SolarSystemDemo::GetSolarSystem()
	{
		return mSolarSystem;
	}

	const SolarSystem& SolarSystemDemo::GetSolarSystem() const
	{
		return mSolarSystem;
	}

	bool SolarSystemDemo::AnimationEnabled() const
	{
		return mAnimationEnabled;
	}

	void SolarSystemDemo::SetAnimationEnabled(bool enabled)
	{
		mAnimationEnabled = enabled;
	}

	void SolarSystemDemo::ToggleAnimation()
	{
		mAnimationEnabled = !mAnimationEnabled;
	}

	float SolarSystemDemo::AmbientLightIntensity() const
	{
		return mSolarSystem.AmbientLightIntensity();
	}

	void SolarSystemDemo::SetAmbientLightIntensity(float intensity)
	{
		mSolarSystem.SetAmbientLightIntensity(intensity);
	}

	float SolarSystemDemo::PointLightIntensity() const
	{
		return mSolarSystem.SunIntensity();
	}

	void SolarSystemDemo::SetPointLightIntensity(float intensity)
	{
		mSolarSystem.SetSunIntensity(intensity);
	}

	const XMFLOAT3& SolarSystemDemo::LightPosition() const
	{
		return mPointLight.Position();
	}

	XMVECTOR SolarSystemDemo::LightPositionVector() const
	{
		return mPointLight.PositionVector();
	}

	void SolarSystemDemo::SetLightPosition(const XMFLOAT3& position)
	{
		mPointLight.SetPosition(position);
		mSolarSystem.SetSunPosition({ position.x, position.y, position.z });
	}

	void SolarSystemDemo::SetLightPosition(FXMVECTOR position)
	{
		mPointLight.SetPosition(position);

		XMFLOAT3 materialPosition;
		XMStoreFloat3(&materialPosition, position);
		mSolarSystem.SetSunPosition({ materialPosition.x, materialPosition.y, materialPosition.z });
	}

	float SolarSystemDemo::LightRadius() const
	{
		return mSolarSystem.LightRadius();
	}

	void SolarSystemDemo::SetLightRadius(float radius)
	{
		mSolarSystem.SetLightRadius(radius);
	}

	float SolarSystemDemo::SpecularIntensity() const
	{
		return mSolarSystem.SpecularIntensity();
	}

	void SolarSystemDemo::SetSpecularIntensity(float intensity)
	{
		mSolarSystem.SetSpecularIntensity(intensity);
	}

	float SolarSystemDemo::SpecularPower() const
	{
		return mSolarSystem.SpecularPower();
	}

	void SolarSystemDemo::SetSpecularPower(float power)
	{
		mSolarSystem.SetSpecularPower(power);
	}

	void SolarSystemDemo::Initialize()
	{
		JsonEntityParseHelper::SharedData data(mSolarSystem);
		JsonParseMaster parser(&data);
		JsonEntityParseHelper entityParseHelper;
		parser.AddHelper(entityParseHelper);
		parser.ParseFromFile("Content/SolarSystem.json"s);

		mGame->GetWorldState().Sector = &mSolarSystem;
		
		auto direct3DDevice = mGame->Direct3DDevice();
		const auto model = mGame->Content().Load<Model>(L"Models\\Sphere.obj.bin"s);
		Mesh* mesh = model->Meshes().At(0).get();
		VertexPositionTextureNormal::CreateVertexBuffer(direct3DDevice, *mesh, not_null<ID3D11Buffer**>(mVertexBuffer.put()));

		assert(mGame->GetWorldState().RenderingManager);
		
		RenderingManager& renderingManager = *mGame->GetWorldState().RenderingManager;
		mIndexBuffer = renderingManager.CreateMeshIndexBuffer(*mesh);
		mIndexCount = narrow<uint32_t>(mesh->Indices().Size());

		mSolarSystem.Initialize(mGame->GetWorldState());

		SetLightPosition(XMFLOAT3(0.0f, 0.0, 0.0f));

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		auto* firstPersonCamera = mCamera->As<FirstPersonCamera>();

		if (firstPersonCamera != nullptr)
		{
			firstPersonCamera->AddPositionUpdatedCallback([this]() 
			{
				std::function<void(Entity&)> updateChildren;

				updateChildren = [this, &updateChildren](Entity& parent)
				{
					parent.ForEachChild([this, &updateChildren](Entity& entity)
					{
						SolarBody* solarBody = entity.As<SolarBody>();
						if (solarBody)
						{
							solarBody->GetMaterial().UpdateCameraPosition(mCamera->Position());
						}

						updateChildren(entity);
					});
				};

				updateChildren(mSolarSystem);
			});
		}
	}

	void SolarSystemDemo::Update(const GameTime&)
	{
		if (!mAnimationEnabled) return;
		
		mSolarSystem.Update(mGame->GetWorldState());
		mUpdateMaterial = true;
	}

	void SolarSystemDemo::Draw(const GameTime&)
	{
		DrawChildren(mSolarSystem);
		mUpdateMaterial = false;
	}

	void SolarSystemDemo::DrawChildren(Library::Entity& parent)
	{
		parent.ForEachChild([this](Entity& entity)
		{
			SolarBody* solarBody = entity.As<SolarBody>();

			if (solarBody)
			{
				SolarBodyMaterial& material = solarBody->GetMaterial();

				if (mUpdateMaterial)
				{
					auto mat4x4 = XMFLOAT4X4(&solarBody->WorldTransform()[0][0]);
					const XMMATRIX worldMatrix = XMLoadFloat4x4(&mat4x4);
					const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
					material.UpdateTransforms(wvp, XMMatrixTranspose(worldMatrix));
				}

				material.DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), static_cast<BufferD3D11*>(mIndexBuffer)->Native(), mIndexCount);
			}

			DrawChildren(entity);
		});
	}

	void SolarSystemDemo::HotReload()
	{
		JsonEntityParseHelper::SharedData data(mSolarSystem);
		JsonParseMaster parser(&data);
		JsonEntityParseHelper entityParseHelper;
		parser.AddHelper(entityParseHelper);
		parser.ParseFromFile("Content/SolarSystem.json"s);

		mSolarSystem.Initialize(mGame->GetWorldState());
	}
}

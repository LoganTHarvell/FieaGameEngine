#include "pch.h"
#include "RenderingGame.h"
#include "GameException.h"
#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "GamePadComponent.h"
#include "FpsComponent.h"
#include "SolarSystemDemo.h"
#include "Grid.h"
#include "FirstPersonCamera.h"
#include "SamplerStatesD3D11.h"
#include "RasterizerStates.h"
#include "VectorHelper.h"
#include "ImGuiComponent.h"
#include "imgui_impl_dx11.h"
#include "UtilityWin32.h"
#include <limits>

#include "Skybox.h"

using namespace std;
using namespace DirectX;
using namespace Library;

namespace Demo
{
	RenderingGame::RenderingGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback)
	{
	}

	void RenderingGame::Initialize()
	{
		SamplerStatesD3D11::Initialize(Direct3DDevice()); 
		RasterizerStates::Initialize(Direct3DDevice());

		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mMouse = make_shared<MouseComponent>(*this, MouseModes::Absolute);
		mComponents.push_back(mMouse);
		mServices.AddService(MouseComponent::TypeIdClass(), mMouse.get());

		mGamePad = make_shared<GamePadComponent>(*this);
		mComponents.push_back(mGamePad);
		mServices.AddService(GamePadComponent::TypeIdClass(), mGamePad.get());

		mCamera = make_shared<FirstPersonCamera>(*this);
		mCamera->SetAspectRatio(AspectRatio());
		mCamera->SetFarPlaneDistance(20000);
		mCamera->MovementRate() = 400;
		mComponents.push_back(mCamera);
		mServices.AddService(Camera::TypeIdClass(), mCamera.get());

		mGrid = make_shared<Grid>(*this, mCamera);
		mGrid->SetVisible(false);
		mComponents.push_back(mGrid);

		mSolarSystemDemo = make_shared<SolarSystemDemo>(*this, mCamera);
		mComponents.push_back(mSolarSystemDemo);

		auto skybox = make_shared<Skybox>(*this, mCamera, L"Textures\\Skybox.dds", 1000.0f);
		mComponents.push_back(std::move(skybox));

		auto imGui = make_shared<ImGuiComponent>(*this);
		mComponents.push_back(imGui);
		mServices.AddService(ImGuiComponent::TypeIdClass(), imGui.get());
		auto imGuiWndProcHandler = make_shared<UtilityWin32::WndProcHandler>(ImGui_ImplWin32_WndProcHandler);
		UtilityWin32::AddWndProcHandler(imGuiWndProcHandler);

		auto helpTextImGuiRenderBlock = make_shared<ImGuiComponent::RenderBlock>([this]()
		{
			ImGui::Begin("Controls");
			ImGui::SetNextWindowPos(ImVec2(10, 10));

			{
				stringstream fpsLabel;
				fpsLabel << setprecision(3) << "Frame Rate: " << mFpsComponent->FrameRate() << "    Total Elapsed Time: " << mGameTime.TotalGameTimeSeconds().count();
				ImGui::Text(fpsLabel.str().c_str());
			}

			ImGui::Text("Camera (WASD + Left-Click-Mouse-Look)");
			ImGui::Text("Move Point Light (Num-Pad 8/2, 4/6, 3/9)");			

			{
				stringstream movementSpeedLabel;
				movementSpeedLabel << setprecision(3) << "Movement Speed: " << mCamera->MovementRate();
				ImGui::Text(movementSpeedLabel.str().c_str());
			}
			{
				stringstream distanceScaleLabel;
				distanceScaleLabel << setprecision(3) << "Distance Scale (+Q/-Z): " << mSolarSystemDemo->GetSolarSystem().DistanceScale();
				ImGui::Text(distanceScaleLabel.str().c_str());
			}
			{
				stringstream timeScaleLabel;
				timeScaleLabel << setprecision(3) << "Time Scale (+E/-C): " << mSolarSystemDemo->GetSolarSystem().TimeScale();
				ImGui::Text(timeScaleLabel.str().c_str());
			}
			//{
			//	stringstream gridVisibleLabel;
			//	gridVisibleLabel << "Toggle Grid (G): " << (mGrid->Visible() ? "Visible" : "Not Visible");
			//	ImGui::Text(gridVisibleLabel.str().c_str());
			//}
			{
				stringstream animationEnabledLabel;
				animationEnabledLabel << "Toggle Animation (Space): " << (mSolarSystemDemo->AnimationEnabled() ? "Enabled" : "Disabled");
				ImGui::Text(animationEnabledLabel.str().c_str());
			}
			{
				stringstream ambientLightIntensityLabel;
				ambientLightIntensityLabel << setprecision(2) << "Ambient Light Intensity (+PgUp/-PgDown): " << mSolarSystemDemo->AmbientLightIntensity();
				ImGui::Text(ambientLightIntensityLabel.str().c_str());
			}
			{
				stringstream pointLightIntensityLabel;
				pointLightIntensityLabel << setprecision(2) << "Point Light Intensity (+Home/-End): " << mSolarSystemDemo->PointLightIntensity();
				ImGui::Text(pointLightIntensityLabel.str().c_str());
			}
			{
				stringstream specularIntensityLabel;
				specularIntensityLabel << setprecision(2) << "Specular Intensity (+Insert/-Delete): " << mSolarSystemDemo->SpecularIntensity();
				ImGui::Text(specularIntensityLabel.str().c_str());
			}
			//{
			//	stringstream specularPowerLabel;
			//	specularPowerLabel << "Specular Power (+O/-P): " << mSolarSystemDemo->SpecularPower();
			//	ImGui::Text(specularPowerLabel.str().c_str());
			//}
			//{
			//	stringstream pointLightRadiusLabel;
			//	pointLightRadiusLabel << "Point Light Radius (+B/-N): " << mSolarSystemDemo->LightRadius();
			//	ImGui::Text(pointLightRadiusLabel.str().c_str());
			//}

			ImGui::End();
		});
		imGui->AddRenderBlock(helpTextImGuiRenderBlock);

		mFpsComponent = make_shared<FpsComponent>(*this);
		mFpsComponent->SetVisible(false);
		mComponents.push_back(mFpsComponent);

		Game::Initialize();

		mCamera->SetPosition(-500, 100, 200);
		const glm::quat quaternion = glm::angleAxis(-1.2f, glm::vec3(0, 1, 0));
		const XMFLOAT4X4 rotationMatrix(&glm::toMat4(quaternion)[0][0]);
		mCamera->ApplyRotation(rotationMatrix);

		mAmbientLightIntensity = mSolarSystemDemo->AmbientLightIntensity();
		mPointLightIntensity = mSolarSystemDemo->PointLightIntensity();
		mSpecularIntensity = mSolarSystemDemo->SpecularIntensity();
		mSpecularPower = mSolarSystemDemo->SpecularPower();
	}

	void RenderingGame::Update(const GameTime &gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape) || mGamePad->WasButtonPressedThisFrame(GamePadButtons::Back))
		{
			Exit();
		}
		
		if (mKeyboard->WasKeyPressedThisFrame(Keys::R))
		{
			mSolarSystemDemo->HotReload();
		}

		if (mMouse->WasButtonPressedThisFrame(MouseButtons::Left))
		{
			mMouse->SetMode(MouseModes::Relative);
		}

		if (mMouse->WasButtonReleasedThisFrame(MouseButtons::Left))
		{
			mMouse->SetMode(MouseModes::Absolute);
		}

		if (mKeyboard->WasKeyPressedThisFrame(Keys::Space))
		{
			mSolarSystemDemo->ToggleAnimation();
		}

		if (mKeyboard->WasKeyPressedThisFrame(Keys::G))
		{
			mGrid->SetVisible(!mGrid->Visible());
		}

		if (mKeyboard->IsKeyDown(Keys::Q))
		{
			mSolarSystemDemo->GetSolarSystem().DistanceScale() += 1;
		}

		if (mKeyboard->IsKeyDown(Keys::Z))
		{
			float& distanceScale = mSolarSystemDemo->GetSolarSystem().DistanceScale();
			distanceScale -= 1;
			
			if (distanceScale < 0) distanceScale = 0;
		}

		if (mKeyboard->IsKeyDown(Keys::E))
		{
			mSolarSystemDemo->GetSolarSystem().TimeScale() += 0.05f;
		}

		if (mKeyboard->IsKeyDown(Keys::C))
		{
			float& timeScale = mSolarSystemDemo->GetSolarSystem().TimeScale();
			timeScale -= 0.05f;
		}

		if (mCamera->DefaultMovementRate < 10)
		{
			mCamera->MovementRate() = 10;
		}
		else if (mCamera->MovementRate() > 800)
		{
			mCamera->MovementRate() = 800;
		}
		else
		{
			mCamera->MovementRate() += clamp(static_cast<float>(mMouse->Wheel()), -1.0f, 1.0f);
		}
		
		UpdateAmbientLightIntensity(gameTime);
		UpdatePointLight(gameTime);
		UpdateSpecularLight(gameTime);

		Game::Update(gameTime);
	}

	void RenderingGame::Draw(const GameTime &gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.get(), BackgroundColor.f);
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Game::Draw(gameTime);

		HRESULT hr = mSwapChain->Present(1, 0);

		// If the device was removed either by a disconnection or a driver upgrade, we must recreate all device resources.
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			HandleDeviceLost();
		}
		else
		{
			ThrowIfFailed(hr, "IDXGISwapChain::Present() failed.");
		}
	}

	void RenderingGame::Shutdown()
	{
		mGrid = nullptr;
		mFpsComponent = nullptr;
		mSolarSystemDemo = nullptr;
		RasterizerStates::Shutdown();
		SamplerStatesD3D11::Shutdown();
		Game::Shutdown();		
	}

	void RenderingGame::Exit()
	{
		PostQuitMessage(0);
	}

	void RenderingGame::UpdateAmbientLightIntensity(const GameTime& gameTime)
	{
		if (mKeyboard->IsKeyDown(Keys::PageUp) && mAmbientLightIntensity < 1.0f)
		{
			mAmbientLightIntensity += gameTime.ElapsedGameTimeSeconds().count();
			mAmbientLightIntensity = min(mAmbientLightIntensity, 1.0f);
			mSolarSystemDemo->SetAmbientLightIntensity(mAmbientLightIntensity);
		}
		else if (mKeyboard->IsKeyDown(Keys::PageDown) && mAmbientLightIntensity > 0.0f)
		{
			mAmbientLightIntensity -= gameTime.ElapsedGameTimeSeconds().count();
			mAmbientLightIntensity = max(mAmbientLightIntensity, 0.0f);
			mSolarSystemDemo->SetAmbientLightIntensity(mAmbientLightIntensity);
		}
	}

	void RenderingGame::UpdatePointLight(const GameTime& gameTime)
	{
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		// Update light intensity
		if (mKeyboard->IsKeyDown(Keys::Home) && mPointLightIntensity < 1.0f)
		{
			mPointLightIntensity += elapsedTime;
			mPointLightIntensity = min(mPointLightIntensity, 1.0f);
			mSolarSystemDemo->SetPointLightIntensity(mPointLightIntensity);
		}
		else if (mKeyboard->IsKeyDown(Keys::End) && mPointLightIntensity > 0.0f)
		{
			mPointLightIntensity -= elapsedTime;
			mPointLightIntensity = max(mPointLightIntensity, 0.0f);
			mSolarSystemDemo->SetPointLightIntensity(mPointLightIntensity);
		}

		// Move light
		XMFLOAT3 movementAmount = Vector3Helper::Zero;
		if (mKeyboard->IsKeyDown(Keys::NumPad4))
		{
			movementAmount.x = -1.0f;
		}

		if (mKeyboard->IsKeyDown(Keys::NumPad6))
		{
			movementAmount.x = 1.0f;
		}

		if (mKeyboard->IsKeyDown(Keys::NumPad9))
		{
			movementAmount.y = 1.0f;
		}

		if (mKeyboard->IsKeyDown(Keys::NumPad3))
		{
			movementAmount.y = -1.0f;
		}

		if (mKeyboard->IsKeyDown(Keys::NumPad8))
		{
			movementAmount.z = -1.0f;
		}

		if (mKeyboard->IsKeyDown(Keys::NumPad2))
		{
			movementAmount.z = 1.0f;
		}

		const float LightMovementRate = 10.0f;
		const float LightModulationRate = static_cast<float>(numeric_limits<uint8_t>::max());
		if (movementAmount.x != 0.0f || movementAmount.y != 0.0f || movementAmount.z != 0.0f)
		{
			XMVECTOR movement = XMLoadFloat3(&movementAmount) * LightMovementRate * elapsedTime;
			mSolarSystemDemo->SetLightPosition(mSolarSystemDemo->LightPositionVector() + movement);
		}

		// Update the light's radius
		if (mKeyboard->IsKeyDown(Keys::B))
		{
			float radius = mSolarSystemDemo->LightRadius() + LightModulationRate * elapsedTime;
			mSolarSystemDemo->SetLightRadius(radius);
		}
		else if (mKeyboard->IsKeyDown(Keys::N))
		{
			float radius = mSolarSystemDemo->LightRadius() - LightModulationRate * elapsedTime;
			radius = max(radius, 0.0f);
			mSolarSystemDemo->SetLightRadius(radius);
		}
	}

	void RenderingGame::UpdateSpecularLight(const Library::GameTime & gameTime)
	{
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();

		if (mKeyboard->IsKeyDown(Keys::Insert) && mSpecularIntensity < 1.0f)
		{
			mSpecularIntensity += elapsedTime;
			mSpecularIntensity = min(mSpecularIntensity, 1.0f);
			mSolarSystemDemo->SetSpecularIntensity(mSpecularIntensity);
		}
		else if (mKeyboard->IsKeyDown(Keys::Delete) && mSpecularIntensity > 0.0f)
		{
			mSpecularIntensity -= elapsedTime;
			mSpecularIntensity = max(mSpecularIntensity, 0.0f);
			mSolarSystemDemo->SetSpecularIntensity(mSpecularIntensity);
		}

		const auto ModulationRate = numeric_limits<uint8_t>::max();
		if (mKeyboard->IsKeyDown(Keys::O) && mSpecularPower < numeric_limits<uint8_t>::max())
		{
			mSpecularPower += ModulationRate * elapsedTime;
			mSpecularPower = min(mSpecularPower, static_cast<float>(numeric_limits<uint8_t>::max()));
			mSolarSystemDemo->SetSpecularPower(mSpecularPower);
		}
		else if (mKeyboard->IsKeyDown(Keys::P) && mSpecularPower > 1.0f)
		{
			mSpecularPower -= ModulationRate * elapsedTime;
			mSpecularPower = max(mSpecularPower, 1.0f);
			mSolarSystemDemo->SetSpecularPower(mSpecularPower);
		}
	}
}
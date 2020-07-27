#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <map>
#include <gsl\gsl>
#include "DrawableGameComponent.h"
#include "GaussianBlur.h"
#include "FullScreenRenderTarget.h"
#include "FullScreenQuad.h"
#include <winrt\Windows.Foundation.h>

namespace Library
{
	class FullScreenQuadMaterial;

	struct BloomSettings
	{
		float BloomThreshold{ 0.45f };
		float BlurAmount{ 2.0f };
		float BloomIntensity{ 1.25f };
		float BloomSaturation{ 1.0f };
		float SceneIntensity{ 1.0f };
		float SceneSaturation{ 1.0f };
	};

	enum class BloomDrawModes
	{
		Normal = 0,
		GlowMap,
		BlurredGlowMap,
		End
	};

	class Bloom final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS_ABSTRACT(Bloom, DrawableGameComponent)

	public:
		Bloom(Game& game, const BloomSettings& bloomSettings = DefaultBloomSettings);
		Bloom(const Bloom&) = delete;
		Bloom(Bloom&&) = default;
		Bloom& operator=(const Bloom&) = delete;
		Bloom& operator=(Bloom&&) = default;
		~Bloom() = default;

		ID3D11ShaderResourceView* SceneTexture() const;
		void SetSceneTexture(winrt::com_ptr<ID3D11ShaderResourceView> sceneTexture);

		const BloomSettings& GetBloomSettings() const;
		void SetBloomSettings(const BloomSettings& bloomSettings);

		BloomDrawModes DrawMode() const;
		const std::string& DrawModeString() const;
		void SetDrawMode(BloomDrawModes drawMode);
		
		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

		inline static const BloomSettings DefaultBloomSettings{ 0.45f, 2.0f, 1.25f, 1.0f, 1.0f, 1.0f };

	private:
		static const std::map<BloomDrawModes, std::string> DrawModeDisplayNames;

		struct PixelCBufferPerObject
		{
			float BloomThreshold;
			float BloomIntensity;
			float BloomSaturation;
			float SceneIntensity;
			float SceneSaturation;
			DirectX::XMFLOAT3 Padding;

			PixelCBufferPerObject(const BloomSettings& bloomSettings) :
				BloomThreshold(bloomSettings.BloomThreshold), BloomIntensity(bloomSettings.BloomIntensity),
				BloomSaturation(bloomSettings.BloomSaturation), SceneIntensity(bloomSettings.SceneIntensity),
				SceneSaturation(bloomSettings.SceneSaturation), Padding()
			{
			}

			PixelCBufferPerObject& operator=(const BloomSettings& bloomSettings)
			{
				BloomThreshold = bloomSettings.BloomThreshold;
				BloomIntensity = bloomSettings.BloomIntensity;
				BloomSaturation = bloomSettings.BloomSaturation;
				SceneIntensity = bloomSettings.SceneIntensity;
				SceneSaturation = bloomSettings.SceneSaturation;

				return *this;
			}
		};

		enum class BloomShaderClass
		{
			Extract = 0,
			Composite,
			NoBloom
		};

		void DrawNormal(const GameTime& gameTime);
		void DrawGlowMap(const GameTime& gameTime);
		void DrawBlurredGlowMap(const GameTime& gameTime);

		FullScreenQuad mFullScreenQuad;
		FullScreenRenderTarget mRenderTarget;
		GaussianBlur mGaussianBlur;
		winrt::com_ptr<ID3D11ShaderResourceView> mSceneTexture;
		Buffer* mPixelCBufferPerObject{ nullptr };
		PixelCBufferPerObject mPixelCBufferPerObjectData;
		BloomDrawModes mDrawMode{ BloomDrawModes::Normal };
		std::map<BloomDrawModes, std::function<void(const GameTime& gameTime)>> mDrawFunctions;
		std::map<BloomShaderClass, winrt::com_ptr<ID3D11ClassInstance>> mShaderClassInstances;
		BloomSettings mBloomSettings;
	};
}
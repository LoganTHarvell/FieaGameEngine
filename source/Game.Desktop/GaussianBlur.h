#pragma once

#include "DrawableGameComponent.h"
#include "FullScreenRenderTarget.h"
#include "FullScreenQuad.h"
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <functional>

namespace Library
{
	class PixelShader;
	class FullScreenQuadMaterial;

	class GaussianBlur final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS_ABSTRACT(GaussianBlur, DrawableGameComponent)

	public:
		GaussianBlur(Game& game, float blurAmount = DefaultBlurAmount);
		GaussianBlur(const GaussianBlur&) = delete;
		GaussianBlur(GaussianBlur&&) = default;
		GaussianBlur& operator=(const GaussianBlur&) = delete;
		GaussianBlur& operator=(GaussianBlur&&) = default;
		~GaussianBlur() = default;

		ID3D11ShaderResourceView* SceneTexture() const;
		void SetSceneTexture(winrt::com_ptr<ID3D11ShaderResourceView> sceneTexture);

		ID3D11ShaderResourceView* OutputTexture() const;

		float BlurAmount() const;
		void SetBlurAmount(float blurAmount);

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;
		void DrawToTexture(const GameTime& gameTime);

		inline static const float DefaultBlurAmount = 1.0f;

	private:
		static const uint32_t SampleCount = 9;

		struct PixelCBufferPerFrame
		{
			/* Matches the following cbuffer:
			cbuffer CBufferPerFrame
			{
			float2 SampleOffsets[SAMPLE_COUNT];
			float SampleWeights[SAMPLE_COUNT];
			}

			By default, HLSL does not pack arrays in cbuffers.
			Thus, each array element is a 4-float vector.
			*/

			struct _SampleOffset
			{
				DirectX::XMFLOAT2 Offset;
				DirectX::XMFLOAT2 Padding;
			} SampleOffsets[SampleCount];

			struct _SampleWeight
			{
				float Weight;
				DirectX::XMFLOAT3 Padding;
			} SampleWeights[SampleCount];
		};

		void InitializeSampleOffsets();
		void InitializeSampleWeights();
		float GetWeight(float x) const;
				
		FullScreenQuad mFullScreenQuad;
		FullScreenRenderTarget mHorizontalBlurTarget;
		FullScreenRenderTarget mVerticalBlurTarget;
		winrt::com_ptr<ID3D11ShaderResourceView> mSceneTexture;
		winrt::com_ptr<ID3D11ShaderResourceView> mOutputTexture;
		std::shared_ptr<PixelShader> mBlurPixelShader;
		std::shared_ptr<PixelShader> mNoBlurPixelShader;
		winrt::com_ptr<ID3D11Buffer> mHorizontalCBufferPerFrame;
		winrt::com_ptr<ID3D11Buffer> mVerticalCBufferPerFrame;
		PixelCBufferPerFrame mHorizontalCBufferData;
		PixelCBufferPerFrame mVerticalCBufferData;
		std::function<void()> mHorizontalCBufferPerFrameMaterialCallback;
		std::function<void()> mVerticalCBufferPerFrameMaterialCallback;

		float mBlurAmount;
	};
}
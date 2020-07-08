#pragma once

#include <windows.h>
#include <cstdint>
#include <string>
#include <memory>
#include <functional>

#include <d3d11_4.h>
#include <dxgi1_6.h>

#include "GameClock.h"
#include "GameTime.h"
#include "ServiceContainer.h"
#include "RenderTarget.h"
#include "ContentManager.h"
#include "RenderingManagerD3D11.h"
#include "WorldState.h"

namespace Library
{
	class GameComponent;

	class IDeviceNotify
	{
	public:
		virtual ~IDeviceNotify() { };

		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;

	protected:
		IDeviceNotify() { };
	};

    class Game : public RenderTarget
    {
		RTTI_DECLARATIONS_ABSTRACT(Game, RenderTarget)

    public:
        Game(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(Game&&) = delete;
		virtual ~Game() = default;

		gsl::not_null<ID3D11Device3*> Direct3DDevice() const;
		gsl::not_null<ID3D11DeviceContext3*> Direct3DDeviceContext() const;
		gsl::not_null<IDXGISwapChain1*> SwapChain() const;
		gsl::not_null<ID3D11RenderTargetView*> RenderTargetView() const;
		gsl::not_null<ID3D11DepthStencilView*> DepthStencilView() const;
		SIZE RenderTargetSize() const;
		float AspectRatio() const;
		bool IsFullScreen() const;
		const D3D11_TEXTURE2D_DESC& BackBufferDesc() const;
		const D3D11_VIEWPORT& Viewport() const;
		std::uint32_t MultiSamplingCount() const;
		std::uint32_t MultiSamplingQualityLevels() const;

		const std::vector<std::shared_ptr<GameComponent>>& Components() const;
		const ServiceContainer& Services() const;			

        virtual void Initialize();
		virtual void Run();
		virtual void Shutdown();  

		virtual void Update(const GameTime& gameTime);
		virtual void Draw(const GameTime& gameTime);

		void UpdateRenderTargetSize();
		void RegisterDeviceNotify(IDeviceNotify* deviceNotify);
		
		std::function<void*()> GetWindowCallback() const;

		WorldState& GetWorldState();
		ContentManager& Content();

    protected:
		void InitializeContentReaders();
    	
		virtual void HandleDeviceLost();

		virtual void Begin() override;
		virtual void End() override;

		virtual void CreateDeviceIndependentResources();
		virtual void CreateDeviceResources();
		virtual void CreateWindowSizeDependentResources();

		inline static const D3D_FEATURE_LEVEL DefaultFeatureLevel{ D3D_FEATURE_LEVEL_9_1 };
		inline static const std::uint32_t DefaultFrameRate{ 60 };
		inline static const std::uint32_t DefaultMultiSamplingCount{ 4 };
		inline static const std::uint32_t DefaultBufferCount{ 2 };

		winrt::com_ptr<ID3D11Device3> mDirect3DDevice;
		winrt::com_ptr<ID3D11DeviceContext3> mDirect3DDeviceContext;
		winrt::com_ptr<IDXGISwapChain1> mSwapChain;
		D3D_FEATURE_LEVEL mFeatureLevel = DefaultFeatureLevel;

		D3D11_TEXTURE2D_DESC mBackBufferDesc;
		winrt::com_ptr<ID3D11RenderTargetView> mRenderTargetView;
		winrt::com_ptr<ID3D11DepthStencilView> mDepthStencilView;
		D3D11_VIEWPORT mViewport;

		std::uint32_t mFrameRate{ DefaultFrameRate };
		bool mIsFullScreen{ false };
		std::uint32_t mMultiSamplingCount{ DefaultMultiSamplingCount };
		std::uint32_t mMultiSamplingQualityLevels{ 0 };


		std::function<void*()> mGetWindow;
		std::function<void(SIZE&)> mGetRenderTargetSize;
		SIZE mRenderTargetSize;
		IDeviceNotify* mDeviceNotify;

		RenderingManagerD3D11 mRenderingManager;
		ContentManager mContentManager;
    	
        GameClock mGameClock;
        GameTime mGameTime;
		WorldState mWorldState;
    	
		std::vector<std::shared_ptr<GameComponent>> mComponents;
		ServiceContainer mServices;

		bool mReadersInitialized{ false };
    };
}

#include "Game.inl"
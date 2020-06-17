#include "pch.h"
#include "RenderStateHelper.h"
#include "Game.h"

namespace Library
{
	RenderStateHelper::RenderStateHelper(Game& game) :
		mGame(game)
	{
	}

	void RenderStateHelper::ResetAll(gsl::not_null<ID3D11DeviceContext*> deviceContext)
	{
		ResetRasterizerState(deviceContext);
		ResetBlendState(deviceContext);
		ResetDepthStencilState(deviceContext);
	}

	void RenderStateHelper::ResetRasterizerState(gsl::not_null<ID3D11DeviceContext*> deviceContext)
	{
		deviceContext->RSSetState(nullptr);
	}

	void RenderStateHelper::ResetBlendState(gsl::not_null<ID3D11DeviceContext*> deviceContext)
	{
		deviceContext->OMSetBlendState(nullptr, nullptr, UINT_MAX);
	}

	void RenderStateHelper::ResetDepthStencilState(gsl::not_null<ID3D11DeviceContext*> deviceContext)
	{
		deviceContext->OMSetDepthStencilState(nullptr, UINT_MAX);
	}

	ID3D11RasterizerState* RenderStateHelper::RasterizerState()
	{
		return mRasterizerState.get();
	}

	ID3D11BlendState* RenderStateHelper::BlendState()
	{
		return mBlendState.get();
	}

	ID3D11DepthStencilState* RenderStateHelper::DepthStencilState()
	{
		return mDepthStencilState.get();
	}

	void RenderStateHelper::SaveRasterizerState()
	{
		mRasterizerState = nullptr;
		mGame.Direct3DDeviceContext()->RSGetState(mRasterizerState.put());
	}

	void RenderStateHelper::RestoreRasterizerState() const
	{
		mGame.Direct3DDeviceContext()->RSSetState(mRasterizerState.get());
	}

	void RenderStateHelper::SaveBlendState()
	{
		mBlendState = nullptr;
		mGame.Direct3DDeviceContext()->OMGetBlendState(mBlendState.put(), mBlendFactor.data(), &mSampleMask);
	}

	void RenderStateHelper::RestoreBlendState() const
	{
		mGame.Direct3DDeviceContext()->OMSetBlendState(mBlendState.get(), mBlendFactor.data(), mSampleMask);
	}

	void RenderStateHelper::SaveDepthStencilState()
	{
		mDepthStencilState = nullptr;
		mGame.Direct3DDeviceContext()->OMGetDepthStencilState(mDepthStencilState.put(), &mStencilRef);
	}

	void RenderStateHelper::RestoreDepthStencilState() const
	{
		mGame.Direct3DDeviceContext()->OMSetDepthStencilState(mDepthStencilState.get(), mStencilRef);
	}

	void RenderStateHelper::SaveAll()
	{
		SaveRasterizerState();
		SaveBlendState();
		SaveDepthStencilState();
	}

	void RenderStateHelper::RestoreAll() const
	{
		RestoreRasterizerState();
		RestoreBlendState();
		RestoreDepthStencilState();
	}

	void RenderStateHelper::ClearAll()
	{
		mRasterizerState = nullptr;
		mBlendState = nullptr;
		mDepthStencilState = nullptr;
	}
}
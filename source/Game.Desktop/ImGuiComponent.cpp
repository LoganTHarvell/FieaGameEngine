#include "pch.h"
#include "ImGuiComponent.h"
#include "imgui_impl_dx11.h"
#include "Game.h"
#include <map>

using namespace std;
using namespace DirectX;

namespace Library
{
	ImGuiComponent::ImGuiComponent(Game& game, Styles style, bool useCustomDraw) :
		DrawableGameComponent(game), mStyle(style), mUseCustomDraw(useCustomDraw)
	{
	}

	ImGuiComponent::Styles ImGuiComponent::Style() const
	{
		return mStyle;
	}

	void ImGuiComponent::SetStyle(Styles style)
	{
		static const map<Styles, function<void(ImGuiStyle*)>> styleMap
		{
			{ Styles::Classic, ImGui::StyleColorsClassic },
			{ Styles::Light, ImGui::StyleColorsLight },
			{ Styles::Dark, ImGui::StyleColorsDark }
		};

		mStyle = style;
		styleMap.at(mStyle)(nullptr);
	}

	void ImGuiComponent::Initialize()
	{
		ImGui::CreateContext();

		auto getWindow = mGame->GetWindowCallback();
		HWND window = reinterpret_cast<HWND>(getWindow());
		ImGui_ImplDX11_Init(window, mGame->Direct3DDevice(), mGame->Direct3DDeviceContext());

		SetStyle(mStyle);
	}

	void ImGuiComponent::Shutdown()
	{		
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiComponent::Draw(const GameTime&)
	{
		if (mUseCustomDraw == false)
		{
			ImGui_ImplDX11_NewFrame();

			for (auto& block : mRenderBlocks)
			{
				(*block)();
			}

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
	}

	bool ImGuiComponent::UseCustomDraw() const
	{
		return mUseCustomDraw;
	}

	void ImGuiComponent::SetUseCustomDraw(bool useCustomDraw)
	{
		mUseCustomDraw = useCustomDraw;
	}

	void ImGuiComponent::CustomDraw()
	{
		assert(mUseCustomDraw);
		ImGui::Render();
	}

	const std::vector<std::shared_ptr<ImGuiComponent::RenderBlock>>& ImGuiComponent::RenderBlocks() const
	{
		return mRenderBlocks;
	}

	void ImGuiComponent::AddRenderBlock(std::shared_ptr<RenderBlock> block)
	{
		mRenderBlocks.push_back(block);
	}

	void ImGuiComponent::RemoveRenderBlock(std::shared_ptr<RenderBlock> block)
	{
		mRenderBlocks.erase(find(mRenderBlocks.cbegin(), mRenderBlocks.cend(), block));
	}
}
#pragma once

#include <imgui.h>
#include <functional>
#include <vector>
#include <sstream>
#include "DrawableGameComponent.h"

namespace Library
{
	class ImGuiComponent final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS_ABSTRACT(ImGuiComponent, DrawableGameComponent)

	public:
		enum class Styles
		{
			Classic,
			Light,
			Dark
		};

		using RenderBlock = std::function<void(void)>;

		ImGuiComponent(Game& game, Styles style = Styles::Dark, bool useCustomDraw = false);

		Styles Style() const;
		void SetStyle(Styles style);

		virtual void Initialize() override;
		virtual void Shutdown() override;
		virtual void Draw(const GameTime& gameTime) override;

		// Use this when opting to invoke ImGui::Begin/End/Render statements manually
		// and not through the RenderBlock interface.
		bool UseCustomDraw() const;
		void SetUseCustomDraw(bool useCustomDraw);
		void CustomDraw();

		const std::vector<std::shared_ptr<RenderBlock>>& RenderBlocks() const;
		void AddRenderBlock(std::shared_ptr<RenderBlock> block);
		void RemoveRenderBlock(std::shared_ptr<RenderBlock> block);

	private:		
		std::vector<std::shared_ptr<RenderBlock>> mRenderBlocks;
		Styles mStyle;
		bool mUseCustomDraw;
	};

	template <typename T>
	void AddImGuiTextField(const std::string& description, const T& value, std::streamsize precision = 0)
	{
		std::stringstream label;
		if (precision > 0)
		{
			label << std::setprecision(precision);
		}
		label << description << value;
		ImGui::Text(label.str().c_str());
	}
}
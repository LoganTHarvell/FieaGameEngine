#pragma once
#include "Attributed.h"
#include "Factory.h"

using namespace Library;

namespace GameDesktop
{
	class RenderComponent : public Attributed
	{
		RTTI_DECLARATIONS(RenderComponent, Attributed)
	public:
		inline static const std::string CurrentSpriteKey = "CurrentSprite";
		inline static const std::string ColumnCountKey = "ColumnCount";
		inline static const std::string RowCountKey = "RowCount";
		inline static const std::string ColumnIndexKey = "ColumnIndex";
		inline static const std::string RowIndexKey = "RowIndex";
		static const TypeManager::TypeInfo& TypeInfo();

	public:
		explicit RenderComponent(const std::string& currentSprite = std::string(), int32_t columnCount = 0, int32_t rowCount = 0, int32_t columnIndex = 0, int32_t rowIndex = 0);
		RenderComponent(const RenderComponent&) = default;
		RenderComponent(RenderComponent&&) = default;
		RenderComponent& operator=(const RenderComponent&) = default;
		RenderComponent& operator=(RenderComponent&&) = default;
		~RenderComponent() = default;

		std::string& GetCurrentSprite();
		const std::string& GetCurrentSprite() const;
		int32_t GetColumnCount() const;
		int32_t GetRowCount() const;
		int32_t GetColumnIndex() const;
		int32_t GetRowIndex() const;

		void SetColumnCount(int32_t columnCount) { mColumnCount = columnCount; };
		void SetRowCount(int32_t rowCount) { mRowCount = rowCount; };
		void SetColumnIndex(int32_t columnIndex);
		void SetRowIndex(int32_t rowIndex);
		void SetCurrentSprite(const std::string& sprite);

		virtual gsl::owner<RenderComponent*> Clone() const override;
	private:
		std::string mCurrentSprite;
		int32_t mColumnCount;
		int32_t mRowCount;
		int32_t mColumnIndex;
		int32_t mRowIndex;
	};
	ConcreteFactory(RenderComponent, Scope)
}


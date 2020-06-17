#include "pch.h"
#include "RenderComponent.h"

namespace GameDesktop
{
	const Library::TypeManager::TypeInfo& RenderComponent::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ CurrentSpriteKey, Types::String, false, 1, offsetof(RenderComponent, mCurrentSprite)},
				{ ColumnCountKey, Types::Integer, false, 1, offsetof(RenderComponent, mColumnCount)},
				{ RowCountKey, Types::Integer, false, 1, offsetof(RenderComponent, mRowCount)},
				{ ColumnIndexKey, Types::Integer, false, 1, offsetof(RenderComponent, mColumnIndex)},
				{ RowIndexKey, Types::Integer, false, 1, offsetof(RenderComponent, mRowIndex)},
			},

			Attributed::TypeIdClass()
		};
		return typeInfo;
	}

	RenderComponent::RenderComponent(const std::string& currentSprite, int32_t columnCount, int32_t rowCount, int32_t columnIndex, int32_t rowIndex) :
		Attributed(TypeIdClass()), mCurrentSprite(currentSprite), mColumnCount(columnCount), mRowCount(rowCount), mColumnIndex(columnIndex), mRowIndex(rowIndex)
	{
	}

	std::string& RenderComponent::GetCurrentSprite()
	{
		return mCurrentSprite;
	}

	const std::string& RenderComponent::GetCurrentSprite() const
	{
		return mCurrentSprite;
	}

	int32_t RenderComponent::GetColumnCount() const
	{
		return mColumnCount;
	}

	int32_t RenderComponent::GetRowCount() const
	{
		return mRowCount;
	}

	int32_t RenderComponent::GetColumnIndex() const
	{
		return mColumnIndex;
	}

	int32_t RenderComponent::GetRowIndex() const
	{
		return mRowIndex;
	}

	void RenderComponent::SetColumnIndex(int32_t columnIndex)
	{
		mColumnIndex = columnIndex;
	}

	void RenderComponent::SetRowIndex(int32_t rowIndex)
	{
		mRowIndex = rowIndex;
	}

	void RenderComponent::SetCurrentSprite(const std::string& sprite)
	{
		mCurrentSprite = sprite;
	}

	gsl::owner<RenderComponent*> RenderComponent::Clone() const
	{
		return new RenderComponent(*this);
	}

}
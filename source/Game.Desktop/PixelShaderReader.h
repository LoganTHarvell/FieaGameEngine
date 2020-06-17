#pragma once

#include "ContentTypeReader.h"
#include "PixelShader.h"

namespace Library
{
	class PixelShaderReader : public ContentTypeReader<PixelShader>
	{
		RTTI_DECLARATIONS_ABSTRACT(PixelShaderReader, AbstractContentTypeReader)

	public:
		PixelShaderReader(Game& game);
		PixelShaderReader(const PixelShaderReader&) = default;
		PixelShaderReader& operator=(const PixelShaderReader&) = default;
		PixelShaderReader(PixelShaderReader&&) = default;
		PixelShaderReader& operator=(PixelShaderReader&&) = default;
		~PixelShaderReader() = default;

	protected:
		virtual std::shared_ptr<PixelShader> _Read(const std::wstring& assetName) override;
	};

	class PixelShaderWithClassLinkageReader
	{
	public:
		PixelShaderWithClassLinkageReader(Game& game, winrt::com_ptr<ID3D11ClassLinkage> classLinkage);

		std::shared_ptr<PixelShader> operator()(const std::wstring& assetName) const;

	private:
		gsl::not_null<Game*> mGame;
		winrt::com_ptr<ID3D11ClassLinkage> mClassLinkage;
	};
}
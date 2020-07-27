#pragma once

#include <winrt/Windows.Foundation.h>
#include <d3d11.h>

#include "RenderingManager.h"
#include "Shader.h"
#include "Vector.h"

namespace Library
{
	// Forward Declarations
	struct VertexAttribute;

	class VertexShaderD3D11 final : public VertexShader
	{
		RTTI_DECLARATIONS(VertexShaderD3D11, VertexShader)

	public:
		explicit VertexShaderD3D11(const winrt::com_ptr<ID3D11VertexShader>& vertexShader=winrt::com_ptr<ID3D11VertexShader>());
		~VertexShaderD3D11() = default;
		VertexShaderD3D11(const VertexShaderD3D11&) = default;
		VertexShaderD3D11& operator=(const VertexShaderD3D11&) = default;
		VertexShaderD3D11(VertexShaderD3D11&&) = default;
		VertexShaderD3D11& operator=(VertexShaderD3D11&&) = default;

	public:
		const Vector<char>& CompiledShader() const;
		winrt::com_ptr<ID3D11VertexShader> Native() const;
		winrt::com_ptr<ID3D11InputLayout> InputLayout() const;

		virtual void CreateInputLayout(const RenderingManager& renderingManager, const gsl::span<const VertexAttribute>& vertexAttributes, bool releaseCompiledShader=false) override;

	private:
		friend class VertexShaderReader;

		VertexShaderD3D11(Vector<char> compiledShader, const winrt::com_ptr<ID3D11VertexShader>& vertexShader);

		Vector<char> mCompiledShader;
		winrt::com_ptr<ID3D11VertexShader> mShader;
		winrt::com_ptr<ID3D11InputLayout> mInputLayout;
	};

	class HullShaderD3D11 final : public HullShader
	{
		RTTI_DECLARATIONS(HullShaderD3D11, HullShader)

	public:
		explicit HullShaderD3D11(const winrt::com_ptr<ID3D11HullShader>& hullShader=winrt::com_ptr<ID3D11HullShader>());
		~HullShaderD3D11() = default;
		HullShaderD3D11(const HullShaderD3D11&) = default;
		HullShaderD3D11& operator=(const HullShaderD3D11&) = default;
		HullShaderD3D11(HullShaderD3D11&&) = default;
		HullShaderD3D11& operator=(HullShaderD3D11&&) = default;

	public:
		winrt::com_ptr<ID3D11HullShader> Native() const;

	private:
		winrt::com_ptr<ID3D11HullShader> mShader;
	};

	class DomainShaderD3D11 final : public DomainShader
	{
		RTTI_DECLARATIONS(DomainShaderD3D11, DomainShader)

	public:
		explicit DomainShaderD3D11(const winrt::com_ptr<ID3D11DomainShader>& domainShader=winrt::com_ptr<ID3D11DomainShader>());
		~DomainShaderD3D11() = default;
		DomainShaderD3D11(const DomainShaderD3D11&) = default;
		DomainShaderD3D11& operator=(const DomainShaderD3D11&) = default;
		DomainShaderD3D11(DomainShaderD3D11&&) = default;
		DomainShaderD3D11& operator=(DomainShaderD3D11&&) = default;

	public:
		winrt::com_ptr<ID3D11DomainShader> Native() const;

	private:
		winrt::com_ptr<ID3D11DomainShader> mShader;
	};

	class GeometryShaderD3D11 final : public GeometryShader
	{
		RTTI_DECLARATIONS(GeometryShaderD3D11, GeometryShader)

	public:
		explicit GeometryShaderD3D11(const winrt::com_ptr<ID3D11GeometryShader>& geometryShader=winrt::com_ptr<ID3D11GeometryShader>());
		~GeometryShaderD3D11() = default;
		GeometryShaderD3D11(const GeometryShaderD3D11&) = default;
		GeometryShaderD3D11& operator=(const GeometryShaderD3D11&) = default;
		GeometryShaderD3D11(GeometryShaderD3D11&&) = default;
		GeometryShaderD3D11& operator=(GeometryShaderD3D11&&) = default;

	public:
		winrt::com_ptr<ID3D11GeometryShader> Native() const;

	private:
		winrt::com_ptr<ID3D11GeometryShader> mShader;
	};

	class PixelShaderD3D11 final : public PixelShader
	{
		RTTI_DECLARATIONS(PixelShaderD3D11, PixelShader)

	public:
		explicit PixelShaderD3D11(const winrt::com_ptr<ID3D11PixelShader>& pixelShader=winrt::com_ptr<ID3D11PixelShader>());
		~PixelShaderD3D11() = default;
		PixelShaderD3D11(const PixelShaderD3D11&) = default;
		PixelShaderD3D11& operator=(const PixelShaderD3D11&) = default;
		PixelShaderD3D11(PixelShaderD3D11&&) = default;
		PixelShaderD3D11& operator=(PixelShaderD3D11&&) = default;

	public:
		winrt::com_ptr<ID3D11PixelShader> Native() const;

	private:
		winrt::com_ptr<ID3D11PixelShader> mShader;
	};

	class ComputeShaderD3D11 final : public ComputeShader
	{
		RTTI_DECLARATIONS(ComputeShaderD3D11, ComputeShader)

	public:
		explicit ComputeShaderD3D11(const winrt::com_ptr<ID3D11ComputeShader>& computeShader=winrt::com_ptr<ID3D11ComputeShader>());
		~ComputeShaderD3D11() = default;
		ComputeShaderD3D11(const ComputeShaderD3D11&) = default;
		ComputeShaderD3D11& operator=(const ComputeShaderD3D11&) = default;
		ComputeShaderD3D11(ComputeShaderD3D11&&) = default;
		ComputeShaderD3D11& operator=(ComputeShaderD3D11&&) = default;

	public:
		winrt::com_ptr<ID3D11ComputeShader> Native() const;

	private:
		winrt::com_ptr<ID3D11ComputeShader> mShader;
	};
}

// Inline File
#include "ShaderD3D11.inl"

#pragma once

#include <DirectXMath.h>

namespace Library
{
	class MatrixHelper final
	{
	public:
		inline static const DirectX::XMFLOAT4X4 Identity
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		inline static const DirectX::XMFLOAT4X4 Zero
		{
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
		};

		static void GetForward(DirectX::FXMMATRIX matrix, DirectX::XMFLOAT3& vector);
		static void GetUp(DirectX::FXMMATRIX matrix, DirectX::XMFLOAT3 &vector);
		static void GetRight(DirectX::FXMMATRIX matrix, DirectX::XMFLOAT3& vector);
		static void GetTranslation(DirectX::FXMMATRIX matrix, DirectX::XMFLOAT3& vector);
		static void GetTranslation(const DirectX::XMFLOAT4X4& matrix, DirectX::XMFLOAT3& vector);

		static void SetForward(DirectX::XMMATRIX& matrix, DirectX::XMFLOAT3& forward);
		static void SetUp(DirectX::XMMATRIX& matrix, DirectX::XMFLOAT3& up);
		static void SetRight(DirectX::XMMATRIX& matrix, DirectX::XMFLOAT3& right);
		static void SetTranslation(DirectX::XMMATRIX& matrix, DirectX::XMFLOAT3& translation);		

		MatrixHelper() = delete;
		MatrixHelper(const MatrixHelper&) = delete;
		MatrixHelper& operator=(const MatrixHelper&) = delete;
		MatrixHelper(const MatrixHelper&&) = delete;
		MatrixHelper& operator=(MatrixHelper&&) = delete;
		~MatrixHelper() = default;
	};
}
namespace Library
{
	inline bool Vector2Helper::Equals(const DirectX::XMFLOAT2 & lhs, const DirectX::XMFLOAT2 & rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	inline bool Vector3Helper::Equals(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}

	inline bool Vector4Helper::Equals(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
	}
}
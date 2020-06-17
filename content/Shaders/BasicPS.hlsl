cbuffer CBufferPerObject
{
	float4 SurfaceColor;
}

float4 main(float4 position : SV_Position) : SV_TARGET
{
	return SurfaceColor;
}
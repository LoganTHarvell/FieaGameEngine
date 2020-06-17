cbuffer CBufferPerObject
{
	float4x4 WorldViewProjection;
}

struct VS_INPUT
{
	float4 ObjectPosition: POSITION;
};

float4 main(VS_INPUT IN) : SV_Position
{
	return mul(IN.ObjectPosition, WorldViewProjection);
}
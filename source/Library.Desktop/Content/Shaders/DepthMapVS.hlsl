cbuffer CBufferPerObject
{
	float4x4 WorldLightViewProjection;
}

float4 main(float4 ObjectPosition : POSITION) : SV_Position
{
	return mul(ObjectPosition, WorldLightViewProjection);
}
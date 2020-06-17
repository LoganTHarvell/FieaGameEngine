#define SAMPLE_COUNT 9

cbuffer CBufferPerFrame
{
	float2 SampleOffsets[SAMPLE_COUNT]; // Arrays are not packed (by default). Each element is stored in a 4-component vector.	
	float SampleWeights[SAMPLE_COUNT];
}

Texture2D ColorMap;
SamplerState TextureSampler;

struct VS_OUTPUT
{
	float4 Position : SV_Position;
	float2 TextureCoordinates : TEXCOORD;
};

float4 main(VS_OUTPUT IN) : SV_TARGET
{
	float4 color = (float4)0;

	[unroll]
	for (int i = 0; i < SAMPLE_COUNT; i++)
	{
		color += ColorMap.Sample(TextureSampler, IN.TextureCoordinates + SampleOffsets[i]) * SampleWeights[i];
	}

	return color;
}
interface IBloomShader
{
	float4 ComputeFinalColor(float2 uv);
};

cbuffer CBufferPerObject
{
	float BloomThreshold;
	float BloomIntensity;
	float BloomSaturation;
	float SceneIntensity;
	float SceneSaturation;
};

IBloomShader BloomShader;
Texture2D ColorMap;
Texture2D GlowMap;
SamplerState TextureSampler;

struct VS_OUTPUT
{
	float4 Position : SV_Position;
	float2 TextureCoordinates : TEXCOORD;
};

float4 AdjustSaturation(float4 color, float saturation)
{
	static const float3 GrayScaleIntensity = { 0.299f, 0.587f, 0.114f };
	float intensity = dot(color.rgb, GrayScaleIntensity);

	return float4(lerp(intensity.rrr, color.rgb, saturation), color.a);
}

class ExtractBloomShader : IBloomShader
{
	float4 ComputeFinalColor(float2 uv)
	{
		float4 color = ColorMap.Sample(TextureSampler, uv);

		return saturate((color - BloomThreshold) / (1 - BloomThreshold));
	}
};

class CompositeBloomShader : IBloomShader
{
	float4 ComputeFinalColor(float2 uv)
	{
		float4 sceneColor = ColorMap.Sample(TextureSampler, uv);
		float4 bloomColor = GlowMap.Sample(TextureSampler, uv);

		sceneColor = AdjustSaturation(sceneColor, SceneSaturation) * SceneIntensity;
		bloomColor = AdjustSaturation(bloomColor, BloomSaturation) * BloomIntensity;

		sceneColor *= (1 - saturate(bloomColor));

		return sceneColor + bloomColor;
	}
};

class NoBloomShader : IBloomShader
{
	float4 ComputeFinalColor(float2 uv)
	{
		return ColorMap.Sample(TextureSampler, uv);
	}
};

float4 main(VS_OUTPUT IN) : SV_TARGET
{
	return BloomShader.ComputeFinalColor(IN.TextureCoordinates);
}
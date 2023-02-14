Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 direction_to_camera : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return Texture.Sample(TextureSampler, 1.0f - input.texcoord);
}
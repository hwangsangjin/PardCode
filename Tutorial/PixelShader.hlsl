struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
	float3 color1 : COLOR1;
};

cbuffer CONSTANT : register(b0)
{
	unsigned int time;
}

float4 main(PS_INPUT input) : SV_TARGET
{
	return float4(lerp(input.color, input.color1, (sin(time / 500.0f) + 1.0f) / 2.0f), 1.0f);
}
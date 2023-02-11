struct VS_INPUT
{
	float4 position : POSITION;
	float3 color : COLOR;
	float3 color1 : COLOR1;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
	float3 color1 : COLOR1;
};

cbuffer CONSTANT : register(b0)
{
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 projection;
	unsigned int time;
}

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	// World space
	output.position = mul(input.position, world);
	
	// View space
	output.position = mul(output.position, view);
	
	// Projection space
	output.position = mul(output.position, projection);
	
	output.color = input.color;
	output.color1 = input.color1;
	return output;
}
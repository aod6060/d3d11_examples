/*
	terrain_ps.hlsl
*/

cbuffer ConstPS
{
	float tcScale;
	float3 unused1;
};

struct OutputVS
{
	float4 positions : SV_POSITION;
	float2 texCoords : TEXCOORDS;
};

Texture2D tex0;
SamplerState sampType;

float4 main(OutputVS input) : SV_Target
{
	return tex0.Sample(sampType, input.texCoords * tcScale);
}
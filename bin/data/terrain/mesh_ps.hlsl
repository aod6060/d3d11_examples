/**
	mesh_ps.hlsl
*/

struct OutputVS
{
	float4 positions : SV_POSITION;
	float2 texCoords : TEXCOORD;
	float3 normals : NORMAL;
};

Texture2D tex0;
SamplerState sampType;

float4 main(OutputVS input) : SV_Target
{
	return tex0.Sample(sampType, input.texCoords);
	//return float4(input.normals, 1.0);
}
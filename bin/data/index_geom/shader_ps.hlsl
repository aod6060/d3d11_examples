/**
	shader_ps.hlsl
*/

struct OutputVS
{
	float4 positions : SV_POSITION;
	float4 colors : COLOR;
	float2 texCoords : TEXCOORD;
};

Texture2D tex0;
SamplerState sampType;

float4 main(OutputVS input) : SV_Target
{
	//return float4(input.texCoords, 0.0, 1.0);
	float4 color = tex0.Sample(sampType, input.texCoords);
	color.rgb *= input.colors.rgb;
	return color;
}
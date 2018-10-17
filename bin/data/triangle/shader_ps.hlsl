/**
	shader_ps.hlsl
*/

struct OutputVS
{
	float4 positions : SV_POSITION;
};


float4 main(OutputVS input) : SV_Target
{
	return float4(1.0, 1.0, 1.0, 1.0);
}
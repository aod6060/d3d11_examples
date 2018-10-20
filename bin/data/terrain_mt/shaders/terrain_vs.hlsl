/*
	terrain_vs.hlsl
*/
// Global
cbuffer ConstVS
{
	float4x4 proj;
	float4x4 view;
	float4x4 model;
};

// Attributes
struct InputVS
{
	float3 vertices : POSITION;
	float2 texCoords : TEXCOORD;
};

struct OutputVS
{
	float4 positions : SV_POSITION;
	float2 texCoords : TEXCOORDS;
};

OutputVS main(InputVS input)
{
	OutputVS output;
	// Local to Screen Positions
	output.positions = mul(model, float4(input.vertices, 1.0));
	output.positions = mul(view, output.positions);
	output.positions = mul(proj, output.positions);
	// TexCoords
	output.texCoords = input.texCoords;
	// Return OutputVS
	return output;
}
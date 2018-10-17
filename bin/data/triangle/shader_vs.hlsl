/*
	shader_vs.hlsl
	
	This is the shader for triangle
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
};

struct OutputVS
{
	float4 positions : SV_POSITION;
};

OutputVS main(InputVS input)
{
	OutputVS output;
	// local to screen positions
	output.positions = mul(model, float4(input.vertices, 1.0));
	output.positions = mul(view, output.positions);
	output.positions = mul(proj, output.positions);
	// Return outputVS
	return output;
}
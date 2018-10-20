/*
	mesh_vs.hlsl
	
	This is the shader for triangle color
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
	float3 normals : NORMAL;
};

struct OutputVS
{
	float4 positions : SV_POSITION;
	float2 texCoords : TEXCOORD;
	float3 normals : NORMAL;
};

OutputVS main(InputVS input)
{
	OutputVS output;
	// local to screen positions
	output.positions = mul(model, float4(input.vertices, 1.0));
	output.positions = mul(view, output.positions);
	output.positions = mul(proj, output.positions);
	// TexCoords
	output.texCoords = input.texCoords;
	// Normals
	output.normals = input.normals;
	// Return outputVS
	return output;
}
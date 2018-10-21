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

// Blend Map
Texture2D blendMap;
SamplerState blendMapST;
// Channel Black
Texture2D channelBlack;
SamplerState channelBlackST;
// Channel Red
Texture2D channelRed;
SamplerState channelRedST;
// Channel Green
Texture2D channelGreen;
SamplerState channelGreenST;
// Channel Blue
Texture2D channelBlue;
SamplerState channelBlueST;

float4 main(OutputVS input) : SV_Target
{
	float3 bm = blendMap.Sample(blendMapST, input.texCoords).rgb;
	
	float3 cblk = channelBlack.Sample(channelBlackST, input.texCoords * tcScale).rgb * ( 1.0 - (bm.r + bm.g + bm.b));
	float3 cr   = channelRed.Sample(channelRedST, input.texCoords * tcScale);
	float3 cg   = channelGreen.Sample(channelGreenST, input.texCoords * tcScale);
	float3 cb   = channelBlue.Sample(channelBlueST, input.texCoords * tcScale);
	
	float3 color = cr * bm.r + cg * bm.g + cb * bm.b + cblk;
	
	return float4(color, 1.0);
}
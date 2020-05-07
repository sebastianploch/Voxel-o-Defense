
cbuffer constantBuffer : register(b0)
{
    matrix mvpMat;
    matrix worldMat;
};

struct VS_INPUT
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

VS_OUTPUT VSMain(VS_INPUT Input)
{
	VS_OUTPUT Output;
	
    Output.position = mul(Input.position, mvpMat);
    Output.normal = mul(Input.normal, (float3x3)worldMat);
    Output.texCoord = Input.texCoord;
	
	return Output;
}
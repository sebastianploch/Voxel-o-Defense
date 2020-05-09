
cbuffer constantBuffer : register( b0 )
{
    matrix mvpMat;
    matrix worldMat;
};

struct VS_INPUT
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
	float4 position : SV_POSITION;
};

VS_OUTPUT VSMain(VS_INPUT Input)
{
	VS_OUTPUT Output;
	
    Output.position = mul(mvpMat, Input.position);
    Output.normal = Input.normal;
    Output.texCoord = Input.texCoord;

	return Output;
}

cbuffer constantBuffer : register( b0 )
{
    matrix projection;
    matrix view;
    matrix world;
    float time;
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
	
    Output.position = mul(world, Input.position);
    Output.position = mul(view, Output.position);
    Output.position = mul(projection, Output.position);
    Output.normal = Input.normal;
    Output.texCoord = Input.texCoord;

	return Output;
}
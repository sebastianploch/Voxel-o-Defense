
cbuffer constantBuffer : register(b0)
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
    float2 texCoord0 : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
};

struct VS_OUTPUT
{
    float2 texCoord0 : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float3 normal : NORMAL;
    float4 position : SV_POSITION;
    float3 posW : TEXCOORD2;
};

VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;

    Output.posW = Input.position.xyz;

    Output.position = mul(world, Input.position);
    Output.position = mul(view, Output.position);
    Output.position = mul(projection, Output.position);
    Output.normal = Input.normal;
    Output.texCoord0 = Input.texCoord0;
    Output.texCoord1 = Input.texCoord1;

    return Output;
}
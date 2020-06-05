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
    float4 colour : COLOR0;
};

struct VS_OUTPUT
{
    float4 colour : COLOR0;
    float4 position : SV_POSITION;
};

VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;
	
    Output.position = mul(world, Input.position);
    Output.position = mul(view, Output.position);
    Output.position = mul(projection, Output.position);
    Output.colour = Input.colour;

    return Output;
}
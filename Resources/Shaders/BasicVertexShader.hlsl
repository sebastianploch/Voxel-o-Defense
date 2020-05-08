
cbuffer constantBuffer : register( b0 )
{
    matrix mvpMat;
    matrix worldMat;
};

struct VS_INPUT
{
    float4 position : SV_Position;
	float4 colour : COLOR;
};

struct VS_OUTPUT
{
    float4 colour : COLOR;
	float4 position : SV_POSITION;
};

VS_OUTPUT VSMain(VS_INPUT Input)
{
	VS_OUTPUT Output;
	
    Output.position = mul(mvpMat, Input.position);
    Output.colour = Input.colour;

	return Output;
}

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

    float frequency = 0.45f;
    float magnitude = 0.35f;
    float speed = 1.2f;
    Output.position.y += sin(time * speed + ((Input.position.x + Input.position.z) * frequency)) * magnitude;

    Output.position = mul(view, Output.position);
    Output.position = mul(projection, Output.position);
    Output.normal = Input.normal;
    Output.texCoord = Input.texCoord;

	return Output;
}
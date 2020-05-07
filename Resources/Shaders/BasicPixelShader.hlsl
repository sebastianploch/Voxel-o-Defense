
struct PS_INPUT
{
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

float4 PSMain(PS_INPUT Input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
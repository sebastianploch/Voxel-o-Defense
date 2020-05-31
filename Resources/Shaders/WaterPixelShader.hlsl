
Texture2D diffuseTexture : register( t0 );
SamplerState samplerLinear : register( s0 );

struct PS_INPUT
{
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 PSMain(PS_INPUT Input) : SV_TARGET
{
    float4 pixelColour = diffuseTexture.Sample(samplerLinear, Input.texCoord);
    
    return pixelColour;
}

Texture2D diffuseTexture : register(t0);
SamplerState samplerLinear : register(s0);

cbuffer constantBuffer : register(b0) {
    matrix projection;
    matrix view;
    matrix world;
    float time;
};

struct PS_INPUT
{
    float2 texCoord0 : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float3 normal : NORMAL;
    float4 position : SV_Position;
    float3 posW : TEXCOORD2;
};

float4 PSMain(PS_INPUT Input) : SV_TARGET
{
    //                 Tile size   Whole texture atlas size
    float tileSize = float(32.0f / 512.0f);
    //                    Wrap UV to one texture                                                                      Add offset for current voxel texture
    float2 newUv = float2(fmod(Input.texCoord0.x * tileSize, tileSize), fmod(Input.texCoord0.y * tileSize, tileSize)) + Input.texCoord1;
    
    //Temporary lighting solution to help differentiate blocks
    float lightingVal = Input.normal.y * 1 + Input.normal.x * 2 + Input.normal.z * 3;

    float4 pixelColour = diffuseTexture.Sample(samplerLinear, newUv);

    float frequency = 0.45f;
    float magnitude = 0.35f;
    float speed = 1.2f;
    if (Input.posW.y < 11)
        return lerp(float4(0, 0.2f, 0.3f, 1), pixelColour, (Input.posW.y + 2) / 20.0f);

    return pixelColour / abs(lightingVal);
}
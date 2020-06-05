
struct PS_INPUT
{
    float4 colour : COLOR0;
};

float4 PSMain(PS_INPUT Input) : SV_TARGET
{
    return Input.colour;
}
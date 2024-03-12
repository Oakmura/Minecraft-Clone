Texture2D gTestTexture : register(t0);
Texture2D gFrameTexture : register(t1);
SamplerState gSampler : register(s0);

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{
    float4 color = gFrameTexture.Sample(gSampler, input.uv);
    color.rgb += input.color.rgb;
    color.a = (color.r + color.b > 1.0f) ? 0.0f : 1.0f;
    
    return color;
}
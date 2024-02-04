Texture2D gTestTexture : register(t0);
Texture2D gFrameTexture : register(t1);
SamplerState gSampler : register(s0);

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    // return float4(gFrameTexture.Sample(gSampler, input.uv).rgb, 1.0f);
    return float4(gTestTexture.Sample(gSampler, input.uv).rgb, 1.0f);
    //return float4(input.color, 1.0f);
}

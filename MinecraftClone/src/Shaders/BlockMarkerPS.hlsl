#include "Common.hlsli"

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{
    float4 color = gFrameTex.Sample(gSampler, input.uv);
    color.rgb += input.color.rgb;
    color.a = (color.r + color.b > 1.0f) ? 0.0f : 1.0f;
    
    return color;
}
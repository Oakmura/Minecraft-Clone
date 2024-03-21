#include "Common.hlsli"

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{
    float3 color = gWaterTex.Sample(gLinearWrapSampler, input.uv).xyz;
    
    return float4(color, 0.5f);
}
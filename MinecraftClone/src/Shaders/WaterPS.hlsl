#include "Common.hlsli"

struct PSInput
{
    float4 Position : SV_POSITION;
    float3 PositionWorld : Position;
    float2 Texcoord : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 texColor = gWaterTex.Sample(gAnisoWrapSampler, input.Texcoord).rgb;
    texColor = pow(abs(texColor), gGamma);
    
    // fog
    float dist = length(CameraPosWorld - input.PositionWorld) / MAX_FOG_DIST;
    float alpha = lerp(0.88f, 0.0f, dist * FogStrength);
    
    texColor = pow(abs(texColor), gInvGamma);
    return float4(texColor, alpha);
}
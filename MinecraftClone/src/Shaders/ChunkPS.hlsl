#include "Common.hlsli"

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 posWorld : POSITION;
    float2 uv : TEXCOORD;
    float shading : TEXCOORD1;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 texColor = gBlockTexArray.Sample(gAnisoWrapSampler, input.uv).rgb;
    texColor = pow(abs(texColor), gGamma);
    
    texColor *= input.shading;
    
    // underwater effect
    if (CameraPosWorld.y < WaterLine)
    {
        texColor *= float3(0.0f, 0.3f, 1.0f);
    }
    
    // fog
    float dist = length(CameraPosWorld - input.posWorld) / MAX_FOG_DIST;
    texColor = lerp(texColor, BackgroundColor.xyz, dist * FogStrength);

    texColor = pow(abs(texColor), gInvGamma);
    return float4(texColor, 1.0f);
}

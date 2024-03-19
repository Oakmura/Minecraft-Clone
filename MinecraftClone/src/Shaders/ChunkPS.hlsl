#include "Common.hlsli"

static const float gGammaValue = 2.2f;
static const float3 gGamma = gGammaValue;
static const float3 gInvGamma = 1 / gGamma;

static const float MAX_FOG_DIST = 1000.0f;

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
    texColor = pow(texColor, gGamma);
    
    texColor *= input.shading;
    
    // underwater effect
    if (CameraPosWorld.y < WaterLine)
    {
        texColor *= float3(0.0f, 0.3f, 1.0f);
    }
    
    // fog
    float dist = abs(CameraPosWorld - input.posWorld) / MAX_FOG_DIST;
    texColor = lerp(texColor, BackgroundColor.xyz, dist * FogStrength);
    
    texColor = pow(texColor, gInvGamma);
    return float4(texColor, 1.0f);
}

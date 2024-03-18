#include "Common.hlsli"

struct PSInput
{
    float4 Position : SV_POSITION;
    float3 PositionWorld : Position;
    float2 Texcoord : TEXCOORD;
};

static const float gGammaValue = 2.2f;
static const float3 gGamma = gGammaValue;
static const float3 gInvGamma = 1 / gGamma;

static const float MAX_FOG_DIST = 1000.0f;

float4 main(PSInput input) : SV_TARGET
{
    float3 texColor = gWaterTex.Sample(gSampler, input.Texcoord).rgb;
    texColor = pow(texColor, gGamma);
    
    // fog
    float dist = abs(CameraPosWorld - input.PositionWorld) / MAX_FOG_DIST;
    float alpha = lerp(0.88f, 0.0f, dist * FogStrength);
    
    texColor = pow(texColor, gInvGamma);
    return float4(texColor, alpha);
}
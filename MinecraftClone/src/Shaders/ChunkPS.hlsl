Texture2D gTestTexture : register(t0);
Texture2D gVoxelTexture : register(t1);
SamplerState gSampler : register(s0);

cbuffer ChunkCB : register(b0)
{
    float3 CameraPosWorld;
    float1 Dummy;
    float3 BackgroundColor;
    float FogStrength;
};

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
    float3 texColor = gTestTexture.Sample(gSampler, input.uv).rgb;
    texColor = pow(texColor, gGamma);
    
    texColor *= input.shading;
    
    // fog
    float dist = abs(CameraPosWorld - input.posWorld) / MAX_FOG_DIST;
    texColor = lerp(texColor, BackgroundColor.xyz, dist * FogStrength);
    
    texColor = pow(texColor, gInvGamma);
    return float4(texColor, 1.0f);
}

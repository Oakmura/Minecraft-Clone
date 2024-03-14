cbuffer ChunkCB : register(b0)
{
    float3 CameraPosWorld;
    float Dummy;
    float3 BackgroundColor;
    float FogStrength;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float3 PositionWorld : Position;
};

static const float MAX_FOG_DIST = 1000.0f;
static const float3 CLOUD_COLOR = { 1.0f, 1.0f, 1.0f };

float4 main(PSInput input) : SV_TARGET
{
    // fog
    float dist = abs(CameraPosWorld - input.PositionWorld) / MAX_FOG_DIST;
    float3 color = lerp(CLOUD_COLOR, BackgroundColor, dist * FogStrength);
    
    return float4(color, 0.8f);
}
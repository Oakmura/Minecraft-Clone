Texture2D gTestTexture : register(t0);
Texture2D gFrameTexture : register(t1);
SamplerState gSampler : register(s0);

static const float gGammaValue = 2.2f;
static const float3 gGamma = gGammaValue;
static const float3 gInvGamma = 1 / gGamma;

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 texColor = gFrameTexture.Sample(gSampler, input.uv).rgb;
    texColor = pow(texColor, gGamma);
    
    texColor *= input.color;
    
    texColor = pow(texColor, gInvGamma);
    return float4(texColor, 1.0f);
}

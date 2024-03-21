#include "Common.hlsli"

struct VSInput
{
    float3 Position : POSITION;
    float2 Texcoord : TEXCOORD;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float2 Texcoord : TEXCOORD;
};

PSInput main(VSInput input)
{
    PSInput output;
    
    output.Position = float4(input.Position, 1.0f);
    output.Position = mul(output.Position, Model);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Proj);
    
    output.Texcoord = input.Texcoord;
    
    return output;
}
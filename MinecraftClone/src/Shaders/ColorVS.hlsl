#include "Common.hlsli"

struct VSInput
{
    float3 pos : POSITION;
    float3 color : COLOR0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

PSInput main(VSInput input)
{
    PSInput output;
    float4 pos = float4(input.pos, 1.0f);
    pos = mul(pos, Model);
    pos = mul(pos, View);
    pos = mul(pos, Proj);

    output.pos = pos;
    output.color = input.color;

    return output;
}

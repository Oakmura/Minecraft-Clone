#include "Common.hlsli"

struct VSInput
{
    float3 Position : POSITION;
    float2 Texcoord : TEXCOORD;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float3 PositionWorld : POSITION;
    float2 Texcoord : TEXCOORD;
};

PSInput main(VSInput input)
{
    float3 pos = input.Position;
    pos.xz *= WaterArea;
    pos.xz -= 0.33f * WaterArea;
    pos.y += WaterLine2;
    
    PSInput output;
    output.PositionWorld = pos;
    
    output.Position = float4(pos, 1.0f);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Proj);
    
    output.Texcoord = input.Texcoord;
    
    return output;
}
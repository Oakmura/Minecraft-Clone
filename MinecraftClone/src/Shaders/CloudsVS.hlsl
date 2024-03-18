#include "Common.hlsli"

struct VSInput
{
    int3 Position : POSITION;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float3 PositionWorld : Position;
};

PSInput main(VSInput input)
{
    float3 pos = input.Position;
    pos.xz -= WorldCenterXZ;
    pos.xz *= CloudScale;
    pos.xz += WorldCenterXZ;
    
    float time = 300 * sin(0.01f * Time);
    pos.xz += time;
    
    PSInput output;
    output.Position = float4(pos, 1.0f);
    output.PositionWorld = output.Position;
    
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Proj);
    
    return output;
}
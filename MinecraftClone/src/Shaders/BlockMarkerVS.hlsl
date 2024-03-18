#include "Common.hlsli"

struct VSInput
{
    float3 Position : POSITION;
    float2 Texcoord : TEXCOORD;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
};

static const float3 markerColor[2] =
{
    float3(1.0f, 0.0f, 0.0f),
    float3(0.0f, 0.0f, 1.0f)
};

PSInput main(VSInput input, uint vertexID : SV_VertexID)
{
    PSInput output;
    
    const float3 offset = float3(0.5f, 0.5f, 0.5f);
    const float3 scale = float3(1.01f, 1.01f, 1.01f);
    // const float3 scale = float3(0.5f, 0.5f, 0.5f);
    float3 scaledPosition = (input.Position - offset) * scale + offset;
    
    float4 pos = float4(input.Position, 1.0f);
    
    pos = mul(pos, Model);
    pos = mul(pos, View);
    pos = mul(pos, Proj);
    
    // const float3 offset = float3(0.5f, 0.5f, 0.5f);
    // const float3 scale = float3(1.01f, 1.01f, 1.01f);
    // float3 scaledPosition = (input.Position - offset) * scale + offset;
    
    output.pos = pos;
    output.color = markerColor[InteractionMode];
    output.uv = input.Texcoord;

    return output;
}
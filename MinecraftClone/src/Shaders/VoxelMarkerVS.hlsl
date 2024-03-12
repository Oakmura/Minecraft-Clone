cbuffer modelCB : register(b0)
{
    matrix model;
};

cbuffer viewProjectionCB : register(b1)
{
    matrix view;
    matrix projection;
};

cbuffer interactionModeCB : register(b2)
{
    uint interactionMode;
}

struct VSInput
{
    int3 Position : POSITION;
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
    
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    // const float3 offset = float3(0.5f, 0.5f, 0.5f);
    // const float3 scale = float3(1.01f, 1.01f, 1.01f);
    // float3 scaledPosition = (input.Position - offset) * scale + offset;
    
    output.pos = pos;
    output.color = markerColor[interactionMode];
    output.uv = input.Texcoord;

    return output;
}
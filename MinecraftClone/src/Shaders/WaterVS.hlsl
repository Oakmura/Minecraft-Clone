cbuffer WaterCB : register(b0)
{
    float WaterLine;
    int WaterArea;
    float2 Dummy;
};

cbuffer ViewProjectionCB: register(b1)
{
    matrix View;
    matrix Proj;
}

struct VSInput
{
    int3 Position : POSITION;
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
    PSInput output;
    output.Position = float4(input.Position, 1.0f);
    
    output.Position.xz *= WaterArea;
    output.Position.xz -= 0.33 * WaterArea;
    output.Position.y += WaterLine;
    
    output.PositionWorld = output.Position.xyz;
    
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Proj);
    
    output.Texcoord = input.Texcoord * WaterArea;
    
    return output;
}
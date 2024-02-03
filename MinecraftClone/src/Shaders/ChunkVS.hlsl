cbuffer MvpCB : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
};

struct VSInput
{
    uint3 Position : POSITION;
    uint1 VoxelType : COLOR0;
    uint1 FaceType : COLOR1;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

float3 hash31(uint1 p)
{
    float3 p3 = frac(float3(p * 21.2, p * 21.2, p * 21.2) * float3(0.1031, 0.1030, 0.0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return frac((p3.xxy + p3.yzz) * p3.zyx) + 0.05; 
}

PSInput main(VSInput input)
{
    PSInput output;
    
    float4 pos = float4(input.Position, 1.0f);
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.color = hash31(input.VoxelType);
    output.pos = pos;

    return output;
}
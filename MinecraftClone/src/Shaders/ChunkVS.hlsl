cbuffer modelCB : register(b0)
{
    matrix model;
};

cbuffer viewProjectionCB : register(b1)
{
    matrix view;
    matrix projection;
};

struct VSInput
{
    uint3 Position : POSITION;
    float2 Texcoord : TEXCOORD;
    uint1 VoxelType : COLOR0;
    uint1 FaceType : COLOR1;
    uint1 aoFactor : COLOR2;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float shading : TEXCOORD1;
};

float3 hash31(float p)
{
    float3 p3 = frac(float3(p * 21.2, p * 21.2, p * 21.2) * float3(0.1031, 0.1030, 0.0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return frac((p3.xxy + p3.yzz) * p3.zyx) + 0.05; 
}

static const float2 uvMap[] =
{
    float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f),
    float2(1.0f, 1.0f),
};

static const float faceShading[] =
{
    0.5f, // front
    0.8f, // back
    0.8f, // left
    0.5f, // right
    1.0f, // top
    0.5f, // bottom
};

static const float aoFactor[] =
{
    0.1f,
    0.25f,
    0.5f,
    1.0f,
};

PSInput main(VSInput input, uint vertexID : SV_VertexID)
{
    PSInput output;
    
    float4 pos = float4(input.Position, 1.0f);
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.pos = pos;
    output.color = hash31(input.VoxelType);
    // output.uv = uvMap[vertexID % 4];
    output.uv = input.Texcoord;
    output.shading = faceShading[(int)input.FaceType] * aoFactor[(int)input.aoFactor];

    return output;
}
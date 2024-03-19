#ifndef __COMMON_HLSLI__
#define __COMMON_HLSLI__

SamplerState gLinearWrapSampler : register(s0);
SamplerState gLinearClampSampler : register(s1);
SamplerState gPointWrapSampler : register(s2);
SamplerState gPointClampSampler : register(s3);
SamplerState gLinearMirrorSampler : register(s4);
SamplerState gAnisoWrapSampler : register(s5);

// texture
Texture2D gBlockTexArray : register(t0);
Texture2D gFrameTex: register(t1);
Texture2D gWaterTex: register(t2);

// PS
cbuffer GlobalCB : register(b0)
{
    float3 CameraPosWorld;
    float1 WaterLine;
    float3 BackgroundColor;
    float FogStrength;
};

// VS
cbuffer ModelCB : register(b0)
{
    matrix Model;
};

cbuffer ViewProjCB : register(b1)
{
    matrix View;
    matrix Proj;
};

cbuffer InteractionModeCB : register(b5)
{
    uint InteractionMode;
}

cbuffer CloudsCB : register(b5)
{
    float Time;
    float CloudScale;
    float Dummy;
    int WorldCenterXZ;
};

cbuffer WaterCB : register(b5)
{
    float WaterLine2; // #TODO duplicate in GlobalConstants
    int WaterArea;
    float3 Dummy2;
};

#endif // __COMMON_HLSLI__
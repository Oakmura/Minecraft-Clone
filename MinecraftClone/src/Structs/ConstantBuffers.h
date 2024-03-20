#pragma once

__declspec(align(256)) struct GlobalCB // (ps, 0)
{
    SimpleMath::Vector3 CameraPosition;
    float WaterLine;

    SimpleMath::Vector3 BackgroundColor;
    float FogStrength;
};

__declspec(align(256)) struct ModelMatrixCB // (vs, 0)
{
    SimpleMath::Matrix Model;
};

__declspec(align(256)) struct ViewProjCB // (vs, 1)
{
    SimpleMath::Matrix View;
    SimpleMath::Matrix Projection;
};

enum eInteractionMode;
__declspec(align(256)) struct InteractionModeCB // (vs, 5)
{
    eInteractionMode InteractionMode;
};

__declspec(align(256)) struct WaterCB // (vs, 5)
{
    float WaterLine;
    int WaterArea;
    SimpleMath::Vector2 Dummy;
};

__declspec(align(256)) struct CloudsCB // (vs, 5)
{
    float Time;
    float CloudScale;
    float Dummy;
    float WorldCenterXZ;
};

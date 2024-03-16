#pragma once

#include "Utils/D3D11Utils.h"

struct GlobalCB
{
    SimpleMath::Vector3 CameraPosition;
    float WaterLine;

    SimpleMath::Vector3 BackgroundColor;
    float FogStrength;
};

struct ViewProjCB
{
    SimpleMath::Matrix View;
    SimpleMath::Matrix Projection;
};

__declspec(align(256)) struct ModelMatrixCB
{
    SimpleMath::Matrix Model;
};

enum eInteractionMode;
__declspec(align(256)) struct InteractionModeCB
{
    eInteractionMode InteractionMode;
};

__declspec(align(256)) struct WaterCB
{
    float WaterLine;
    int WaterArea;
    SimpleMath::Vector2 Dummy;
};

__declspec(align(256)) struct CloudsCB
{
    float Time;
    float CloudScale;
    float Dummy;
    int WorldCenterXZ;
};

template<typename T>
class ConstantBuffer
{
public:
    ConstantBuffer() = default;
    ~ConstantBuffer() { RELEASE_COM(mGPU); }
    ConstantBuffer(const ConstantBuffer& rhs) = delete;
    ConstantBuffer& operator=(const ConstantBuffer& rhs) = delete;

public:
    void CreateBuffer(ID3D11Device& device)
    {
        D3D11Utils::CreateConstantBuffer(device, mCPU, &mGPU);
    }

    void UpdateBuffer(ID3D11DeviceContext& context)
    {
        D3D11Utils::UpdateBuffer(context, mCPU, mGPU);
    }

    inline T& GetCPU() { return mCPU; }
    inline ID3D11Buffer*& GetGPU() { return mGPU; }

private:
    T mCPU;
    ID3D11Buffer* mGPU = nullptr;
};
#pragma once

#include "Utils/D3D11Utils.h"
#include "Core/GraphicsEngine.h"

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
    int WorldCenterXZ;
};

enum class eShader
{
    Vertex,
    Pixel,
};

template<typename T>
class ConstantBuffer final
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

    void UseOn(eShader shaderUsage, uint32_t slot)
    {
        GraphicsEngine& ge = GraphicsEngine::GetInstance();
        if (shaderUsage == eShader::Vertex)
        {
            ge.GetDeviceContext().VSSetConstantBuffers(slot, 1, &mGPU);
        }
        else
        {
            ge.GetDeviceContext().PSSetConstantBuffers(slot, 1, &mGPU);
        }
    }

    inline T& GetCPU() { return mCPU; }
    inline ID3D11Buffer*& GetGPU() { return mGPU; }

private:
    T mCPU;
    ID3D11Buffer* mGPU = nullptr;
};
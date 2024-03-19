#pragma once

#include "Structs/ConstantBuffers.h"
#include "Utils/D3D11Utils.h"
#include "Core/GraphicsEngine.h"

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
        shaderUsage == eShader::Vertex 
            ? ge.GetDeviceContext().VSSetConstantBuffers(slot, 1, &mGPU) 
            : ge.GetDeviceContext().PSSetConstantBuffers(slot, 1, &mGPU);
    }

    inline T& GetCPU() { return mCPU; }
    inline ID3D11Buffer*& GetGPU() { return mGPU; }

private:
    T mCPU;
    ID3D11Buffer* mGPU = nullptr;
};
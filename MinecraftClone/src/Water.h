#pragma once

#include "Settings.h"
#include "Graphics/Vertex.h"
#include "Graphics/ConstantBuffers.h"
#include "Managers/GraphicsResourceManager.h"

class Water final
{
public:
    Water();
    ~Water();
    Water(const Water& rhs) = delete;
    Water& operator=(const Water& rhs) = delete;

    void Render();

private:
    ID3D11InputLayout* mIL;
    ID3D11VertexShader* mVS;
    ID3D11PixelShader* mPS;

    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;

    ID3D11Texture2D* mWaterTex;
    ID3D11ShaderResourceView* mWaterSRV;

    ConstantBuffer<WaterCB> mWaterCB;
};


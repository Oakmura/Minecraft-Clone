#pragma once

#include "Settings.h"
#include "Managers/GraphicsResourceManager.h"

__declspec(align(256)) struct WaterCB
{
    float WaterLine;
    int WaterArea;
    SimpleMath::Vector2 Dummy;
};

struct WaterVertex
{
    IntVector3D Position;
    SimpleMath::Vector2 Texcoord;
};

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

    WaterCB mWaterCbCPU;
    ID3D11Buffer* mWaterCbGPU;
};


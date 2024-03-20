#pragma once

#include "Settings.h"
#include "Generators/GeometryGenerator.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/Texture.h"
#include "Core/GraphicsEngine.h"

class Water final
{
public:
    Water();
    ~Water();
    Water(const Water& rhs) = delete;
    Water& operator=(const Water& rhs) = delete;

    void Render();

private:
    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;

    ConstantBuffer<WaterCB> mWaterCB;
};
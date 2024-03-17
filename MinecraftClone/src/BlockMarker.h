#pragma once

#include "Geometry/GeometryGenerator.h"
#include "Graphics/ConstantBuffers.h"
#include "Managers/GraphicsEngine.h"
#include "BlockHandler.h"

class BlockMarker final
{
public:
    BlockMarker();
    ~BlockMarker();
     
    void Update(const BlockHandler& blockHandler);
    void Render(const BlockHandler& blockHandler);

private:
    ID3D11InputLayout* mIL;
    ID3D11VertexShader* mVS;
    ID3D11PixelShader* mPS;

    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;

    ConstantBuffer<ModelMatrixCB> mModelMatrixCB;
    ConstantBuffer<InteractionModeCB> mInteractionModeCB;
};


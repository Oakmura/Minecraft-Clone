#pragma once

#include "Graphics/Vertex.h"
#include "Graphics/ConstantBuffers.h"
#include "Managers/GraphicsResourceManager.h"
#include "BlockHandler.h"

class BlockMarker final
{
public:
    BlockMarker();
    ~BlockMarker();
     
    void Update(const BlockHandler& blockHandler);
    void Render(const BlockHandler& blockHandler);

private:
    std::vector<uint32_t> mIndices;

    ID3D11InputLayout* mIL;
    ID3D11VertexShader* mVS;
    ID3D11PixelShader* mPS;

    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;

    ConstantBuffer<ModelMatrixCB> mModelMatrixCB;
    ConstantBuffer<InteractionModeCB> mInteractionModeCB;
};


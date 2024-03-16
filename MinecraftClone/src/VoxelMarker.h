#pragma once

#include "Graphics/ConstantBuffers.h"
#include "Managers/GraphicsResourceManager.h"
#include "VoxelHandler.h"

struct VoxelMarkerVertex
{
    IntVector3D Position;
    SimpleMath::Vector2 Texcoord;
};

class VoxelMarker final
{
public:
    VoxelMarker();
    ~VoxelMarker();
     
    void Update(const VoxelHandler& voxelHandler);
    void Render(const VoxelHandler& voxelHandler);

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


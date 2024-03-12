#pragma once

#include "Managers/GraphicsResourceManager.h"
#include "VoxelHandler.h"

__declspec(align(256)) struct InteractionModeCB
{
    eInteractionMode InteractionMode;
};

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

    SimpleMath::Matrix mModelCPU;
    ID3D11Buffer* mModelGPU;

    InteractionModeCB mInteractionModeCPU;
    ID3D11Buffer* mInteractionModeGPU;
};


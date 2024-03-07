#pragma once

#include "Managers/GraphicsResourceManager.h"

class VoxelMarker // #TODO: �Ź� ��ġ�� ��������
{
public:

    VoxelMarker();
    ~VoxelMarker();

    void Update();
    void Render();

private:
    std::vector<uint32_t> mIndices;

    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;

    SimpleMath::Matrix mModelCPU;
    ID3D11Buffer* mModelGPU;
    ID3D11Buffer* mInteractionModeCB;
};


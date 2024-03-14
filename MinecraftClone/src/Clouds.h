#pragma once

#include "Managers/GraphicsResourceManager.h"

#include "Chunk.h" // #TODO remove Temporary for WORLD_CENTER_XZ and CHUNK_SIZE
#include "World.h"

__declspec(align(256)) struct CloudsCB
{
    float Time;
    float CloudScale;
    float Dummy;
    int WorldCenterXZ;
};

struct CloudsVertex
{
    IntVector3D Position;
};

class Clouds final
{
public:
    Clouds();
    ~Clouds();
    Clouds(const Clouds& rhs) = delete;
    Clouds& operator=(const Clouds& rhs) = delete;

    void Update(const float dt);
    void Render();

private:
    void buildVertices(const std::vector<uint8_t>& cloudData);

private:
    enum { CLOUD_HEIGHT = WORLD_HEIGHT * CHUNK_SIZE * 2 };

    std::vector<IntVector3D> mVertices;
    std::vector<uint32_t> mIndices;

    ID3D11InputLayout* mIL;
    ID3D11VertexShader* mVS;
    ID3D11PixelShader* mPS;

    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;

    CloudsCB mCloudsCbCPU;
    ID3D11Buffer* mCloudsCbGPU;
};
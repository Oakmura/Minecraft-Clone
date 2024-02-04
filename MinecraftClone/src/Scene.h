#pragma once

#include "Managers/GraphicsResourceManager.h"

enum class eVoxelType : uint8_t
{
    EMPTY = 0,
    DEFAULT = 1,
};

enum class eFaceType : uint8_t
{
    FRONT = 0,
    BACK = 1,
    LEFT = 2,
    RIGHT = 3,
    TOP = 4,
    BOTTOM = 5,
};

struct VoxelVertex
{
    IntVector3D Position;
    Vector2 Texcoord;
    eVoxelType VoxelType;
    eFaceType FaceType;
};

class Scene final
{
    friend class Renderer;
public:
    Scene(GraphicsResourceManager& GRM);
    ~Scene();

private:
    bool isEmptyVoxel(const int x, const int y, const int z);
    void addNewIndex(uint32_t* outIndexOffset);

private:
    enum { CHUNK_SIZE = 32 };
    enum { CHUNK_HALF_SIZE = CHUNK_SIZE / 2 };
    enum { CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE };
    enum { CHUNK_VOLUME = CHUNK_AREA * CHUNK_SIZE };

    enum { MAX_NUM_VERTEX_PER_VOXEL = 18 };
    enum { MAX_NUM_INDEX_PER_VOXEL = 36 };

    std::vector<eVoxelType> mVoxelTypes;
    std::vector<VoxelVertex> mVoxels;
    std::vector<uint32_t> mIndices;

    ID3D11VertexShader* mVS;
    ID3D11PixelShader* mPS;
    ID3D11InputLayout* mIL;

    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;

    ID3D11Texture2D* mFrameTex;
    ID3D11ShaderResourceView* mFrameSRV;

    ID3D11Texture2D* mTestTex;
    ID3D11ShaderResourceView* mTestSRV;
};


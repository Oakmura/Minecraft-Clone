#pragma once

#include "Managers/GraphicsResourceManager.h"

enum { CHUNK_SIZE = 32 };
enum { CHUNK_HALF_SIZE = CHUNK_SIZE / 2 };
enum { CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE };
enum { CHUNK_VOLUME = CHUNK_AREA * CHUNK_SIZE };

enum { MAX_NUM_VERTEX_PER_VOXEL = 18 };
enum { MAX_NUM_INDEX_PER_VOXEL = 36 };

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
    eVoxelType VoxelType;
    eFaceType FaceType;
    uint8_t AmbientOcclusionFactor;
};

class Chunk
{
    friend class ChunkBuilder;
    friend class Renderer;
public:
    Chunk() = default;
    ~Chunk();

    void BuildVoxels(GraphicsResourceManager& GRM, World& world, const Vector3& pos);
    void Render(GraphicsResourceManager& GRM);

private:
    std::vector<eVoxelType> mVoxelTypes;
    std::vector<VoxelVertex> mVoxels;
    std::vector<uint32_t> mIndices;

    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;
    
    Matrix mModelCPU;
    ID3D11Buffer* mModelGPU;
};


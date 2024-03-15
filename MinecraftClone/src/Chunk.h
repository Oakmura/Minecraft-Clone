#pragma once

#include "Settings.h"
#include "Managers/GraphicsResourceManager.h"

enum class eVoxelType : uint8_t
{
    Empty = 0,
    Sand = 1,
    Grass = 2,
    Dirt = 3,
    Stone = 4,
    Snow = 5,
    Leaves = 6,
    Wood = 7,
};

enum class eFaceType : uint8_t
{
    Front = 0,
    Back = 1,
    Left = 2,
    Right = 3,
    Top = 4,
    Bottom = 5,
};

enum class eVertexType : uint8_t
{
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
};

struct VoxelVertex
{
    IntVector3D Position;
    SimpleMath::Vector2 Texcoord;
    eVoxelType VoxelType;
    eFaceType FaceType;
    uint8_t AmbientOcclusionFactor;
};

class World;

class Chunk final
{
    friend class ChunkBuilder;
public:
    Chunk();
    ~Chunk();

    void BuildVoxels(const IntVector3D& pos);
    void BuildChunkMesh();
    void RebuildChunkMesh(World& world);
    void Render();

    eVoxelType GetVoxelType(int voxelIndex);
    void SetVoxel(int voxelIndex, eVoxelType voxelType);

private:
    std::vector<eVoxelType> mVoxelTypes;
    std::vector<VoxelVertex> mVoxels;
    std::vector<uint32_t> mIndices;

    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;
    
    SimpleMath::Matrix mModelCPU;
    ID3D11Buffer* mModelGPU;

    IntVector3D mPosition;
};


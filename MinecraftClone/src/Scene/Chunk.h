#pragma once

#include "Settings.h"
#include "Structs/Vertex.h"
#include "Graphics/ConstantBuffer.h"
#include "Core/GraphicsEngine.h"

enum class eBlockType : uint8_t
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

class World;

class Chunk final
{
    friend class ChunkGenerator;
public:
    Chunk();
    ~Chunk();
    Chunk(const Chunk& rhs) = delete;
    Chunk& operator=(const Chunk& rhs) = delete;

    void BuildBlocks(const IntVector3D& pos);
    void BuildChunkMesh();
    void RebuildChunkMesh(World& world);
    void Render();

    inline const SimpleMath::Vector3& GetCenter() const { return mCenter; }
    eBlockType GetBlockType(int blockIndex);
    void SetBlock(int blockIndex, eBlockType blockType);

private:
    std::vector<eBlockType> mBlockTypes;
    std::vector<BlockVertex> mBlocks;
    std::vector<uint32_t> mIndices;

    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;

    ConstantBuffer<ModelMatrixCB> mModelCB;

    IntVector3D mPosition;
    SimpleMath::Vector3 mCenter;
};


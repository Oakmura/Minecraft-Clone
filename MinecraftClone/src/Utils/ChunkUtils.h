#pragma once

#include "Scene/Chunk.h"
#include "Settings.h"

struct BlockInfo
{
    eBlockType BlockType = eBlockType::Empty;
    uint32_t BlockIndex;
    IntVector3D BlockLocalPos;
    Chunk* Chunk;
};

class ChunkUtils final
{
public:
    ChunkUtils() = delete;
    ~ChunkUtils() = delete;
    ChunkUtils(const ChunkUtils& rhs) = delete;
    ChunkUtils operator=(const ChunkUtils& rhs) = delete;

    static void Init(World* world);

    static int GetBlockIndex(const IntVector3D& localPos);
    static bool GetBlockInfo(BlockInfo* outBlockInfo, const IntVector3D& blockWorldPos);

    static int GetChunkIndexLocal(const IntVector3D& localPos);
    static int GetChunkIndexWorld(const IntVector3D& worldPos);

    static bool ChunkUtils::GetChunkXYZ(IntVector3D* outPos, const IntVector3D& worldPos);

private:
    static World* sWorld;
};
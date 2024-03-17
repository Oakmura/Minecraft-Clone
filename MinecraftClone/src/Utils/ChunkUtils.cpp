#include "Precompiled.h"

#include "ChunkUtils.h"
#include "Scene/World.h"

World* ChunkUtils::sWorld = nullptr;

void ChunkUtils::Init(World * world)
{
    sWorld = world;
}

int ChunkUtils::GetBlockIndex(const IntVector3D& localPos)
{
    ASSERT(localPos.mX >= 0 && localPos.mX < def::CHUNK_SIZE && localPos.mY >= 0 && 
        localPos.mY < def::CHUNK_SIZE && localPos.mZ >= 0 && localPos.mZ < def::CHUNK_SIZE, "invalid localPos");

    return localPos.mX + localPos.mZ * def::CHUNK_SIZE + localPos.mY * def::CHUNK_AREA;
}

bool ChunkUtils::GetBlockInfo(BlockInfo* outBlockInfo, const IntVector3D& blockWorldPos)
{
    int chunkIndex = ChunkUtils::GetChunkIndexWorld(blockWorldPos);
    if (chunkIndex == -1)
    {
        return false;
    }

    IntVector3D chunkXYZ;
    ChunkUtils::GetChunkXYZ(&chunkXYZ, blockWorldPos);

    outBlockInfo->Chunk = sWorld->GetChunkPtr(chunkIndex);
    outBlockInfo->BlockLocalPos = blockWorldPos - chunkXYZ * def::CHUNK_SIZE;
    outBlockInfo->BlockIndex = outBlockInfo->BlockLocalPos.mX + outBlockInfo->BlockLocalPos.mZ * def::CHUNK_SIZE + outBlockInfo->BlockLocalPos.mY * def::CHUNK_AREA;
    outBlockInfo->BlockType = static_cast<eBlockType>(outBlockInfo->Chunk->GetBlockType(outBlockInfo->BlockIndex));

    ASSERT(outBlockInfo->BlockType >= eBlockType::Empty && outBlockInfo->BlockType <= eBlockType::Wood, "unidentified block type");

    return outBlockInfo->BlockType == eBlockType::Empty ? false : true;
}

int ChunkUtils::GetChunkIndexLocal(const IntVector3D& localPos)
{
    ASSERT(localPos.mX >= 0 && localPos.mX < def::CHUNK_SIZE && localPos.mY >= 0 && localPos.mY < def::CHUNK_SIZE && localPos.mZ >= 0 && localPos.mZ < def::CHUNK_SIZE, "invalid localPos");

    return localPos.mX + localPos.mZ * def::WORLD_WIDTH + localPos.mY * def::WORLD_AREA;
}

int ChunkUtils::GetChunkIndexWorld(const IntVector3D& worldPos)
{
    ASSERT(worldPos.mX >= -1 && worldPos.mY >= -1 && worldPos.mZ >= -1, "unexpected world pos");

    if (worldPos.mX < 0 || worldPos.mY < 0 || worldPos.mZ < 0) // (-1/32 == 0 in cpp BUT -1/32 == -1 in python)
    {
        return -1;
    }

    int cx = worldPos.mX / def::CHUNK_SIZE;
    int cy = worldPos.mY / def::CHUNK_SIZE;
    int cz = worldPos.mZ / def::CHUNK_SIZE;

    ASSERT(cx >= 0 && cy >= 0 && cz >= 0, "unexpected chunk coordinates");

    if (cx >= def::WORLD_WIDTH || cy >= def::WORLD_HEIGHT || cz >= def::WORLD_DEPTH)
    {
        return -1;
    }

    return cx + cz * def::WORLD_DEPTH + cy * def::WORLD_AREA;
}

bool ChunkUtils::GetChunkXYZ(IntVector3D* outPos, const IntVector3D& worldPos) // #TODO a lot of duplicates in GetChunkIndexWorld()
{
    ASSERT(worldPos.mX >= -1 && worldPos.mY >= -1 && worldPos.mZ >= -1, "unexpected world pos");

    if (worldPos.mX < 0 || worldPos.mY < 0 || worldPos.mZ < 0) // (-1/32 == 0 in cpp BUT -1/32 == -1 in python)
    {
        return false;
    }

    int cx = worldPos.mX / def::CHUNK_SIZE;
    int cy = worldPos.mY / def::CHUNK_SIZE;
    int cz = worldPos.mZ / def::CHUNK_SIZE;

    ASSERT(cx >= 0 && cy >= 0 && cz >= 0, "unexpected chunk coordinates");

    if (cx >= def::WORLD_WIDTH || cy >= def::WORLD_HEIGHT || cz >= def::WORLD_DEPTH)
    {
        return false;
    }

    *outPos = { cx, cy, cz };

    return true;
}
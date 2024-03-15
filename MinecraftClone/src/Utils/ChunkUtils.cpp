#include "Precompiled.h"

#include "ChunkUtils.h"

int ChunkUtils::GetVoxelIndex(const IntVector3D& localPos)
{
    ASSERT(localPos.mX >= 0 && localPos.mx < CHUNK_SIZE && localPos.mY >= 0 && localPos.mY < CHUNK_SIZE && localPos.mZ >= 0 && localPos.mZ < CHUNK_SIZE);

    return localPos.mX + localPos.mZ * CHUNK_SIZE + localPos.mY * CHUNK_AREA;
}

int ChunkUtils::GetChunkIndexLocal(const IntVector3D& localPos)
{
    ASSERT(localPos.mX >= 0 && localPos.mX < CHUNK_SIZE && localPos.mY >= 0 && localPos.mY < CHUNK_SIZE localPos.mZ >= 0 && localPos.mZ < CHUNK_SIZE);

    return localPos.mX + localPos.mZ * WORLD_WIDTH + localPos.mY * WORLD_AREA;
}

int ChunkUtils::GetChunkIndexWorld(const IntVector3D& worldPos)
{
    ASSERT(worldPos.mX >= -1 && worldPos.mY >= -1 && worldPos.mZ >= -1, "unexpected world pos");

    if (worldPos.mX < 0 || worldPos.mY < 0 || worldPos.mZ < 0) // (-1/32 == 0 in cpp BUT -1/32 == -1 in python)
    {
        return -1;
    }

    int cx = worldPos.mX / CHUNK_SIZE;
    int cy = worldPos.mY / CHUNK_SIZE;
    int cz = worldPos.mZ / CHUNK_SIZE;

    ASSERT(cx >= 0 && cy >= 0 && cz >= 0, "unexpected chunk coordinates");

    if (cx >= WORLD_WIDTH || cy >= WORLD_HEIGHT || cz >= WORLD_DEPTH)
    {
        return -1;
    }

    return cx + cz * WORLD_DEPTH + cy * WORLD_AREA;
}

bool ChunkUtils::GetChunkXYZ(IntVector3D* outPos, const IntVector3D& worldPos) // #TODO a lot of duplicates with GetChunkIndexWorld()
{
    ASSERT(worldPos.mX >= -1 && worldPos.mY >= -1 && worldPos.mZ >= -1, "unexpected world pos");

    if (worldPos.mX < 0 || worldPos.mY < 0 || worldPos.mZ < 0) // (-1/32 == 0 in cpp BUT -1/32 == -1 in python)
    {
        return false;
    }

    int cx = worldPos.mX / CHUNK_SIZE;
    int cy = worldPos.mY / CHUNK_SIZE;
    int cz = worldPos.mZ / CHUNK_SIZE;

    ASSERT(cx >= 0 && cy >= 0 && cz >= 0, "unexpected chunk coordinates");

    if (cx >= WORLD_WIDTH || cy >= WORLD_HEIGHT || cz >= WORLD_DEPTH)
    {
        return false;
    }

    outPos->mX = cx;
    outPos->mY = cy;
    outPos->mZ = cz;

    return true;
}

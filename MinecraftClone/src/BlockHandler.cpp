#include "Precompiled.h"

#include "Scene/Player.h"
#include "Scene/World.h"
#include "Utils/ChunkUtils.h"
#include "BlockHandler.h"

World* BlockHandler::sWorld = nullptr;

void BlockHandler::Init(World* world)
{
    sWorld = world;
}

void BlockHandler::Update(Player& player)
{
    rayCast(player);
}

void BlockHandler::SwitchInteractionMode()
{
    mInteractionMode = static_cast<eInteractionMode>(mInteractionMode ^ 1);
}

void BlockHandler::SetBlock()
{
    ASSERT(mInteractionMode == eInteractionMode::Add || mInteractionMode == eInteractionMode::Remove, "unidentified interaction mode");

    mInteractionMode == eInteractionMode::Add ? addBlock() : removeBlock();
}

void BlockHandler::addBlock()
{
    if (mFocusedBlockInfo.BlockType == eBlockType::Empty)
    {
        return;
    }

    BlockInfo blockInfo;
    IntVector3D worldPosToQuery = mFocusedBlockWorldPos + mFocusedBlockNormal;
    ChunkUtils::GetBlockInfo(&blockInfo, worldPosToQuery);

    if (blockInfo.BlockType != eBlockType::Empty)
    {
        return;
    }

    blockInfo.Chunk->SetBlock(blockInfo.BlockIndex, mNewBlockType);
    blockInfo.Chunk->RebuildChunkMesh(*sWorld);
}

void BlockHandler::removeBlock()
{
    if (mFocusedBlockInfo.BlockType == eBlockType::Empty)
    {
        return;
    }

    mFocusedBlockInfo.Chunk->SetBlock(mFocusedBlockInfo.BlockIndex, eBlockType::Empty);
    mFocusedBlockInfo.Chunk->RebuildChunkMesh(*sWorld);
    rebuildAdjacentChunks();
}

void BlockHandler::rebuildAdjacentChunk(const IntVector3D& adjacentChunkPosWorld)
{
    BlockInfo blockInfo;
    if (ChunkUtils::GetChunkIndexWorld(adjacentChunkPosWorld) == -1)
    {
        return;
    }

    blockInfo.Chunk->RebuildChunkMesh(*sWorld);
}

void BlockHandler::rebuildAdjacentChunks()
{
    if (mFocusedBlockInfo.BlockLocalPos.mX == 0)
    {
        rebuildAdjacentChunk({ mFocusedBlockWorldPos.mX - 1, mFocusedBlockWorldPos.mY, mFocusedBlockWorldPos.mZ });
    }
    else if (mFocusedBlockInfo.BlockLocalPos.mX == def::CHUNK_SIZE - 1)
    {
        rebuildAdjacentChunk({ mFocusedBlockWorldPos.mX + 1, mFocusedBlockWorldPos.mY, mFocusedBlockWorldPos.mZ });
    }
    
    if (mFocusedBlockInfo.BlockLocalPos.mY == 0)
    {
        rebuildAdjacentChunk({ mFocusedBlockWorldPos.mX, mFocusedBlockWorldPos.mY - 1, mFocusedBlockWorldPos.mZ });
    }
    else if (mFocusedBlockInfo.BlockLocalPos.mY == def::CHUNK_SIZE - 1)
    {
        rebuildAdjacentChunk({ mFocusedBlockWorldPos.mX, mFocusedBlockWorldPos.mY + 1, mFocusedBlockWorldPos.mZ });
    }

    if (mFocusedBlockInfo.BlockLocalPos.mZ == 0)
    {
        rebuildAdjacentChunk({ mFocusedBlockWorldPos.mX, mFocusedBlockWorldPos.mY, mFocusedBlockWorldPos.mZ - 1 });
    }
    else if (mFocusedBlockInfo.BlockLocalPos.mZ == def::CHUNK_SIZE - 1)
    {
        rebuildAdjacentChunk({ mFocusedBlockWorldPos.mX, mFocusedBlockWorldPos.mY, mFocusedBlockWorldPos.mZ + 1 });
    }

}

#define FRAC0(x) (x - floorf(x))
#define FRAC1(x) (1 - x + floorf(x))

bool BlockHandler::rayCast(Player& player)
{
    const SimpleMath::Vector3& startPosition = player.GetPosition();
    const SimpleMath::Vector3 endPosition = startPosition + player.GetForward() * MAX_RAY_DIST;

    float distX = endPosition.x - startPosition.x;
    int dx = BasicUtils::GetSign(distX);
    float deltaX = dx != 0 ? min(dx / distX, 10000000.f) : 10000000.f;
    float maxX = dx > 0 ? deltaX * FRAC1(startPosition.x) : deltaX * FRAC0(startPosition.x);

    float distY = endPosition.y - startPosition.y;
    int dy = BasicUtils::GetSign(distY);
    float deltaY = dy != 0 ? min(dy / distY, 10000000.f) : 10000000.f;
    float maxY = dy > 0 ? deltaY * FRAC1(startPosition.y) : deltaY * FRAC0(startPosition.y);

    float distZ = endPosition.z - startPosition.z;
    int dz = BasicUtils::GetSign(distZ);
    float deltaZ = dz != 0 ? min(dz / distZ, 10000000.f) : 10000000.f;
    float maxZ = dz > 0 ? deltaZ * FRAC1(startPosition.z) : deltaZ * FRAC0(startPosition.z);

    IntVector3D currentPosition = IntVector3D((int)startPosition.x, (int)startPosition.y, (int)startPosition.z);
    mFocusedBlockInfo.BlockType = eBlockType::Empty;
    mFocusedBlockNormal = IntVector3D();
    int stepDir = -1;

    BlockInfo blockInfo;
    while (maxX <= 1.f && maxY <= 1.f && maxZ <= 1.f)
    {
        if (ChunkUtils::GetBlockInfo(&blockInfo, currentPosition))
        {
            mFocusedBlockInfo = blockInfo;
            mFocusedBlockWorldPos = currentPosition;

            if (stepDir == 0)
            {
                mFocusedBlockNormal.mX = -dx;
            }
            else if (stepDir == 1)
            {
                mFocusedBlockNormal.mY = -dy;
            }
            else
            {
                mFocusedBlockNormal.mZ = -dz;
            }

            return true;
        }

        if (maxX < maxY)
        {
            if (maxX < maxZ)
            {
                currentPosition.mX += dx;
                maxX += deltaX;
                stepDir = 0;
            }
            else
            {
                currentPosition.mZ += dz;
                maxZ += deltaZ;
                stepDir = 2;
            }
        }
        else
        {
            if (maxY < maxZ)
            {
                currentPosition.mY += dy;
                maxY += deltaY;
                stepDir = 1;
            }
            else
            {
                currentPosition.mZ += dz;
                maxZ += deltaZ;
                stepDir = 2;
            }
        }
    }

    return false;
}

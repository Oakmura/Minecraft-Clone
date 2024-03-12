#include "Precompiled.h"

#include "Player.h"
#include "World.h"
#include "VoxelHandler.h"

World* VoxelHandler::sWorld = nullptr;

void VoxelHandler::Init(World* world)
{
    sWorld = world;
}

void VoxelHandler::Update(Player& player)
{
    rayCast(player);

    if (mFocusedVoxelInfo.VoxelType != eVoxelType::Empty)
    {
        LOG_TRACE("Voxel Local Pos {0} {1} {2}", mFocusedVoxelInfo.VoxelLocalPos.mX, mFocusedVoxelInfo.VoxelLocalPos.mY, mFocusedVoxelInfo.VoxelLocalPos.mZ);
        LOG_INFO("Voxel World Pos {0} {1} {2}", mFocusedVoxelWorldPos.mX, mFocusedVoxelWorldPos.mY, mFocusedVoxelWorldPos.mZ);
        LOG_WARN("Voxel Normal {0} {1} {2}", mFocusedVoxelNormal.mX, mFocusedVoxelNormal.mY, mFocusedVoxelNormal.mZ);
    }
}

void VoxelHandler::SwitchInteractionMode()
{
    LOG_INFO("Switched Interaction Mode to {0}", mInteractionMode);

    mInteractionMode = static_cast<eInteractionMode>(mInteractionMode ^ 1);
}

void VoxelHandler::SetVoxel()
{
    ASSERT(mInteractionMode == eInteractionMode::Add || mInteractionMode == eInteractionMode::Remove, "unidentified interaction mode");

    mInteractionMode == eInteractionMode::Add ? addVoxel() : removeVoxel();
}

int VoxelHandler::getSign(const float val)
{
    return (0.0f < val) - (val < 0.0f);
}

void VoxelHandler::addVoxel()
{
    if (mFocusedVoxelInfo.VoxelType == eVoxelType::Empty)
    {
        return;
    }

    VoxelInfo voxelInfo;
    IntVector3D worldPosToQuery = mFocusedVoxelWorldPos + mFocusedVoxelNormal;
    getVoxelInfo(&voxelInfo, worldPosToQuery);

    if (voxelInfo.VoxelType != eVoxelType::Empty)
    {
        return;
    }

    voxelInfo.Chunk->SetVoxel(voxelInfo.VoxelIndex, mNewVoxelType);
    voxelInfo.Chunk->RebuildChunkMesh(*sWorld);
}

void VoxelHandler::removeVoxel()
{
    if (mFocusedVoxelInfo.VoxelType == eVoxelType::Empty)
    {
        return;
    }

    mFocusedVoxelInfo.Chunk->SetVoxel(mFocusedVoxelInfo.VoxelIndex, eVoxelType::Empty);
    mFocusedVoxelInfo.Chunk->RebuildChunkMesh(*sWorld);
    rebuildAdjacentChunks();
}

void VoxelHandler::rebuildAdjacentChunk(const IntVector3D& adjacentChunkPosWorld)
{
    VoxelInfo voxelInfo;
    if (!getVoxelInfo(&voxelInfo, adjacentChunkPosWorld)) // #TODO if chunk index is out of bounds
    {
        return;
    }

    voxelInfo.Chunk->RebuildChunkMesh(*sWorld);
}

void VoxelHandler::rebuildAdjacentChunks()
{
    if (mFocusedVoxelInfo.VoxelLocalPos.mX == 0)
    {
        rebuildAdjacentChunk({ mFocusedVoxelWorldPos.mX - 1, mFocusedVoxelWorldPos.mY, mFocusedVoxelWorldPos.mZ });
    }
    else if (mFocusedVoxelInfo.VoxelLocalPos.mX == CHUNK_SIZE - 1)
    {
        rebuildAdjacentChunk({ mFocusedVoxelWorldPos.mX + 1, mFocusedVoxelWorldPos.mY, mFocusedVoxelWorldPos.mZ });
    }
    
    if (mFocusedVoxelInfo.VoxelLocalPos.mY == 0)
    {
        rebuildAdjacentChunk({ mFocusedVoxelWorldPos.mX, mFocusedVoxelWorldPos.mY - 1, mFocusedVoxelWorldPos.mZ });
    }
    else if (mFocusedVoxelInfo.VoxelLocalPos.mY == CHUNK_SIZE - 1)
    {
        rebuildAdjacentChunk({ mFocusedVoxelWorldPos.mX, mFocusedVoxelWorldPos.mY + 1, mFocusedVoxelWorldPos.mZ });
    }

    if (mFocusedVoxelInfo.VoxelLocalPos.mZ == 0)
    {
        rebuildAdjacentChunk({ mFocusedVoxelWorldPos.mX, mFocusedVoxelWorldPos.mY, mFocusedVoxelWorldPos.mZ - 1 });
    }
    else if (mFocusedVoxelInfo.VoxelLocalPos.mZ == CHUNK_SIZE - 1)
    {
        rebuildAdjacentChunk({ mFocusedVoxelWorldPos.mX, mFocusedVoxelWorldPos.mY, mFocusedVoxelWorldPos.mZ + 1 });
    }

}

#define FRAC0(x) (x - floorf(x))
#define FRAC1(x) (1 - x + floorf(x))

bool VoxelHandler::rayCast(Player& player)
{
    const SimpleMath::Vector3& startPosition = player.GetPosition();
    const SimpleMath::Vector3 endPosition = startPosition + player.GetForward() * MAX_RAY_DIST;

    float distX = endPosition.x - startPosition.x;
    int dx = getSign(distX);
    float deltaX = dx != 0 ? min(dx / distX, 10000000.f) : 10000000.f;
    float maxX = dx > 0 ? deltaX * FRAC1(startPosition.x) : deltaX * FRAC0(startPosition.x);

    float distY = endPosition.y - startPosition.y;
    int dy = getSign(distY);
    float deltaY = dy != 0 ? min(dy / distY, 10000000.f) : 10000000.f;
    float maxY = dy > 0 ? deltaY * FRAC1(startPosition.y) : deltaY * FRAC0(startPosition.y);

    float distZ = endPosition.z - startPosition.z;
    int dz = getSign(distZ);
    float deltaZ = dz != 0 ? min(dz / distZ, 10000000.f) : 10000000.f;
    float maxZ = dz > 0 ? deltaZ * FRAC1(startPosition.z) : deltaZ * FRAC0(startPosition.z);

    IntVector3D currentPosition = IntVector3D((int)startPosition.x, (int)startPosition.y, (int)startPosition.z);
    mFocusedVoxelInfo.VoxelType = eVoxelType::Empty;
    mFocusedVoxelNormal = IntVector3D();
    int stepDir = -1;

    VoxelInfo voxelInfo;
    while (maxX <= 1.f && maxY <= 1.f && maxZ <= 1.f)
    {
        if (getVoxelInfo(&voxelInfo, currentPosition))
        {
            mFocusedVoxelInfo = voxelInfo;
            mFocusedVoxelWorldPos = currentPosition;

            if (stepDir == 0)
            {
                mFocusedVoxelNormal.mX = -dx;
            }
            else if (stepDir == 1)
            {
                mFocusedVoxelNormal.mY = -dy;
            }
            else
            {
                mFocusedVoxelNormal.mZ = -dz;
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

bool VoxelHandler::getVoxelInfo(VoxelInfo* outVoxelInfo, const IntVector3D& voxelWorldPos)
{
    if (voxelWorldPos.mX < 0 || voxelWorldPos.mY < 0 || voxelWorldPos.mZ < 0)
    {
        return false;
    }

    int cx = voxelWorldPos.mX / CHUNK_SIZE;
    int cy = voxelWorldPos.mY / CHUNK_SIZE;
    int cz = voxelWorldPos.mZ / CHUNK_SIZE;

    if (cx < 0 || cx >= WORLD_WIDTH || cy < 0 || cy >= WORLD_HEIGHT || cz < 0 || cz >= WORLD_DEPTH)
    {
        return false;
    }
    
    int chunkIndex = cx + cz * WORLD_DEPTH + cy * WORLD_AREA;

    outVoxelInfo->Chunk = sWorld->GetChunkPtr(chunkIndex);
    outVoxelInfo->VoxelLocalPos = voxelWorldPos - IntVector3D(cx, cy, cz) * CHUNK_SIZE;
    outVoxelInfo->VoxelIndex = outVoxelInfo->VoxelLocalPos.mX + outVoxelInfo->VoxelLocalPos.mZ * CHUNK_SIZE + outVoxelInfo->VoxelLocalPos.mY * CHUNK_AREA;
    outVoxelInfo->VoxelType = static_cast<eVoxelType>(outVoxelInfo->Chunk->GetVoxelType(outVoxelInfo->VoxelIndex));

    return outVoxelInfo->VoxelType == eVoxelType::Empty ? false : true;
}

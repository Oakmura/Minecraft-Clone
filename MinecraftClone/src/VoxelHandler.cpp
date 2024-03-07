#include "Precompiled.h"

#include "Player.h"
#include "World.h"
#include "VoxelHandler.h"

World* VoxelHandler::mWorld = nullptr;

void VoxelHandler::Init(World* world)
{
    mWorld = world;
}

VoxelHandler::VoxelHandler()
    : mInteractionMode(eInteractionMode::REMOVE)
    , mNewVoxelType(eVoxelType::DEFAULT)
    , mbOnFocus(false)
{
}

VoxelHandler::~VoxelHandler()
{
}

void VoxelHandler::Update(Player& player)
{
    rayCast(player);
}

void VoxelHandler::ToggleInteractionMode()
{
    mInteractionMode = static_cast<eInteractionMode>(mInteractionMode ^ 1);
}

void VoxelHandler::SetVoxel()
{
    ASSERT(mInteractionMode == eInteractionMode::ADD || mInteractionMode == eInteractionMode::REMOVE, "unidentified interaction mode");

    if (!mbOnFocus)
    {
        return;
    }

    mInteractionMode == eInteractionMode::ADD ? addVoxel() : removeVoxel();
}

void VoxelHandler::addVoxel()
{
    VoxelInfo voxelInfo;
    IntVector3D worldPosToQuery = mFocusedVoxelWorldPos + mFocusedVoxelNormal;
    getVoxelInfo(&voxelInfo, worldPosToQuery);

    if (voxelInfo.VoxelType != eVoxelType::EMPTY)
    {
        return;
    }

    mFocusedVoxelInfo.Chunk->SetVoxel(mFocusedVoxelInfo.VoxelIndex, mNewVoxelType);
    mFocusedVoxelInfo.Chunk->RebuildChunkMesh(*mWorld);
}

void VoxelHandler::removeVoxel()
{
    if (mFocusedVoxelInfo.VoxelType == eVoxelType::EMPTY)
    {
        return;
    }

    mFocusedVoxelInfo.Chunk->SetVoxel(mFocusedVoxelInfo.VoxelIndex, eVoxelType::EMPTY);
    mFocusedVoxelInfo.Chunk->RebuildChunkMesh(*mWorld);
}

bool VoxelHandler::rayCast(Player& player)
{
    const SimpleMath::Vector3& startPosition = player.GetPosition();
    const SimpleMath::Vector3 endPosition = startPosition + player.GetForward() * MAX_RAY_DIST;

    IntVector3D currentPosition = IntVector3D((int)startPosition.x, (int)startPosition.y, (int)startPosition.z);
    mFocusedVoxelInfo.VoxelType = eVoxelType::EMPTY;
    mFocusedVoxelNormal = IntVector3D();

    float distX = endPosition.x - startPosition.x;
    int dx;
    if (distX > 0.f)
    {
        dx = 1;
    }
    else if (distX < 0.f)
    {
        dx = -1;
    }
    else
    {
        dx = 0;
    }
    float deltaX = dx != 0 ? min(dx / (distX), 10000000.f) : 10000000.f;
    float maxX = (float)(dx > 0 ? deltaX * (1.0 - fmod((double)startPosition.x, 1.0)) : deltaX * fmod((double)startPosition.x, 1.0));

    float distY = endPosition.y - startPosition.y;
    int dy;
    if (distY > 0.f)
    {
        dy = 1;
    }
    else if (distY < 0.f)
    {
        dy = -1;
    }
    else
    {
        dy = 0;
    }
    float deltaY = dy != 0 ? min(dy / (distY), 10000000.f) : 10000000.f;
    float maxY = (float)(dy > 0 ? deltaY * (1.0 - fmod((double)startPosition.y, 1.0)) : deltaY * fmod((double)startPosition.y, 1.0));

    float distZ = endPosition.z - startPosition.z;
    int dz;
    if (distZ > 0.f)
    {
        dz = 1;
    }
    else if (distZ < 0.f)
    {
        dz = -1;
    }
    else
    {
        dz = 0;
    }
    float deltaZ = dz != 0 ? min(dz / (distZ), 10000000.f) : 10000000.f;
    float maxZ = (float)(dz > 0 ? deltaZ * (1.0 - fmod((double)startPosition.z, 1.0)) : deltaZ * fmod((double)startPosition.z, 1.0));

    int stepDir = -1;
    VoxelInfo voxelInfo;
    while (maxX < 1.f && maxY < 1.f && maxZ < 1.f)
    {
        if (getVoxelInfo(&voxelInfo, currentPosition))
        {
            mFocusedVoxelInfo = voxelInfo;
            mFocusedVoxelWorldPos = currentPosition;

            switch (stepDir)
            {
            case 0:
                mFocusedVoxelNormal.mX = -dx;
                break;
            case 1:
                mFocusedVoxelNormal.mY = -dy;
                break;
            case 2:
            case -1:
                mFocusedVoxelNormal.mZ = -dz;
                break;
            default:
                ASSERT(false, "undefined stepDir");
                mbOnFocus = false;
                return false;
            }

            mbOnFocus = true;
            return true;
        }

        if (maxX < maxY)
        {
            if (maxX < maxZ)
            {
                currentPosition.mX += static_cast<int>(dx);
                maxX += deltaX;
                stepDir = 0;
            }
            else
            {
                currentPosition.mZ += static_cast<int>(dz);
                maxZ += deltaZ;
                stepDir = 2;
            }
        }
        else
        {
            if (maxY < maxZ)
            {
                currentPosition.mY += static_cast<int>(dy);
                maxY += deltaY;
                stepDir = 1;
            }
            else
            {
                currentPosition.mZ += static_cast<int>(dz);
                maxZ += deltaZ;
                stepDir = 2;
            }
        }
    }

    mbOnFocus = false;
    return false;
}

bool VoxelHandler::getVoxelInfo(VoxelInfo* outVoxelInfo, IntVector3D& voxelWorldPos)
{
    int cx = voxelWorldPos.mX / CHUNK_SIZE;
    int cy = voxelWorldPos.mY / CHUNK_SIZE;
    int cz = voxelWorldPos.mZ / CHUNK_SIZE;

    VoxelInfo voxelInfo;
    if (cx < 0 || cx >= WORLD_WIDTH || cy < 0 || cy >= WORLD_HEIGHT || cz < 0 || cz >= WORLD_DEPTH)
    {
        return false;
    }
    
    int chunkIndex = cx + cz * WORLD_WIDTH + cy * WORLD_AREA;

    outVoxelInfo->Chunk = mWorld->GetChunkPtr(chunkIndex);
    outVoxelInfo->VoxelLocalPos = voxelWorldPos - IntVector3D(cx, cy, cz) * CHUNK_SIZE;
    outVoxelInfo->VoxelIndex = outVoxelInfo->VoxelLocalPos.mX + outVoxelInfo->VoxelLocalPos.mZ * CHUNK_SIZE + outVoxelInfo->VoxelLocalPos.mY * CHUNK_AREA;
    outVoxelInfo->VoxelType = static_cast<eVoxelType>(outVoxelInfo->VoxelIndex);

    return true;
}

#include "Precompiled.h"

#include "ChunkBuilder.h"
#include <noise/noise.h>

#define VOXEL_INDEX(x, y, z) (x + z * CHUNK_SIZE + y * CHUNK_AREA)

void ChunkBuilder::BuildChunk(GraphicsResourceManager& GRM, World& world, Chunk* outChunk, const Vector3& pos)
{
    noise::module::Perlin perlinNoise;
    perlinNoise.SetSeed(15);

    noise::model::Plane planeNoise;
    planeNoise.SetModule(perlinNoise);

    int cx = static_cast<int>(pos.x * CHUNK_SIZE);
    int cy = static_cast<int>(pos.y * CHUNK_SIZE);
    int cz = static_cast<int>(pos.z * CHUNK_SIZE);

    outChunk->mVoxelTypes.resize(CHUNK_VOLUME);
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        int wx = x + cx;
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            int wz = z + cz;

            int worldHeight = static_cast<int>(planeNoise.GetValue(wx * 0.01, wz * 0.01) * 32 + 32);
            int localHeight = min(worldHeight - cy, CHUNK_SIZE);

            for (int y = 0; y < localHeight; ++y)
            {
                int wy = y + cy;
                outChunk->mVoxelTypes[VOXEL_INDEX(x, y, z)] = static_cast<eVoxelType>(wy + 1);
            }
        }
    }

    uint32_t indexOffset = 0;
    outChunk->mVoxels.reserve(CHUNK_VOLUME * MAX_NUM_VERTEX_PER_VOXEL);
    outChunk->mIndices.reserve(CHUNK_VOLUME * MAX_NUM_INDEX_PER_VOXEL);
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        int wx = x + cx;
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            int wz = z + cz;
            for (int y = 0; y < CHUNK_SIZE; ++y)
            {
                eVoxelType voxelType = outChunk->mVoxelTypes[VOXEL_INDEX(x, y, z)];
                if (voxelType == eVoxelType::EMPTY)
                {
                    continue;
                }

                int wy = y + cy;
                uint8_t topLeft, topRight, bottomRight, bottomLeft;

                if (isEmptyVoxel(world, IntVector3D(x, y + 1, z), IntVector3D(wx, wy + 1, wz))) // top
                {
                    getAmbientOcclusionFactor(world, IntVector3D(x, y + 1, z), IntVector3D(wx, wy + 1, wz), 
                        ePlane::Y, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z), voxelType, eFaceType::TOP, bottomLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z + 1), voxelType, eFaceType::TOP, topLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z + 1), voxelType, eFaceType::TOP, topRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z), voxelType, eFaceType::TOP, bottomRight });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(world, IntVector3D(x, y - 1, z), IntVector3D(wx, wy - 1, wz))) // bottom
                {
                    getAmbientOcclusionFactor(world, IntVector3D(x, y - 1, z), IntVector3D(wx, wy - 1, wz),
                        ePlane::Y, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z + 1), voxelType, eFaceType::BOTTOM, topLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z), voxelType, eFaceType::BOTTOM, bottomLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z), voxelType, eFaceType::BOTTOM, bottomRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z + 1), voxelType, eFaceType::BOTTOM, topRight });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(world, IntVector3D(x - 1, y, z), IntVector3D(wx - 1, wy, wz))) // left
                {
                    getAmbientOcclusionFactor(world, IntVector3D(x - 1, y, z), IntVector3D(wx - 1, wy, wz),
                        ePlane::X, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z + 1), voxelType, eFaceType::LEFT, bottomLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z + 1), voxelType, eFaceType::LEFT, topLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z), voxelType, eFaceType::LEFT, topRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z), voxelType, eFaceType::LEFT, bottomRight });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(world, IntVector3D(x + 1, y, z), IntVector3D(wx + 1, wy, wz))) // right
                {
                    getAmbientOcclusionFactor(world, IntVector3D(x + 1, y, z), IntVector3D(wx + 1, wy, wz),
                        ePlane::X, &topLeft, &topRight, &bottomRight, &bottomLeft);


                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z), voxelType, eFaceType::RIGHT, bottomRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z), voxelType, eFaceType::RIGHT, topRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z + 1), voxelType, eFaceType::RIGHT, topLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z + 1), voxelType, eFaceType::RIGHT, bottomLeft });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(world, IntVector3D(x, y, z - 1), IntVector3D(wx, wy, wz - 1))) // front
                {
                    getAmbientOcclusionFactor(world, IntVector3D(x, y, z - 1), IntVector3D(wx, wy, wz - 1),
                        ePlane::Z, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z), voxelType, eFaceType::FRONT, bottomLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z), voxelType, eFaceType::FRONT, topLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z), voxelType, eFaceType::FRONT, topRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z), voxelType, eFaceType::FRONT, bottomRight });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(world, IntVector3D(x, y, z + 1), IntVector3D(wx, wy, wz + 1))) // back
                {
                    getAmbientOcclusionFactor(world, IntVector3D(x, y, z + 1), IntVector3D(wx, wy, wz + 1),
                        ePlane::Z, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z + 1), voxelType, eFaceType::BACK, bottomRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z + 1), voxelType, eFaceType::BACK, topRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z + 1), voxelType, eFaceType::BACK, topLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z + 1), voxelType, eFaceType::BACK, bottomLeft });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }
            }
        }
    }
    
    if (!outChunk->mVoxels.empty())
    {
        D3D11Utils::CreateVertexBuffer(*GRM.mDevice, outChunk->mVoxels, &outChunk->mVB);
        D3D11Utils::CreateIndexBuffer(*GRM.mDevice, outChunk->mIndices, &outChunk->mIB);
    }
    outChunk->mIndexCount = UINT(outChunk->mIndices.size());
}

bool ChunkBuilder::isEmptyVoxel(World& world, const IntVector3D& localPos, const IntVector3D& worldPos)
{
    int chunkIndex = getChunkIndex(worldPos);
    if (chunkIndex == -1)
    {
        return false;
    }

    const Chunk& chunk = world.GetChunk(chunkIndex);
    
    int vx = (localPos.mX + CHUNK_SIZE) % CHUNK_SIZE;
    int vy = (localPos.mY + CHUNK_SIZE) % CHUNK_SIZE;
    int vz = (localPos.mZ + CHUNK_SIZE) % CHUNK_SIZE;

    int voxelIndex = vx + vz * CHUNK_SIZE + vy * CHUNK_AREA;

    if (chunk.mVoxelTypes.empty())
    {
        return false;
    }
    else
    {
        return chunk.mVoxelTypes[voxelIndex] == eVoxelType::EMPTY ? true : false;
    }
}

int ChunkBuilder::getChunkIndex(const IntVector3D& worldPos)
{
    if (worldPos.mX == -1 || worldPos.mY == -1 || worldPos.mZ == -1)
    {
        return -1;
    }

    int cx = worldPos.mX / CHUNK_SIZE;
    int cy = worldPos.mY / CHUNK_SIZE;
    int cz = worldPos.mZ / CHUNK_SIZE;

    if (cx >= WORLD_WIDTH || cy >= WORLD_HEIGHT || cz >= WORLD_DEPTH)
    {
        return -1;
    }

    int chunkIndex = cx + cz * WORLD_WIDTH + cy * WORLD_AREA;

    return chunkIndex;
}

void ChunkBuilder::addNewIndex(std::vector<uint32_t>& indices, uint32_t* outIndexOffset)
{
    indices.push_back(*outIndexOffset + 0);
    indices.push_back(*outIndexOffset + 1);
    indices.push_back(*outIndexOffset + 2);
    indices.push_back(*outIndexOffset + 0);
    indices.push_back(*outIndexOffset + 2);
    indices.push_back(*outIndexOffset + 3);
    *outIndexOffset += 4;
}

void ChunkBuilder::getAmbientOcclusionFactor(World& world, const IntVector3D& localPos, const IntVector3D& worldPos, ePlane plane, 
    uint8_t* outTopLeft, uint8_t* outTopRight, uint8_t* outBottomRight, uint8_t* outBottomLeft)
{
    int x = localPos.mX; int y = localPos.mY; int z = localPos.mZ;
    int wx = worldPos.mX; int wy = worldPos.mY; int wz = worldPos.mZ;

    int upLeft, up, upRight, right, rightDown, down, leftDown, left; // clockwise;
    switch (plane)
    {
    case ePlane::X:
        upLeft      = isEmptyVoxel(world, IntVector3D(x, y + 1, z + 1), IntVector3D(wx, wy + 1, wz + 1));
        up          = isEmptyVoxel(world, IntVector3D(x, y + 1, z), IntVector3D(wx, wy + 1, wz));
        upRight     = isEmptyVoxel(world, IntVector3D(x, y + 1, z - 1), IntVector3D(wx, wy + 1, wz - 1));
        right       = isEmptyVoxel(world, IntVector3D(x, y, z - 1), IntVector3D(wx, wy, wz - 1));
        rightDown   = isEmptyVoxel(world, IntVector3D(x, y - 1, z - 1), IntVector3D(wx, wy - 1, wz - 1));
        down        = isEmptyVoxel(world, IntVector3D(x, y - 1, z), IntVector3D(wx, wy - 1, wz));
        leftDown    = isEmptyVoxel(world, IntVector3D(x, y - 1, z + 1), IntVector3D(wx, wy - 1, wz + 1));
        left        = isEmptyVoxel(world, IntVector3D(x, y, z + 1), IntVector3D(wx, wy, wz + 1));
        break;
    case ePlane::Y:
        upLeft      = isEmptyVoxel(world, IntVector3D(x - 1, y, z + 1), IntVector3D(wx - 1, wy, wz + 1));
        up          = isEmptyVoxel(world, IntVector3D(x, y, z + 1), IntVector3D(wx, wy, wz + 1));
        upRight     = isEmptyVoxel(world, IntVector3D(x + 1, y, z + 1), IntVector3D(wx + 1, wy, wz + 1));
        right       = isEmptyVoxel(world, IntVector3D(x + 1, y, z), IntVector3D(wx + 1, wy, wz));
        rightDown   = isEmptyVoxel(world, IntVector3D(x + 1, y, z - 1), IntVector3D(wx + 1, wy, wz - 1));
        down        = isEmptyVoxel(world, IntVector3D(x, y, z - 1), IntVector3D(wx, wy, wz - 1));
        leftDown    = isEmptyVoxel(world, IntVector3D(x - 1, y, z - 1), IntVector3D(wx - 1, wy, wz - 1));
        left        = isEmptyVoxel(world, IntVector3D(x - 1, y, z), IntVector3D(wx - 1, wy, wz));
        break;
    case ePlane::Z:
        upLeft      = isEmptyVoxel(world, IntVector3D(x - 1, y + 1, z), IntVector3D(wx - 1, wy + 1, wz));
        up          = isEmptyVoxel(world, IntVector3D(x, y + 1, z), IntVector3D(wx, wy + 1, wz));
        upRight     = isEmptyVoxel(world, IntVector3D(x + 1, y + 1, z), IntVector3D(wx + 1, wy + 1, wz));
        right       = isEmptyVoxel(world, IntVector3D(x + 1, y, z), IntVector3D(wx + 1, wy, wz));
        rightDown   = isEmptyVoxel(world, IntVector3D(x + 1, y - 1, z), IntVector3D(wx + 1, wy - 1, wz));
        down        = isEmptyVoxel(world, IntVector3D(x, y - 1, z), IntVector3D(wx, wy - 1, wz));
        leftDown    = isEmptyVoxel(world, IntVector3D(x - 1, y - 1, z), IntVector3D(wx - 1, wy - 1, wz));
        left        = isEmptyVoxel(world, IntVector3D(x - 1, y, z), IntVector3D(wx - 1, wy, wz));
        break;
    default:
        ASSERT(false, "unidentified ePlane");
        return;
    }

    *outTopLeft = left + upLeft + up;
    *outTopRight = up + upRight + right;
    *outBottomRight = right + rightDown + down;
    *outBottomLeft = down + leftDown + left;
}

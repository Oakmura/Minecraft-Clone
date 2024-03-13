#include "Precompiled.h"

#include "ChunkBuilder.h"
#include <OpenSimplexNoise.h>

#define VOXEL_INDEX(x, y, z) ((x) + (z) * CHUNK_SIZE + (y) * CHUNK_AREA)

World* ChunkBuilder::mWorld = nullptr;

void ChunkBuilder::Init(World* world)
{
    mWorld = world;
}

void ChunkBuilder::BuildChunk(Chunk* outChunk, const IntVector3D& pos)
{
    IntVector3D chunkPos = pos * CHUNK_SIZE;
    int cx = chunkPos.mX;
    int cy = chunkPos.mY;
    int cz = chunkPos.mZ;

    outChunk->mVoxelTypes.resize(CHUNK_VOLUME);
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        int wx = x + cx;
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            int wz = z + cz;

            int worldHeight = generateHeight(wx, wz);
            int localHeight = min(worldHeight - cy, CHUNK_SIZE);

            for (int y = 0; y < localHeight; ++y)
            {
                int wy = y + cy;

                // outChunk->mVoxelTypes[VOXEL_INDEX(x, y, z)] = static_cast<eVoxelType>(chunkColor);
                generateVoxelType(*outChunk, { x, y, z }, { wx, wy, wz }, worldHeight);
            }
        }
    }
}

void ChunkBuilder::BuildChunkMesh(Chunk* outChunk, const IntVector3D& pos)
{
    IntVector3D chunkPos = pos * CHUNK_SIZE;
    int cx = chunkPos.mX;
    int cy = chunkPos.mY;
    int cz = chunkPos.mZ;

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
                if (voxelType == eVoxelType::Empty)
                {
                    continue;
                }

                int wy = y + cy;
                uint8_t topLeft, topRight, bottomRight, bottomLeft;

                if (isEmptyVoxel(IntVector3D(x, y + 1, z), IntVector3D(wx, wy + 1, wz))) // top
                {
                    getAmbientOcclusionFactor(IntVector3D(x, y + 1, z), IntVector3D(wx, wy + 1, wz),
                        ePlane::Y, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z), calculateTexcoord(voxelType, eFaceType::Top, eVertexType::BottomLeft), voxelType, eFaceType::Top, bottomLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z + 1), calculateTexcoord(voxelType, eFaceType::Top, eVertexType::TopLeft), voxelType, eFaceType::Top, topLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z + 1), calculateTexcoord(voxelType, eFaceType::Top, eVertexType::TopRight), voxelType, eFaceType::Top, topRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z), calculateTexcoord(voxelType, eFaceType::Top, eVertexType::BottomRight), voxelType, eFaceType::Top, bottomRight });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(IntVector3D(x, y - 1, z), IntVector3D(wx, wy - 1, wz))) // bottom
                {
                    getAmbientOcclusionFactor(IntVector3D(x, y - 1, z), IntVector3D(wx, wy - 1, wz),
                        ePlane::Y, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z + 1), calculateTexcoord(voxelType, eFaceType::Bottom, eVertexType::TopLeft), voxelType, eFaceType::Bottom, topLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z), calculateTexcoord(voxelType, eFaceType::Bottom, eVertexType::BottomLeft), voxelType, eFaceType::Bottom, bottomLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z), calculateTexcoord(voxelType, eFaceType::Bottom, eVertexType::BottomRight), voxelType, eFaceType::Bottom, bottomRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z + 1), calculateTexcoord(voxelType, eFaceType::Bottom, eVertexType::TopRight), voxelType, eFaceType::Bottom, topRight });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(IntVector3D(x - 1, y, z), IntVector3D(wx - 1, wy, wz))) // left
                {
                    getAmbientOcclusionFactor(IntVector3D(x - 1, y, z), IntVector3D(wx - 1, wy, wz),
                        ePlane::X, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z + 1), calculateTexcoord(voxelType, eFaceType::Left, eVertexType::BottomLeft), voxelType, eFaceType::Left, bottomLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z + 1), calculateTexcoord(voxelType, eFaceType::Left, eVertexType::TopLeft), voxelType, eFaceType::Left, topLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z), calculateTexcoord(voxelType, eFaceType::Left, eVertexType::TopRight), voxelType, eFaceType::Left, topRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z), calculateTexcoord(voxelType, eFaceType::Left, eVertexType::BottomRight), voxelType, eFaceType::Left, bottomRight });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(IntVector3D(x + 1, y, z), IntVector3D(wx + 1, wy, wz))) // right
                {
                    getAmbientOcclusionFactor(IntVector3D(x + 1, y, z), IntVector3D(wx + 1, wy, wz),
                        ePlane::X, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z), calculateTexcoord(voxelType, eFaceType::Right, eVertexType::BottomRight), voxelType, eFaceType::Right, bottomRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z), calculateTexcoord(voxelType, eFaceType::Right, eVertexType::TopRight), voxelType, eFaceType::Right, topRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z + 1), calculateTexcoord(voxelType, eFaceType::Right, eVertexType::TopLeft), voxelType, eFaceType::Right, topLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z + 1), calculateTexcoord(voxelType, eFaceType::Right, eVertexType::BottomLeft), voxelType, eFaceType::Right, bottomLeft });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(IntVector3D(x, y, z - 1), IntVector3D(wx, wy, wz - 1))) // front
                {
                    getAmbientOcclusionFactor(IntVector3D(x, y, z - 1), IntVector3D(wx, wy, wz - 1),
                        ePlane::Z, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z), calculateTexcoord(voxelType, eFaceType::Front, eVertexType::BottomLeft), voxelType, eFaceType::Front, bottomLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z), calculateTexcoord(voxelType, eFaceType::Front, eVertexType::TopLeft), voxelType, eFaceType::Front, topLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z), calculateTexcoord(voxelType, eFaceType::Front, eVertexType::TopRight), voxelType, eFaceType::Front, topRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z), calculateTexcoord(voxelType, eFaceType::Front, eVertexType::BottomRight), voxelType, eFaceType::Front, bottomRight });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(IntVector3D(x, y, z + 1), IntVector3D(wx, wy, wz + 1))) // back
                {
                    getAmbientOcclusionFactor(IntVector3D(x, y, z + 1), IntVector3D(wx, wy, wz + 1),
                        ePlane::Z, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z + 1), calculateTexcoord(voxelType, eFaceType::Back, eVertexType::BottomRight), voxelType, eFaceType::Back, bottomRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z + 1), calculateTexcoord(voxelType, eFaceType::Back, eVertexType::TopRight), voxelType, eFaceType::Back, topRight });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z + 1), calculateTexcoord(voxelType, eFaceType::Back, eVertexType::TopLeft), voxelType, eFaceType::Back, topLeft });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z + 1), calculateTexcoord(voxelType, eFaceType::Back, eVertexType::BottomLeft), voxelType, eFaceType::Back, bottomLeft });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }
            }
        }
    }

    if (!outChunk->mVoxels.empty())
    {
        GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

        D3D11Utils::CreateVertexBuffer(GRM.GetDevice(), outChunk->mVoxels, &outChunk->mVB);
        D3D11Utils::CreateIndexBuffer(GRM.GetDevice(), outChunk->mIndices, &outChunk->mIB);

        outChunk->mIndexCount = UINT(outChunk->mIndices.size());
    }
}

bool ChunkBuilder::isEmptyVoxel(const IntVector3D& localPos, const IntVector3D& worldPos)
{
    ASSERT(localPos.mX >= -1 && localPos.mX <= CHUNK_SIZE && localPos.mY >= -1 && localPos.mY <= CHUNK_SIZE && localPos.mZ >= -1 && localPos.mZ <= CHUNK_SIZE, "unexpected local pos");

    int chunkIndex = getChunkIndex(worldPos);
    if (chunkIndex == -1)
    {
        return false;
    }

    const Chunk& chunk = mWorld->GetChunk(chunkIndex);
    
    int vx = (localPos.mX + CHUNK_SIZE) % CHUNK_SIZE;
    int vy = (localPos.mY + CHUNK_SIZE) % CHUNK_SIZE;
    int vz = (localPos.mZ + CHUNK_SIZE) % CHUNK_SIZE;

    int voxelIndex = VOXEL_INDEX(vx, vy, vz);

    return chunk.mVoxelTypes[voxelIndex] == eVoxelType::Empty ? true : false;
}

int ChunkBuilder::getChunkIndex(const IntVector3D& worldPos)
{
    ASSERT(worldPos.mX >= -1 && worldPos.mY >= -1 && worldPos.mZ >= -1, "unexpected world pos");

    if (worldPos.mX == -1 || worldPos.mY == -1 || worldPos.mZ == -1) // (-1/32 == 0 in cpp BUT -1/32 == -1 in python)
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

void ChunkBuilder::getAmbientOcclusionFactor(const IntVector3D& localPos, const IntVector3D& worldPos, ePlane plane, 
    uint8_t* outTopLeft, uint8_t* outTopRight, uint8_t* outBottomRight, uint8_t* outBottomLeft)
{
    int x = localPos.mX; int y = localPos.mY; int z = localPos.mZ;
    int wx = worldPos.mX; int wy = worldPos.mY; int wz = worldPos.mZ;

    int upLeft, up, upRight, right, rightDown, down, leftDown, left; // clockwise;
    switch (plane)
    {
    case ePlane::X:
        upLeft      = isEmptyVoxel(IntVector3D(x, y + 1, z + 1), IntVector3D(wx, wy + 1, wz + 1));
        up          = isEmptyVoxel(IntVector3D(x, y + 1, z), IntVector3D(wx, wy + 1, wz));
        upRight     = isEmptyVoxel(IntVector3D(x, y + 1, z - 1), IntVector3D(wx, wy + 1, wz - 1));
        right       = isEmptyVoxel(IntVector3D(x, y, z - 1), IntVector3D(wx, wy, wz - 1));
        rightDown   = isEmptyVoxel(IntVector3D(x, y - 1, z - 1), IntVector3D(wx, wy - 1, wz - 1));
        down        = isEmptyVoxel(IntVector3D(x, y - 1, z), IntVector3D(wx, wy - 1, wz));
        leftDown    = isEmptyVoxel(IntVector3D(x, y - 1, z + 1), IntVector3D(wx, wy - 1, wz + 1));
        left        = isEmptyVoxel(IntVector3D(x, y, z + 1), IntVector3D(wx, wy, wz + 1));
        break;
    case ePlane::Y:
        upLeft      = isEmptyVoxel(IntVector3D(x - 1, y, z + 1), IntVector3D(wx - 1, wy, wz + 1));
        up          = isEmptyVoxel(IntVector3D(x, y, z + 1), IntVector3D(wx, wy, wz + 1));
        upRight     = isEmptyVoxel(IntVector3D(x + 1, y, z + 1), IntVector3D(wx + 1, wy, wz + 1));
        right       = isEmptyVoxel(IntVector3D(x + 1, y, z), IntVector3D(wx + 1, wy, wz));
        rightDown   = isEmptyVoxel(IntVector3D(x + 1, y, z - 1), IntVector3D(wx + 1, wy, wz - 1));
        down        = isEmptyVoxel(IntVector3D(x, y, z - 1), IntVector3D(wx, wy, wz - 1));
        leftDown    = isEmptyVoxel(IntVector3D(x - 1, y, z - 1), IntVector3D(wx - 1, wy, wz - 1));
        left        = isEmptyVoxel(IntVector3D(x - 1, y, z), IntVector3D(wx - 1, wy, wz));
        break;
    case ePlane::Z:
        upLeft      = isEmptyVoxel(IntVector3D(x - 1, y + 1, z), IntVector3D(wx - 1, wy + 1, wz));
        up          = isEmptyVoxel(IntVector3D(x, y + 1, z), IntVector3D(wx, wy + 1, wz));
        upRight     = isEmptyVoxel(IntVector3D(x + 1, y + 1, z), IntVector3D(wx + 1, wy + 1, wz));
        right       = isEmptyVoxel(IntVector3D(x + 1, y, z), IntVector3D(wx + 1, wy, wz));
        rightDown   = isEmptyVoxel(IntVector3D(x + 1, y - 1, z), IntVector3D(wx + 1, wy - 1, wz));
        down        = isEmptyVoxel(IntVector3D(x, y - 1, z), IntVector3D(wx, wy - 1, wz));
        leftDown    = isEmptyVoxel(IntVector3D(x - 1, y - 1, z), IntVector3D(wx - 1, wy - 1, wz));
        left        = isEmptyVoxel(IntVector3D(x - 1, y, z), IntVector3D(wx - 1, wy, wz));
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

DirectX::SimpleMath::Vector2 ChunkBuilder::calculateTexcoord(eVoxelType voxelType, eFaceType faceType, eVertexType vertexType)
{
    static float xTexSize = 1 / 3.0f;
    static float yTexSize = 1 / 8.0f;

    SimpleMath::Vector2 texcoord = { xTexSize, yTexSize };
    if (faceType == eFaceType::Top)
    {
        texcoord.x *= 2.0f;
    }
    else if (faceType == eFaceType::Bottom)
    {
        texcoord.x *= 0.0f;
    }
    else
    {
        texcoord.x *= 1.0f;
    }
    texcoord.y *= (int)voxelType;

    switch (vertexType)
    {
    case eVertexType::TopLeft:
        texcoord.x += xTexSize;
        break;
    case eVertexType::TopRight:
        break;
    case eVertexType::BottomLeft:
        texcoord.x += xTexSize;
        texcoord.y += yTexSize;
        break;
    case eVertexType::BottomRight:
        texcoord.y += yTexSize;
        break;
    default:
        ASSERT("unidentified vertex type {0}", (int)vertexType);
        break;
    }

    ASSERT(texcoord.x >= 0.0f && texcoord.x <= 1.0f && texcoord.y >= 0.0f && texcoord.y <= 1.0f, "unexpected texcoord calculated");

    return texcoord;
}

int ChunkBuilder::generateHeight(int x, int z)
{
    static OpenSimplexNoise::Noise simplexNoise(27);

    // island mask
    float island = static_cast<float>(1 / (std::pow(0.0025 * std::hypot(x - WORLD_CENTER_XZ, z - WORLD_CENTER_XZ), 20) + 0.0001));
    island = std::fmin(island, 1.0f);
    
    // amplitude
    float a1 = static_cast<float>(WORLD_CENTER_Y);
    float a2 = a1 * 0.5f;
    float a4 = a1 * 0.25f;
    float a8 = a1 * 0.125f;

    // frequency
    float f1 = 0.005f;
    float f2 = f1 * 2.0f;
    float f4 = f1 * 4.0f;
    float f8 = f1 * 8.0f;

    if (simplexNoise.eval(0.1 * x, 0.1 * z) < 0.0)
    {
        a1 /= 1.07f;
    }

    float height = 0.0f;
    height += static_cast<float>(simplexNoise.eval(x * f1, z * f1) * a1 + a1);
    height += static_cast<float>(simplexNoise.eval(x * f2, z * f2) * a2 - a2);
    height += static_cast<float>(simplexNoise.eval(x * f4, z * f4) * a4 + a4);
    height += static_cast<float>(simplexNoise.eval(x * f8, z * f8) * a8 - a8);

    height = std::fmax(height, 1.0f);
    height *= island;

    return static_cast<int>(height);
}

void ChunkBuilder::generateVoxelType(Chunk& chunk, const IntVector3D& localPos, const IntVector3D& worldPos, int worldHeight)
{
    static OpenSimplexNoise::Noise simplexNoise(27);

    static std::random_device rdev;
    static std::mt19937 rgen(rdev());
    static std::uniform_int_distribution<int> idist(0, 6);

    eVoxelType voxelType = eVoxelType::Empty;
    if (worldPos.mY < worldHeight - 1)
    {
        // cave system
        double noise3D = simplexNoise.eval(worldPos.mX * 0.09, worldPos.mY * 0.09, worldPos.mZ * 0.09);
        double noise2D = simplexNoise.eval(worldPos.mX * 0.1, worldPos.mZ * 0.1) * 3.0 + 3.0;
        if (noise3D > 0 && noise2D < (double)worldPos.mY && worldPos.mY < worldHeight - 10)
        {
            voxelType = eVoxelType::Empty;
        }
        else
        {
            voxelType = eVoxelType::Stone;
        }
    }
    else
    {
        int rY = worldPos.mY - idist(rgen);

        if (rY >= eTerrainLevel::Snow && rY < worldHeight)
        {
            voxelType = eVoxelType::Snow;
        }
        else if (rY >= eTerrainLevel::Stone)
        {
            voxelType = eVoxelType::Stone;
        }
        else if (rY >= eTerrainLevel::Dirt)
        {
            voxelType = eVoxelType::Dirt;
        }
        else if (rY >= eTerrainLevel::Grass)
        {
            voxelType = eVoxelType::Grass;
        }
        else
        {
            voxelType = eVoxelType::Sand;
        }
    }

    chunk.mVoxelTypes[VOXEL_INDEX(localPos.mX, localPos.mY, localPos.mZ)] = voxelType;
    
    if (worldPos.mY < eTerrainLevel::Dirt)
    {
        placeTree(chunk, localPos, voxelType);
    }
}

void ChunkBuilder::placeTree(Chunk& chunk, const IntVector3D& localPos, eVoxelType voxelType)
{
    static std::random_device rdev;
    static std::mt19937 rgen(rdev());
    static std::uniform_real_distribution<> idist(0.0, 1.0);

    float random = idist(rgen);
    ASSERT(random < 1.0, "unexpected random number generated");

    if (voxelType != eVoxelType::Grass || random > sTreeProbability) // tree should be on top of grass
    {
        return;
    }
    else if (localPos.mY + TREE_HEIGHT >= CHUNK_SIZE) // checks whether there is enough area to place tree
    {
        return;
    }
    else if (localPos.mX - TREE_HALF_WIDTH < 0 || localPos.mX + TREE_HALF_WIDTH >= CHUNK_SIZE) 
    {
        return;
    }
    else if (localPos.mZ - TREE_HALF_WIDTH < 0 || localPos.mZ + TREE_HALF_WIDTH >= CHUNK_SIZE)
    {
        return;
    }

    chunk.mVoxelTypes[VOXEL_INDEX(localPos.mX, localPos.mY, localPos.mZ)] = eVoxelType::Dirt; // place dirt on root of tree

    // leaves
    int m = 0;
    for (int i = 0, iy = TREE_HALF_HEIGHT; iy < TREE_HEIGHT - 1; ++i, ++iy)
    {
        int k = iy % 2;
        float rng = static_cast<int>(idist(rgen) * 2);
        ASSERT(rng < 2.0, "unexpected random number generated");

        for (int ix = -TREE_HALF_WIDTH + m; ix < TREE_HALF_WIDTH - m * rng; ++ix)
        {   
            for (int iz = -TREE_HALF_WIDTH + m * rng; iz < TREE_HALF_WIDTH - m; ++iz)
            {
                if ((ix + iz) % 4)
                {
                    chunk.mVoxelTypes[VOXEL_INDEX(localPos.mX + ix + k, localPos.mY + iy, localPos.mZ + iz + k)] = eVoxelType::Leaves;
                }
            }

            if (i > 0)
            {
                m += 1;
            }
            else if (i > 1)
            {
                m += 3;
            }
        }
    }

    // tree trunk
    for (int iy = 1; iy < TREE_HEIGHT - 2; ++iy)
    {
        chunk.mVoxelTypes[VOXEL_INDEX(localPos.mX, localPos.mY + iy, localPos.mZ)] = eVoxelType::Wood;
    }

    // top
    chunk.mVoxelTypes[VOXEL_INDEX(localPos.mX, localPos.mY + TREE_HEIGHT - 2, localPos.mZ)] = eVoxelType::Leaves;
}

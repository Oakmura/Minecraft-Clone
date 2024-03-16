#include "Precompiled.h"

#include "ChunkBuilder.h"
#include <OpenSimplexNoise.h>

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

    outChunk->mBlockTypes.resize(CHUNK_VOLUME);
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

                generateBlockType(*outChunk, { x, y, z }, { wx, wy, wz }, worldHeight);
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

    outChunk->mBlocks.reserve(CHUNK_VOLUME * MAX_NUM_VERTEX_PER_BLOCK);
    outChunk->mIndices.reserve(CHUNK_VOLUME * MAX_NUM_INDEX_PER_BLOCK);
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        int wx = x + cx;
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            int wz = z + cz;
            for (int y = 0; y < CHUNK_SIZE; ++y)
            {
                eBlockType blockType = outChunk->mBlockTypes[ChunkUtils::GetBlockIndex({ x, y, z })];
                if (blockType == eBlockType::Empty)
                {
                    continue;
                }

                int wy = y + cy;
                uint8_t topLeft, topRight, bottomRight, bottomLeft;

                if (isEmptyBlock(IntVector3D(x, y + 1, z), IntVector3D(wx, wy + 1, wz))) // top
                {
                    getAmbientOcclusionFactor(IntVector3D(x, y + 1, z), IntVector3D(wx, wy + 1, wz),
                        ePlane::Y, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mBlocks.push_back({ IntVector3D(x, y + 1, z), calculateTexcoord(blockType, eFaceType::Top, eVertexType::BottomLeft), blockType, eFaceType::Top, bottomLeft });
                    outChunk->mBlocks.push_back({ IntVector3D(x, y + 1, z + 1), calculateTexcoord(blockType, eFaceType::Top, eVertexType::TopLeft), blockType, eFaceType::Top, topLeft });
                    outChunk->mBlocks.push_back({ IntVector3D(x + 1, y + 1, z + 1), calculateTexcoord(blockType, eFaceType::Top, eVertexType::TopRight), blockType, eFaceType::Top, topRight });
                    outChunk->mBlocks.push_back({ IntVector3D(x + 1, y + 1, z), calculateTexcoord(blockType, eFaceType::Top, eVertexType::BottomRight), blockType, eFaceType::Top, bottomRight });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyBlock(IntVector3D(x, y - 1, z), IntVector3D(wx, wy - 1, wz))) // bottom
                {
                    getAmbientOcclusionFactor(IntVector3D(x, y - 1, z), IntVector3D(wx, wy - 1, wz),
                        ePlane::Y, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mBlocks.push_back({ IntVector3D(x, y, z + 1), calculateTexcoord(blockType, eFaceType::Bottom, eVertexType::TopLeft), blockType, eFaceType::Bottom, topLeft });
                    outChunk->mBlocks.push_back({ IntVector3D(x, y, z), calculateTexcoord(blockType, eFaceType::Bottom, eVertexType::BottomLeft), blockType, eFaceType::Bottom, bottomLeft });
                    outChunk->mBlocks.push_back({ IntVector3D(x + 1, y, z), calculateTexcoord(blockType, eFaceType::Bottom, eVertexType::BottomRight), blockType, eFaceType::Bottom, bottomRight });
                    outChunk->mBlocks.push_back({ IntVector3D(x + 1, y, z + 1), calculateTexcoord(blockType, eFaceType::Bottom, eVertexType::TopRight), blockType, eFaceType::Bottom, topRight });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyBlock(IntVector3D(x - 1, y, z), IntVector3D(wx - 1, wy, wz))) // left
                {
                    getAmbientOcclusionFactor(IntVector3D(x - 1, y, z), IntVector3D(wx - 1, wy, wz),
                        ePlane::X, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mBlocks.push_back({ IntVector3D(x, y, z + 1), calculateTexcoord(blockType, eFaceType::Left, eVertexType::BottomLeft), blockType, eFaceType::Left, bottomLeft });
                    outChunk->mBlocks.push_back({ IntVector3D(x, y + 1, z + 1), calculateTexcoord(blockType, eFaceType::Left, eVertexType::TopLeft), blockType, eFaceType::Left, topLeft });
                    outChunk->mBlocks.push_back({ IntVector3D(x, y + 1, z), calculateTexcoord(blockType, eFaceType::Left, eVertexType::TopRight), blockType, eFaceType::Left, topRight });
                    outChunk->mBlocks.push_back({ IntVector3D(x, y, z), calculateTexcoord(blockType, eFaceType::Left, eVertexType::BottomRight), blockType, eFaceType::Left, bottomRight });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyBlock(IntVector3D(x + 1, y, z), IntVector3D(wx + 1, wy, wz))) // right
                {
                    getAmbientOcclusionFactor(IntVector3D(x + 1, y, z), IntVector3D(wx + 1, wy, wz),
                        ePlane::X, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mBlocks.push_back({ IntVector3D(x + 1, y, z), calculateTexcoord(blockType, eFaceType::Right, eVertexType::BottomRight), blockType, eFaceType::Right, bottomRight });
                    outChunk->mBlocks.push_back({ IntVector3D(x + 1, y + 1, z), calculateTexcoord(blockType, eFaceType::Right, eVertexType::TopRight), blockType, eFaceType::Right, topRight });
                    outChunk->mBlocks.push_back({ IntVector3D(x + 1, y + 1, z + 1), calculateTexcoord(blockType, eFaceType::Right, eVertexType::TopLeft), blockType, eFaceType::Right, topLeft });
                    outChunk->mBlocks.push_back({ IntVector3D(x + 1, y, z + 1), calculateTexcoord(blockType, eFaceType::Right, eVertexType::BottomLeft), blockType, eFaceType::Right, bottomLeft });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyBlock(IntVector3D(x, y, z - 1), IntVector3D(wx, wy, wz - 1))) // front
                {
                    getAmbientOcclusionFactor(IntVector3D(x, y, z - 1), IntVector3D(wx, wy, wz - 1),
                        ePlane::Z, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mBlocks.push_back({ IntVector3D(x, y, z), calculateTexcoord(blockType, eFaceType::Front, eVertexType::BottomLeft), blockType, eFaceType::Front, bottomLeft });
                    outChunk->mBlocks.push_back({ IntVector3D(x, y + 1, z), calculateTexcoord(blockType, eFaceType::Front, eVertexType::TopLeft), blockType, eFaceType::Front, topLeft });
                    outChunk->mBlocks.push_back({ IntVector3D(x + 1, y + 1, z), calculateTexcoord(blockType, eFaceType::Front, eVertexType::TopRight), blockType, eFaceType::Front, topRight });
                    outChunk->mBlocks.push_back({ IntVector3D(x + 1, y, z), calculateTexcoord(blockType, eFaceType::Front, eVertexType::BottomRight), blockType, eFaceType::Front, bottomRight });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyBlock(IntVector3D(x, y, z + 1), IntVector3D(wx, wy, wz + 1))) // back
                {
                    getAmbientOcclusionFactor(IntVector3D(x, y, z + 1), IntVector3D(wx, wy, wz + 1),
                        ePlane::Z, &topLeft, &topRight, &bottomRight, &bottomLeft);

                    outChunk->mBlocks.push_back({ IntVector3D(x + 1, y, z + 1), calculateTexcoord(blockType, eFaceType::Back, eVertexType::BottomRight), blockType, eFaceType::Back, bottomRight });
                    outChunk->mBlocks.push_back({ IntVector3D(x + 1, y + 1, z + 1), calculateTexcoord(blockType, eFaceType::Back, eVertexType::TopRight), blockType, eFaceType::Back, topRight });
                    outChunk->mBlocks.push_back({ IntVector3D(x, y + 1, z + 1), calculateTexcoord(blockType, eFaceType::Back, eVertexType::TopLeft), blockType, eFaceType::Back, topLeft });
                    outChunk->mBlocks.push_back({ IntVector3D(x, y, z + 1), calculateTexcoord(blockType, eFaceType::Back, eVertexType::BottomLeft), blockType, eFaceType::Back, bottomLeft });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }
            }
        }
    }

    if (!outChunk->mBlocks.empty())
    {
        GraphicsEngine& GRM = GraphicsEngine::GetInstance();

        D3D11Utils::CreateVertexBuffer(GRM.GetDevice(), outChunk->mBlocks, &outChunk->mVB);
        D3D11Utils::CreateIndexBuffer(GRM.GetDevice(), outChunk->mIndices, &outChunk->mIB);

        outChunk->mIndexCount = UINT(outChunk->mIndices.size());
    }
}

bool ChunkBuilder::isEmptyBlock(const IntVector3D& localPos, const IntVector3D& worldPos)
{
    ASSERT(localPos.mX >= -1 && localPos.mX <= CHUNK_SIZE && localPos.mY >= -1 && localPos.mY <= CHUNK_SIZE && localPos.mZ >= -1 && localPos.mZ <= CHUNK_SIZE, "unexpected local pos");

    int chunkIndex = ChunkUtils::GetChunkIndexWorld(worldPos);
    if (chunkIndex == -1)
    {
        return false;
    }

    const Chunk& chunk = mWorld->GetChunk(chunkIndex);
    
    int vx = (localPos.mX + CHUNK_SIZE) % CHUNK_SIZE;
    int vy = (localPos.mY + CHUNK_SIZE) % CHUNK_SIZE;
    int vz = (localPos.mZ + CHUNK_SIZE) % CHUNK_SIZE;

    int blockIndex = ChunkUtils::GetBlockIndex({ vx, vy, vz });

    return chunk.mBlockTypes[blockIndex] == eBlockType::Empty ? true : false;
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
        upLeft      = isEmptyBlock(IntVector3D(x, y + 1, z + 1), IntVector3D(wx, wy + 1, wz + 1));
        up          = isEmptyBlock(IntVector3D(x, y + 1, z), IntVector3D(wx, wy + 1, wz));
        upRight     = isEmptyBlock(IntVector3D(x, y + 1, z - 1), IntVector3D(wx, wy + 1, wz - 1));
        right       = isEmptyBlock(IntVector3D(x, y, z - 1), IntVector3D(wx, wy, wz - 1));
        rightDown   = isEmptyBlock(IntVector3D(x, y - 1, z - 1), IntVector3D(wx, wy - 1, wz - 1));
        down        = isEmptyBlock(IntVector3D(x, y - 1, z), IntVector3D(wx, wy - 1, wz));
        leftDown    = isEmptyBlock(IntVector3D(x, y - 1, z + 1), IntVector3D(wx, wy - 1, wz + 1));
        left        = isEmptyBlock(IntVector3D(x, y, z + 1), IntVector3D(wx, wy, wz + 1));
        break;
    case ePlane::Y:
        upLeft      = isEmptyBlock(IntVector3D(x - 1, y, z + 1), IntVector3D(wx - 1, wy, wz + 1));
        up          = isEmptyBlock(IntVector3D(x, y, z + 1), IntVector3D(wx, wy, wz + 1));
        upRight     = isEmptyBlock(IntVector3D(x + 1, y, z + 1), IntVector3D(wx + 1, wy, wz + 1));
        right       = isEmptyBlock(IntVector3D(x + 1, y, z), IntVector3D(wx + 1, wy, wz));
        rightDown   = isEmptyBlock(IntVector3D(x + 1, y, z - 1), IntVector3D(wx + 1, wy, wz - 1));
        down        = isEmptyBlock(IntVector3D(x, y, z - 1), IntVector3D(wx, wy, wz - 1));
        leftDown    = isEmptyBlock(IntVector3D(x - 1, y, z - 1), IntVector3D(wx - 1, wy, wz - 1));
        left        = isEmptyBlock(IntVector3D(x - 1, y, z), IntVector3D(wx - 1, wy, wz));
        break;
    case ePlane::Z:
        upLeft      = isEmptyBlock(IntVector3D(x - 1, y + 1, z), IntVector3D(wx - 1, wy + 1, wz));
        up          = isEmptyBlock(IntVector3D(x, y + 1, z), IntVector3D(wx, wy + 1, wz));
        upRight     = isEmptyBlock(IntVector3D(x + 1, y + 1, z), IntVector3D(wx + 1, wy + 1, wz));
        right       = isEmptyBlock(IntVector3D(x + 1, y, z), IntVector3D(wx + 1, wy, wz));
        rightDown   = isEmptyBlock(IntVector3D(x + 1, y - 1, z), IntVector3D(wx + 1, wy - 1, wz));
        down        = isEmptyBlock(IntVector3D(x, y - 1, z), IntVector3D(wx, wy - 1, wz));
        leftDown    = isEmptyBlock(IntVector3D(x - 1, y - 1, z), IntVector3D(wx - 1, wy - 1, wz));
        left        = isEmptyBlock(IntVector3D(x - 1, y, z), IntVector3D(wx - 1, wy, wz));
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

DirectX::SimpleMath::Vector2 ChunkBuilder::calculateTexcoord(eBlockType blockType, eFaceType faceType, eVertexType vertexType)
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
    texcoord.y *= (int)blockType;

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

void ChunkBuilder::generateBlockType(Chunk& chunk, const IntVector3D& localPos, const IntVector3D& worldPos, int worldHeight)
{
    static OpenSimplexNoise::Noise simplexNoise(27);

    static std::random_device rdev;
    static std::mt19937 rgen(rdev());
    static std::uniform_int_distribution<int> idist(0, 6);

    eBlockType blockType = eBlockType::Empty;
    if (worldPos.mY < worldHeight - 1)
    {
        // cave system
        double noise3D = simplexNoise.eval(worldPos.mX * 0.09, worldPos.mY * 0.09, worldPos.mZ * 0.09);
        double noise2D = simplexNoise.eval(worldPos.mX * 0.1, worldPos.mZ * 0.1) * 3.0 + 3.0;
        if (noise3D > 0 && noise2D < (double)worldPos.mY && worldPos.mY < worldHeight - 10)
        {
            blockType = eBlockType::Empty;
        }
        else
        {
            blockType = eBlockType::Stone;
        }
    }
    else
    {
        int rY = worldPos.mY - idist(rgen);

        if (rY >= eTerrainLevel::Snow && rY < worldHeight)
        {
            blockType = eBlockType::Snow;
        }
        else if (rY >= eTerrainLevel::Stone)
        {
            blockType = eBlockType::Stone;
        }
        else if (rY >= eTerrainLevel::Dirt)
        {
            blockType = eBlockType::Dirt;
        }
        else if (rY >= eTerrainLevel::Grass)
        {
            blockType = eBlockType::Grass;
        }
        else
        {
            blockType = eBlockType::Sand;
        }
    }

    chunk.mBlockTypes[ChunkUtils::GetBlockIndex({ localPos.mX, localPos.mY, localPos.mZ })] = blockType;
    
    if (worldPos.mY < eTerrainLevel::Dirt)
    {
        placeTree(chunk, localPos, blockType);
    }
}

void ChunkBuilder::placeTree(Chunk& chunk, const IntVector3D& localPos, eBlockType blockType)
{
    static std::random_device rdev;
    static std::mt19937 rgen(rdev());
    static std::uniform_real_distribution<> idist(0.0, 1.0);

    float random = idist(rgen);
    ASSERT(random < 1.0, "unexpected random number generated");

    if (blockType != eBlockType::Grass || random > sTreeProbability) // tree should be on top of grass
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

    chunk.mBlockTypes[ChunkUtils::GetBlockIndex({ localPos.mX, localPos.mY, localPos.mZ })] = eBlockType::Dirt; // place dirt on root of tree

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
                    chunk.mBlockTypes[ChunkUtils::GetBlockIndex({ localPos.mX + ix + k, localPos.mY + iy, localPos.mZ + iz + k })] = eBlockType::Leaves;
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
        chunk.mBlockTypes[ChunkUtils::GetBlockIndex({ localPos.mX, localPos.mY + iy, localPos.mZ })] = eBlockType::Wood;
    }

    // top
    chunk.mBlockTypes[ChunkUtils::GetBlockIndex({localPos.mX, localPos.mY + TREE_HEIGHT - 2, localPos.mZ})] = eBlockType::Leaves;
}

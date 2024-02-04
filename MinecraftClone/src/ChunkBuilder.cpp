#include "Precompiled.h"

#include "ChunkBuilder.h"
#include <noise/noise.h>

#define VOXEL_INDEX(x, y, z) (x + z * CHUNK_SIZE + y * CHUNK_AREA)

void ChunkBuilder::BuildChunk(GraphicsResourceManager& GRM, Chunk* outChunk)
{
    noise::module::Perlin perlinNoise;
    perlinNoise.SetSeed(30);

    outChunk->mVoxelTypes.reserve(CHUNK_VOLUME);
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            for (int y = 0; y < CHUNK_SIZE; ++y)
            {
                double noise = perlinNoise.GetValue(x * 0.1, y * 0.1, z * 0.1) + 1;
                int(noise) == 0 ? outChunk->mVoxelTypes.push_back(static_cast<eVoxelType>(0)) : outChunk->mVoxelTypes.push_back(static_cast<eVoxelType>(x + y + z));
            }
        }
    }

    uint32_t indexOffset = 0;
    outChunk->mVoxels.reserve(CHUNK_VOLUME * MAX_NUM_VERTEX_PER_VOXEL);
    outChunk->mIndices.reserve(CHUNK_VOLUME * MAX_NUM_INDEX_PER_VOXEL);
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            for (int y = 0; y < CHUNK_SIZE; ++y)
            {
                eVoxelType voxelType = outChunk->mVoxelTypes[VOXEL_INDEX(x, y, z)];
                if (voxelType == eVoxelType::EMPTY)
                {
                    continue;
                }

                if (isEmptyVoxel(outChunk->mVoxelTypes, x, y + 1, z))
                {
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z), voxelType, eFaceType::TOP });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z + 1), voxelType, eFaceType::TOP });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z + 1), voxelType, eFaceType::TOP });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z), voxelType, eFaceType::TOP });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(outChunk->mVoxelTypes, x, y - 1, z))
                {
                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z + 1), voxelType, eFaceType::BOTTOM });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z), voxelType, eFaceType::BOTTOM });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z), voxelType, eFaceType::BOTTOM });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z + 1), voxelType, eFaceType::BOTTOM });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(outChunk->mVoxelTypes, x - 1, y, z))
                {
                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z + 1), voxelType, eFaceType::LEFT });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z + 1), voxelType, eFaceType::LEFT });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z), voxelType, eFaceType::LEFT });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z), voxelType, eFaceType::LEFT });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(outChunk->mVoxelTypes, x + 1, y, z))
                {
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z), voxelType, eFaceType::RIGHT });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z), voxelType, eFaceType::RIGHT });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z + 1), voxelType, eFaceType::RIGHT });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z + 1), voxelType, eFaceType::RIGHT });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(outChunk->mVoxelTypes, x, y, z - 1))
                {
                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z), voxelType, eFaceType::FRONT });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z), voxelType, eFaceType::FRONT });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z), voxelType, eFaceType::FRONT });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z), voxelType, eFaceType::FRONT });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }

                if (isEmptyVoxel(outChunk->mVoxelTypes, x, y, z + 1))
                {
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y, z + 1), voxelType, eFaceType::BACK });
                    outChunk->mVoxels.push_back({ IntVector3D(x + 1, y + 1, z + 1), voxelType, eFaceType::BACK });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y + 1, z + 1), voxelType, eFaceType::BACK });
                    outChunk->mVoxels.push_back({ IntVector3D(x, y, z + 1), voxelType, eFaceType::BACK });

                    addNewIndex(outChunk->mIndices, &indexOffset);
                }
            }
        }
    }

    D3D11Utils::CreateVertexBuffer(*GRM.mDevice, outChunk->mVoxels, &outChunk->mVB);
    D3D11Utils::CreateIndexBuffer(*GRM.mDevice, outChunk->mIndices, &outChunk->mIB);
    outChunk->mIndexCount = UINT(outChunk->mIndices.size());

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R8_UINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 1, DXGI_FORMAT_R8_UINT, 0, 13, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(*GRM.mDevice, L"src/Shaders/ChunkVS.hlsl", inputElements, &outChunk->mVS, &outChunk->mIL);
    D3D11Utils::CreatePixelShader(*GRM.mDevice, L"src/Shaders/ChunkPS.hlsl", &outChunk->mPS);

    D3D11Utils::CreateMipsTexture(*GRM.mDevice, *GRM.mContext, "../Resources/frame.png", &outChunk->mFrameTex, &outChunk->mFrameSRV);
    D3D11Utils::CreateMipsTexture(*GRM.mDevice, *GRM.mContext, "../Resources/test.png", &outChunk->mTestTex, &outChunk->mTestSRV);
    // D3D11Utils::CreateTexture(*GRM.mDevice, "../Resources/strawberry.png", &mTestTex, &mTestSRV);
}

bool ChunkBuilder::isEmptyVoxel(std::vector<eVoxelType>& voxelTypes, const int x, const int y, const int z)
{
    if (x < 0 || x >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE)
    {
        return true;
    }

    return voxelTypes[VOXEL_INDEX(x, y, z)] == eVoxelType::EMPTY ? true : false;
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

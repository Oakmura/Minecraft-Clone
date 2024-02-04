#include "Precompiled.h"

#include "Scene.h"
#include <noise/noise.h>

#define VOXEL_INDEX(x, y, z) (x + z * CHUNK_SIZE + y * CHUNK_AREA)

Scene::Scene(GraphicsResourceManager& GRM)
{
    noise::module::Perlin perlinNoise;
    perlinNoise.SetSeed(30);

    mVoxelTypes.reserve(CHUNK_VOLUME);
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            for (int y = 0; y < CHUNK_SIZE; ++y)
            {
                double noise = perlinNoise.GetValue(x * 0.1, y * 0.1, z * 0.1) + 1;
                int(noise) == 0 ? mVoxelTypes.push_back(static_cast<eVoxelType>(0)) : mVoxelTypes.push_back(static_cast<eVoxelType>(x+y+z));
            }
        }
    }

    uint32_t indexOffset = 0;
    mVoxels.reserve(CHUNK_VOLUME * MAX_NUM_VERTEX_PER_VOXEL);
    mIndices.reserve(CHUNK_VOLUME * MAX_NUM_INDEX_PER_VOXEL);
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            for (int y = 0; y < CHUNK_SIZE; ++y)
            {
                eVoxelType voxelType = mVoxelTypes[VOXEL_INDEX(x, y, z)];
                if (voxelType == eVoxelType::EMPTY)
                {
                    continue;
                }

                if (isEmptyVoxel(x, y + 1, z))
                {
                    mVoxels.push_back({ IntVector3D(x, y + 1, z), voxelType, eFaceType::TOP });
                    mVoxels.push_back({ IntVector3D(x, y + 1, z + 1), voxelType, eFaceType::TOP });
                    mVoxels.push_back({ IntVector3D(x + 1, y + 1, z + 1), voxelType, eFaceType::TOP });
                    mVoxels.push_back({ IntVector3D(x + 1, y + 1, z), voxelType, eFaceType::TOP });

                    addNewIndex(&indexOffset);
                }

                if (isEmptyVoxel(x, y - 1, z))
                {
                    mVoxels.push_back({ IntVector3D(x, y, z + 1), voxelType, eFaceType::BOTTOM });
                    mVoxels.push_back({ IntVector3D(x, y, z), voxelType, eFaceType::BOTTOM });
                    mVoxels.push_back({ IntVector3D(x + 1, y, z), voxelType, eFaceType::BOTTOM });
                    mVoxels.push_back({ IntVector3D(x + 1, y, z + 1), voxelType, eFaceType::BOTTOM });

                    addNewIndex(&indexOffset);
                }

                if (isEmptyVoxel(x - 1, y, z))
                {
                    mVoxels.push_back({ IntVector3D(x, y, z + 1), voxelType, eFaceType::LEFT });
                    mVoxels.push_back({ IntVector3D(x, y + 1, z + 1), voxelType, eFaceType::LEFT });
                    mVoxels.push_back({ IntVector3D(x, y + 1, z), voxelType, eFaceType::LEFT });
                    mVoxels.push_back({ IntVector3D(x, y, z), voxelType, eFaceType::LEFT });

                    addNewIndex(&indexOffset);
                }

                if (isEmptyVoxel(x + 1, y, z))
                {
                    mVoxels.push_back({ IntVector3D(x + 1, y, z), voxelType, eFaceType::RIGHT });
                    mVoxels.push_back({ IntVector3D(x + 1, y + 1, z), voxelType, eFaceType::RIGHT });
                    mVoxels.push_back({ IntVector3D(x + 1, y + 1, z + 1), voxelType, eFaceType::RIGHT });
                    mVoxels.push_back({ IntVector3D(x + 1, y, z + 1), voxelType, eFaceType::RIGHT });

                    addNewIndex(&indexOffset);
                }

                if (isEmptyVoxel(x, y, z - 1))
                {
                    mVoxels.push_back({ IntVector3D(x, y, z), voxelType, eFaceType::FRONT });
                    mVoxels.push_back({ IntVector3D(x, y + 1, z), voxelType, eFaceType::FRONT });
                    mVoxels.push_back({ IntVector3D(x + 1, y + 1, z), voxelType, eFaceType::FRONT });
                    mVoxels.push_back({ IntVector3D(x + 1, y, z), voxelType, eFaceType::FRONT });

                    addNewIndex(&indexOffset);
                }

                if (isEmptyVoxel(x, y, z + 1))
                {
                    mVoxels.push_back({ IntVector3D(x + 1, y, z + 1), voxelType, eFaceType::BACK });
                    mVoxels.push_back({ IntVector3D(x + 1, y + 1, z + 1), voxelType, eFaceType::BACK });
                    mVoxels.push_back({ IntVector3D(x, y + 1, z + 1), voxelType, eFaceType::BACK });
                    mVoxels.push_back({ IntVector3D(x, y, z + 1), voxelType, eFaceType::BACK });

                    addNewIndex(&indexOffset);
                }
            }
        }
    }

    D3D11Utils::CreateVertexBuffer(*GRM.mDevice, mVoxels, &mVB);
    D3D11Utils::CreateIndexBuffer(*GRM.mDevice, mIndices, &mIB);
    mIndexCount = UINT(mIndices.size());

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R8_UINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 1, DXGI_FORMAT_R8_UINT, 0, 13, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(*GRM.mDevice, L"src/Shaders/ChunkVS.hlsl", inputElements, &mVS, &mIL);
    D3D11Utils::CreatePixelShader(*GRM.mDevice, L"src/Shaders/ChunkPS.hlsl", &mPS);
    
    D3D11Utils::CreateMipsTexture(*GRM.mDevice, *GRM.mContext, "../Resources/frame.png", &mFrameTex, &mFrameSRV);
    D3D11Utils::CreateMipsTexture(*GRM.mDevice, *GRM.mContext, "../Resources/test.png", &mTestTex, &mTestSRV);
    // D3D11Utils::CreateTexture(*GRM.mDevice, "../Resources/strawberry.png", &mTestTex, &mTestSRV);
}

Scene::~Scene()
{
    RELEASE_COM(mVS);
    RELEASE_COM(mPS);
    RELEASE_COM(mIL);
    RELEASE_COM(mVB);
    RELEASE_COM(mIB);

    RELEASE_COM(mFrameTex);
    RELEASE_COM(mFrameSRV);

    RELEASE_COM(mTestTex);
    RELEASE_COM(mTestSRV);
}

bool Scene::isEmptyVoxel(const int x, const int y, const int z)
{
    if (x < 0 || x >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE)
    {
        return true;
    }

    return mVoxelTypes[VOXEL_INDEX(x, y, z)] == eVoxelType::EMPTY ? true : false;
}

void Scene::addNewIndex(uint32_t* outIndexOffset)
{
    mIndices.push_back(*outIndexOffset + 0);
    mIndices.push_back(*outIndexOffset + 1);
    mIndices.push_back(*outIndexOffset + 2);
    mIndices.push_back(*outIndexOffset + 0);
    mIndices.push_back(*outIndexOffset + 2);
    mIndices.push_back(*outIndexOffset + 3);
    *outIndexOffset += 4;
}

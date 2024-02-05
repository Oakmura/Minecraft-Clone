#include "Precompiled.h"

#include "World.h"

#define CHUNK_INDEX(x, y, z) (x + z * WORLD_WIDTH + y * WORLD_AREA)

World::World(GraphicsResourceManager& GRM)
{
    for (int x = 0; x < WORLD_WIDTH; ++x)
    {
        for (int y = 0; y < WORLD_HEIGHT; ++y)
        {
            for (int z = 0; z < WORLD_DEPTH; ++z)
            {
                mChunks[CHUNK_INDEX(x, y, z)].BuildVoxels(GRM, Vector3(x, y, z));
            }
        }
    }

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

World::~World()
{
    RELEASE_COM(mVS);
    RELEASE_COM(mPS);
    RELEASE_COM(mIL);

    RELEASE_COM(mFrameTex);
    RELEASE_COM(mFrameSRV);

    RELEASE_COM(mTestTex);
    RELEASE_COM(mTestSRV);
}

void World::Render(GraphicsResourceManager& GRM)
{
    GRM.mContext->IASetInputLayout(mIL);
    GRM.mContext->VSSetShader(mVS, 0, 0);

    ID3D11ShaderResourceView* srvs[2] = { mTestSRV, mFrameSRV };
    GRM.mContext->PSSetShader(mPS, 0, 0);
    GRM.mContext->PSSetShaderResources(0, 2, srvs);

    for (Chunk& chunk : mChunks)
    {
        chunk.Render(GRM);
    }
}

#include "Precompiled.h"

#include "World.h"
#include "Utils/ChunkUtils.h"
#include "ChunkBuilder.h"
#include "VoxelHandler.h"

World::World(const SimpleMath::Vector3& cameraPosition)
{
    ChunkUtils::Init(this);
    ChunkBuilder::Init(this);
    VoxelHandler::Init(this);

    for (int x = 0; x < WORLD_WIDTH; ++x)
    {
        for (int y = 0; y < WORLD_HEIGHT; ++y)
        {
            for (int z = 0; z < WORLD_DEPTH; ++z)
            {
                mChunks[ChunkUtils::GetChunkIndexLocal({ x, y, z })].BuildVoxels({ x, y, z });
            }
        }
    }

    for (int x = 0; x < WORLD_WIDTH; ++x)
    {
        for (int y = 0; y < WORLD_HEIGHT; ++y)
        {
            for (int z = 0; z < WORLD_DEPTH; ++z)
            {
                mChunks[ChunkUtils::GetChunkIndexLocal({ x, y, z })].BuildChunkMesh();
            }
        }
    }

    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R8_UINT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 1, DXGI_FORMAT_R8_UINT, 0, 21, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 2, DXGI_FORMAT_R8_UINT, 0, 22, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(GRM.GetDevice(), L"src/Shaders/ChunkVS.hlsl", inputElements, &mVS, &mIL);
    D3D11Utils::CreatePixelShader(GRM.GetDevice(), L"src/Shaders/ChunkPS.hlsl", &mPS);
    
    D3D11Utils::CreateMipsTexture(GRM.GetDevice(), GRM.GetDeviceContext(), "../Resources/frame.png", &mFrameTex, &mFrameSRV);
    D3D11Utils::CreateMipsTexture(GRM.GetDevice(), GRM.GetDeviceContext(), "../Resources/tex_array_0.png", &mBlockTexArray, &mBlockTexSRV);

    mGlobalCB.GetCPU().CameraPosition = cameraPosition;
    mGlobalCB.GetCPU().WaterLine = 5.6f;
    mGlobalCB.GetCPU().BackgroundColor = { 0.58f, 0.83f, 0.99f };
    mGlobalCB.GetCPU().FogStrength = 1.0f;
    D3D11Utils::CreateConstantBuffer(GRM.GetDevice(), mGlobalCB.GetCPU(), &mGlobalCB.GetGPU());
}

World::~World()
{
    RELEASE_COM(mVS);
    RELEASE_COM(mPS);
    RELEASE_COM(mIL);

    RELEASE_COM(mFrameTex);
    RELEASE_COM(mFrameSRV);

    RELEASE_COM(mBlockTexArray);
    RELEASE_COM(mBlockTexSRV);
}

void World::Update(const SimpleMath::Vector3& cameraPosition)
{
    mGlobalCB.GetCPU().CameraPosition = cameraPosition;
    D3D11Utils::UpdateBuffer(GraphicsResourceManager::GetInstance().GetDeviceContext(), mGlobalCB.GetCPU(), mGlobalCB.GetGPU());
}

void World::Render()
{
    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    GRM.GetDeviceContext().IASetInputLayout(mIL);
    GRM.GetDeviceContext().VSSetShader(mVS, nullptr, 0);

    ID3D11ShaderResourceView* srvs[2] = { mBlockTexSRV, mFrameSRV };
    GRM.GetDeviceContext().PSSetShader(mPS, nullptr, 0);
    GRM.GetDeviceContext().PSSetShaderResources(0, 2, srvs);
    GRM.GetDeviceContext().PSSetConstantBuffers(0, 1, &mGlobalCB.GetGPU());

    for (Chunk& chunk : mChunks)
    {
        chunk.Render();
    }
}

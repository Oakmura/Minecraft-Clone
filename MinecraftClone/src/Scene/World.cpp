#include "Precompiled.h"

#include "World.h"
#include "Utils/ChunkUtils.h"
#include "Generators/ChunkGenerator.h"
#include "BlockHandler.h"
#include "Graphics/TextureLibrary.h"
#include "Utils/Hasher.h"

World::World(const SimpleMath::Vector3& cameraPosition)
{
    ChunkUtils::Init(this);
    ChunkGenerator::Init(this);
    BlockHandler::Init(this);

    for (int x = 0; x < def::WORLD_WIDTH; ++x)
    {
        for (int y = 0; y < def::WORLD_HEIGHT; ++y)
        {
            for (int z = 0; z < def::WORLD_DEPTH; ++z)
            {
                mChunks[ChunkUtils::GetChunkIndexLocal({ x, y, z })].BuildBlocks({ x, y, z });
            }
        }
    }

    for (int x = 0; x < def::WORLD_WIDTH; ++x)
    {
        for (int y = 0; y < def::WORLD_HEIGHT; ++y)
        {
            for (int z = 0; z < def::WORLD_DEPTH; ++z)
            {
                mChunks[ChunkUtils::GetChunkIndexLocal({ x, y, z })].BuildChunkMesh();
            }
        }
    }

    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ID3D11Device& device = ge.GetDevice();

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R8_UINT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 1, DXGI_FORMAT_R8_UINT, 0, 21, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 2, DXGI_FORMAT_R8_UINT, 0, 22, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(device, L"src/Shaders/ChunkVS.hlsl", inputElements, &mVS, &mIL);
    D3D11Utils::CreatePixelShader(device, L"src/Shaders/ChunkPS.hlsl", &mPS);

    mGlobalCB.GetCPU().CameraPosition = cameraPosition;
    mGlobalCB.GetCPU().WaterLine = 5.6f;
    mGlobalCB.GetCPU().BackgroundColor = { 0.58f, 0.83f, 0.99f };
    mGlobalCB.GetCPU().FogStrength = 1.0f;
    D3D11Utils::CreateConstantBuffer(device, mGlobalCB.GetCPU(), &mGlobalCB.GetGPU());
}

World::~World()
{
    RELEASE_COM(mVS);
    RELEASE_COM(mPS);
    RELEASE_COM(mIL);
}

void World::Update(const SimpleMath::Vector3& cameraPosition)
{
    mGlobalCB.GetCPU().CameraPosition = cameraPosition;
    D3D11Utils::UpdateBuffer(GraphicsEngine::GetInstance().GetDeviceContext(), mGlobalCB.GetCPU(), mGlobalCB.GetGPU());
}

void World::Render()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ID3D11DeviceContext& context = ge.GetDeviceContext();

    context.IASetInputLayout(mIL);
    context.VSSetShader(mVS, nullptr, 0);
    context.PSSetShader(mPS, nullptr, 0);

    TextureLibrary& texLibrary = ge.GetTextureLibrary();
    Texture& blockTexArray = texLibrary.Get(Hasher::Hash("blocks_array.png"));
    Texture& frameTex = texLibrary.Get(Hasher::Hash("frame.png"));

    blockTexArray.UseOn(0);
    frameTex.UseOn(1);

    mGlobalCB.UseOn(eShader::Pixel, 0);

    for (Chunk& chunk : mChunks)
    {
        chunk.Render();
    }
}

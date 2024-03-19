#include "Precompiled.h"

#include "Water.h"
#include "Utils/Hasher.h"
#include "Graphics/TextureLibrary.h"

Water::Water()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ID3D11Device& device = ge.GetDevice();

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(device, L"src/Shaders/WaterVS.hlsl", inputElements, &mVS, &mIL);
    D3D11Utils::CreatePixelShader(device, L"src/Shaders/WaterPS.hlsl", &mPS);

    MeshData square = GeometryGenerator::MakeSquare();

    D3D11Utils::CreateVertexBuffer(device, square.Vertices, &mVB);
    D3D11Utils::CreateIndexBuffer(device, square.Indices, &mIB);
    mIndexCount = UINT(square.Indices.size());

    mWaterCB.GetCPU().WaterLine = 5.6f;
    mWaterCB.GetCPU().WaterArea = 5 * def::CHUNK_SIZE * def::WORLD_WIDTH;
    D3D11Utils::CreateConstantBuffer(device, mWaterCB.GetCPU(), &mWaterCB.GetGPU());
}

Water::~Water()
{
    RELEASE_COM(mIL);
    RELEASE_COM(mVS);
    RELEASE_COM(mPS);

    RELEASE_COM(mVB);
    RELEASE_COM(mIB);
}

void Water::Render()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ID3D11DeviceContext& context = ge.GetDeviceContext();

    context.IASetInputLayout(mIL);
    context.VSSetShader(mVS, nullptr, 0);
    context.PSSetShader(mPS, nullptr, 0);

    UINT offset = 0;
    UINT stride = sizeof(Vertex);

    context.IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    context.IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    mWaterCB.UseOn(eShader::Vertex, 5);

    TextureLibrary& texLibrary = ge.GetTextureLibrary();
    Texture& waterTex = texLibrary.Get(Hasher::Hash("water.png"));
    waterTex.UseOn(2);

    context.DrawIndexed(mIndexCount, 0, 0);
}
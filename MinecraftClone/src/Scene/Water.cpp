#include "Precompiled.h"

#include "Water.h"
#include "Utils/Hasher.h"
#include "Graphics/TextureLibrary.h"

Water::Water()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(ge.GetDevice(), L"src/Shaders/WaterVS.hlsl", inputElements, &mVS, &mIL);
    D3D11Utils::CreatePixelShader(ge.GetDevice(), L"src/Shaders/WaterPS.hlsl", &mPS);

    MeshData square = GeometryGenerator::MakeSquare();

    D3D11Utils::CreateVertexBuffer(ge.GetDevice(), square.Vertices, &mVB);
    D3D11Utils::CreateIndexBuffer(ge.GetDevice(), square.Indices, &mIB);
    mIndexCount = UINT(square.Indices.size());

    mWaterCB.GetCPU().WaterLine = 5.6f;
    mWaterCB.GetCPU().WaterArea = 5 * def::CHUNK_SIZE * def::WORLD_WIDTH;
    D3D11Utils::CreateConstantBuffer(ge.GetDevice(), mWaterCB.GetCPU(), &mWaterCB.GetGPU());
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

    ge.GetDeviceContext().IASetInputLayout(mIL);
    ge.GetDeviceContext().VSSetShader(mVS, nullptr, 0);
    ge.GetDeviceContext().PSSetShader(mPS, nullptr, 0);

    UINT offset = 0;
    UINT stride = sizeof(Vertex);

    ge.GetDeviceContext().IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    ge.GetDeviceContext().IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    ge.GetDeviceContext().VSSetConstantBuffers(0, 1, &mWaterCB.GetGPU());

    TextureLibrary& texLibrary = ge.GetTextureLibrary();
    Texture& waterTex = texLibrary.Get(Hasher::Hash("water.png"));
    waterTex.UseOn(0);

    ge.GetDeviceContext().DrawIndexed(mIndexCount, 0, 0);
}
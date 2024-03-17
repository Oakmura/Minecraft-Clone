#include "Precompiled.h"

#include "Water.h"

Water::Water()
{
    GraphicsEngine& GRM = GraphicsEngine::GetInstance();

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(GRM.GetDevice(), L"src/Shaders/WaterVS.hlsl", inputElements, &mVS, &mIL);
    D3D11Utils::CreatePixelShader(GRM.GetDevice(), L"src/Shaders/WaterPS.hlsl", &mPS);

    MeshData square = GeometryGenerator::MakeSquare();

    D3D11Utils::CreateVertexBuffer(GRM.GetDevice(), square.Vertices, &mVB);
    D3D11Utils::CreateIndexBuffer(GRM.GetDevice(), square.Indices, &mIB);
    mIndexCount = UINT(square.Indices.size());

    D3D11Utils::CreateTexture(GRM.GetDevice(), "../Resources/water.png", &mWaterTex, &mWaterSRV);

    mWaterCB.GetCPU().WaterLine = 5.6f;
    mWaterCB.GetCPU().WaterArea = 5 * CHUNK_SIZE * WORLD_WIDTH;
    D3D11Utils::CreateConstantBuffer(GRM.GetDevice(), mWaterCB.GetCPU(), &mWaterCB.GetGPU());
}

Water::~Water()
{
    RELEASE_COM(mIL);
    RELEASE_COM(mVS);
    RELEASE_COM(mPS);

    RELEASE_COM(mVB);
    RELEASE_COM(mIB);

    RELEASE_COM(mWaterTex);
    RELEASE_COM(mWaterSRV);
}

void Water::Render()
{
    GraphicsEngine& GRM = GraphicsEngine::GetInstance();

    GRM.GetDeviceContext().IASetInputLayout(mIL);
    GRM.GetDeviceContext().VSSetShader(mVS, nullptr, 0);
    GRM.GetDeviceContext().PSSetShader(mPS, nullptr, 0);

    UINT offset = 0;
    UINT stride = sizeof(Vertex);

    GRM.GetDeviceContext().IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    GRM.GetDeviceContext().IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    GRM.GetDeviceContext().VSSetConstantBuffers(0, 1, &mWaterCB.GetGPU());
    GRM.GetDeviceContext().PSSetShaderResources(0, 1, &mWaterSRV);

    GRM.GetDeviceContext().DrawIndexed(mIndexCount, 0, 0);
}
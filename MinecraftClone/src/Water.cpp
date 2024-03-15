#include "Precompiled.h"

#include "Water.h"

Water::Water()
{
    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_SINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    D3D11Utils::CreateVertexShaderAndInputLayout(GRM.GetDevice(), L"src/Shaders/WaterVS.hlsl", inputElements, &mVS, &mIL);
    D3D11Utils::CreatePixelShader(GRM.GetDevice(), L"src/Shaders/WaterPS.hlsl", &mPS);

    std::vector<IntVector3D> positions;
    positions.push_back({ 0, 0, 0 });
    positions.push_back({ 0, 0, 1 });
    positions.push_back({ 1, 0, 1 });
    positions.push_back({ 1, 0, 0 });

    std::vector<SimpleMath::Vector2> texcoords;
    texcoords.push_back({0.0f, 0.0f});
    texcoords.push_back({0.0f, 1.0f});
    texcoords.push_back({1.0f, 1.0f});
    texcoords.push_back({1.0f, 0.0f});

    std::vector<WaterVertex> vertices;
    for (size_t i = 0; i < positions.size(); ++i)
    {
        WaterVertex v;
        v.Position = positions[i];
        v.Texcoord = texcoords[i];
        vertices.push_back(v);
    }

    std::vector<uint32_t> indices =
    {
        0, 1, 2,
        1, 2, 3,
    };

    D3D11Utils::CreateVertexBuffer(GRM.GetDevice(), vertices, &mVB);
    D3D11Utils::CreateIndexBuffer(GRM.GetDevice(), indices, &mIB);
    mIndexCount = UINT(indices.size());

    D3D11Utils::CreateTexture(GRM.GetDevice(), "../Resources/water.png", &mWaterTex, &mWaterSRV);

    mWaterCbCPU.WaterLine = 5.6f;
    mWaterCbCPU.WaterArea = 5 * CHUNK_SIZE * WORLD_WIDTH;
    D3D11Utils::CreateConstantBuffer(GRM.GetDevice(), mWaterCbCPU, &mWaterCbGPU);
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

    RELEASE_COM(mWaterCbGPU);
}

void Water::Render()
{
    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    GRM.GetDeviceContext().IASetInputLayout(mIL);
    GRM.GetDeviceContext().VSSetShader(mVS, nullptr, 0);
    GRM.GetDeviceContext().PSSetShader(mPS, nullptr, 0);

    UINT offset = 0;
    UINT stride = sizeof(WaterVertex);

    GRM.GetDeviceContext().IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    GRM.GetDeviceContext().IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    GRM.GetDeviceContext().VSSetConstantBuffers(0, 1, &mWaterCbGPU);
    GRM.GetDeviceContext().PSSetShaderResources(0, 1, &mWaterSRV);

    GRM.GetDeviceContext().DrawIndexed(mIndexCount, 0, 0);
}
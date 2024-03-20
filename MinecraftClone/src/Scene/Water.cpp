#include "Precompiled.h"

#include "Water.h"
#include "Utils/Hasher.h"

Water::Water()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ID3D11Device& device = ge.GetDevice();

    MeshData square = GeometryGenerator::MakeSquare();

    D3D11Utils::CreateVertexBuffer(device, square.Vertices, &mVB);
    D3D11Utils::CreateIndexBuffer(device, square.Indices, &mIB);
    mIndexCount = UINT(square.Indices.size());

    mWaterCB.GetCPU().WaterLine = 5.6f;
    mWaterCB.GetCPU().WaterArea = 8 * def::CHUNK_SIZE * def::WORLD_WIDTH;
    D3D11Utils::CreateConstantBuffer(device, mWaterCB.GetCPU(), &mWaterCB.GetGPU());
}

Water::~Water()
{
    RELEASE_COM(mVB);
    RELEASE_COM(mIB);
}

void Water::Render()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    GraphicsResourceLibrary& grl = ge.GetResourceLibrary();
    ID3D11DeviceContext& context = ge.GetDeviceContext();

    context.IASetInputLayout(&grl.GetIL(Hasher::Hash("water")));
    context.VSSetShader(&grl.GetVS(Hasher::Hash("water")), nullptr, 0);
    context.PSSetShader(&grl.GetPS(Hasher::Hash("water")), nullptr, 0);

    UINT offset = 0;
    UINT stride = sizeof(Vertex);

    context.IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    context.IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    mWaterCB.UseOn(eShader::Vertex, 5);
    grl.GetTex(Hasher::Hash("water.png")).UseOn(2);

    context.DrawIndexed(mIndexCount, 0, 0);
}
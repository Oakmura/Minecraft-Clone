#include "Precompiled.h"

#include "Managers/GraphicsResourceManager.h"
#include "Renderer.h"

Renderer::Renderer(GraphicsResourceManager& GRM)
{
    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    rastDesc.FrontCounterClockwise = false;
    rastDesc.DepthClipEnable = true;

    GRM.mDevice->CreateRasterizerState(&rastDesc, &mRS);

    std::vector<Vector3> positions;
    std::vector<Vector3> colors;

    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 1.0f));

    std::vector<Vertex> vertices;
    for (size_t i = 0; i < positions.size(); ++i) 
    {
        Vertex v;
        v.Position = positions[i];
        v.Color = colors[i];
        vertices.push_back(v);
    }

    std::vector<uint32_t> indices = 
    {
        0, 1, 2, 0, 2, 3
    };

    D3D11Utils::CreateVertexBuffer(*GRM.mDevice, vertices, &mVB);
    D3D11Utils::CreateIndexBuffer(*GRM.mDevice, indices, &mIB);
    mIndexCount = UINT(indices.size());

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = 
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    
    D3D11Utils::CreateVertexShaderAndInputLayout(*GRM.mDevice, L"src/Shaders/ColorVS.hlsl", inputElements, &mVS, &mIL);
    D3D11Utils::CreatePixelShader(*GRM.mDevice, L"src/Shaders/ColorPS.hlsl", &mPS);
}

Renderer::~Renderer()
{
    RELEASE_COM(mRS);

    RELEASE_COM(mVS);
    RELEASE_COM(mPS);
    RELEASE_COM(mIL);

    RELEASE_COM(mVB);
    RELEASE_COM(mIB);
}

void Renderer::Update(GraphicsResourceManager& GRM, const float dt)
{

}

void Renderer::Render(GraphicsResourceManager& GRM)
{
    const float clearColor[4] = { 0.58f, 0.83f, 0.99f, 1.f };
    GRM.mContext->ClearRenderTargetView(GRM.mBackBufferRTV, clearColor);
    GRM.mContext->ClearDepthStencilView(GRM.mDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    UINT offset = 0;
    UINT stride = sizeof(Vertex);
    GRM.mContext->IASetInputLayout(mIL);
    GRM.mContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    GRM.mContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R16_UINT, 0);
    GRM.mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    GRM.mContext->RSSetViewports(1, &GRM.mVP);
    GRM.mContext->RSSetState(mRS);

    GRM.mContext->VSSetShader(mVS, 0, 0);
    GRM.mContext->PSSetShader(mPS, 0, 0);

    GRM.mContext->OMSetRenderTargets(1, &GRM.mBackBufferRTV, GRM.mDSV);

    GRM.mContext->DrawIndexed(mIndexCount, 0, 0);
}
#include "Precompiled.h"

#include "Managers/GraphicsResourceManager.h"
#include "Managers/UserInterface.h"
#include "Renderer.h"

Renderer::Renderer(GraphicsResourceManager& GRM)
{
    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    // rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
    rastDesc.FrontCounterClockwise = false;
    rastDesc.DepthClipEnable = true;

    GRM.mDevice->CreateRasterizerState(&rastDesc, &mRS);

    std::vector<Vector3> positions;
    std::vector<Vector3> colors;

    const float scale = 1.0f;

    // À­¸é
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));

    // ¾Æ·§¸é
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));

    // ¾Õ¸é
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));

    // µÞ¸é
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));

    // ¿ÞÂÊ
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));

    // ¿À¸¥ÂÊ
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));

    std::vector<uint32_t> indices = {
        0,  1,  2,  0,  2,  3,  // À­¸é
        4,  5,  6,  4,  6,  7,  // ¾Æ·§¸é
        8,  9,  10, 8,  10, 11, // ¾Õ¸é
        12, 13, 14, 12, 14, 15, // µÞ¸é
        16, 17, 18, 16, 18, 19, // ¿ÞÂÊ
        20, 21, 22, 20, 22, 23  // ¿À¸¥ÂÊ
    };

    std::vector<Vertex> vertices;
    for (size_t i = 0; i < positions.size(); ++i) 
    {
        Vertex v;
        v.Position = positions[i];
        v.Color = colors[i];
        vertices.push_back(v);
    }

    D3D11Utils::CreateVertexBuffer(*GRM.mDevice, vertices, &mVB);
    D3D11Utils::CreateIndexBuffer(*GRM.mDevice, indices, &mIB);
    mIndexCount = UINT(indices.size());

    mCbCPU.Model = Matrix();
    mCbCPU.View = Matrix();
    mCbCPU.Projection = Matrix();
    D3D11Utils::CreateConstantBuffer(*GRM.mDevice, mCbCPU, &mCbGPU);

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
    RELEASE_COM(mCbGPU);
}

void Renderer::Update(GraphicsResourceManager& GRM, Player& player, const float dt)
{
    player.Update(dt);

    mCbCPU.View = player.GetViewMatrix().Transpose();
    mCbCPU.Projection = player.GetProjMatrix().Transpose();
    D3D11Utils::UpdateBuffer(*GRM.mContext, mCbCPU, mCbGPU);
}

void Renderer::Render(GraphicsResourceManager& GRM, Scene& scene)
{
    GRM.mContext->ClearRenderTargetView(GRM.mBackBufferRTV, mBackgroundColor2);
    GRM.mContext->ClearDepthStencilView(GRM.mDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    UINT offset = 0;
    UINT stride = sizeof(VoxelVertex);
    GRM.mContext->IASetInputLayout(scene.mIL);
    GRM.mContext->IASetVertexBuffers(0, 1, &scene.mVB, &stride, &offset);
    GRM.mContext->IASetIndexBuffer(scene.mIB, DXGI_FORMAT_R32_UINT, 0);
    GRM.mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    GRM.mContext->RSSetViewports(1, &GRM.mVP);
    GRM.mContext->RSSetState(mRS);

    GRM.mContext->VSSetConstantBuffers(0, 1, &mCbGPU);
    GRM.mContext->VSSetShader(scene.mVS, 0, 0);
    GRM.mContext->PSSetShader(scene.mPS, 0, 0);

    GRM.mContext->OMSetDepthStencilState(GRM.mDSS, 0);
    GRM.mContext->OMSetRenderTargets(1, &GRM.mBackBufferRTV, GRM.mDSV);

    GRM.mContext->DrawIndexed(scene.mIndexCount, 0, 0);
}
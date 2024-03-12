#include "Precompiled.h"

#include "Renderer.h"
#include "Managers/GraphicsResourceManager.h"
#include "Managers/UserInterface.h"

Renderer::Renderer()
{
    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    // rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
    rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    //rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
    rastDesc.FrontCounterClockwise = false;
    rastDesc.DepthClipEnable = true;
    GRM.GetDevice().CreateRasterizerState(&rastDesc, &mRS);

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    // sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    // sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
    // sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
    sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    sampDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    DX_CALL(GRM.GetDevice().CreateSamplerState(&sampDesc, &mSS));

    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(blendDesc));
    blendDesc.AlphaToCoverageEnable = false; // MSAA
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    DX_CALL(GRM.GetDevice().CreateBlendState(&blendDesc, &mBS));

    mCbCPU.View = SimpleMath::Matrix();
    mCbCPU.Projection = SimpleMath::Matrix();
    D3D11Utils::CreateConstantBuffer(GRM.GetDevice(), mCbCPU, &mCbGPU);
}

Renderer::~Renderer()
{
    RELEASE_COM(mRS);
    RELEASE_COM(mSS);
    RELEASE_COM(mBS);

    RELEASE_COM(mCbGPU);
}

void Renderer::Render(Scene& scene, const SimpleMath::Matrix& playerViewMatrix, const SimpleMath::Matrix& playerProjMatrix, const VoxelHandler& voxelHandler)
{
    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    mCbCPU.View = playerViewMatrix.Transpose();
    mCbCPU.Projection = playerProjMatrix.Transpose();
    D3D11Utils::UpdateBuffer(GRM.GetDeviceContext(), mCbCPU, mCbGPU);

    GRM.GetDeviceContext().ClearRenderTargetView(GRM.GetBackBufferRTV(), mBackgroundColor2);
    GRM.GetDeviceContext().ClearDepthStencilView(&GRM.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    GRM.GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    GRM.GetDeviceContext().RSSetViewports(1, &GRM.GetViewport());
    GRM.GetDeviceContext().RSSetState(mRS);

    GRM.GetDeviceContext().VSSetConstantBuffers(1, 1, &mCbGPU);
    GRM.GetDeviceContext().PSSetSamplers(0, 1, &mSS);

    const float defaultBS[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GRM.GetDeviceContext().OMSetBlendState(mBS, defaultBS, 0xffffffff);
    GRM.GetDeviceContext().OMSetDepthStencilState(&GRM.GetDepthStencilState(), 0);
    GRM.GetDeviceContext().OMSetRenderTargets(1, &GRM.GetBackBufferRTV(), &GRM.GetDepthStencilView());

    scene.Render(voxelHandler);
}

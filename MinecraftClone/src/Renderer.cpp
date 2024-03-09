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
    GRM.mDevice->CreateRasterizerState(&rastDesc, &mRS);

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
    GRM.mDevice->CreateSamplerState(&sampDesc, &mSS);

    mCbCPU.View = SimpleMath::Matrix();
    mCbCPU.Projection = SimpleMath::Matrix();
    D3D11Utils::CreateConstantBuffer(*GRM.mDevice, mCbCPU, &mCbGPU);
}

Renderer::~Renderer()
{
    RELEASE_COM(mRS);
    RELEASE_COM(mSS);
    RELEASE_COM(mCbGPU);
}

void Renderer::Update(Scene& scene, const float dt)
{
    scene.Update(dt);

    mCbCPU.View = scene.GetPlayer().GetViewMatrix().Transpose();
    mCbCPU.Projection = scene.GetPlayer().GetProjMatrix().Transpose();
    D3D11Utils::UpdateBuffer(*GraphicsResourceManager::GetInstance().GetDeviceContext(), mCbCPU, mCbGPU);
}

void Renderer::Render(Scene& scene)
{
    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    GRM.mContext->ClearRenderTargetView(GRM.mBackBufferRTV, mBackgroundColor2);
    GRM.mContext->ClearDepthStencilView(GRM.mDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    GRM.mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    GRM.mContext->RSSetViewports(1, &GRM.mVP);
    GRM.mContext->RSSetState(mRS);

    GRM.mContext->VSSetConstantBuffers(1, 1, &mCbGPU);
    GRM.mContext->PSSetSamplers(0, 1, &mSS);

    GRM.mContext->OMSetDepthStencilState(GRM.mDSS, 0);
    GRM.mContext->OMSetRenderTargets(1, &GRM.mBackBufferRTV, GRM.mDSV);

    scene.Render();
}
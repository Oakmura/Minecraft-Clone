#include "Precompiled.h"

#include "Core/Renderer.h"
#include "Core/GraphicsEngine.h"
#include "Core/ImGuiUI.h"

Renderer::Renderer()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();

    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    // rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
    rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    // rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
    rastDesc.FrontCounterClockwise = false;
    rastDesc.DepthClipEnable = true;
    ge.GetDevice().CreateRasterizerState(&rastDesc, &mRS);

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    DX_CALL(ge.GetDevice().CreateSamplerState(&sampDesc, &mLinearSS));

    sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    sampDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
    DX_CALL(ge.GetDevice().CreateSamplerState(&sampDesc, &mAnisoSS));

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
    DX_CALL(ge.GetDevice().CreateBlendState(&blendDesc, &mBS));

    mViewProjCB.GetCPU().View = SimpleMath::Matrix();
    mViewProjCB.GetCPU().Projection = SimpleMath::Matrix();
    D3D11Utils::CreateConstantBuffer(ge.GetDevice(), mViewProjCB.GetCPU(), &mViewProjCB.GetGPU());
}

Renderer::~Renderer()
{
    RELEASE_COM(mRS);
    RELEASE_COM(mAnisoSS);
    RELEASE_COM(mLinearSS);
    RELEASE_COM(mBS);
}

void Renderer::Update(const SimpleMath::Matrix& playerViewMatrix, const SimpleMath::Matrix& playerProjMatrix)
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();

    mViewProjCB.GetCPU().View = playerViewMatrix.Transpose();
    mViewProjCB.GetCPU().Projection = playerProjMatrix.Transpose();
    D3D11Utils::UpdateBuffer(ge.GetDeviceContext(), mViewProjCB.GetCPU(), mViewProjCB.GetGPU());
}

void Renderer::Render(Scene& scene, const BlockHandler& blockHandler)
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();

    ge.GetDeviceContext().ClearRenderTargetView(ge.GetBackBufferRTV(), mBackgroundColor);
    ge.GetDeviceContext().ClearDepthStencilView(&ge.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    ge.GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    ge.GetDeviceContext().RSSetViewports(1, &ge.GetViewport());
    ge.GetDeviceContext().RSSetState(mRS);

    ge.GetDeviceContext().VSSetConstantBuffers(1, 1, &mViewProjCB.GetGPU());

    if (mbAnisoSS)
    {
        ge.GetDeviceContext().PSSetSamplers(0, 1, &mAnisoSS);
    }
    else
    {
        ge.GetDeviceContext().PSSetSamplers(0, 1, &mLinearSS);
    }

    const float defaultBS[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    ge.GetDeviceContext().OMSetBlendState(mBS, defaultBS, 0xffffffff);
    ge.GetDeviceContext().OMSetDepthStencilState(&ge.GetDepthStencilState(), 0);
    ge.GetDeviceContext().OMSetRenderTargets(1, &ge.GetBackBufferRTV(), &ge.GetDepthStencilView());

    scene.Render(blockHandler);
}

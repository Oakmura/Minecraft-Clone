#include "Precompiled.h"

#include "Core/Renderer.h"
#include "Core/GraphicsEngine.h"
#include "Core/ImGuiUI.h"
#include "Utils/Hasher.h"

Renderer::Renderer()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();

    mViewProjCB.GetCPU().View = SimpleMath::Matrix();
    mViewProjCB.GetCPU().Projection = SimpleMath::Matrix();
    D3D11Utils::CreateConstantBuffer(ge.GetDevice(), mViewProjCB.GetCPU(), &mViewProjCB.GetGPU());
}

void Renderer::Update(const SimpleMath::Matrix& playerViewMatrix, const SimpleMath::Matrix& playerProjMatrix)
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();

    mViewProjCB.GetCPU().View = playerViewMatrix.Transpose();
    mViewProjCB.GetCPU().Projection = playerProjMatrix.Transpose();
    D3D11Utils::UpdateBuffer(ge.GetDeviceContext(), mViewProjCB.GetCPU(), mViewProjCB.GetGPU());
}

void Renderer::Render(Scene& scene)
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    ID3D11DeviceContext& context = ge.GetDeviceContext();

    context.VSSetSamplers(0, UINT(ge.GetSamplers().size()), ge.GetSamplers().data());
    context.PSSetSamplers(0, UINT(ge.GetSamplers().size()), ge.GetSamplers().data());

    context.ClearRenderTargetView(ge.GetBackBufferRTV(), mBackgroundColor);
    context.ClearDepthStencilView(&ge.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    context.RSSetViewports(1, &ge.GetViewport());
    context.OMSetRenderTargets(1, &ge.GetBackBufferRTV(), &ge.GetDepthStencilView());

    GraphicsPsoLibrary& psoLibrary = ge.GetGraphicsPsoLibrary();
    psoLibrary.Get(Hasher::Hash("defaultSolidAlpha")).SetPipelineState();

    mViewProjCB.UseOn(eShader::Vertex, 1);

    scene.Render();
}

#pragma once

#include "Graphics/ConstantBuffers.h"
#include "Scene.h"

class BlockHandler;

class Renderer final
{
    friend class ImGuiUI;
public:
    Renderer();
    ~Renderer();

    void Update(const SimpleMath::Matrix& playerViewMatrix, const SimpleMath::Matrix& playerProjMatrix);
    void Render(Scene& scene, const BlockHandler& blockHandler);

private:
    ID3D11RasterizerState* mRS;

    ID3D11SamplerState* mLinearSS;
    ID3D11SamplerState* mAnisoSS;

    ID3D11BlendState* mBS;

    ConstantBuffer<ViewProjCB> mViewProjCB;

    float mBackgroundColor[4] = { 0.58f, 0.83f, 0.99f, 1.f };
    float mBackgroundColor2[4] = { 0.f, 0.16f, .25f, 1.f };

    bool mbAnisoSS = false;
};
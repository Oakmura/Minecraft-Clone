#pragma once

#include "Scene.h"

struct VPConstantBuffer
{
    SimpleMath::Matrix View;
    SimpleMath::Matrix Projection;
};

class Renderer final
{
    friend class UserInterface;
public:
    Renderer();
    ~Renderer();

    void Update(Scene& scene, const float dt);
    void Render(Scene& scene);

private:
    ID3D11RasterizerState* mRS;
    ID3D11SamplerState* mSS;

    VPConstantBuffer mCbCPU;
    ID3D11Buffer* mCbGPU;

    float mBackgroundColor[4] = { 0.5f, 0.83f, 0.99f, 1.f };
    float mBackgroundColor2[4] = { 0.f, 0.16f, .25f, 1.f };
};
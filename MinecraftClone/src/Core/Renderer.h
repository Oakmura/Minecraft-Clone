#pragma once

#include "Graphics/ConstantBuffer.h"
#include "Scene/Scene.h"

class BlockHandler;

class Renderer final
{
    friend class ImGuiUI;
public:
    Renderer();
    ~Renderer() = default;
    Renderer(const Renderer& rhs) = delete;
    Renderer& operator=(const Renderer& rhs) = delete;

    void Update(const SimpleMath::Matrix& playerViewMatrix, const SimpleMath::Matrix& playerProjMatrix);
    void Render(Scene& scene);

private:
    ConstantBuffer<ViewProjCB> mViewProjCB;

    float mBackgroundColor[4] = { 0.58f, 0.83f, 0.99f, 1.f };
};
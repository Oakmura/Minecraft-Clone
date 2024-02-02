#pragma once

#include "Camera.h"

struct Vertex
{
    Vector3 Position;
    Vector3 Color;
};

struct MVPConstantBuffer
{
    Matrix Model;
    Matrix View;
    Matrix Projection;
};

class Renderer final
{
    friend class UserInterface;
public:
    Renderer(GraphicsResourceManager& GRM);
    ~Renderer();

    void Update(GraphicsResourceManager& GRM, const float dt);
    void Render(GraphicsResourceManager& GRM);

    inline void OnScreenResize(const IntVector2D& screenSize) { mMainCamera.SetViewportSize(screenSize); };

private:
    Camera mMainCamera;

    ID3D11RasterizerState* mRS;

    ID3D11VertexShader* mVS;
    ID3D11PixelShader* mPS;
    ID3D11InputLayout* mIL;

    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mIndexCount;

    MVPConstantBuffer mCbCPU;
    ID3D11Buffer* mCbGPU;

    float mBackgroundColor[4] = { 0.5f, 0.83f, 0.99f, 1.f };
    // float mBackgroundColor[4] = { 1.f, 1.f, 1.f, 1.f };
};
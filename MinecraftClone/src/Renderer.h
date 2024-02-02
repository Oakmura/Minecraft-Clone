#pragma once

#include "Player.h"

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

    void Update(GraphicsResourceManager& GRM, Player& player, const float dt);
    void Render(GraphicsResourceManager& GRM);

private:
    ID3D11RasterizerState* mRS;

    ID3D11VertexShader* mVS;
    ID3D11PixelShader* mPS;
    ID3D11InputLayout* mIL;

    ID3D11Buffer* mVB;
    ID3D11Buffer* mIB;
    UINT mVertexCount;
    UINT mIndexCount;

    MVPConstantBuffer mCbCPU;
    ID3D11Buffer* mCbGPU;

    float mBackgroundColor[4] = { 0.5f, 0.83f, 0.99f, 1.f };
};
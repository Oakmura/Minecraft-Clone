#pragma once

#include "Chunk.h"

class World
{
public:
    World(GraphicsResourceManager& GRM);
    ~World();

    void Render(GraphicsResourceManager& GRM);

private:
    enum { WORLD_WIDTH = 2 };
    enum { WORLD_DEPTH = WORLD_WIDTH };
    enum { WORLD_HEIGHT = 2 };

    enum { WORLD_AREA = WORLD_WIDTH * WORLD_DEPTH };
    enum { WORLD_VOLUME = WORLD_AREA * WORLD_HEIGHT };

    Chunk mChunks[WORLD_VOLUME];

    ID3D11VertexShader* mVS;
    ID3D11PixelShader* mPS;
    ID3D11InputLayout* mIL;

    ID3D11Texture2D* mFrameTex;
    ID3D11ShaderResourceView* mFrameSRV;

    ID3D11Texture2D* mTestTex;
    ID3D11ShaderResourceView* mTestSRV;
};
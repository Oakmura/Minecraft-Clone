#pragma once

#include "Chunk.h"
#include "Managers/GraphicsResourceManager.h"

enum { WORLD_WIDTH = 20 };
enum { WORLD_HEIGHT = 2 };
enum { WORLD_DEPTH = WORLD_WIDTH };

enum { WORLD_CENTER_Y = WORLD_HEIGHT * CHUNK_HALF_SIZE };
enum { WORLD_CENTER_XZ = WORLD_WIDTH * CHUNK_HALF_SIZE };

enum { WORLD_AREA = WORLD_WIDTH * WORLD_DEPTH };
enum { WORLD_VOLUME = WORLD_AREA * WORLD_HEIGHT };

class World final
{
public:
    World();
    ~World();

    void Update();
    void Render();

    inline const Chunk& GetChunk(int i) const { ASSERT(i >= 0 && i < WORLD_VOLUME, "index out of range"); return mChunks[i]; };
    inline Chunk* GetChunkPtr(int i) { ASSERT(i >= 0 && i < WORLD_VOLUME, "index out of range"); return &mChunks[i]; };

private:
    Chunk mChunks[WORLD_VOLUME];

    ID3D11VertexShader* mVS;
    ID3D11PixelShader* mPS;
    ID3D11InputLayout* mIL;

    ID3D11Texture2D* mFrameTex;
    ID3D11ShaderResourceView* mFrameSRV;

    ID3D11Texture2D* mTestTex;
    ID3D11ShaderResourceView* mTestSRV;
};
#pragma once

#include "Settings.h"
#include "Graphics/ConstantBuffers.h"
#include "Chunk.h"
#include "Managers/GraphicsEngine.h"

class World final
{
    friend class ImGuiUI;
public:
    World(const SimpleMath::Vector3& cameraPosition);
    ~World();

    void Update(const SimpleMath::Vector3& cameraPosition);
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

    ID3D11Texture2D* mBlockTexArray;
    ID3D11ShaderResourceView* mBlockTexSRV;

    ConstantBuffer<GlobalCB> mGlobalCB;
};
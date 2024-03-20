#pragma once

#include "Settings.h"
#include "Graphics/ConstantBuffer.h"
#include "Scene/Chunk.h"
#include "Core/GraphicsEngine.h"

class World final
{
public:
    World();
    ~World() = default;
    World(const World& rhs) = delete;
    World& operator=(const World& rhs) = delete;

    void Render();

    inline const Chunk& GetChunk(int i) const { ASSERT(i >= 0 && i < def::WORLD_VOLUME, "index out of range"); return mChunks[i]; };
    inline Chunk* GetChunkPtr(int i) { ASSERT(i >= 0 && i < def::WORLD_VOLUME, "index out of range"); return &mChunks[i]; };

private:
    Chunk mChunks[def::WORLD_VOLUME];
};
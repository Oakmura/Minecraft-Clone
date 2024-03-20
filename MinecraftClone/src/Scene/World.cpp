#include "Precompiled.h"

#include "World.h"
#include "Utils/ChunkUtils.h"
#include "Generators/ChunkGenerator.h"
#include "BlockHandler.h"
#include "Utils/Hasher.h"

World::World()
{
    ChunkUtils::Init(this);
    ChunkGenerator::Init(this);
    BlockHandler::Init(this);

    for (int x = 0; x < def::WORLD_WIDTH; ++x)
    {
        for (int y = 0; y < def::WORLD_HEIGHT; ++y)
        {
            for (int z = 0; z < def::WORLD_DEPTH; ++z)
            {
                mChunks[ChunkUtils::GetChunkIndexLocal({ x, y, z })].BuildBlocks({ x, y, z });
            }
        }
    }

    for (int x = 0; x < def::WORLD_WIDTH; ++x)
    {
        for (int y = 0; y < def::WORLD_HEIGHT; ++y)
        {
            for (int z = 0; z < def::WORLD_DEPTH; ++z)
            {
                mChunks[ChunkUtils::GetChunkIndexLocal({ x, y, z })].BuildChunkMesh();
            }
        }
    }
}

void World::Render()
{
    GraphicsEngine& ge = GraphicsEngine::GetInstance();
    GraphicsResourceLibrary& grl = ge.GetResourceLibrary();
    ID3D11DeviceContext& context = ge.GetDeviceContext();

    context.IASetInputLayout(&grl.GetIL(Hasher::Hash("chunk")));
    context.VSSetShader(&grl.GetVS(Hasher::Hash("chunk")), nullptr, 0);
    context.PSSetShader(&grl.GetPS(Hasher::Hash("chunk")), nullptr, 0);

    grl.GetTex(Hasher::Hash("blocks_array.png")).UseOn(0);
    grl.GetTex(Hasher::Hash("frame.png")).UseOn(1);

    for (Chunk& chunk : mChunks)
    {
        chunk.Render();
    }
}

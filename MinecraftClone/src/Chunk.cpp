#include "Precompiled.h"

#include "Chunk.h"
#include "ChunkBuilder.h"

Chunk::Chunk(GraphicsResourceManager& GRM)
{
    ChunkBuilder::BuildChunk(GRM, this);
}

Chunk::~Chunk()
{
    RELEASE_COM(mVS);
    RELEASE_COM(mPS);
    RELEASE_COM(mIL);
    RELEASE_COM(mVB);
    RELEASE_COM(mIB);

    RELEASE_COM(mFrameTex);
    RELEASE_COM(mFrameSRV);

    RELEASE_COM(mTestTex);
    RELEASE_COM(mTestSRV);
}

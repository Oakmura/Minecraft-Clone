#include "Precompiled.h"

#include "Chunk.h"
#include "ChunkBuilder.h"

Chunk::Chunk()
    : mVB(nullptr)
    , mIB(nullptr)
    , mIndexCount(0)
{
}

Chunk::~Chunk()
{
    RELEASE_COM(mVB);
    RELEASE_COM(mIB);
}

void Chunk::BuildBlocks(const IntVector3D& pos)
{
    mPosition = pos;
    ChunkBuilder::BuildChunk(this, pos);

    SimpleMath::Vector3 translation = { (float)pos.mX * CHUNK_SIZE, (float)pos.mY * CHUNK_SIZE, (float)pos.mZ * CHUNK_SIZE };
    mModelCB.GetCPU().Model = SimpleMath::Matrix::CreateTranslation(translation).Transpose();
    D3D11Utils::CreateConstantBuffer(GraphicsEngine::GetInstance().GetDevice(), mModelCB.GetCPU(), &mModelCB.GetGPU());
}

void Chunk::BuildChunkMesh()
{
    ChunkBuilder::BuildChunkMesh(this, mPosition);
}

void Chunk::RebuildChunkMesh(World& world)
{
    mBlocks.clear();
    mIndices.clear();

    RELEASE_COM(mVB);
    RELEASE_COM(mIB);

    mIndexCount = 0;

    ChunkBuilder::BuildChunkMesh(this, mPosition);
}

void Chunk::Render()
{
    UINT offset = 0;
    UINT stride = sizeof(BlockVertex);

    GraphicsEngine& GRM = GraphicsEngine::GetInstance();

    GRM.GetDeviceContext().IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    GRM.GetDeviceContext().IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    GRM.GetDeviceContext().VSSetConstantBuffers(0, 1, &mModelCB.GetGPU());

    GRM.GetDeviceContext().DrawIndexed(mIndexCount, 0, 0);
}

eBlockType Chunk::GetBlockType(int blockIndex)
{
    ASSERT(blockIndex >= 0 && blockIndex < CHUNK_VOLUME, "index out of bounds");

    return mBlockTypes[blockIndex];
}

void Chunk::SetBlock(int blockIndex, eBlockType blockType)
{
    ASSERT(blockIndex >= 0 && blockIndex < mBlockTypes.size(), "index out of bounds");

    mBlockTypes[blockIndex] = blockType;
}

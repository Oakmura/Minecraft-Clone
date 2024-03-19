#include "Precompiled.h"

#include "Chunk.h"
#include "Frustum.h"
#include "Geometry/ChunkBuilder.h"

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
    mCenter = SimpleMath::Vector3(mPosition.mX + 0.5f, mPosition.mY + 0.5f, mPosition.mZ + 0.5f) * def::CHUNK_SIZE;
    ChunkBuilder::BuildChunk(this, pos);

    SimpleMath::Vector3 translation = { (float)pos.mX * def::CHUNK_SIZE, (float)pos.mY * def::CHUNK_SIZE, (float)pos.mZ * def::CHUNK_SIZE };
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
    if (!Frustum::IsOnFrustum(*this))
    {
        return;
    }

    UINT offset = 0;
    UINT stride = sizeof(BlockVertex);

    GraphicsEngine& ge = GraphicsEngine::GetInstance();

    ge.GetDeviceContext().IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    ge.GetDeviceContext().IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    mModelCB.UseOn(eShader::Vertex, 0);

    ge.GetDeviceContext().DrawIndexed(mIndexCount, 0, 0);
}

eBlockType Chunk::GetBlockType(int blockIndex)
{
    ASSERT(blockIndex >= 0 && blockIndex < def::CHUNK_VOLUME, "index out of bounds");

    return mBlockTypes[blockIndex];
}

void Chunk::SetBlock(int blockIndex, eBlockType blockType)
{
    ASSERT(blockIndex >= 0 && blockIndex < mBlockTypes.size(), "index out of bounds");

    mBlockTypes[blockIndex] = blockType;
}

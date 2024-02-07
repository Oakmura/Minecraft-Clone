#include "Precompiled.h"

#include "Chunk.h"
#include "ChunkBuilder.h"

Chunk::~Chunk()
{
    RELEASE_COM(mVB);
    RELEASE_COM(mIB);
    RELEASE_COM(mModelGPU);
}

void Chunk::BuildVoxels(GraphicsResourceManager& GRM, const Vector3& pos)
{
    ChunkBuilder::BuildChunk(this, pos);

    mModelCPU = Matrix::CreateTranslation(pos * CHUNK_SIZE).Transpose();
    D3D11Utils::CreateConstantBuffer(*GRM.GetDevice(), mModelCPU, &mModelGPU);
}

void Chunk::BuildChunkMesh(GraphicsResourceManager& GRM, World& world, const Vector3& pos)
{
    ChunkBuilder::BuildChunkMesh(GRM, world, this, pos);
}

void Chunk::Render(GraphicsResourceManager& GRM)
{
    UINT offset = 0;
    UINT stride = sizeof(VoxelVertex);

    GRM.mContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    GRM.mContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    GRM.mContext->VSSetConstantBuffers(0, 1, &mModelGPU);

    GRM.mContext->DrawIndexed(mIndexCount, 0, 0);
}
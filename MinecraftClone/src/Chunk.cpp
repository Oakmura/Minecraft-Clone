#include "Precompiled.h"

#include "Chunk.h"
#include "ChunkBuilder.h"

Chunk::Chunk()
    : mVB(nullptr)
    , mIB(nullptr)
    , mModelGPU(nullptr)
    , mIndexCount(0)
{
}

Chunk::~Chunk()
{
    RELEASE_COM(mVB);
    RELEASE_COM(mIB);
    RELEASE_COM(mModelGPU);
}

void Chunk::BuildVoxels(const SimpleMath::Vector3& pos)
{
    mPosition = pos;
    ChunkBuilder::BuildChunk(this, pos);

    mModelCPU = SimpleMath::Matrix::CreateTranslation(pos * CHUNK_SIZE).Transpose();
    D3D11Utils::CreateConstantBuffer(GraphicsResourceManager::GetInstance().GetDevice(), mModelCPU, &mModelGPU);
}

void Chunk::BuildChunkMesh()
{
    ChunkBuilder::BuildChunkMesh(this, mPosition);
}

void Chunk::RebuildChunkMesh(World& world)
{
    mVoxels.clear();
    mIndices.clear();

    RELEASE_COM(mVB);
    RELEASE_COM(mIB);

    mIndexCount = 0;

    ChunkBuilder::BuildChunkMesh(this, mPosition);
}

void Chunk::Render()
{
    UINT offset = 0;
    UINT stride = sizeof(VoxelVertex);

    GraphicsResourceManager& GRM = GraphicsResourceManager::GetInstance();

    GRM.GetDeviceContext().IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
    GRM.GetDeviceContext().IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

    GRM.GetDeviceContext().VSSetConstantBuffers(0, 1, &mModelGPU);

    GRM.GetDeviceContext().DrawIndexed(mIndexCount, 0, 0);
}

void Chunk::SetVoxel(int voxelIndex, eVoxelType voxelType)
{
    ASSERT(voxelIndex >= 0 && voxelIndex < mVoxelTypes.size(), "index out of bounds");

    mVoxelTypes[voxelIndex] = voxelType;
}
